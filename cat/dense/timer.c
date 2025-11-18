#include <sys/times.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>

static struct tms start_time;
static struct tms end_time;

static struct timespec ns_start_time;
static struct timespec ns_end_time;


void timer_start()
{
  // records time then called
  times(&start_time);
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ns_start_time);
}

int timer_stop()
{
  int us;
  int ms;
  int secs;

  // returns milliseconds simce timer_start was called
  times(&end_time);
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ns_end_time);

  // compute_time_delta

  secs = ns_end_time.tv_sec - ns_start_time.tv_sec;

  if (ns_end_time.tv_nsec < ns_start_time.tv_nsec) {
    secs--;
    ns_end_time.tv_nsec += 1000000000;
  }

  us = (ns_end_time.tv_nsec - ns_start_time.tv_nsec)/1000;

  return secs * 1000000 + us;

  // compute time delta
  ms = end_time.tms_utime - start_time.tms_utime;
  ms += end_time.tms_stime - start_time.tms_stime;

  ms = ms * 1000.0 / ((float) sysconf(_SC_CLK_TCK));
  return ms;
}

