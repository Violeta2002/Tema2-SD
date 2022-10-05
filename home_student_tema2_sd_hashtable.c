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

void
ht_put(hashtable_t *ht, void *key, unsigned int key_size,
	void *value, unsigned int value_size, int resize_number);

// functia de comparare a doua stringuri
int
compare_function_strings(void *a, void *b)
{
	char *str_a = (char *)a;
	char *str_b = (char *)b;

	return strcmp(str_a, str_b);
}

// functie ce calculeaza hashul unui string
unsigned int
hash_function_string(void *a)
{
	/*
	 * Credits: http://www.cse.yorku.ca/~oz/hash.html
	 */
	unsigned char *puchar_a = (unsigned char*) a;
	int64_t hash = 5381;
	int c;

	while ((c = *puchar_a++))
		hash = ((hash << 5u) + hash) + c; /* hash * 33 + c */

	return hash;
}

// functie ce creeaza un hashtable
hashtable_t *
ht_create(unsigned int hmax, unsigned int (*hash_function)(void*),
		int (*compare_function)(void*, void*))
{
	hashtable_t *ht = malloc(sizeof(hashtable_t));
    DIE(ht == NULL, "malloc failed");
	if( ht == NULL)
		return NULL;

    // initializez sizeul si hmax-ul cu datele intiale
	ht->size = 0;
	ht->hmax = hmax;
	ht->hash_function = *hash_function;
	ht->compare_function = *compare_function;

    // aloc si creez hmax liste simple inlantuite
	ht->buckets = malloc(hmax *sizeof(linked_list_t*));
    DIE(ht->buckets == NULL, "malloc failed");

	for(unsigned int i = 0; i < hmax; i++)
		ht->buckets[i] = ll_create(sizeof(info));

    ht->reachability = NULL;
    ht->rating = NULL;
    ht->borrowed = NULL;
    ht->purchases = NULL;

	return ht;
}

// intoarce nodul din hashtable ce are drept cheie key
void *
ht_get(hashtable_t *ht, void *key)
{
	int hash = ht->hash_function(key) % ht->hmax;
	ll_node_t *aux = ht->buckets[hash]->head;
	while(aux != NULL)
	{
		if (ht->compare_function(key, ((info *)aux->data)->key) == 0)
			return ((info *)aux->data)->key;
		aux = aux->next;
	}
	return NULL;
}

// intoarce valoarea nodului din hashtable ce are
// drept cheie key
void *
ht_get_value(hashtable_t *ht, void *key)
{
	int hash = ht->hash_function(key) % ht->hmax;
	ll_node_t *aux = ht->buckets[hash]->head;
	while(aux != NULL)
	{
		if (ht->compare_function(key, ((info *)aux->data)->key) == 0)
			return ((info *)aux->data)->value;
		aux = aux->next;
	}
	return NULL;
}

// verifica existenta unei cheie in hashtable
int
ht_has_key(hashtable_t *ht, void *key)
{
	return (ht_get(ht, key) != NULL);
}

// elibereaza din memorie un hashtable
void
ht_free(hashtable_t *ht)
{
    ll_node_t *aux;
    // parcurge fiecare lista din hashtable si
    // elibereaza nodurile
	for (unsigned int i = 0; i < ht->hmax; i++) {
		if (ht->buckets[i]->head != NULL) {
			aux = ht->buckets[i]->head;
			while (aux != NULL) {
				free(((info *)aux->data)->value);
				free(((info *)aux->data)->key);
				aux = aux->next;
			}
		}
        // elibereaza listele si apoi lista intreaga
		ll_free(&ht->buckets[i]);
	}

	free(ht->buckets);

	free(ht);
}

// adaugarea unuei perechi key-value in hashtable
void
ht_put(hashtable_t *ht, void *key, unsigned int key_size,
	void *value, unsigned int value_size, int resize_number)
{
    // calculez hash-ul cheii
	info data;
	int hash = ht->hash_function(key) % ht->hmax;
	linked_list_t *bucket = ht->buckets[hash];

	ll_node_t *node = bucket->head;
	for(unsigned int i = 0; i < bucket->size; i++) {
		if(ht->compare_function(key, ((info *)node->data)->key) != 0)
			node = node->next;
		else
			i = bucket->size;
	}

    // daca exista deja cheia, actualizeaza valoarea ei din hashtable
    if (ht_has_key(ht, key)) {
		ll_node_t *curr;
		curr = ht->buckets[hash]->head;
		while (ht->compare_function(((struct info *)(curr->data))->key, key)) {
			curr = curr->next;
		}
		memcpy(((char *)(((struct info *)(curr->data))->value)), value, value_size);
		return;
	}
    // daca nu exista deja cheia in hashtable, o adauga
    // pe pozitia 0 din lista
		data.key = malloc(key_size);
        DIE(data.key == NULL, "malloc failed");
		memcpy(data.key, key, key_size);

		data.value = malloc(value_size);
        DIE(data.value == NULL, "malloc failed");
		memcpy(data.value, value, value_size);

		ll_add_nth_node(bucket, 0, &data);

    // incrementez size-ul hashtable-ului dupa adaugare
	ht->size++;

    // daca numarul de carti adaugate este mai mare decat
    // capacitatea bibliotecii, se va face resize
    if (ht->size > ht->hmax) {
		ht_resize(&ht, resize_number);
	}
}

// functie ce sterge o intrare din hashtable
// ce are drept cheie "key"
void
ht_remove_entry(hashtable_t *ht, void *key)
{
    // calculeaza hash-ul lui key si parcurge listele
    // din hashtable
	int hash = ht->hash_function(key) % ht->hmax;
	linked_list_t *bucket = ht->buckets[hash];
	int pos;
	ll_node_t *node = bucket->head;
	for(unsigned int i = 0; i < bucket->size; i++) {
		if(ht->compare_function(key, ((info *)node->data)->key) == 0) {
			pos = i;
			break;
		}
		else
			node = node->next;
	}
    // daca exista cheia, atunci perechea key-value va fi stearsa din hashtable
	if(node) {
		ll_node_t *removed = ll_remove_nth_node(bucket, pos);
		free(((info *)removed->data)->value);
		free(((info *)removed->data)->key);
		free(removed->data);
		free(removed);
        // in urma stergerii, decrementez size-ul
		ht->size--;
	}
}

// intoarce size-ul hashtable-ului
unsigned int
ht_get_size(hashtable_t *ht)
{
	if (ht == NULL)
		return 0;

	return ht->size;
}

// intoarce hmax, capacitatea hashtable-ului
unsigned int
ht_get_hmax(hashtable_t *ht)
{
	if (ht == NULL)
		return 0;

	return ht->hmax;
}
