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

void borrow_book(hashtable_t **ht, hashtable_t *users, char *params)
{
    // preiau parametrii din comanda
    char *user_name = malloc(sizeof(char) * MAX_STRING_SIZE);
    DIE(user_name == NULL, "malloc failed");
    char *book_name = malloc(sizeof(char) * MAX_STRING_SIZE);
    DIE(book_name == NULL, "malloc failed");
    char *days = malloc(sizeof(char) * MAX_STRING_SIZE);
    DIE(days == NULL, "malloc failed");
    char *word = strtok(params, " \n\"");
    strcpy(user_name, word);
    word = strtok(NULL, "\"\n");
    strcpy(book_name, word);
    word = strtok(NULL, " \n");
    strcpy(days, word);

    // verific daca user-ul exista in baza de date
    if (ht_get(users, user_name) == NULL) {
        printf("You are not registered yet.\n");
    } else {
    int hash = users->hash_function(user_name) % users->hmax;
	ll_node_t *aux = users->buckets[hash]->head;
	while (aux != NULL)
	{
		if (users->compare_function(user_name, ((info *)aux->data)->key) == 0)
			break;
		aux = aux->next;
	}

    // verific daca userul are un numar negativ de puncte, adica este banat
    if (atoi(((char *)((info *)(aux->data))->value)) < 0) {
            printf("You are banned from this library.\n");
            free(user_name);
            free(book_name);
            free(days);
            return;
    } else {
    // verific daca utilizatorul a imprumutat deja o carte sau nu
    if (ht_get((*ht)->borrowed, user_name) != NULL) {
        printf("You have already borrowed a book.\n");
    // verific daca exista sau nu cartea in biblioteca
    } else if (ht_get((*ht), book_name) == NULL) {
        printf("The book is not in the library.\n");
    // verific daca este imprumutata cartea
    } else if (ht_get((*ht)->reachability, book_name) != NULL) {
        printf("The book is borrowed.\n");
    } else {
    // daca nu e imprumututata o adaug in hashtable-ul borrowed ce are drept
    // cheie usernameul si in hashtable-ul reachability ce are drept cheie
    // bookname-ul
    ht_put((hashtable_t*)((*ht)->borrowed), user_name, strlen(user_name) + 1
                                        , book_name, strlen(book_name) + 1, 1);
    ht_put((hashtable_t *)((*ht)->reachability), book_name
                        , strlen(book_name) + 1, days, strlen(days) + 1, 1);
    }
    }
}
    free(user_name);
    free(book_name);
    free(days);
}

void return_book(hashtable_t **ht, hashtable_t *users, char *params)
{
    // preiau parametrii din comanda
    char *user_name = malloc(sizeof(char) * MAX_STRING_SIZE);
    char *book_name = malloc(sizeof(char) * MAX_STRING_SIZE);
    char *days = malloc(sizeof(char) * MAX_STRING_SIZE);
    char *rating = malloc(sizeof(char) * MAX_STRING_SIZE);
    char *word = strtok(params, " \n\"");
    strcpy(user_name, word);
    word = strtok(NULL, "\"\n");
    strcpy(book_name, word);
    word = strtok(NULL, " \n");
    strcpy(days, word);
    word = strtok(NULL, " \n");
    strcpy(rating, word);
    // verific aparitia eventualelor mesaje de eroare
    if  (ht_get_value(users, user_name) != NULL &&
                atoi(((char *)(ht_get_value(users, user_name)))) < 0) {
        printf("You are banned from this library.\n");
    } else if (ht_get((*ht)->borrowed, user_name) != NULL &&
    strcmp((char *)(ht_get_value((*ht)->borrowed, user_name))
                                        , book_name) != 0) {
        printf("You didn't borrow this book.\n");
    } else if (ht_get((*ht)->borrowed, user_name) == NULL) {
        printf("You didn't borrow this book.\n");
    } else {
        // calculez punctajul utilizatorului
        char *days_borrowed = (char *)ht_get_value((*ht)->reachability
                                                        , book_name);
        int diff = atoi(days_borrowed) - atoi(days);
        int hash = users->hash_function(user_name) % users->hmax;
	        ll_node_t *aux = users->buckets[hash]->head;
	        while (aux != NULL)
	        {
		        if (users->compare_function(user_name
                                    , ((info *)aux->data)->key) == 0)
			    break;
		    aux = aux->next;
	        }
        if (diff > 0) {
            int points = atoi((char *)((info *)(aux->data))->value) + diff;
            char *points_str = malloc(sizeof(char) * MAX_STRING_SIZE);
            sprintf(points_str, "%d", points);
            ht_remove(users, user_name);
            ht_put(users, user_name, strlen(user_name) + 1, points_str,
                                            strlen(points_str) + 1, 1);
            free(points_str);
        } else if (diff < 0) {
            int points = atoi((char *)((info *)(aux->data))->value)
                                                        + (2 * diff);
            char *points_str = malloc(sizeof(char) * MAX_STRING_SIZE);
            sprintf(points_str, "%d", points);
            ht_remove(users, user_name);
            ht_put(users, user_name, strlen(user_name) + 1, points_str
                                                , strlen(points_str) + 1, 1);
            free(points_str);
            if (points < 0){
                printf("The user %s has been banned from this library.\n"
                                                                , user_name);
            }
        }
        // calculez noul raiting al cartii
        double old_rating = atof(ht_get_value((*ht)->rating, book_name));
        double new_rating = (old_rating + atof(rating));
        char *new_rating_str = malloc(sizeof(char) * MAX_STRING_SIZE);
        sprintf(new_rating_str, "%.3f", new_rating);
        ht_remove((*ht)->rating, book_name);
        ht_put((*ht)->rating, book_name, strlen(book_name) + 1, new_rating_str
                                            , strlen(new_rating_str) + 1, 1);
        free(new_rating_str);

        // incrementez numarul de imprumuturi ale cartii
        int purchase = atoi(ht_get_value((*ht)->purchases, book_name)) + 1;
        char *purchase_str = malloc(sizeof(char) * MAX_STRING_SIZE);
        sprintf(purchase_str, "%d", purchase);
        ht_remove((*ht)->purchases, book_name);
        ht_put((*ht)->purchases, book_name, strlen(book_name) + 1, purchase_str
                                            , strlen(purchase_str) + 1, 1);
        free(purchase_str);

        // returnez cartea, adica sterg din hashtable-urile borrowed
        // si reachability cartea
        ht_remove((hashtable_t *)((*ht)->borrowed), user_name);
        ht_remove((hashtable_t *)((*ht)->reachability), book_name);
    }
    free(user_name);
    free(book_name);
    free(days);
    free(rating);
}

