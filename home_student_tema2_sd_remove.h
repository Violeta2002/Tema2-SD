// Copyright 2021-2022 Comsa Iuliana Violeta <comsa.iulianavioleta@gmail.com>
#ifndef HOME_STUDENT_TEMA2_SD_REMOVE_H_
#define HOME_STUDENT_TEMA2_SD_REMOVE_H_

#include "home_student_tema2_sd_hashtable.h"

void remove_book(hashtable_t **ht, char *params);

int ht_remove(hashtable_t *ht, char *key);

void remove_def(hashtable_t *ht, char *params);

#endif  // HOME_STUDENT_TEMA2_SD_REMOVE_H_
