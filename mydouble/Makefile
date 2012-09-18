CC= gcc
SRC = mydouble.c
INCLUDE= -I. -I../mystring -I../myint -I../myfloat -I../mystring -I../error -I../data -I../token -I../utils -I../stack -I../lval -I../fun -I../arg -I../global -I../hash -I../lval
CFLAGS = -c -Wall -g  $(INCLUDE)

obj: $(SRC)
	$(CC) $(CFLAGS) $^

clean:
	rm -f *.o *~
    
