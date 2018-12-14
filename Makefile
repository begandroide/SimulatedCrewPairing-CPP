CC=g++
OUT=out
SRC=src
CFLAGS=-g
LIB=lib

all: individual.o flight.o agency.o natureV.o operators.o population.o main.o fly

fly:
	g++ -O3 $(CFLAGS) -o $(OUT)/fly $(OUT)/individual.o $(OUT)/flight.o $(OUT)/agency.o  $(OUT)/natureV.o $(OUT)/operators.o $(OUT)/main.o 

main.o:
	g++ -O3 $(CFLAGS) -c $(SRC)/main.cpp -o $(OUT)/main.o

individual.o:
	g++ -O3 $(CFLAGS) -c $(LIB)/individual.cpp -o $(OUT)/individual.o

flight.o:
	g++ -O3 $(CFLAGS) -c $(LIB)/flight.cpp -o $(OUT)/flight.o

agency.o:
	g++ -O3 $(CFLAGS) -c $(LIB)/agency.cpp -o $(OUT)/agency.o

natureV.o:
	g++ -O3 $(CFLAGS) -c $(LIB)/natureV.cpp -o $(OUT)/natureV.o

operators.o:
	g++ -O3 $(CFLAGS) -c $(LIB)/operators.cpp -o $(OUT)/operators.o

population.o:
	g++ -O3 $(CFLAGS) -c $(LIB)/population.hpp -o $(OUT)/population.o

run:
	./out/fly ./instances/instancia.csv
clean:
	rm -f $(OUT)/*.o
	rm -f $()

test1: fly
	/usr/bin/time $(OUT)/fly instances/instancia.csv
test2: fly
	/usr/bin/time $(OUT)/fly instances/Instancias\ -\ 58\ vuelos.csv
test3: fly
	/usr/bin/time $(OUT)/fly instances/Instancias\ -\ 96\ vuelos.csv