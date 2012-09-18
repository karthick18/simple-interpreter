CC= gcc
SRC = mystring.c
INCLUDE= -I. -I../myfloat -I../myint -I../mydouble -I../mystring -I../error -I../data -I../token -I../stack -I../lval -I../utils -I../fun -I../arg -I../scope -I../global -I../hash
CFLAGS = -c -Wall -g  $(INCLUDE)

obj: $(SRC)
	$(CC) $(CFLAGS) $^

clean:
	rm -f *.o *~
    
