/*
 *  自定义内存池
 *
 *  使用说明：
 *
 */

// 内存池规划
int memory_pool_schedule(int node_num, int node_size)
{
    return 0;
}

// 申请节点
void *memory_get(int node_num, int node_size)
{
    return NULL;
}

// 持续申请节点
void *memory_persist_get(int node_num, int node_size)
{
    return NULL;
}

// 释放节点
int memory_free(int node_num, int node_size)
{
    return 0;
}

// 内存池销毁
int memory_pool_destory(int node_num, int node_size)
{
    return 0;
}


