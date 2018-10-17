#include "timer.h"
#include "debug.h"

void timer_init(int period)
{
    MISC("=> timer_init");
    struct itimerval it_val;

    if (signal(SIGALRM, (void (*)(int)) my_fun) == SIG_ERR)
        error_handler("timer_init | signal");

    it_val.it_value.tv_sec = period/1000;
    it_val.it_value.tv_usec = (period*1000)%(1000000);
    it_val.it_interval = it_val.it_value;

    if (setitimer(ITIMER_REAL, &it_val, NULL) == -1)
        error_handler("timer_init | setitimer");
    MISC("<= timer_init");
}

void my_fun(void)
{
    INFO("Timer expired!");
}
