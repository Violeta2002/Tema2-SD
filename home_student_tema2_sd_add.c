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

// functia de resize ce dubleaza hmax-ul anterior
// pentru a putea introduce mai multe carti
void
ht_resize(hashtable_t **ht, int resize_number)
{
    // creeaza un nou hashtable ce are hmax * 2 intrari
	hashtable_t *new_ht = ht_create((*ht)->hmax * 2, (*ht)->hash_function
                                            , (*ht)->compare_function);

	for (unsigned int i = 0; i < (*ht)->hmax; i++) {
		ll_node_t *curr = (*ht)->buckets[i]->head;

    // copiez in noul hashtable, intrarile din cel vechi
    // astfel se recalculeaza hash-ul fiecare nod, fiind
    // asezat pe pozitia potrivita
		while (curr != NULL) {
			char *key = (char *)((struct info *)(curr->data))->key;
            // resize pentru book, ce contine un hashtable in interiorul
            // altui hashtable
            if (resize_number == 0) {
                info *value = ((struct info*)(curr->data))->value;
                hashtable_t * stct = ((hashtable_t*)(value));
                ht_put(new_ht, key, strlen(key) + 1, stct
                                    , sizeof(hashtable_t), 0);
            // resize pentru un hashtable
            } else if (resize_number == 1) {
                char *value = (char *)((struct info *)(curr->data))->value;
                ht_put(new_ht, key, strlen(key) + 1, value
                                                , strlen(value) + 1, 1);
            }
			curr = curr->next;
		}
	}

    // eliberarea nodurilor din vechiul hashtable
	for (unsigned int i = 0; i < (*ht)->hmax; i++) {
		ll_node_t *curr = (*ht)->buckets[i]->head;

		while (curr != NULL) {
            free(((struct info *)(curr->data))->key);
            free(((struct info *)(curr->data))->value);
            curr = curr->next;
		}

		ll_free(&(*ht)->buckets[i]); /* Adresa unei liste inlantuite */
	}

    // eliberez buckets-urile, dar nu si hashtable-ul
    // intruct vechile buckets-uri vor fi inlocuite cu cele noi
    // si se va acutaliza hmax
	free((*ht)->buckets);
	(*ht)->buckets = new_ht->buckets;
	(*ht)->hmax = new_ht->hmax;
	free(new_ht);
}

