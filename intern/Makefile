CC= gcc
SRC = intern.c 
INCLUDE= -I. -I../mystring -I../myint -I../mydouble -I../myfloat -I../error -I../data -I../token -I../stack  -I../utils  -I../fun -I../global -I../lval -I../hash -I../arg

CFLAGS = -c -Wall -g  $(INCLUDE)

obj: $(SRC)
	$(CC) $(CFLAGS) $^

clean:
	rm -f *.o *~
    
