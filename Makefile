build: main.c home_student_tema2_sd_top_books.c home_student_tema2_sd_commands.c home_student_tema2_sd_get.c home_student_tema2_sd_exit.c home_student_tema2_sd_add.c home_student_tema2_sd_hashtable.c home_student_tema2_sd_list.c home_student_tema2_sd_top_users.c home_student_tema2_sd_remove.c
	gcc -Wall -Wextra -std=c99 -o main main.c home_student_tema2_sd_top_books.c home_student_tema2_sd_commands.c home_student_tema2_sd_get.c home_student_tema2_sd_exit.c home_student_tema2_sd_add.c home_student_tema2_sd_hashtable.c home_student_tema2_sd_list.c home_student_tema2_sd_top_users.c home_student_tema2_sd_remove.c -lm

clean:
	-rm -f main
	-rm -f *~

.PHONY: pack clean