void lost_book(hashtable_t **ht, hashtable_t *users, char *params)
{
    // preiau parametrii din comanda
    char *user_name = malloc(sizeof(char) * MAX_STRING_SIZE);
    DIE(user_name == NULL, "malloc failed");
    char *book_name = malloc(sizeof(char) * MAX_STRING_SIZE);
    DIE(book_name == NULL, "malloc failed");
    char *word = strtok(params, " \n\"");
    strcpy(user_name, word);
    word = strtok(NULL, "\"\n");
    strcpy(book_name, word);

    // verific aparitia eventualelor mesaje de eroare
    if (ht_get(users, user_name) == NULL) {
        printf("You are not registered yet.\n");
    } else if (ht_get_value(users, user_name) != NULL &&
                atoi(((char *)(ht_get_value(users, user_name)))) < 0) {
        printf("You are banned from this library.\n");
    } else {
    // cartea este pierduta, deci este stearsa din baza de date
    // adica din toate hashtable-urile care o contin
    ht_remove((*ht)->reachability, book_name);
    ht_remove((*ht)->rating, book_name);
    ht_remove((*ht)->purchases, book_name);
    ht_remove((*ht)->borrowed, user_name);

    // sterg cartea din library
    int hash = (*ht)->hash_function(book_name) % (*ht)->hmax;
	linked_list_t *bucket = (*ht)->buckets[hash];
	int pos;
	ll_node_t *node = bucket->head;
	for (unsigned int i = 0; i < bucket->size; i++) {
		if ((*ht)->compare_function(book_name, ((info *)node->data)->key) == 0) {
			pos = i;
			break;
		}
		else
			node = node->next;
	}
	if (node) {
		ll_node_t *removed = ll_remove_nth_node(bucket, pos);
		ht_free((hashtable_t *)(((info *)removed->data)->value));
		free(((info *)removed->data)->key);
		free(removed->data);
		free(removed);
		(*ht)->size--;
	}

    hash = users->hash_function(user_name) % users->hmax;
	ll_node_t *aux = users->buckets[hash]->head;
	while(aux != NULL)
	{
		if (users->compare_function(user_name, ((info *)aux->data)->key) == 0)
			break;
		aux = aux->next;
	}
    // calculez punctele user-ului
    int points = atoi((char *)((info *)(aux->data))->value) - 50;
    char *points_str = malloc(sizeof(char) * MAX_STRING_SIZE);
    DIE(points_str == NULL, "malloc failed");
    sprintf(points_str, "%d", points);
    ht_remove(users, user_name);
    ht_put(users, user_name, strlen(user_name) + 1, points_str
                                        , strlen(points_str) + 1, 1);
    free(points_str);
    // daca in urma pierderii userul are un numar negativ de puncte,
    // acesta este banat
    if (points < 0) {
        printf("The user %s has been banned from this library.\n", user_name);
    }
    }
    free(user_name);
    free(book_name);
}
