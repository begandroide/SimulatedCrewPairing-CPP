CC=g++
OUT=out
SRC=src
CFLAGS=-g
LIB=lib

all: individual.o flight.o agency.o nature.o main.o fly

fly:
	g++ -o $(OUT)/fly $(OUT)/individual.o $(OUT)/flight.o $(OUT)/agency.o  $(OUT)/nature.o $(OUT)/main.o 

main.o:
	g++ -c $(SRC)/main.cpp -o $(OUT)/main.o

individual.o:
	g++ -c $(LIB)/individual.cpp -o $(OUT)/individual.o

flight.o:
	g++ -c $(LIB)/flight.cpp -o $(OUT)/flight.o

agency.o:
	g++ -c $(LIB)/agency.cpp -o $(OUT)/agency.o

nature.o:
	g++ -c $(LIB)/nature.cpp -o $(OUT)/nature.o

clean:
	rm -f $(OUT)/*.o
	rm -f $()