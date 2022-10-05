// Copyright 2021-2022 Comsa Iuliana Violeta <comsa.iulianavioleta@gmail.com>
#ifndef HOME_STUDENT_TEMA2_SD_TOP_BOOKS_H_
#define HOME_STUDENT_TEMA2_SD_TOP_BOOKS_H_

// structura in care retin datele unei carti, necesare
// la sortarea acestora
typedef struct book_info book_info;
struct book_info {
    double rating;
    int purchases;
    char *book_name;
};

int cmp_func(const void *a, const void *b);

void top_books(hashtable_t *ht);

#endif  // HOME_STUDENT_TEMA2_SD_TOP_BOOKS_H_
