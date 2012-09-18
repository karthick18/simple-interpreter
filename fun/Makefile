CC= gcc
SRC = fun.c
INCLUDE= -I. -I../mystring -I../myint -I../mydouble -I../myfloat -I../error -I../data -I../token -I../stack -I../arg -I../utils -I../hash -I../lval -I../exp -I../scope
CFLAGS = -c -Wall -g  $(INCLUDE)

obj: $(SRC)
	$(CC) $(CFLAGS) $^

clean:
	rm -f *.o *~
    
