
#include <time.h>

int ts_now(struct timespec *ts);

void ts_add_ms(struct timespec *ts, int ms);

void ts_sub_ts(struct timespec *dst, const struct timespec *from, const struct timespec *sub);

int ts_cmp(const struct timespec *a, const struct timespec *b);
