// Copyright 2021-2022 Comsa Iuliana Violeta <comsa.iulianavioleta@gmail.com>
#ifndef HOME_STUDENT_TEMA2_SD_ADD_H_
#define HOME_STUDENT_TEMA2_SD_ADD_H_

void
ht_resize(hashtable_t **ht, int resize_number);

void add_def(hashtable_t *ht, char *params);

void add_user(hashtable_t **ht, char *params);

void add_book(hashtable_t **ht, char *params);

#endif  // HOME_STUDENT_TEMA2_SD_ADD_H_
