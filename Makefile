CC=g++
OUT=out
SRC=src
CFLAGS=-g
LIB=lib

all: individual.o flight.o agency.o nature.o operators.o main.o fly

fly:
	g++ -O3 -o $(OUT)/fly $(OUT)/individual.o $(OUT)/flight.o $(OUT)/agency.o  $(OUT)/nature.o $(OUT)/operators.o $(OUT)/main.o 

main.o:
	g++ -O3 -c $(SRC)/main.cpp -o $(OUT)/main.o

individual.o:
	g++ -O3 -c $(LIB)/individual.cpp -o $(OUT)/individual.o

flight.o:
	g++ -O3 -c $(LIB)/flight.cpp -o $(OUT)/flight.o

agency.o:
	g++ -O3 -c $(LIB)/agency.cpp -o $(OUT)/agency.o

nature.o:
	g++ -O3 -c $(LIB)/nature.cpp -o $(OUT)/nature.o

operators.o:
	g++ -O3 -c $(LIB)/operators.cpp -o $(OUT)/operators.o

clean:
	rm -f $(OUT)/*.o
	rm -f $()

test1: fly
	/usr/bin/time $(OUT)/fly instances/instancia.csv
test2: fly
	/usr/bin/time $(OUT)/fly instances/Instancias\ -\ 58\ vuelos.csv
test3: fly
	/usr/bin/time $(OUT)/fly instances/Instancias\ -\ 96\ vuelos.csv