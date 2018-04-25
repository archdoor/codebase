#ifndef __MEMORY_POOL_H__
#define __MEMORY_POOL_H__

#include "duple_list.h"

#define MEMORY_LEISURE 0x00 
#define MEMORY_OCCUPIED 0xFF 

typedef struct 
{
    unsigned char leisure_flag; // 是否空闲
}memory_node;

typedef struct
{
    int node_num;   // 内存节点总个数
    int node_size;  // 内存节点长度
    int free_node_num;      // 空闲内存节点个数
    duple_list *memory_list;    // 内存节点链表
    pthread_mutex_t m_mutex;    // 互斥量
}memory_pool;


// 内存池规划
int memory_pool_schedule(int node_num, int node_size);
// 申请节点
void *memory_get(int node_num, int node_size);
// 持续申请节点
void *memory_persist_get(int node_num, int node_size);
// 释放节点
int memory_free(int node_num, int node_size);
// 内存池销毁
int memory_pool_destory(int node_num, int node_size);


#endif



