/*
 *  自定义内存池
 *
 *  使用说明：
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "memory_pool.h"

// 创建内存池
memory_pool_t *memory_pool_new(int node_num, int node_size)
{
    memory_pool_t *pool = (memory_pool_t *)malloc(node_num * node_size);
    if ( pool == NULL )
    {
        return NULL;
    }

    if ( pthread_mutex_init(&pool->mutex, NULL) != 0 )
    {
        free(pool);
        return NULL;
    }

    // 空闲链表
    pool->memory_list_leisure = duple_list_new();
    if ( pool->memory_list_leisure == NULL )
    {
        free(pool);
        return NULL;
    }

    // 占用链表
    pool->memory_list_occupied = duple_list_new();
    if ( pool->memory_list_occupied == NULL )
    {
        free(pool);
        return NULL;
    }

    // 内存节点
    int i = 0;
    memory_node_t *node = NULL;

    for ( i = 0; i < node_num; ++i )
    {
        node = (memory_node_t *)duple_list_node_new(MEMORY_NODE_T_SIZE + node_size);
        if ( node == NULL )
        {
            free(pool);
            duple_list_destroy(pool->memory_list_leisure);
        }

        node->buff = (char *)(node) + MEMORY_NODE_T_SIZE;

        duple_list_push_back(pool->memory_list_leisure, node);
    }

    return pool;
}

// 申请节点
void *memory_get(memory_pool_t *pool)
{
    pthread_mutex_lock(&pool->mutex);

    void *node = duple_list_pop_front(pool->memory_list_leisure);
    if ( node != NULL )
    {
        duple_list_push_back(pool->memory_list_occupied, node);
    }

    pthread_mutex_unlock(&pool->mutex);

    return NULL;
}

// 持续申请节点
void *memory_persist_get(memory_pool_t *pool)
{
    void *node = memory_get(pool);
    while ( node == NULL )
    {
        usleep(100);
        node = memory_get(pool);
    }

    return node;
}

// 释放节点
int memory_free(memory_pool_t *pool, void *node)
{
    pthread_mutex_lock(&pool->mutex);

    if ( duple_list_pop(pool->memory_list_occupied, node) != 0 )
    {
        pthread_mutex_unlock(&pool->mutex);
        return -1;
    }

    duple_list_push_back(pool->memory_list_leisure, node);

    pthread_mutex_unlock(&pool->mutex);

    return 0;
}

// 内存池销毁
void memory_pool_destroy(memory_pool_t *pool)
{
    pthread_mutex_lock(&pool->mutex);

    duple_list_destroy(pool->memory_list_leisure);
    duple_list_destroy(pool->memory_list_occupied);

    pthread_mutex_unlock(&pool->mutex);

    pthread_mutex_destroy(&pool->mutex);

    free(pool);
}


