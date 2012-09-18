#Makefile for the Interpreter-Future Stuff.
OBJECTS= arg.o data.o error.o exp.o exp_op.o exp_val.o fun.o hash.o \
	main.o mydouble.o myfloat.o myint.o mystring.o stack.o \
	token.o utils.o lval.o global.o scope.o intern.o
	
DIRS = arg data error exp fun \
	hash main mydouble myfloat myint mystring stack \
	token utils lval global scope intern
all: make_objects evaluate

make_objects:
	make clean
	for each_dir in $(DIRS);\
	do \
	cd $$each_dir; make clean; make obj;cp *.o ../; cd ..;\
	done; cd exp;make -f Makefile.val obj ;make -f Makefile.op obj;cp *.o ../

evaluate: $(OBJECTS)
	gcc -g -o $@ $^
clean:
	$(foreach dir, $(DIRS), make -C $(dir) clean;) \
	rm -f *.o *~ evaluate
