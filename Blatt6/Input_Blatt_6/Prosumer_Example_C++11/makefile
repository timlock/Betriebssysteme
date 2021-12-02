EXE=./prosumer

GPP=g++

OSTYPE := $(shell uname)

all: $(EXE)

$(EXE): prosumer.o 
ifeq ($(OSTYPE), Linux)
	$(GPP) -o prosumer -std=c++11 -pthread  prosumer.o
endif

prosumer.o: prosumer.cpp
	$(GPP) -std=c++11 -pthread -c prosumer.cpp

run: $(EXE) 
	$(EXE)

clean:
	$(RM) $(EXE) *.o
