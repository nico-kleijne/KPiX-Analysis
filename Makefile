# Variables
CC=g++ 
INC=$(PWD)/include
LDIR=$(PWD)/lib
#KPDIR=/home/mengqing/kpix #please give your kpix dir
SLIB=-Wl,-rpath=$(LDIR)
CFLAGS=-Wall $(shell xml2-config --cflags) $(shell root-config --cflags) -I/home/mengqing/kpix/kpix -I/home/mengqing/kpix/generic -I$(INC) 
LFLAGS=$(shell xml2-config --libs) $(shell root-config --glibs) -L$(LDIR) -lkpix

# dir:
SRC=$(PWD)/src
BIN=$(PWD)/bin

objects=$(patsubst $(SRC)/%.cxx,$(BIN)/%,$(wildcard $(SRC)/*.cxx))

#default:
all: $(objects)
ana: analysis

$(BIN)/%: $(SRC)/%.cxx
	$(CC) -o $@ $< $(CFLAGS) $(SLIB) $(LFLAGS)

analysis: $(SRC)/analysis.cxx
	$(CC) -o $(BIN)/analysis $(SRC)/analysis.cxx $(CFLAGS) $(LFLAGS)

clean:
	rm $(BIN)/*
