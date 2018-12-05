#include <vector>
#include <cstdio>
#include <iostream>
#include <string>
using namespace std;

#include "../lib/individual.hpp"
#include "../lib/flight.hpp"
#include "../lib/agency.hpp"
#include "../lib/nature.hpp"

static Agency agency; //vector to save flights, pushed in order by id;
static Nature nature;
void test();
long number_individuals;

int main(int argc, char const *argv[])
{     
     //procedure to load flights into vector flights
     if(*++argv!=NULL){
          //agency.loadFlights(*argv);
          nature.agency.loadFlights(*argv);
          if(*++argv!=NULL){
               number_individuals = strtol(*argv,(char**)argv,10);
               cout<<"number of individuals per generation: "+to_string(number_individuals)<<endl;
          }else{
               printf("Number of Individual per generation not given, set default: 23\n");
          }
     }else{
          cout << "Error: not input airport file, insert one <instance.*>" <<endl;
          cout << "error(1)" <<endl;
          exit(0);
     }
  
     nature.makePopulation(0,10); //0 is generation number zero

     for(int z = 0; z < nature.population.at(0).size();z++){
          cout<<"-----individual number -> "+to_string(z+1)<<endl;
          for(int x = 0; x < nature.population.at(0).at(z).getSize();x++){
               cout<<nature.population.at(0).at(z).getChromosomes().at(x)<<endl;
          }
          cout<<"price of this: in hours$";
          cout<<nature.population.at(0).at(z).getTime()<<endl;
     }
     /*data loaded in vector flights*/

     //nature.agency.resume(); //to show all table flights
     //nature.agency.showAirports();
     //test();
     return 0;
}

void test(){
    //Flight vuelo(1,"cppo","stgo","10:10","11:20");
    // printf("vuelo -> %d ,%s\n", vuelo.id,vuelo.aeropuerto_init.c_str());
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
