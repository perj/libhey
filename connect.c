
#include <stdbool.h>
#include <string.h>

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#include "hey.h"
#include "connect.h"
#include "lookup.h"
#include "poller.h"

int
hey_connect_addr(const struct hey_addr *addr, bool *ready)
{
	int s = socket(addr->family, SOCK_STREAM, 0);
	if (s == -1)
		goto fail;

	if (fcntl(s, F_SETFL, O_NONBLOCK))
		goto fail;

	if (connect(s, (struct sockaddr*)&addr->addr, addr->addrlen) == 0)
	{
		*ready = true;
		return s;
	}
	if (errno == EINPROGRESS)
	{
		*ready = false;
		return s;
	}

fail:
	if (s != -1)
		close(s);
	return -1;
}

struct hey_next
{
	const struct hey_addr *nextaddrs[2];
	int nextnum, nextidx;
};

static void
hey_next_init(struct hey_next *next, const struct hey_lookup *lookup)
{
	next->nextaddrs[0] = lookup->inet4;
	next->nextaddrs[1] = lookup->inet6;
	next->nextnum = 2;
	next->nextidx = lookup->pref_af == AF_INET6 ? 1 : 0;
}

static const struct hey_addr *
hey_next_addr(struct hey_next *next)
{
	const struct hey_addr *addr = next->nextaddrs[next->nextidx %= next->nextnum];

	while (!addr)
	{
		if (--(next->nextnum) == 0)
			return NULL;

		memmove(next->nextaddrs + next->nextidx, next->nextaddrs + next->nextidx + 1,
				(next->nextnum - next->nextidx) * sizeof (*next->nextaddrs));
		addr = next->nextaddrs[next->nextidx %= next->nextnum];
	}

	next->nextaddrs[next->nextidx++] = addr->next;
	return addr;
}

static int
hey_connect_poll(struct hey_poller *poller, int *fds, int *nfds, int *err)
{
	int idx;
	bool ready = false;

	/* Don't leave loop if no space left in fds */
	while (!ready || *nfds == 10)
	{
		idx = hey_poller_poll(poller, fds, *nfds, &ready);
		if (idx == POLLER_ABS_TIMEOUT)
			break;
		if (idx == POLLER_DELTA_TIMEOUT)
			ready = true;
		else if (!ready)
		{
			socklen_t l = sizeof(*err);

			getsockopt(fds[idx], SOL_SOCKET, SO_ERROR, err, &l);
			close(fds[idx]);
			if (--*nfds == 0)
			{
				idx = POLLER_DELTA_TIMEOUT;
				break;
			}
			memmove(fds + idx, fds + idx + 1, (*nfds - idx) * sizeof (*fds));
		}
	}
	return idx;
}

int
hey_do_connect(struct hey_lookup *lookup, int absto, int deltato)
{
	struct hey_next next;
	const struct hey_addr *addr;
	struct hey_poller poller;
	int fds[10];
	int nfds = 0;
	int idx = POLLER_DELTA_TIMEOUT;
	int err = 0;

	hey_next_init(&next, lookup);
	hey_poller_init(&poller, absto, deltato);

	while (idx == POLLER_DELTA_TIMEOUT && (addr = hey_next_addr(&next)))
	{
		bool ready;
		int s = hey_connect_addr(addr, &ready);

		if (s < 0)
			continue;

		idx = nfds;
		fds[nfds++] = s;

		idx = hey_connect_poll(&poller, fds, &nfds, &err);
	}

	hey_poller_cleanup(&poller);

	while (--nfds >= 0)
	{
		if (nfds != idx)
			close(fds[nfds]);
	}
	if (idx < 0)
	{
		if (err)
			errno = err;
		return -1;
	}
	return fds[idx];
}
