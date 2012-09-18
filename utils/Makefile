CC= gcc
SRC = utils.c
INCLUDE= -I. -I../mystring -I../myfloat -I../mydouble -I../mystring -I../error -I../myint -I../token -I../stack -I../data -I../lval -I../fun -I../arg -I../exp -I../global -I../hash -I../scope -I../intern -DZERO
CFLAGS = -c -Wall -g  $(INCLUDE)

obj: $(SRC)
	$(CC) $(CFLAGS) $^
clean:
	rm -f *.o *~
