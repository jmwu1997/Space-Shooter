//
//  set.c
//  set
//
//  Created by bikang on 16/9/18.
//  Copyright (c) 2016年 bikang. All rights reserved.
//
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "set.h"


typedef struct {
    int x,y,r,g,b;
} point;
typedef struct {
    point pixel_map[60];
    int pixel_number;
}object;
typedef struct {
    object obj;
    int hp;
    long id;
    int score;
}stars;
typedef struct {
    object obj;
    long id;
}bullet;

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

void print_list_int(List *list){
    if(list_size(list) == 0){
        printf("empty list\n");
        return;
    }

    ListElmt *pl;
    pl = list_head(list);
    int i;
    for(i=0;i<list_size(list);i++){
        printf("%ld\n",(*(stars*)pl->data).id);
        pl = pl->next;
    }
    printf("\n");
}

stars* access_elem_by_id(List* list,long id){
    if(list_size(list) == 0){
        return NULL;
    }
    ListElmt *pl;
    pl = list_head(list);
    int i;
    for(i=0;i<list_size(list);i++){
        if(id == (*(stars*)pl->data).id){
            return pl->data;
        }
        pl = pl->next;
    }
    return NULL;
}
int is_elem_in_list(List* list,long id){
    if(access_elem_by_id(list,id)!=NULL){
        return 1;
    }
    else return 0;
}
void remove_elem_by_id(List* list,long id){

    // Store head node 
    ListElmt* temp = list->head; 
    ListElmt* prev; 

    // If head node itself holds the key to be deleted 
    if (temp != NULL && (*(stars*)temp->data).id == id) 
    { 
        list->head = temp->next;   // Changed head 
        free(temp);              // free old head 
        list->size-=1;
        return; 
    } 
  
    // Search for the key to be deleted, keep track of the 
    // previous node as we need to change 'prev->next' 
    while (temp != NULL && (*(stars*)temp->data).id != id) 
    { 
        prev = temp; 
        temp = temp->next; 
    } 
  
    // If key was not present in linked list 
    if (temp == NULL) {
        
        return; 
    }
    // Unlink the node from linked list 
    prev->next = temp->next; 
    free(temp);  // Free memory 
    list->size-=1;
    return;
}


//------------------------------------------------
//初始化集合
// void set_init(Set *set,int(*match)(const void *k1,const void *k2),void(*destroy)(void*data)){
//     list_init(set,destroy);
//     set->match = match;
//     return;
// }

// //插入数据
// int set_insert(Set *set,const void *data){
//     if(set_is_member(set, data)){
//         return 1;
//     }
//     return list_ins_next(set, list_tail(set), data);
// }

// //删除数据
// int set_remove(Set *set,void **data){

//     ListElmt *item,*pre;
//     pre = NULL;
//     for(item = list_head(set);item != NULL;item = list_next(item)){
//         if(set->match(*data,list_data(item))) break;
//         pre = item;
//     }
//     if(item == NULL) return -1;
//     return list_rem_next(set, pre, data);
// }
// //并集
// int set_union(Set *setu,const Set *set1,const Set *set2){
//     ListElmt *item;
//     void *data;
//     set_init(setu, set1->match, NULL);
//     for(item = list_head(set1);item != NULL;item = list_next(item)){
//         data = list_data(item);
//         if(list_ins_next(setu, list_tail(setu), data) != 0){
//             set_destroy(setu);
//             return -1;
//         }
//     }

//     for(item = list_head(set2);item != NULL;item = list_next(item)){
//         data = list_data(item);
//         if(!set_is_member(setu, list_data(item))){
//             if(list_ins_next(setu, list_tail(setu), data) != 0){
//                 set_destroy(setu);
//                 return -1;
//             }
//         }
//     }
//     return 0;
// }

// //交集
// int set_intersection(Set *seti,const Set *set1,const Set *set2){
//     ListElmt *item;
//     void *data;
//     set_init(seti, set1->match, NULL);

//     for(item = list_head(set1);item != NULL;item = list_next(item)){
//         if(set_is_member(set2, list_data(item))){
//             data = list_data(item);
//             if(list_ins_next(seti, list_tail(seti), data) != 0){
//                 set_destroy(seti);
//                 return -1;
//             }
//         }
//     }
//     return 0;
// }

// //差集
// int set_difference(Set *setd,const Set *set1,const Set *set2){
//     ListElmt *item;
//     void *data;
//     set_init(setd, set1->match, NULL);

//     for(item = list_head(set1);item != NULL;item = list_next(item)){
//         if(!set_is_member(set2, list_data(item))){
//             data = list_data(item);
//             if(list_ins_next(setd, list_tail(setd), data) != 0){
//                 set_destroy(setd);
//                 return -1;
//             }
//         }
//     }
//     return 0;
// }


// //是否是他的成员
// int set_is_member(const Set *set,const void *data){
//     ListElmt *item;
//     for(item = list_head(set);item != NULL;item = list_next(item)){
//         if(set->match(data,list_data(item))) return 1;
//     }
//     return 0;
// }

// //set1是否是set2的子集
// int set_is_subset(const Set *set1,const Set *set2){
//     ListElmt *item;
//     // set1
//     if(set_size(set1) > set_size(set2)) return 0;
//     for(item = list_head(set1);item != NULL;item = list_next(item)){
//         if(!set_is_member(set2, list_data(item))) return 0;
//     }
//     return 1;
// }

// //是否相等
// int set_is_equal(const Set *set1,const Set *set2){
//     if(set_size(set1) != set_size(set2)) return 0;

//     return set_is_subset(set1, set2);
//     return 0;
// }

// void print_set(Set *set){
//     ListElmt *item;
//     if(set_size(set) == 0) {
//         puts("set is empty\n");
//         return;
//     }

//     for(item = list_head(set);item != NULL;item = list_next(item)){
//         printf("%d,",*(int*)list_data(item));
//     }
//     printf("\n");
//     return;
// }
// int match_data(const void *d1 ,const void *d2){
//     if(*(int*)d1 == *(int*)d2) return 1;
//     return 0;
// }
// void t_match(){
//     int i = 1; int *pi = &i;
//     int i2 = 2;int *pi2 = &i2;
//     printf("match_result:%d",match_data(pi, pi2));
// }