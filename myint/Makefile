CC= gcc
SRC = myint.c
INCLUDE= -I. -I../mystring -I../myfloat -I../mydouble -I../mystring -I../error -I../data -I../token -I../stack -I../utils -I../lval -I../fun -I../arg  -I../scope -I../global -I../hash

CFLAGS = -c -Wall -g  $(INCLUDE)

obj: $(SRC)
	$(CC) $(CFLAGS) $^

clean:
	rm -f *.o *~
    
