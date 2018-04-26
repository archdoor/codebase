#include <stdio.h>
#include <signal.h>
#include "memory_pool.h"

static memory_pool_t *g_pool = NULL;
void *g_memory[6] = {0};
int g_count = 0;


void trigger_get_memory(int sig)
{
    g_memory[g_count] = memory_get(g_pool);
    if ( g_memory[g_count] != NULL )
    {
        g_count++;
        printf("get success:%d\n", g_count);
    }
    else
    {
        printf("get fail\n");
    }
}

void trigger_free_memory(int sig)
{
    g_count--;
    if ( memory_free(g_pool, g_memory[g_count]) == 0 )
    {
        printf("free success:%d\n", g_count);
    }
    else
    {
        g_count++;
        printf("free fail\n");
    }
}

int main()
{
    signal(SIGTERM, trigger_get_memory);
    signal(SIGINT, trigger_free_memory);

    g_pool = memory_pool_new(5, 1024);
    if ( g_pool == NULL )
    {
        printf("new pool error\n");
        return -1;
    }
    printf("g_pool:%p\n", g_pool);

    sigset_t set;
    sigemptyset(&set);
    while(1)
    {
        sigsuspend(&set);
    }

    return 0;
}





