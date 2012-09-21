CC = gcc

TOPDIR = ..

ALL_DIRS := arg data error exp fun \
	hash main mydouble myfloat myint mystring stack \
	token utils lval global scope intern

ALL_SRCS := $(foreach dir, $(ALL_DIRS), $(wildcard $(dir)/*.c))

ALL_OBJS := $(ALL_SRCS:%.c=%.o)

SRCS = $(wildcard *.c)

OBJS = $(SRCS:%.c=%.o)

ALL_INCLUDES = -I.

ALL_INCLUDES += $(addprefix -I$(TOPDIR)/, $(ALL_DIRS))

CUSTOM_INCLUDES = 

DEBUG_FLAGS = -g

LINKER_FLAGS =

CFLAGS = -c -Wall  -Werror $(DEBUG_FLAGS) $(CUSTOM_INCLUDES) $(ALL_INCLUDES)

CLEAN_CMD = rm -f *.o *~

objs: $(OBJS)

%.o:%.c
	$(CC) $(CFLAGS) -c -o $@ $< 

export CC ALL_DIRS ALL_INCLUDES DEBUG_FLAGS CFLAGS CLEAN_CMD