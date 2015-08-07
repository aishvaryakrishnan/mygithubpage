#### GTThread Library Makefile

CFLAGS  = -Wall -pedantic -w
LFLAGS  =
CC      = gcc
RM      = /bin/rm -rf
AR      = ar rc
RANLIB  = ranlib

LIBRARY = gtthread.a

LIB_SRC = gtthread.c gtthread_mutex.c gtthread_sched.c 

LIB_OBJ = $(patsubst %.c,%.o,$(LIB_SRC))

DIN_SRC = dining_phil.c
DIN_OBJ = dining_phil

# pattern rule for object files
%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

all: $(LIBRARY) $(DIN_OBJ)

$(LIBRARY): $(LIB_OBJ)
	$(AR) $(LIBRARY) $(LIB_OBJ)
	$(RANLIB) $(LIBRARY)

clean:
	$(RM) $(LIBRARY) $(LIB_OBJ) $(DIN_OBJ)

$(DIN_OBJ): $(LIBRARY)
	$(CC) $(CFLAGS) $(DIN_SRC) $(LIBRARY) -o $(DIN_OBJ) -I.

.PHONY: depend
depend:
	$(CFLAGS) -- $(LIB_SRC)  2>/dev/null
