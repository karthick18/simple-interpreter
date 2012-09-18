CC= gcc
SRC = scope.c
INCLUDE= -I. -I../mystring -I../myfloat -I../mydouble -I../mystring -I../error -I../myint -I../data -I../utils -I../fun -I../hash -I../arg -I../stack -I../lval -I../global
CFLAGS = -c -Wall -g  $(INCLUDE)

obj: $(SRC)
	$(CC) $(CFLAGS) $^
clean:
	rm -f *.o *~
