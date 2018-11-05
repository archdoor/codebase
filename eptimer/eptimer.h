#ifndef __EPOLL_TIMER_H__
#define __EPOLL_TIMER_H__

/* NOTE:
 *      主函数成功或者失败时的回调函数只能是内部宏定义函数 EPTIEMR_REMOVE/EPTIEMR_DESTORY
 *
 *      1. 定时器主回调函数内，不能对本定时器进行删除操作(影响永久定时器的重置操作)
 *          替代方案：利用主函数执行成功或者失败时的内部宏定义回调函数EPTIEMR_REMOVE/EPTIEMR_DESTORY进行删除/销毁
 */

#include <stdbool.h>
#include <pthread.h>

#define WHEEL_SLOT_NUM 1000
#define EPTIMER_MIN_MSEC 100 // 最小定时器时隙(单位：毫秒)
#define EPTIEMR_REMOVE eptimer_remove
#define EPTIEMR_DESTORY eptiemr_destory
#define EPTIMER_BASE_INITIALIZER {.timer_list = {[0 ... WHEEL_SLOT_NUM-1] = {.mutex = PTHREAD_MUTEX_INITIALIZER}}}

typedef struct _epoll_timer
{
    int tv_sec;     // 周期秒数
    int tv_msec;    // 周期毫秒数
    int offset; // 偏移的时间槽数量

    bool (*callback)(void *);   // 主处理函数
    void *ptr; // 参数
    void (*onsucc)(struct _epoll_timer *);     // 主函数执行成功时回调函数
    void (*onfail)(struct _epoll_timer *);     // 主函数执行失败时回调函数

    int round;  // 偏移的时间轮数
    int slot;  // 当前所在时间槽

    struct _epoll_timer *next;
    struct _epoll_timer *pre;
}eptimer_t;

typedef struct
{
    eptimer_t head;
    pthread_mutex_t mutex;
}eptimer_list_t;

typedef struct
{
    int cur_slot;    // 当前时间轮
    eptimer_list_t timer_list[WHEEL_SLOT_NUM];   // 时间轮各个时间槽上定时器列表
    int epollfd;   // epoll句柄
}eptimer_base_t;

int eptimer_base_start(eptimer_base_t *base);
eptimer_t *eptimer_new(int sec, int msec);
int eptimer_add(eptimer_base_t *base, bool imme, eptimer_t *timer, bool (*callback)(void *), void *ptr, void (*onsucc)(eptimer_t *), void (*onfail)(eptimer_t *));
int eptimer_delete(eptimer_base_t *base, eptimer_t *timer);
int eptimer_free(eptimer_t *timer);



void eptimer_remove(eptimer_t *timer);
void eptiemr_destory(eptimer_t *timer);

#endif