// adauga o carte in library
void add_book(hashtable_t **ht, char *params)
{
    // preia parametrii din comanda
    char *book_name = malloc(sizeof(char) * MAX_STRING_SIZE);
    DIE(book_name == NULL, "malloc failed");
    int def_nr;
    char *params_cpy = malloc(sizeof(char) * MAX_STRING_SIZE);
    DIE(params_cpy == NULL, "malloc failed");
    strcpy(params_cpy, params);
    char *word = strtok(params, "\"\n");
    word = strtok(NULL, "\"\n");
    strcpy(book_name, word);

    char *word_cpy = malloc(sizeof(char) * MAX_STRING_SIZE);
    DIE(word_cpy == NULL, "malloc failed");
    word = strtok(params_cpy, " \n");
    while (word != NULL) {
        strcpy(word_cpy, word);
        word = strtok(NULL, "\"\n");
    }

    def_nr = atoi(word_cpy);
    char *book_name_cpy = malloc(strlen(book_name) + 1);
    DIE(book_name_cpy == NULL, "malloc failed");
    strncpy(book_name_cpy, book_name, strlen(book_name) + 1);

    // creez un hashtable auxiliar in care sa stochez definitiile
    hashtable_t *aux = ht_create(HMAX, hash_function_string
                                , compare_function_strings);

    info *data = (info *)malloc(sizeof(info));

    char info_def[MAX_STRING_SIZE], info_val[MAX_STRING_SIZE];
    data->key = malloc(sizeof(char) * MAX_STRING_SIZE);
    DIE(data->key == NULL, "malloc failed");
    data->value = malloc(sizeof(char) * MAX_STRING_SIZE);
    DIE(data->value == NULL, "malloc failed");

    while (def_nr--) {
        // citesc def_nr definitii pe care le adaug in hashtable-ul aux
        scanf("%s %s", info_def, info_val);
        memcpy((char *)data->key, info_def, strlen(info_def) + 1);
        memcpy((char *)data->value, info_val, strlen(info_val) + 1);
        ht_remove(aux, data->key);
        ht_put(aux, data->key, strlen(info_def) + 1, data->value
                                        , strlen(info_val) + 1, 1);
    }

    // verific daca o carte a fost deja introdusa si rectualizez
    // valoare, dar o si sterg din celelalte hashatable-uri care
    // retin ratingul, nr de imprumuri si disponibilitatea
    if (ht_get((*ht), book_name) != NULL) {
        ht_remove((*ht)->reachability, book_name);
    }
    ht_free_book(ht, book_name_cpy);

    // adaug cartea noua in biblioteca
    ht_put(*ht, book_name, strlen(book_name) + 1, (hashtable_t *)aux
                                        , sizeof(hashtable_t), 0);

    // initializez raitingul cartii cu 0
    ht_put((*ht)->rating, book_name, strlen(book_name) + 1, "0"
                                                , strlen("0") + 1, 1);
    // initializez numarul de imprumuturi cu 0
    ht_put((*ht)->purchases, book_name, strlen(book_name) + 1, "0"
                                                , strlen("0") + 1, 1);

    free(data->key);
    free(data->value);
    free(data);
    free(book_name);
    free(word_cpy);
    free(params_cpy);
    free(aux);
}

int ht_remove(hashtable_t *ht, char *key);
void add_def(hashtable_t *ht, char *params)
{
    // preiau parametrii din comanda
    int nr = 0;
    char *def_val, *def_key, *book_name;
    char *word = strtok(params, "\n\"");
    while (word != NULL) {
        nr++;
        if (nr == 2) {
            book_name = malloc(sizeof(char) * (strlen(word) + 1));
            strcpy(book_name, word);
             word = strtok(NULL, "\" \n");
        } else if (nr == 3) {
           def_key = malloc(sizeof(char) * (strlen(word) + 1));
           strcpy(def_key, word);
           word = strtok(NULL, "\n ");
        } else if (nr == 4) {
            def_val = malloc(sizeof(char) * (strlen(word) + 1));
            strcpy(def_val, word);
            word = strtok(NULL, "\n ");
        } else {
            word = strtok(NULL, "\"\n");
        }
    }

    // verific existenta cartii in librarie si
    // afisez un mesaj de eroare daca nu exisa
    if (ht_get(ht, book_name) == NULL) {
        printf("The book is not in the library.\n");
    } else {
        // daca exista, actualizez hashtable-ul de definitii
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

        hashtable_t *defs = ((info *)(node->data))->value;

        ht_remove(defs, def_key);
        ht_put(defs, def_key, strlen(def_key) + 1, def_val
                                , strlen(def_val) + 1, 1);
    }
    free(book_name);
    free(def_val);
    free(def_key);
}

// functie ce adauga un user intr-un hastable de useri
void add_user(hashtable_t **ht, char *params)
{
    // preia numele user-ului din comanda
    char *user_name = malloc(sizeof(char) * MAX_STRING_SIZE);
    DIE(user_name == NULL, "malloc failed");
    char *word = strtok(params, "\n ");
    strcpy(user_name, word);

    // verifica daca acesta a mai fost introdus in hashtable
    if (ht_has_key((*ht), user_name) == 1)
        printf("User is already registered.\n");
    else
    // daca nu a fost introdus, il introduce si ii initializeaza
    // numarul de puncte cu 100
         ht_put((*ht), user_name, strlen(user_name) +1, "100"
                                    , sizeof("100") + 1, 1);

    free(user_name);
}
