
#include <sys/socket.h>

struct hey_addr
{
	struct hey_addr **rptr;
	struct hey_addr *next;
	int family;
	socklen_t addrlen;
	struct sockaddr_storage addr;
};

struct hey_lookup
{
	struct hey_addr *inet4;
	struct hey_addr *inet6;
	int pref_af;
};

int hey_lookup(struct hey_lookup *dst, enum hey_af af, const char *host, const char *serv);
