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

struct hey;

enum hey_af_flag
{
	hey_aff_inet = 0x1,
	hey_aff_inet6 = 0x2,
	hey_aff_inet6_mapped = 0x4,
	hey_aff_unix = 0x8,
};

enum hey_af
{
	hey_af_any                  = 0xff,
	hey_af_any_inet             = hey_aff_inet | hey_aff_inet6 | hey_aff_inet6_mapped,
	hey_af_inet                 = hey_aff_inet,
	hey_af_inet6                = hey_aff_inet6,
	hey_af_unix                 = hey_aff_unix,
	hey_af_inet6_mapped         = hey_aff_inet6 | hey_aff_inet6_mapped,
	hey_af_inet6_mapped_or_unix = hey_aff_inet6 | hey_aff_inet6_mapped | hey_aff_unix,
};

struct hey *hey_create(enum hey_af af, int use_srv);
void hey_free(struct hey *hey);

void hey_set_cache_ttl(struct hey *hey, int seconds);
void hey_clear_cache(struct hey *hey, const char *host);

int hey_connect(struct hey *hey, const char *host, const char *serv, int timeout);
