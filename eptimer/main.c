#include <stdio.h>
#include <signal.h>
#include "eptimer.h"

eptimer_base_t g_timer_base = EPTIMER_BASE_INITIALIZER;

bool timer_expire1(void *args)
{
    printf("运行定时器1\n");
    return true;
}

bool timer_expire2(void *args)
{
    printf("运行定时器2\n");
    return true;
}

int main()
{
    if ( eptimer_base_start(&g_timer_base) != 0 )
    {
        printf("timer base start error!\n");
        return -1;
    }

    eptimer_t *timer_1 = eptimer_new(1, 100);
    if ( timer_1 == NULL )
    {
        printf("new timer 1 fail\n");
        return -1;
    }

    if ( eptimer_add(&g_timer_base, true, timer_1, timer_expire1, NULL, EPTIEMR_REMOVE, NULL) != 0 )
    {
        printf("add timer 1 fail\n");
        return -1;
    }

    eptimer_t *timer_2 = eptimer_new(1, 0);
    if ( timer_2 == NULL )
    {
        printf("new timer_2 fail\n");
        return -1;
    }

    if ( eptimer_add(&g_timer_base, false, timer_2, timer_expire2, NULL, NULL, NULL) != 0 )
    {
        printf("add timer_2 fail\n");
        return -1;
    }

    sigset_t set;
    sigemptyset(&set);
    while(1)
    {
        sigsuspend(&set);
    }

    eptimer_delete(&g_timer_base, timer_1);
    eptimer_free(timer_1);
    eptimer_delete(&g_timer_base, timer_2);
    eptimer_free(timer_2);

    return 0;
}





