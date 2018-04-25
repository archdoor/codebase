#ifndef __MEMORY_POOL_H__
#define __MEMORY_POOL_H__

#include <pthread.h>
#include "duple_list.h"

#define MEMORY_LEISURE 0x00 
#define MEMORY_OCCUPIED 0xFF 

typedef struct 
{
    void *buff;    // 内存指针
}memory_node_t;

#define MEMORY_NODE_T_SIZE sizeof(memory_node_t)

typedef struct
{
    int node_size;  // 内存节点长度
    duple_list *memory_list_leisure;    // 空闲内存节点链表
    duple_list *memory_list_occupied;    // 占用内存节点链表
    pthread_mutex_t mutex;    // 互斥量
}memory_pool_t;


// 创建内存池
memory_pool_t *memory_pool_new(int node_num, int node_size);
// 申请节点
void *memory_get(memory_pool_t *pool);
// 持续申请节点
void *memory_persist_get(memory_pool_t *pool);
// 释放节点
int memory_free(memory_pool_t *pool, void *node);
// 内存池销毁
void memory_pool_destroy(memory_pool_t *pool);


#endif



