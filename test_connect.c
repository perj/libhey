
#include <string.h>

#include <err.h>
#include <errno.h>
#include <netdb.h>

#include "hey.h"
#include "lookup.h"
#include "connect.h"

void
test_connect(enum hey_af af, const char *host, const char *serv)
{
	struct hey_lookup dst;
	int r;

	r = hey_lookup(&dst, af, host, serv);
	if (r)
		errx(1, "hey_lookup failed: %s", gai_strerror(r));

	r = hey_do_connect(&dst, 10000, 250);
	if (r < 0)
		errx(1, "hey_do_connect failed: %s", strerror(errno));
}

int
main(int argc, char *argv[])
{
	test_connect(hey_af_any_inet, argv[1], argv[2] ?: "http");
}
