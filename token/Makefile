CC= gcc
SRC = token.c
INCLUDE= -I. -I../mystring -I../myint -I../mydouble -I../myfloat -I../error -I../data  -I../stack -I../utils -I../lval -I../hash  -I../global -I../lval -I../hash -I../arg
CFLAGS = -c -Wall -g  $(INCLUDE)

obj: $(SRC)
	$(CC) $(CFLAGS) $^

clean:
	rm -f *.o *~
    
