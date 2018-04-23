/*
 *  自定义双向循环链表
 *
 *  使用说明：
 *      1. 双向循环链表不提供线程安全处理
 *
 */

#include <stdlib.h>
#include <string.h>
#include "duple_list.h"

// 创建链表
duple_list *duple_list_new()
{
    duple_list *list = (duple_list *)malloc(DUPLE_LIST_HEAD_SIZE);
    if ( list == NULL )
    {
        return NULL;
    }

    list->size = 0;
    list->head.next = list->head.pre = &list->head;

    return list;
}

// 创建链表节点
void *duple_list_node_new(int size)
{
    char *buff = (char *)malloc(DUPLE_LIST_NODE_SIZE + size + 1);
    if ( buff == NULL )
    {
        return NULL;
    }

    duple_list_node *node = (duple_list_node *)buff;
    node->next = node->pre = NULL;

    memset(buff + DUPLE_LIST_NODE_SIZE, 0, size + 1);

    return (buff + DUPLE_LIST_NODE_SIZE);
}

// 双向循环链表头插法
void duple_list_push_front(duple_list *list, void *data)
{
    duple_list_node *node = (duple_list_node *)(data - DUPLE_LIST_NODE_SIZE);

    node->pre = &list->head;
    node->next = list->head.next;

    list->head.next = node;
    node->next->pre = node;

    list->size++;
}

// 双向循环链表尾插法
void duple_list_push_back(duple_list *list, void *data)
{
    duple_list_node *node = (duple_list_node *)(data - DUPLE_LIST_NODE_SIZE);

    node->next = &list->head;
    node->pre = list->head.pre;

    list->head.pre = node;
    node->pre->next = node;

    list->size++;
}

// 获取头节点
void *duple_list_get_front(duple_list *list)
{
    if ( list->size == 0 )
    {
        return NULL;
    }
    return ((char *)list->head.next + DUPLE_LIST_NODE_SIZE);
}

// 获取尾节点
void *duple_list_get_back(duple_list *list)
{
    if ( list->size == 0 )
    {
        return NULL;
    }
    return ((char *)list->head.pre + DUPLE_LIST_NODE_SIZE);
}

// 获取下一个节点
void *duple_list_get_next(duple_list *list, void *data)
{
    duple_list_node *node = (duple_list_node *)(data - DUPLE_LIST_NODE_SIZE);
    if ( node->next == &list->head )
    {
        return NULL;
    }
    return ((char *)node->next + DUPLE_LIST_NODE_SIZE);
}

// 取出双向循环链表指定节点(从原链表中删除)
int duple_list_pop(duple_list *list, void *data)
{
    if ( list->size <= 0 )
    {
        return -1;
    }

    duple_list_node *node = (duple_list_node *)(data - DUPLE_LIST_NODE_SIZE);

    node->pre->next = node->next;
    node->next->pre = node->pre;

    list->size--;

    return 0;
}

// 取出双向循环链表首节点(从原链表中删除)
void *duple_list_pop_front(duple_list *list)
{
    if ( list->size == 0 )
    {
        return NULL;
    }

    duple_list_node *node = list->head.next;

    list->head.next = node->next;
    node->next->pre = &list->head;

    list->size--;

    return ((char *)node + DUPLE_LIST_NODE_SIZE);
}

// 取出双向循环链表尾节点(从原链表中删除)
void *duple_list_pop_back(duple_list *list)
{
    if ( list->size == 0 )
    {
        return NULL;
    }

    duple_list_node *node = list->head.pre;

    list->head.pre = node->pre;
    node->pre->next = &list->head;

    list->size--;

    return ((char *)node + DUPLE_LIST_NODE_SIZE);
}

// 双向循环链表删除节点
int duple_list_delete(duple_list *list, void *data)
{
    if ( list->size <= 0 )
    {
        return -1;
    }

    duple_list_node *del = (duple_list_node *)(data - DUPLE_LIST_NODE_SIZE);
    duple_list_node *move = list->head.next;
    while ( move != &list->head )
    {
        if ( move == del )
        {
            move->pre->next = move->next;
            move->next->pre = move->pre;
            free(move);
            list->size--;
            return 0;
        }
        move = move->next;
    }
    return -1;
}

// 释放单节点
void duple_list_node_free(void *data)
{
    duple_list_node *node = (duple_list_node *)(data - DUPLE_LIST_NODE_SIZE);
    free(node);
}

// 列表销毁
void duple_list_destory(duple_list *list)
{
    duple_list_node *move = list->head.next;
    duple_list_node *release = move;

    while ( move != &list->head )
    {
        release = move;
        move = move->next;
        free(release);
        list->size--;
    }

    free(list);
}


