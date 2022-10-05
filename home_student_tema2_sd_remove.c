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

void remove_book(hashtable_t **ht, char *params)
{
    // preiau parametrii din comanda
    char *book_name = malloc(sizeof(char) * MAX_STRING_SIZE);
    DIE(book_name == NULL, "malloc failed");
    char *word = strtok(params, "\"\n");
    word = strtok(NULL, "\"\n");
    strcpy(book_name, word);
    if (ht_has_key((*ht), book_name) == 0) {
        printf("The book is not in the library.\n");
        free(book_name);
        return;
    }
    // calculeaza hash-ul cartii si parcurge fiecare lista
    // din hashtable pana gaseste cartea
    int hash = (*ht)->hash_function(book_name) % (*ht)->hmax;
	linked_list_t *bucket = (*ht)->buckets[hash];
	int pos;
	ll_node_t *node = bucket->head;
	for (unsigned int i = 0; i < bucket->size; i++) {
		if ((*ht)->compare_function(book_name
                        , ((info *)node->data)->key) == 0) {
			pos = i;
			break;
		}
		else
			node = node->next;
	}
    // dupa ce gaseste cartea, o sterge din hashtable
	if (node) {
		ll_node_t *removed = ll_remove_nth_node(bucket, pos);
		ht_free((hashtable_t *)(((info *)removed->data)->value));
		free(((info *)removed->data)->key);
		free(removed->data);
		free(removed);
		(*ht)->size--;
	}
    free(book_name);
}

// functie ce sterge o cheie dintr-un hashatable anume
int ht_remove(hashtable_t *ht, char *key)
{
    int hash = ht->hash_function(key) % ht->hmax;
    linked_list_t *bucket = ht->buckets[hash];
    int pos;
    ll_node_t *node = bucket->head;
    for (unsigned int i = 0; i < bucket->size; i++) {
        if (ht->compare_function(key, ((info *)node->data)->key) == 0) {
            pos = i;
            break;
        }
        else
            node = node->next;
    }
    if (node) {
        ll_node_t *removed = ll_remove_nth_node(bucket, pos);
        free(((info *)removed->data)->value);
        free(((info *)removed->data)->key);
        free(removed->data);
        free(removed);
        ht->size--;
    }
    return 0;
}

void remove_def(hashtable_t *ht, char *params)
{
    // preiau parametrii din comanda
    int nr = 0;
    char *def_key, *book_name;
    char *word = strtok(params, "\n\"");
    while (word != NULL) {
        nr++;
        if (nr == 2) {
            book_name = malloc(sizeof(char) * (strlen(word) + 1));
            DIE(book_name == NULL, "malloc failed");
            strcpy(book_name, word);
            word = strtok(NULL, "\n ");
        } else if (nr == 3) {
            def_key = malloc(sizeof(char) * (strlen(word) + 1));
            DIE(def_key == NULL, "malloc failed");
            strcpy(def_key, word);
            word = strtok(NULL, "\n ");
        } else {
        word = strtok(NULL, "\n\"");
        }
    }

    // verific existenta cartii in biblioteca
    if (ht_get(ht, book_name) == NULL) {
        printf("The book is not in the library.\n");
    } else {
    int hash = ht->hash_function(book_name) % ht->hmax;
    linked_list_t *bucket = ht->buckets[hash];
    ll_node_t *node = bucket->head;
    for (unsigned int i = 0; i < bucket->size; i++) {
        if (ht->compare_function(book_name, ((info *)node->data)->key) == 0) {
            i = bucket->size;
        }
        else
            node = node->next;
    }

    // preiau hashtable-ul de definitii
    hashtable_t *defs = ((info *)(node->data))->value;
    // verific existenta definitiei in cartea daca ca parametru
    if (ht_get(defs, def_key) == NULL)  {
        printf("The definition is not in the book.\n");
    } else {
        // parcurg hashtable-ul pana gasesc definitia
        int hash = ht->hash_function(book_name) % ht->hmax;
        linked_list_t *bucket = ht->buckets[hash];

        ll_node_t *node = bucket->head;
        for (unsigned int i = 0; i < bucket->size; i++) {
            if (ht->compare_function(book_name
                        , ((info *)node->data)->key) != 0)
                node = node->next;
            else
                i = bucket->size;
        }

        // cand gasesc definitia, o sterg din hashtable-ul de definitii
        hashtable_t *defs = ((info *)(node->data))->value;
        ht_remove(defs, def_key);
        }
    }
    free(book_name);
    free(def_key);
}
