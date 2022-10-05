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

// eliberez memoria ocupata de o carte
void ht_free_book(hashtable_t **ht, char *book_name)
{
    // calculez hash-ul cartii din parcurg fiecare lista
    // din hashtable pana gasesc cartea si o sterg din biblioteca
    int hash = (*ht)->hash_function(book_name) % (*ht)->hmax;
	linked_list_t *bucket = (*ht)->buckets[hash];
	int pos;
	ll_node_t *node = bucket->head;
	for(unsigned int i = 0; i < bucket->size; i++) {
		if((*ht)->compare_function(book_name, ((info *)node->data)->key) == 0) {
			pos = i;
			break;
		}
		else
			node = node->next;
	}
	if(node) {
		ll_node_t *removed = ll_remove_nth_node(bucket, pos);
		ht_free((hashtable_t *)(((info *)removed->data)->value));
		free(((info *)removed->data)->key);
		free(removed->data);
		free(removed);
		(*ht)->size--;
	}
    free(book_name);
}

void ht_exit(hashtable_t **ht)
{
    ll_node_t *aux;
    for (unsigned int i = 0; i < (*ht)->hmax; i++) {
        if ((*ht)->buckets[i]->head != NULL) {
            aux = (*ht)->buckets[i]->head;
            while (aux != NULL) {
                free(((info *)aux->data)->key);
                ht_free(((hashtable_t *)((info *)(aux->data))->value));
                aux = aux->next;
            }
        }
        ll_free(&(*ht)->buckets[i]);
    }
    free((*ht)->buckets);
    ht_free((*ht)->borrowed);
    ht_free((*ht)->reachability);
    ht_free((*ht)->rating);
    ht_free((*ht)->purchases);
    free(*ht);
}
