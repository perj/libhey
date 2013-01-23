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
#include <string.h>

#include <sys/socket.h>
#include <netdb.h>

#include "hey.h"
#include "lookup.h"

static const int af_lookup[] =
{
	[ 0 ] = AF_UNSPEC,
	[ hey_aff_inet ] = AF_INET,
	[ hey_aff_inet6 ] = AF_INET6,
	[ hey_aff_inet | hey_aff_inet6 ] = AF_UNSPEC,
};

static struct hey_addr **
hey_add_lookup(struct hey_addr **tail, struct addrinfo *addr)
{
	*tail = malloc(sizeof (**tail));
	if (!*tail)
		return NULL;

	(**tail).rptr = tail;
	(**tail).family = addr->ai_family;
#if 0
	(**tail).socktype = addr->ai_socktype;
	(**tail).protocol = addr->ai_protocol;
#endif
	(**tail).addrlen = addr->ai_addrlen;
	memcpy(&(**tail).addr, addr->ai_addr, addr->ai_addrlen);
	return &(**tail).next;
}

void
hey_remove_lookup(struct hey_addr *addr)
{
	*addr->rptr = addr->next;
	if (addr->next)
		addr->next->rptr = addr->rptr;
	free(addr);
}

static int
hey_real_af(struct addrinfo *ai)
{
	if (ai->ai_family == AF_INET6)
	{
		const struct sockaddr_in6 *in6 = (struct sockaddr_in6*)ai->ai_addr;
		if (IN6_IS_ADDR_V4MAPPED(&in6->sin6_addr))
			return AF_INET;
	}
	return ai->ai_family;
}

int
hey_lookup(struct hey_lookup *dst, enum hey_af af, const char *host, const char *serv)
{
	const struct addrinfo hints =
	{
		.ai_flags = AI_ADDRCONFIG | (af & hey_aff_inet6_mapped ? AI_V4MAPPED | AI_ALL : 0),
		.ai_family = af_lookup[af & (hey_aff_inet | hey_aff_inet6)],
		.ai_socktype = SOCK_STREAM,
	};
	int err;
	struct addrinfo *res, *curr;
	struct hey_addr **tail4 = &dst->inet4;
	struct hey_addr **tail6 = &dst->inet6;

	err = getaddrinfo(host, serv, &hints, &res);
	if (err)
		return err;

	dst->pref_af = hey_real_af(res);

	for (curr = res ; curr ; curr = curr->ai_next)
	{
		if (hey_real_af(curr) == AF_INET6)
			tail6 = hey_add_lookup(tail6, curr);
		else
			tail4 = hey_add_lookup(tail4, curr);
		if (!tail4 || !tail6)
			return EAI_MEMORY;
	}
	*tail4 = NULL;
	*tail6 = NULL;
	return 0;
}

