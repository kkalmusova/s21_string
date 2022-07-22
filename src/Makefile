# Makefile
CC 		 = gcc
FLAG_C   = -c
CC_REM   = rm -rf

FLAG_O_S   =  -fsanitize=address -g -o
FLAG_ER_S  = -Wall -Wextra -Werror -std=c11 -fsanitize=address -g
FLAG_O   =  -g -o
FLAG_ER  = -Wall -Wextra -Werror -g

FLAG_COV = -lcheck -lgcov -fprofile-arcs --coverage
ifeq ($(shell uname), Linux)
FLAG_COV += -pthread -lrt -lm -lsubunit
endif
FLAG_GCOV = -fprofile-arcs -ftest-coverage


S21_LIB_SOURCE	= ./s21_memchr.c ./s21_memcmp.c ./s21_memcpy.c ./s21_memmove.c ./s21_memset.c ./s21_strcat.c ./s21_strncat.c ./s21_strchr.c ./s21_strcmp.c ./s21_strcpy.c ./s21_strerror.c ./s21_strlen.c ./s21_strspn.c ./s21_strncmp.c ./s21_strncpy.c ./s21_to_upper.c ./s21_to_lower.c ./s21_strcspn.c ./s21_strrchr.c ./s21_strstr.c ./s21_strtok.c ./s21_insert.c ./s21_trim.c ./s21_strpbrk.c ./s21_sprintf.c
S21_LIB_OBJECT	= s21_memchr.o s21_memcmp.o s21_memcpy.o s21_memmove.o s21_memset.o s21_strcat.o s21_strncat.o s21_strchr.o s21_strcmp.o s21_strcpy.o s21_strerror.o s21_strlen.o s21_strspn.o s21_strncmp.o s21_strncpy.o s21_to_upper.o s21_to_lower.o s21_strcspn.o s21_strrchr.o s21_strstr.o s21_strtok.o s21_insert.o s21_trim.o s21_strpbrk.o s21_sprintf.o
S21_STRING_TEST = ./libs/s21_string_test.c

all:
	make rebuild

s21_string_test.o:
	$(CC) $(FLAG_C) $(S21_STRING_TEST)

s21_string.o: $(S21_LIB_SOURCE)
	$(CC) $(FLAG_C) $(S21_LIB_SOURCE)

s21_string.a: s21_string.o
	ar rcs s21_string.a $(S21_LIB_OBJECT)
	ranlib s21_string.a
	$(CC_REM) *.o

s21_string_1.a: s21_string.o
	ar rcs s21_string.a $(S21_LIB_OBJECT)
	ranlib s21_string.a

test: s21_string_test.o s21_string_1.a
	$(CC) $(FLAG_O) s21_string_test s21_string_test.o s21_string.a $(FLAG_COV) $(FLAG_GCOV)
	./s21_string_test > _out.txt
	$(CC_REM) *.o *.gcno *.gcda *.gcov *.info ./*.dSYM

test_v: s21_string_1.a
	$(CC) $(FLAG_C) ./libs/additional_tests/test_v.c
	$(CC) $(FLAG_O) s21_string_test test_v.o s21_string.a $(FLAG_COV) $(FLAG_GCOV)
	./s21_string_test > _out.txt
	$(CC_REM) *.o *.gcno *.gcda *.gcov *.info ./*.dSYM

test_m: s21_string_1.a
	$(CC) $(FLAG_C) ./libs/additional_tests/test_m.c
	$(CC) $(FLAG_O) s21_string_test test_m.o s21_string.a $(FLAG_COV) $(FLAG_GCOV)
	./s21_string_test > _out.txt
	$(CC_REM) *.o *.gcno *.gcda *.gcov *.info ./*.dSYM

test_p: s21_string_1.a
	$(CC) $(FLAG_C) ./libs/additional_tests/test_p.c
	$(CC) $(FLAG_O) s21_string_test test_p.o s21_string.a $(FLAG_COV) $(FLAG_GCOV)
	./s21_string_test > _out.txt
	$(CC_REM) *.o *.gcno *.gcda *.gcov *.info ./*.dSYM

test_z: s21_string_1.a
	$(CC) $(FLAG_C) ./libs/additional_tests/test_z.c
	$(CC) $(FLAG_O) s21_string_test test_z.o s21_string.a $(FLAG_COV) $(FLAG_GCOV)
	./s21_string_test > _out.txt
	$(CC_REM) *.o *.gcno *.gcda *.gcov *.info ./*.dSYM

gcov_report: s21_string_1.a
	$(CC) $(FLAG_O) s21_string_test -lcheck $(S21_LIB_SOURCE) $(S21_STRING_TEST) $(FLAG_COV)
	./s21_string_test
	lcov -t "s21_string_test" -o s21_string_test.info -c -d .
	genhtml -o gcov_report s21_string_test.info
	$(CC_REM) *.o *.gcno *.gcda *.gcov *.info ./*.dSYM

html:
	OPEN ./gcov_report/index.html

lint:
	cp ../materials/linters/CPPLINT.cfg ./
	cp ../materials/linters/CPPLINT.cfg ./libs/
	python3 ../materials/linters/cpplint.py --extensions=c *.h *.c
	python3 ../materials/linters/cpplint.py --extensions=h *.h
	python3 ../materials/linters/cpplint.py --extensions=c ./libs/*.c
	$(CC_REM) ./CPPLINT.cfg
	$(CC_REM) ./libs/CPPLINT.cfg
	$(CC_REM) *.gcda

leaks:
	leaks -atExit -- ./s21_string_test
	$(CC_REM) *.gcda

cppcheck:
	cppcheck --enable=all --suppress=missingIncludeSystem *.c ./libs/*.c

valgrind:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind-out.txt ./s21_string_test > _out.txt

asan:
	$(CC) $(FLAG_C) $(FLAG_ER_S) $(S21_LIB_SOURCE)
	ar rcs s21_string.a $(S21_LIB_OBJECT)
	ranlib s21_string.a
	$(CC) $(FLAG_C) $(S21_STRING_TEST)
	$(CC) $(FLAG_O_S) s21_string_test s21_string_test.o s21_string.a $(FLAG_COV) $(FLAG_GCOV)
	./s21_string_test > _out.txt
	$(CC_REM) *.o *.gcno *.gcda *.gcov *.info ./*.dSYM

clean:
	$(CC_REM) *.gcno *.gcda *.gcov *.info
	$(CC_REM) *.out *.o
	$(CC_REM) ./gcov_report ./*.dSYM *.html *.css _out.txt valgrind-out.txt

rebuild: gcov_report test
#	make clean
