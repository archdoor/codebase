#include <stdio.h>
#include <string.h>
#include "duple_list.h"

typedef struct
{
    int a;
    char buff[100];
}MyData;

void print_data(MyData *data)
{
    printf("a:%d buff:%s\n", data->a, data->buff);
}

void print_list(duple_list *list)
{
    printf("num:%d\n", list->size);
    MyData *data = (MyData *)duple_list_get_front(list);
    while ( data != NULL )
    {
        print_data(data);
        data = (MyData *)duple_list_get_next(list, data); 
    }
}

int main()
{
    duple_list *list = duple_list_new();
    if ( list == NULL )
    {
        printf("get new list error\n");
        return -1;
    }

    MyData *data_0 = (MyData *)duple_list_node_new(sizeof(MyData));
    if ( data_0 == NULL )
    {
        printf("get new node error\n");
        return -1;
    }
    data_0->a = 1;
    strcpy(data_0->buff, "aaaa");
    duple_list_push_back(list, data_0);
    print_list(list);
    printf("\n");


    MyData *data_1 = (MyData *)duple_list_node_new(sizeof(MyData));
    if ( data_1 == NULL )
    {
        printf("get new node error\n");
        return -1;
    }
    data_1->a = 2;
    strcpy(data_1->buff, "bbbb");
    duple_list_push_back(list, data_1);
    print_list(list);
    printf("\n");

    MyData *data_2 = (MyData *)duple_list_node_new(sizeof(MyData));
    if ( data_2 == NULL )
    {
        printf("get new node error\n");
        return -1;
    }
    data_2->a = 3;
    strcpy(data_2->buff, "cccc");

    duple_list_node_free(data_2);
    print_list(list);
    printf("\n");


    duple_list_destory(list);


    return 0;
}




