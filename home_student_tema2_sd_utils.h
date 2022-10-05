// Copyright 2021-2022 echipa SD
#ifndef HOME_STUDENT_TEMA2_SD_UTILS_H_
#define HOME_STUDENT_TEMA2_SD_UTILS_H_

#include <errno.h>

#define DIE(assertion, call_description)				\
        do {								\
                if (assertion) {					\
                        fprintf(stderr, "(%s, %d): ",			\
                                        __FILE__, __LINE__);		\
                        perror(call_description);			\
                        exit(errno);					\
                }							\
        } while (0)

#endif  // HOME_STUDENT_TEMA2_SD_UTILS_H_
