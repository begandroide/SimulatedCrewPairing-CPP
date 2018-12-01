#include <vector>
#include <cstdio>
#include <iostream>
#include <string>
using namespace std;

#include "../lib/individual.hpp"
#include "../lib/flight.hpp"
#include "../lib/agency.hpp"

static vector<Flight> flights; //vector to save flights, pushed in order by id;
void test();

int main(int argc, char const *argv[])
{
     
     //procedure to load flights into vector flights
     if(*++argv!=NULL){
          AGENCY_H::getFlights(*argv, &flights);
     }else{
          cout << "Error: not input airport file, insert one <instance.*>" <<endl;
          cout << "error(1)" <<endl;
          exit(0);
     }

     AGENCY_H::resume(flights,flights.size()); //to show resume about flights
     //test();
     return 0;
}

void test(){
     Flight vuelo(1,"cppo","stgo","10:10","11:20");
     printf("vuelo -> %d ,%s\n", vuelo.id,vuelo.aeropuerto_init.c_str());
     //cout << vuelo.aeropuerto_init << endl;
     vector<Individual*> population;
     Individual ind1(5, (vector<int>){2,3,4,5,6});
     Individual ind2(5,100,1,(vector<int>){7,8,9,10,11});
     population.push_back(&ind1);
     population.push_back(&ind2);
     printf("buena compa N %d\n",ind2.getSize());
     printf("Finalize %d\n",population.at(0)->getSize());
     printf("pop -> %d\n",population.at(0)->getChromosomes().at(0));
     printf("pop complete -> %d , %f, %f, %d\n",population.at(1)->getSize(),population.at(1)->getPrice(),population.at(1)->getFitness(),population.at(1)->getChromosomes().at(4));
}
