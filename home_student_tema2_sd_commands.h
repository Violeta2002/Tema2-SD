// Copyright 2021-2022 Comsa Iuliana Violeta <comsa.iulianavioleta@gmail.com>
#ifndef HOME_STUDENT_TEMA2_SD_COMMANDS_H_
#define HOME_STUDENT_TEMA2_SD_COMMANDS_H_

void borrow_book(hashtable_t **ht, hashtable_t *users, char *params);

void return_book(hashtable_t **ht, hashtable_t *users, char *params);

void lost_book(hashtable_t **ht, hashtable_t *users, char *params);

#endif  // HOME_STUDENT_TEMA2_SD_COMMANDS_H_
