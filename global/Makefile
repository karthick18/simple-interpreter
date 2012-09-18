CC= gcc
SRC = global.c
INCLUDE= -I. -I../mystring -I../myfloat -I../mydouble -I../mystring -I../error -I../myint -I../data -I../utils -I../fun -I../local -I../hash -I../arg -I../stack -I../scope -I../lval
CFLAGS = -c -Wall -g  $(INCLUDE)

obj: $(SRC)
	$(CC) $(CFLAGS) $^
clean:
	rm -f *.o *~
