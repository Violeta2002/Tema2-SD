// Copyright 2021-2022 Comsa Iuliana Violeta <comsa.iulianavioleta@gmail.com>
#ifndef HOME_STUDENT_TEMA2_SD_HASHTABLE_H_
#define HOME_STUDENT_TEMA2_SD_HASHTABLE_H_

#include "home_student_tema2_sd_list.h"

// implementarea unui hashtable, realizata in cadrul
// laboratorului de hashtable-uri
typedef struct info info;
struct info {
	void *key;
	void *value;
};

typedef struct hashtable_t hashtable_t;
struct hashtable_t {
	linked_list_t **buckets;
	unsigned int size;  // Numarul de noduri din toate bucketurile
	unsigned int hmax;  // Numarul de bucketuri
	unsigned int (*hash_function)(void*);
	int (*compare_function)(void*, void*);
    void *reachability;  // retine cartile imprumutate si numarul de zile
    void *rating;  // retine cartea si rating-ul acesteia
    void *borrowed;  // retine usernameul si cartea imprumutata
    void *purchases;  // retine cartea si numarul de cumparari
};

int
compare_function_strings(void *a, void *b);

unsigned int
hash_function_string(void *a);

hashtable_t *
ht_create(unsigned int hmax, unsigned int (*hash_function)(void*),
		int (*compare_function)(void*, void*));

void *
ht_get(hashtable_t *ht, void *key);

void *
ht_get_value(hashtable_t *ht, void *key);

int
ht_has_key(hashtable_t *ht, void *key);

void
ht_free(hashtable_t *ht);

void
ht_put(hashtable_t *ht, void *key, unsigned int key_size,
	void *value, unsigned int value_size, int resize_number);

void
ht_remove_entry(hashtable_t *ht, void *key);

unsigned int
ht_get_size(hashtable_t *ht);

unsigned int
ht_get_hmax(hashtable_t *ht);

#endif  // HOME_STUDENT_TEMA2_SD_HASHTABLE_H_
