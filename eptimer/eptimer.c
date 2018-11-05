#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include "eptimer.h"

inline void eptimer_remove(eptimer_t *timer)
{
    timer->pre->next = timer->next;
    if ( timer->next != NULL )
        timer->next->pre = timer->pre;
}

void eptiemr_destory(eptimer_t *timer)
{
    eptimer_remove(timer);
    eptimer_free(timer);
}

static inline void eptimer_seton(eptimer_base_t *base, eptimer_t *timer)
{
    timer->next = base->timer_list[timer->slot].head.next;
    timer->pre = &base->timer_list[timer->slot].head;
    if ( base->timer_list[timer->slot].head.next != NULL )
        base->timer_list[timer->slot].head.next->pre = timer;
    base->timer_list[timer->slot].head.next = timer;
}

static inline void eptimer_reset(eptimer_base_t *base, eptimer_t *timer)
{
    eptimer_remove(timer);

    timer->round = timer->offset / WHEEL_SLOT_NUM;
    timer->slot = (base->cur_slot + timer->offset) % WHEEL_SLOT_NUM;

    eptimer_seton(base, timer);
}

static inline void eptimer_ontime(eptimer_base_t *base, eptimer_t *timer)
{
    if ( timer->callback(timer->ptr) )
    {
        pthread_mutex_lock(&base->timer_list[base->cur_slot].mutex);
        if ( timer->onsucc != NULL )
            timer->onsucc(timer);
        else
            eptimer_reset(base, timer);
        pthread_mutex_unlock(&base->timer_list[base->cur_slot].mutex);
    }
    else
    {
        pthread_mutex_lock(&base->timer_list[base->cur_slot].mutex);
        if ( timer->onfail != NULL )
            timer->onfail(timer);
        else
            eptimer_reset(base, timer);
        pthread_mutex_unlock(&base->timer_list[base->cur_slot].mutex);
    }
}

void eptimer_run(eptimer_base_t *base)
{
    eptimer_t *timer = base->timer_list[base->cur_slot].head.next;
    eptimer_t *next_timer = NULL;

    while ( timer != NULL )
    {
        next_timer = timer->next;
        (timer->round > 0) ? (timer->round--) : (eptimer_ontime(base, timer));
        timer = next_timer;
    }
}

void *timer_base_thread_fn(void *args)
{
    eptimer_base_t *base = (eptimer_base_t *)args;

    struct epoll_event events;
    while ( epoll_wait(base->epollfd, &events, 1, EPTIMER_MIN_MSEC) == 0 )
    {
        base->cur_slot++;
        if ( base->cur_slot == WHEEL_SLOT_NUM )
            base->cur_slot = 0;

        eptimer_run(base);
    }
    return NULL;
}

int eptimer_base_start(eptimer_base_t *base)
{
    base->epollfd = epoll_create(100);
    if ( base->epollfd == -1 )
        return -1;

    pthread_t tid;
    if ( pthread_create(&tid, NULL, timer_base_thread_fn, base) != 0 )
        return -1;
    pthread_detach(tid);

    return 0;
}

eptimer_t *eptimer_new(int sec, int msec)
{
    eptimer_t *timer = (eptimer_t *)calloc(1, sizeof(eptimer_t));
    if ( timer == NULL )
        return NULL;

    timer->tv_sec = sec;
    timer->tv_msec = msec;
    timer->offset = (sec * 1000 + msec) / EPTIMER_MIN_MSEC;

    return timer;
}

int eptimer_add(eptimer_base_t *base, bool imme, eptimer_t *timer, bool (*callback)(void *), void *ptr, void (*onsucc)(eptimer_t *), void (*onfail)(eptimer_t *))
{
    timer->callback = callback;
    timer->onsucc = onsucc;
    timer->onfail = onfail;
    timer->ptr = ptr;
    timer->round = timer->offset / WHEEL_SLOT_NUM;
    timer->slot = (base->cur_slot + timer->offset) % WHEEL_SLOT_NUM;

    pthread_mutex_lock(&base->timer_list[timer->slot].mutex);

    eptimer_seton(base, timer);
    if ( imme )
        eptimer_ontime(base, timer);
     
    pthread_mutex_unlock(&base->timer_list[timer->slot].mutex);
    return 0;
}

int eptimer_delete(eptimer_base_t *base, eptimer_t *timer)
{
    pthread_mutex_lock(&base->timer_list[timer->slot].mutex);

    timer->pre->next = timer->next;
    if ( timer->next != NULL )
        timer->next->pre = timer->pre;

    pthread_mutex_unlock(&base->timer_list[timer->slot].mutex);
    return 0;
}

int eptimer_free(eptimer_t *timer)
{
    free(timer);
    return 0;
}




