#ifndef __DUPLE_LIST_H__
#define __DUPLE_LIST_H__

// 双向循环链表节点
typedef struct _duple_list_node
{
    struct _duple_list_node *next;
    struct _duple_list_node *pre;
}duple_list_node;

// 双向循环链表头
typedef struct _duple_list_head
{
    int size;   // 链表元素个数
    duple_list_node head;   // 链表头节点
}duple_list, duple_list_head;

#define DUPLE_LIST_NODE_SIZE sizeof(duple_list_node)
#define DUPLE_LIST_HEAD_SIZE sizeof(duple_list_head)


// 创建
duple_list *duple_list_new();
void *duple_list_node_new(int size);

// 插入
void duple_list_push_front(duple_list *list, void *data);
void duple_list_push_back(duple_list *list, void *data);

// 查询
void *duple_list_get_front(duple_list *list);
void *duple_list_get_back(duple_list *list);
void *duple_list_get_next(duple_list *list, void *data);

// 删除(不释放节点空间)
int duple_list_pop(duple_list *list, void *data);
void *duple_list_pop_front(duple_list *list);
void *duple_list_pop_back(duple_list *list);

// 删除(释放节点空间)
int duple_list_delete(duple_list *list, void *data);

// 销毁
void duple_list_node_free(void *data);
void duple_list_destory(duple_list *list);


#endif



