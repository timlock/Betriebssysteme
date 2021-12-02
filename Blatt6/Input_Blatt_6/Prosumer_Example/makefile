EXE=./prosumer

GCC=gcc
CC=gcc

OSTYPE := $(shell uname)

all: $(EXE)

$(EXE): prosumer.o 
ifeq ($(OSTYPE), Linux)
	$(GCC) -o prosumer -pthread prosumer.o
endif
ifeq ($(OSTYPE), SunOS)
	$(GCC) -o prosumer prosumer.o
endif	

prosumer.o: prosumer.c
	$(GCC) -c prosumer.c

run: $(EXE) 
	$(EXE)

clean:
	$(RM) $(EXE) *.o
