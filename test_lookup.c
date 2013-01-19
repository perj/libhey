
#include <stdlib.h>
#include <stdio.h>

#include <err.h>
#include <netdb.h>

#include "hey.h"
#include "lookup.h"

const char *
str_af(int af, char *buf, size_t bufsz)
{
	static const char *const afs[] =
	{
		[ AF_INET ] = "AF_INET",
		[ AF_INET6 ] = "AF_INET6",
		[ AF_UNIX ] = "AF_UNIX",
	};
	const char *res = NULL;

	if (af >= 0 && af < sizeof(afs) / sizeof(*afs))
		res = afs[af];

	if (!res)
	{
		snprintf(buf, bufsz, "unknown af %d", af);
		res = buf;
	}

	return res;
}

void
print_addr_chain(struct hey_addr *addr)
{
	char afbuf[256];
	char host[NI_MAXHOST];
	char serv[NI_MAXSERV];

	for ( ; addr ; addr = addr->next) {
		if (getnameinfo((struct sockaddr*)&addr->addr, addr->addrlen, host, sizeof (host), serv, sizeof (serv), NI_NUMERICHOST | NI_NUMERICSERV))
			errx(1, "getnameinfo failed");

		printf("%s: %s %s\n", str_af(addr->family, afbuf, sizeof (afbuf)), host, serv);
	}
}

void
test_lookup(enum hey_af af, const char *host, const char *serv)
{
	struct hey_lookup dst;
	int r;
	char buf[256];

	r = hey_lookup(&dst, af, host, serv);
	if (r)
		errx(1, "hey_lookup failed: %s", gai_strerror(r));

	printf("Preferred AF: %s\n", str_af(dst.pref_af, buf, sizeof (buf)));
	printf("inet6:\n");
	print_addr_chain(dst.inet6);
	printf("inet4:\n");
	print_addr_chain(dst.inet4);
}

int
main(int argc, char *argv[])
{
	test_lookup(hey_af_any_inet, argv[1], argv[2] ?: "http");
}
