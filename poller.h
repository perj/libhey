
#include <stdbool.h>
#include <time.h>

#define POLLER_DELTA_TIMEOUT -1
#define POLLER_ABS_TIMEOUT -2
#define POLLER_FATAL -3

struct hey_poller
{
	struct timespec absto;
	int deltato;
	int kqfd;
	int activefds;
};

int hey_poller_init(struct hey_poller *poller, int absto, int deltato);
int hey_poller_cleanup(struct hey_poller *poller);

int hey_poller_poll(struct hey_poller *poller, int *fds, int nfds, bool *ready);
