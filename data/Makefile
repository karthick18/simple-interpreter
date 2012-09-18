CC= gcc
SRC = data.c
INCLUDE= -I. -I../mystring -I../myfloat -I../mydouble -I../mystring -I../error -I../myint -I../token -I../stack -I../utils  -I../fun -I../arg  -I../lval -I../global -I../hash
CFLAGS = -c -Wall -g  $(INCLUDE)

obj: $(SRC)
	$(CC) $(CFLAGS) $^
clean:
	rm -f *.o *~
