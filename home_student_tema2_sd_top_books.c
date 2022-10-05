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

// functia de comparare necesara qsort-ului
int cmp_func(const void *a, const void *b)
{
    book_info *nr1 = (book_info *)(a);
    book_info *nr2 = (book_info *)(b);

    // compar raitingurile a doua carti si le sortez
    if (nr1->rating < nr2->rating) {
            return 1;
        // daca ratingurile sunt egale, sortez in functie de numarul
        // de imprumuturi
    } else if (nr1->rating == nr2->rating) {
        if (nr1->purchases < nr2->purchases) {
            return 1;
            // daca numarul de impurumuturi este egal, sortez
            // crescator in functie de numele cartilor
        } else if (nr1->purchases == nr2->purchases) {
            if (strcmp(nr1->book_name , nr2->book_name) > 0) {
               return 1;
            }
        } else {
            return -1;
        }
    }
    return 0;
}

void top_books(hashtable_t *ht)
{
    // creez array-ul de tip book_info in care retin
    // bookname-ul, raitingul si nr de imprumuturi
    book_info *books = malloc(sizeof(book_info) *
                    ((hashtable_t *)(ht->rating))->hmax);
    DIE(books == NULL, "malloc failed");
    int i = 0;
    for (unsigned int j = 0; j < ((hashtable_t *)(ht->rating))->hmax; j++) {
        ll_node_t *curr = ((hashtable_t *)(ht->rating))->buckets[j]->head;

        while (curr != NULL) {
            books[i].book_name = malloc(sizeof(char) * MAX_STRING_SIZE);
            DIE(books[i].book_name == NULL, "malloc failed");
            memcpy(books[i].book_name, (char *)((info *)(curr->data))->key
                            , strlen((char *)((info *)(curr->data))->key) + 1);
            books[i].purchases = atoi((char *)ht_get_value(ht->purchases
                                                        , books[i].book_name));
            if (books[i].purchases != 0) {
            books[i].rating = atof((char *)((info *)curr->data)->value)
                                                    / books[i].purchases;
            } else {
                books[i].rating = 0;
            }
            curr = curr->next;
            i++;
        }
    }

    // apelez functia de qsort pentru eficienta
    qsort(books, i, sizeof(book_info), cmp_func);

    int nr = 1;
    for (int j = 0; j < i; j++) {
    int hash1 = ((hashtable_t *)(ht->rating))->
    hash_function(books[j].book_name) % ((hashtable_t *)(ht->rating))->hmax;
    int hash2 = ((hashtable_t *)(ht->purchases))->
    hash_function(books[j].book_name) % ((hashtable_t *)(ht->purchases))->hmax;

    // parcurg hashtable-ul de raitinguri si pe cel de purchases
    // pentru a afisa datele corespunzatoare fiecarei carti
	ll_node_t *aux = ((hashtable_t *)(ht->rating))->buckets[hash1]->head;
    ll_node_t *temp = ((hashtable_t *)(ht->purchases))->buckets[hash2]->head;
    if (ht_get(ht, books[j].book_name) != NULL) {
        printf("%d. Name:%s ", nr, books[j].book_name);
	    while (aux != NULL) {
		if (ht->compare_function(books[j].book_name
                                , ((info *)aux->data)->key) == 0)
			break;
		    aux = aux->next;
	    }
        while (temp != NULL) {
            if (ht->compare_function(books[j].book_name
                                , ((info *)temp->data)->key) == 0)
                break;
            temp = temp->next;
        }
        printf("Rating:%.3f ", books[j].rating);
        printf("Purchases:%d\n", books[j].purchases);
        nr++;
    }
    }
    // eliberez memoria alocata array-ului
    for (int j = 0; j < i; j++) {
        free(books[j].book_name);
    }
    free(books);
}
