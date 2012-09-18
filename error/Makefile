CC= gcc
SRC = error.c
INCLUDE= -I. -I../mystring -I../myfloat -I../mydouble -I../mystring  -I../myint -I../token -I../stack -I../data -I../utils -I../lval -I../global -I../hash -I../fun -I../arg -I../exp
CFLAGS = -c -Wall -g  $(INCLUDE)

obj: $(SRC)
	$(CC) $(CFLAGS) $^
clean:
	rm -f *.o *~
