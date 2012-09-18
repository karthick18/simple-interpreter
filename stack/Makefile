CC= gcc
SRC = stack.c
INCLUDE= -I. -I../mystring -I../myfloat -I../mydouble -I../mystring -I../error -I../myint -I../data -I../utils -I../fun -I../lval -I../hash -I../arg -I../scope -I../global
CFLAGS = -c -Wall -g  $(INCLUDE)

obj: $(SRC)
	$(CC) $(CFLAGS) $^
clean:
	rm -f *.o *~
