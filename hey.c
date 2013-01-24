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

#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <netdb.h>

#include "hey.h"
#include "lookup.h"
#include "connect.h"

int
hey_connect(struct hey *hey, const char *host, const char *serv, int timeout)
{
	struct hey_lookup dst;
	int r;

	if (hey || !host || !serv || timeout < 0)
		return HEY_EINVAL;

	r = hey_lookup(&dst, hey_af_any_inet, host, serv);
	if (r)
		return r;

	r = hey_do_connect(&dst, timeout, 200);

	hey_lookup_cleanup(&dst);
	return r;
}

static const int hey_gai_err_lookup[] =
{
	[ -HEY_EAI_AGAIN ] = EAI_AGAIN,
	[ -HEY_EAI_BADFLAGS ] = EAI_BADFLAGS,
	[ -HEY_EAI_BADHINTS ] = EAI_BADHINTS,
	[ -HEY_EAI_FAIL ] = EAI_FAIL,
	[ -HEY_EAI_FAMILY ] = EAI_FAMILY,
	[ -HEY_EAI_MEMORY ] = EAI_MEMORY,
	[ -HEY_EAI_NONAME ] = EAI_NONAME,
	[ -HEY_EAI_OVERFLOW ] = EAI_OVERFLOW,
	[ -HEY_EAI_PROTOCOL ] = EAI_PROTOCOL,
	[ -HEY_EAI_SERVICE ] = EAI_SERVICE,
	[ -HEY_EAI_SOCKTYPE ] = EAI_SOCKTYPE,
	[ -HEY_EAI_SYSTEM ] = EAI_SYSTEM,
};

const char *hey_strerror(int err, char *buf, size_t buflen)
{
	switch (err)
	{
	case HEY_ETIMEDOUT:
		return "timed out";
	case HEY_EINVAL:
		return "invalid parameter";
	case HEY_ESYSTEM:
		strerror_r(errno, buf, buflen);
		return buf;
	case HEY_EAI_AGAIN:
	case HEY_EAI_BADFLAGS:
	case HEY_EAI_BADHINTS:
	case HEY_EAI_FAIL:
	case HEY_EAI_FAMILY:
	case HEY_EAI_MEMORY:
	case HEY_EAI_NONAME:
	case HEY_EAI_OVERFLOW:
	case HEY_EAI_PROTOCOL:
	case HEY_EAI_SERVICE:
	case HEY_EAI_SOCKTYPE:
	case HEY_EAI_SYSTEM:
		return gai_strerror(hey_gai_err_lookup[-err]);
	case HEY_EAI_UNKNOWN:
		snprintf(buf, buflen, "getaddrinfo unknown error");
		return buf;
	}
	snprintf(buf, buflen, "unknown (%d)", err);
	return buf;
}

