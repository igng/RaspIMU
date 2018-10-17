#ifndef TIMER_H_
#define TIMER_H_

#include <sys/time.h>
#include <unistd.h>
#include <signal.h>

#define PERIOD 1000  // ms

void timer_init(int);
void my_fun(void);

#endif
