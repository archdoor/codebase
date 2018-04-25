#include <signal.h>
#include "memory_pool.h"


void trigger_add_timer(int sig)
{
}

int main()
{
    signal(SIGTERM, trigger_add_timer);


    sigset_t set;
    sigemptyset(&set);
    while(1)
    {
        sigsuspend(&set);
    }

    return 0;
}





