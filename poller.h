
#include <stdbool.h>
#include <time.h>

#define POLLER_TIMEOUT -1
#define POLLER_FATAL -2

struct hey_poller
{
	int kqfd;
	int activefds;
	struct timespec absto;
};

int hey_poller_init(struct hey_poller *poller);
int hey_poller_cleanup(struct hey_poller *poller);

void hey_poller_reset_timeout(struct hey_poller *poller, const struct timespec *absto);
int hey_poller_poll(struct hey_poller *poller, int *fds, int nfds, bool *ready);
