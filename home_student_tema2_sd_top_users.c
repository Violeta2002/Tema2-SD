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

// bubble sort ce sorteaza un array ce contine date de tip info (key, value)
// unde key reprezinta username, iar value punctele userului
void sort_users(info *users, int nr)
{
    for (int i = 0; i < nr; i++) {
        for (int j = 0; j < nr - 1; j++) {
            if (atof(((char *)users[j].value)) <
                        atof(((char *)users[j + 1].value))) {
                info aux = users[j];
                users[j] = users[j+1];
                users[j+1] = aux;
            } else if (atof(((char *)users[j].value)) ==
                        atof(((char *)users[j + 1].value))) {
                if (strcmp(users[j].key, users[j+1].key) > 0) {
                    info aux = users[j];
                    users[j] = users[j+1];
                    users[j+1] = aux;
                }
            }
        }
    }
}

void top_users(hashtable_t *ht)
{
    // creez vectorul de users prin parcurgerea fiecarei
    // liste din hashtable
    info *users = malloc(sizeof(info) * ht->hmax);
    DIE(users == NULL, "malloc failed");
    int i = 0;
    for (unsigned int j = 0; j < ht->hmax; j++) {
        ll_node_t *curr = ht->buckets[j]->head;
        while (curr != NULL) {
            users[i] = *((info *)curr->data);
            curr = curr->next;
            i++;
        }
    }
    // apelez functia de sortare pentru array users si nr de elemente i
    sort_users(users, i);
    for (int j = 0; j < i; j++) {
        if (atoi(((char *)users[j].value)) > 0) {
            printf("%d. Name:%s ", j + 1, ((char *)users[j].key));
            printf("Points:%s\n", ((char *)users[j].value));
        }
    }
    free(users);
}
