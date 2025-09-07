#include <sys/times.h>
#include <unistd.h>
#include <stdio.h>

static struct tms start_time;
static struct tms end_time;


void timer_start()
{
  // records time then called
  times(&start_time);
}

int timer_stop()
{
  int ms;

  // returns milliseconds simce timer_start was called
  times(&end_time);

  // compute time delta
  ms = end_time.tms_utime - start_time.tms_utime;
  ms += end_time.tms_stime - start_time.tms_stime;

  ms = ms * 1000.0 / ((float) sysconf(_SC_CLK_TCK));
  return ms;
}

