// Copyright 2021-2022 Comsa Iuliana Violeta <comsa.iulianavioleta@gmail.com>
#ifndef HOME_STUDENT_TEMA2_SD_LIST_H_
#define HOME_STUDENT_TEMA2_SD_LIST_H_


#define MAX_STRING_SIZE	256
#define HMAX 10

typedef struct ll_node_t
{
    void* data;
    struct ll_node_t* next;
} ll_node_t;

typedef struct linked_list_t
{
    ll_node_t* head;
    unsigned int data_size;
    unsigned int size;
} linked_list_t;

linked_list_t *
ll_create(unsigned int data_size);

void
ll_add_nth_node(linked_list_t* list, unsigned int n, const void* new_data);

ll_node_t *
ll_remove_nth_node(linked_list_t* list, unsigned int n);

unsigned int
ll_get_size(linked_list_t* list);

void
ll_free(linked_list_t** pp_list);

#endif  // HOME_STUDENT_TEMA2_SD_LIST_H_
