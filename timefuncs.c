
#include <time.h>

#include <sys/time.h>

#include "timefuncs.h"

int
ts_now(struct timespec *ts)
{
	struct timeval tv;

	if (gettimeofday(&tv, NULL))
		return -1;

	ts->tv_sec = tv.tv_sec;
	ts->tv_nsec = tv.tv_usec * 1000;
	return 0;
}

void
ts_add_ms(struct timespec *ts, int ms)
{
	ts->tv_sec += ms / 1000;
	ts->tv_nsec += (ms % 1000) * 1000000;
	if (ts->tv_nsec >= 1000000000)
	{
		ts->tv_sec++;
		ts->tv_nsec -= 1000000000;
	}
}

void
ts_sub_ts(struct timespec *dst, const struct timespec *from, const struct timespec *sub)
{
	dst->tv_sec = from->tv_sec - sub->tv_sec;
	if (from->tv_nsec < sub->tv_nsec)
	{
		dst->tv_sec--;
		dst->tv_nsec = from->tv_nsec + 1000000000 - sub->tv_nsec;
	}
	else
		dst->tv_nsec = from->tv_nsec - sub->tv_nsec;
}

int
ts_cmp(const struct timespec *a, const struct timespec *b)
{
	if (a->tv_sec < b->tv_sec)
		return -1;
	if (a->tv_sec > b->tv_sec)
		return 1;
	if (a->tv_nsec < b->tv_nsec)
		return -1;
	if (a->tv_nsec > b->tv_nsec)
		return 1;
	return 0;
}
