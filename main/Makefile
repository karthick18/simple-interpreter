CC= gcc
SRC = main.c
INCLUDE= -I. -I../mystring -I../myfloat -I../mydouble -I../mystring -I../error -I../myint -I../token -I../stack -I../data -I../utils  -I../fun -I../arg -I../exp -I../scope -I../lval -I../global -I../hash
CFLAGS = -c -Wall -g  $(INCLUDE)

obj: $(SRC)
	$(CC) $(CFLAGS) $^
clean:
	rm -f *.o *~
