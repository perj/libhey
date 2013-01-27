/*
 * Copyright (c) 2013 Per Johansson
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

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
	char host[512];
	char serv[64];

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
