// Copyright 2021-2022 Comsa Iuliana Violeta <comsa.iulianavioleta@gmail.com>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "home_student_tema2_sd_hashtable.h"
#include "home_student_tema2_sd_add.h"
#include "home_student_tema2_sd_remove.h"
#include "home_student_tema2_sd_exit.h"
#include "home_student_tema2_sd_utils.h"
#include "home_student_tema2_sd_get.h"
#include "home_student_tema2_sd_commands.h"
#include "home_student_tema2_sd_top_books.h"
#include "home_student_tema2_sd_top_users.h"
#include "home_student_tema2_sd_list.h"

// implementarea unei liste, preluata din laborator

// crearea unei liste
linked_list_t *
ll_create(unsigned int data_size)
{
    linked_list_t* ll;

    ll = malloc(sizeof(*ll));

    ll->head = NULL;
    ll->data_size = data_size;
    ll->size = 0;

    return ll;
}

// adaugarea pe o pozitie din lista
void
ll_add_nth_node(linked_list_t* list, unsigned int n, const void* new_data)
{
    ll_node_t *prev, *curr;
    ll_node_t* new_node;

    if (!list) {
        return;
    }

    if (n > list->size) {
        n = list->size;
    }

    curr = list->head;
    prev = NULL;
    while (n > 0) {
        prev = curr;
        curr = curr->next;
        --n;
    }

    new_node = malloc(sizeof(*new_node));
    DIE(new_node == NULL, "malloc failed");

    new_node->data = malloc(list->data_size);
    DIE(new_node->data == NULL, "malloc failed");
    memcpy(new_node->data, new_data, list->data_size);

    new_node->next = curr;
    if (prev == NULL) {
        list->head = new_node;
    } else {
        prev->next = new_node;
    }

    list->size++;
}

// stergerea unui nod de pe o anumita pozitie in lista
ll_node_t *
ll_remove_nth_node(linked_list_t* list, unsigned int n)
{
    ll_node_t *prev, *curr;

    if (!list || !list->head) {
        return NULL;
    }

    if (n > list->size - 1) {
        n = list->size - 1;
    }

    curr = list->head;
    prev = NULL;
    while (n > 0) {
        prev = curr;
        curr = curr->next;
        --n;
    }

    if (prev == NULL) {
        list->head = curr->next;
    } else {
        prev->next = curr->next;
    }

    list->size--;

    return curr;
}

// size-ul listei
unsigned int
ll_get_size(linked_list_t* list)
{
     if (!list) {
        return -1;
    }

    return list->size;
}

// eliberarea din memorie a unei liste
void
ll_free(linked_list_t** pp_list)
{
    ll_node_t* currNode;

    if (!pp_list || !*pp_list) {
        return;
    }

    while (ll_get_size(*pp_list) > 0) {
        currNode = ll_remove_nth_node(*pp_list, 0);
        free(currNode->data);
        currNode->data = NULL;
        free(currNode);
        currNode = NULL;
    }

    free(*pp_list);
    *pp_list = NULL;
}
