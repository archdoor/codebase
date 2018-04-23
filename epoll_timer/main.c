#include <stdio.h>
#include <signal.h>
#include "epoll_timer.h"

static eptimer_base *g_timer_base = NULL;

void timer_expire(void *args)
{
    printf("运行定时器\n");
}

void trigger_add_timer(int sig)
{
    eptimer *timer = eptimer_new(0, 200);
    if ( timer == NULL )
    {
        printf("new timer fail\n");
        return ;
    }

    if ( eptimer_add(g_timer_base, EPTIMER_PERSIST, timer, timer_expire, NULL) != 0 )
    {
        printf("add timer fail\n");
    }

    printf("添加定时器:%d\n", g_timer_base->size);
}

int main()
{
    signal(SIGTERM, trigger_add_timer);

    g_timer_base = eptimer_base_new();
    if ( g_timer_base == NULL )
    {
        printf("get timer base fail\n");
        return -1;
    }

    if ( eptimer_base_start(g_timer_base) < 0 )
    {
        printf("timer base start fail\n");
        return -1;
    }


    eptimer *timer = eptimer_new(0, 100);
    if ( timer == NULL )
    {
        printf("new timer fail\n");
        return -1;
    }

    if ( eptimer_add(g_timer_base, EPTIMER_ONE_SHOT, timer, timer_expire, NULL) != 0 )
    {
        printf("add timer fail\n");
        return -1;
    }

    timer = eptimer_new(0, 100);
    if ( timer == NULL )
    {
        printf("new timer fail\n");
        return -1;
    }

    if ( eptimer_add(g_timer_base, EPTIMER_PERSIST, timer, timer_expire, NULL) != 0 )
    {
        printf("add timer fail\n");
        return -1;
    }

    sigset_t set;
    sigemptyset(&set);
    while(1)
    {
        sigsuspend(&set);
    }

    return 0;
}





