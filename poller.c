
#include "poller.h"
#include "timefuncs.h"

#include <errno.h>
#include <unistd.h>

#include <sys/event.h>
#include <sys/time.h>

int
hey_poller_init(struct hey_poller *poller)
{
	poller->kqfd = kqueue();
	if (poller->kqfd < 0)
		return -1;
	return 0;
}

void
hey_poller_reset_timeout(struct hey_poller *poller, const struct timespec *absto)
{
	poller->absto = *absto;
}

int
hey_poller_poll(struct hey_poller *poller, int *fds, int nfds, bool *ready)
{
	struct kevent changes[10], event;
	int nchanges = 0;
	struct timespec to;
	int r;

	while (poller->activefds < nfds)
		EV_SET(&changes[nchanges++], fds[poller->activefds++], EVFILT_WRITE, EV_ADD, 0, 0, NULL);

	do {
		if (ts_now(&to))
			return -1;
		if (ts_cmp(&to, &poller->absto) > 0)
			to.tv_sec = to.tv_nsec = 0;
		else
			ts_sub_ts(&to, &poller->absto, &to);

		r = kevent(poller->kqfd, changes, nchanges, &event, 1, &to);
	} while (r == -1 && errno == EINTR);

	if (r < 0)
		return POLLER_FATAL;
	if (r == 0)
		return POLLER_TIMEOUT;

	while (--nfds >= 0)
	{
		if (event.ident == fds[nfds])
			break;
	}

	*ready = event.flags & EV_EOF ? false : true;
	poller->activefds--;
	return nfds;
}

int
hey_poller_cleanup(struct hey_poller *poller)
{
	close(poller->kqfd);
	return 0;
}

