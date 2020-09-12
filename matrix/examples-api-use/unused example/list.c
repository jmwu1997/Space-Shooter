#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "list.h"


void list_init(List *list,void(*destroy)(void *data)){
    list->size = 0;

    list->destroy = destroy;
    list->head = list->tail = NULL;
    return;
}


//在element元素后插入一个元素，
//如果element为空，则插入链表头部
//需要考虑链表为空的情况
int list_ins_next(List *list, ListElmt *element,const void *data){
    //创建新数据
    ListElmt *new_element;
    if((new_element = (ListElmt *)malloc(sizeof(ListElmt))) == NULL)
        return -1;
    new_element->data = (void *)data;
    new_element->next = NULL;
    //如果选定数据为空，插入到表头
    if(element == NULL){
        //如果列表为空，尾部也指向新元素
        if(list_size(list) == 0)
            list->tail = new_element;
        //在头链表前插入
        new_element->next = list->head;
        list->head = new_element;
    }else{
        //如果是最后一个元素
        if(element->next == NULL)
            list->tail = new_element;
        //在元素后插入
        new_element->next = element->next;
        element->next = new_element;
    }
    list->size ++;
    return 0;
}

//删除element后面的那个元素
//如果element为空,则删除链表头元素
int list_rem_next(List *list,ListElmt *element,void **data){

    //队列不为空
    if(list_size(list) == 0){
        return -1;
    }
    ListElmt *old_element;

    if(element == NULL){
        //头部删除一个元素
        *data = list->head->data;
        old_element = list->head;

        list->head = list->head->next;
        if(list_size(list) == 1) {
            list->tail = NULL;
        }
    }else{
        //尾部删除一个元素
        if(element->next == NULL){
            return -1;
        }
        *data = element->next->data;
        old_element = element->next;
        element->next = element->next->next;
        if(element->next == NULL){
            list->tail = element;
        }
    }
    free(old_element);
    list->size --;
    return 0;
}


//删除一个列表，删除列表里面存储的data
void list_destroy(List *list){
    void *data;
    while(list_size(list) > 0){
        if(list_rem_next(list,NULL,(void **) &data) == 0 && list->destroy != NULL){
            list->destroy(data);
        }
    }
    memset(list,0,sizeof(List));
    return;
}