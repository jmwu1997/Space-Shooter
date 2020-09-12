#ifndef LIST_H 
#define LIST_H
#include <stdlib.h>

//定义基础的结构
typedef struct ListElmt_{
    void *data;
    struct ListElmt_ *next;
}ListElmt;

//定义列表
typedef struct list_{
    int size;//大小
    int (*match)(const void *key1,const void *key2);//比较,匹配
    void (*destroy)(void *data);//内存释放函数
    ListElmt *head;//头
    ListElmt *tail;//尾部
}List;

//初始化
void list_init(List *list,void(*destroy)(void *data));
//在element元素后插入一个元素，如果element为空，则插入链表头部
int list_ins_next(List *list, ListElmt *element,const void *data);
//删除element后面的那个元素，如果element为空,则删除链表头元素
int list_rem_next(List *list,ListElmt *element,void **data);
//删除一个列表，删除列表里面存储的data
void list_destroy(List *list);


//定义基础的宏，获取各种基础的数据：获取大小，头，尾部，判断是头，是尾，数据，下一个元素
#define list_size(list) ((list)->size)
#define list_head(list) ((list)->head)
#define list_tail(list) ((list)->tail)
#define list_is_head(list,element) ((element) == (list)->head?1:0)
#define list_is_tail(element) ((element)->next == NULL?1:0)
#define list_data(element)((element)->data)
#define list_next(element)((element)->next)

#endif