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

// afiseaza datele unei carti
void get_book(hashtable_t *ht, char *params)
{
    // preiau parametrii din comanda
    char *word = strtok(params, "\"\n");
    word = strtok(NULL, "\"\n");
    char *book_name = malloc(sizeof(char) * (strlen(word) + 1));
    DIE(book_name == NULL, "malloc failed");
    strcpy(book_name, word);

    // calculez hash-urile cartii din hashtable-urile
    // rating si purchases
    int hash1 = ((hashtable_t *)(ht->rating))->hash_function(book_name)
                                % ((hashtable_t *)(ht->rating))->hmax;
    int hash2 = ((hashtable_t *)(ht->purchases))->hash_function(book_name)
                                % ((hashtable_t *)(ht->purchases))->hmax;

	ll_node_t *aux = ((hashtable_t *)(ht->rating))->buckets[hash1]->head;
    ll_node_t *temp = ((hashtable_t *)(ht->purchases))->buckets[hash2]->head;
    // daca gaseste cartea in biblioteca printeaza datele acesteia
    if (ht_has_key(ht, book_name) == 1) {
        printf("Name:%s ", book_name);
	    while (aux != NULL) {
		if (ht->compare_function(book_name, ((info *)aux->data)->key) == 0)
            break;
		aux = aux->next;
	    }
        while (temp != NULL) {
            if (ht->compare_function(book_name
                                    , ((info *)temp->data)->key) == 0)
                break;
            temp = temp->next;
        }
        if (atoi(((char *)(((info *)temp->data)->value))) != 0) {
        printf("Rating:%.3f ", atof(((char *)(((info *)aux->data)->value)))
                            / atoi(((char *)(((info *)temp->data)->value))));
        } else {
            printf("Rating:%.3f ",
                            atof(((char *)(((info *)aux->data)->value))));
        }
        printf("Purchases:%s\n", ((char *)(((info *)temp->data)->value)));
    } else {
        // daca aceasta nu e in biblioteca, afiseaza un mesaj de eroare
        printf("The book is not in the library.\n");
    }
    free(book_name);
}

void get_def(hashtable_t *ht, char *params)
{
    // preiau parametrii din comanda
    int nr = 0;
    char *def_key = malloc(sizeof(char) * MAX_STRING_SIZE);
    DIE(def_key == NULL, "malloc failed");
    char *book_name = malloc(sizeof(char) * MAX_STRING_SIZE);
    DIE(book_name == NULL, "malloc failed");
    char *word = strtok(params, "\n\"");
    while (word != NULL) {
        nr++;
        if (nr == 2) {
            strcpy(book_name, word);
            word = strtok(NULL, "\n ");
        } else if (nr == 3) {
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
        if (ht->compare_function(book_name
                            , ((info *)node->data)->key) == 0) {
            i = bucket->size;
        }
        else
            node = node->next;
    }

    // creez un hashtable cu definitiile cartii
    hashtable_t *defs = ((info *)(node->data))->value;
    // verific existenta definitiei in carte
    if (ht_get(defs, def_key) == NULL)  {
        printf("The definition is not in the book.\n");
    } else {
        // daca exista definitia, printez valoarea acesteia in
        // cadrul cartii date ca parametru
       hash = defs->hash_function(def_key) % defs->hmax;
       bucket = defs->buckets[hash];
       node = bucket->head;
         for (unsigned int i = 0; i < bucket->size; i++) {
              if (defs->compare_function(def_key
                                    , ((info *)node->data)->key) == 0) {
                  printf("%s\n", ((char *)(((info *)node->data)->value)));
                  free(def_key);
                  free(book_name);
                  return;
              }
              else
                node = node->next;
         }
        }
    }
    free(def_key);
    free(book_name);
}
