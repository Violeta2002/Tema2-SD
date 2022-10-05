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

int main(void)
{
    // creez hashtable-ul bibliotecii
    hashtable_t *library = ht_create(HMAX, hash_function_string
                                                , compare_function_strings);
    // creez hashtable ce retine ratingul fiecarei carti
    library->rating = ht_create(HMAX, hash_function_string
                                                , compare_function_strings);
    // creez hashtable ce retine cartile imprumutate si numarul de zile
    library->reachability = ht_create(HMAX, hash_function_string
                                                , compare_function_strings);
    // creez hashtable ce retine username-ul si numeele cartii
    library->borrowed = ht_create(HMAX, hash_function_string
                                                , compare_function_strings);
    // creez hashtable ce retine numele cartii si nr de imprumuturi
    library->purchases = ht_create(HMAX, hash_function_string
                                                , compare_function_strings);
    // creez hashtable ce retine username-ul si punctele acestuia
    hashtable_t *users = ht_create(HMAX, hash_function_string
                                                , compare_function_strings);
    char *command = malloc(sizeof(char) * MAX_STRING_SIZE);
    DIE(!command, "malloc failed");

    char *params = malloc(sizeof(char) * MAX_STRING_SIZE);
    DIE(!params, "malloc failed");

    scanf("%s", command);
    while (strcmp(command, "EXIT") != 0) {
        if (strcmp(command, "ADD_BOOK") == 0) {
            fgets(params, MAX_STRING_SIZE, stdin);
            add_book(&library, params);
        } else if (strcmp(command, "GET_BOOK") == 0) {
            fgets(params, MAX_STRING_SIZE, stdin);
            get_book(library, params);
        } else if (strcmp(command, "RMV_BOOK") == 0) {
            fgets(params, MAX_STRING_SIZE, stdin);
            remove_book(&library, params);
        } else if (strcmp(command, "ADD_DEF") == 0) {
            fgets(params, MAX_STRING_SIZE, stdin);
            add_def(library, params);
        } else if (strcmp(command, "RMV_DEF") == 0) {
            fgets(params, MAX_STRING_SIZE, stdin);
            remove_def(library, params);
        } else if (strcmp(command, "ADD_USER") == 0) {
            fgets(params, MAX_STRING_SIZE, stdin);
            add_user(&users, params);
        } else if (strcmp(command, "GET_DEF") == 0) {
            fgets(params, MAX_STRING_SIZE, stdin);
            get_def(library, params);
        } else if (strcmp(command, "BORROW") ==0) {
            fgets(params, MAX_STRING_SIZE, stdin);
            borrow_book(&library, users, params);
        } else if (strcmp(command, "RETURN") == 0) {
            fgets(params, MAX_STRING_SIZE, stdin);
            return_book(&library, users, params);
        } else if (strcmp(command, "LOST") == 0) {
            fgets(params, MAX_STRING_SIZE, stdin);
            lost_book(&library, users, params);
        }
        // citesc comenzi cat timp e diferit de "EXIT"
        scanf("%s", command);
    }

// afisez topul cartilor si as userilor si eliberez memoria
printf("Books ranking:\n");
top_books(library);
printf("Users ranking:\n");
top_users(users);
ht_exit(&library);
ht_free(users);
free(command);
free(params);
return 0;
}
