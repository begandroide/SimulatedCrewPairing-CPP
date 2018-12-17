#include <vector>
#include <cstdio>
#include <iostream>
#include <string>
using namespace std;

#include "../lib/individual.hpp"
#include "../lib/flight.hpp"
#include "../lib/agency.hpp"
#include "../lib/nature.hpp"
#include "../lib/operators.hpp"
#include "../lib/duty.cpp"

const string BASE1 = "IST";
const string BASE2 = "ANK";

static Agency agency; //vector to save flights, pushed in order by id;
static Nature nature;
void test();
long number_individuals;

int main(int argc, char const *argv[]) {
     //procedure to load flights into vector flights
     if (*++argv != NULL) {
          nature.agency.loadFlights(*argv);
          if (*++argv != NULL) {
               number_individuals = strtol(*argv, (char **) argv, 10);
               cout << "number of individuals per generation: " + to_string(number_individuals) << endl;
          } else {
               printf("Number of Individual per generation not given, set default: 23\n");
          }
     } else {
          cout << "Error: not input airport file, insert one <instance.*>" << endl;
          cout << "error(1)" << endl;
          exit(0);
     }
     int count = 0;
     vector<Population> tmpSolutions = vector<Population>();
     while(count < 20){
          tmpSolutions.push_back( nature.makePopulation(count, 20) ); //0 is generation number zero, 30 experimental resutl by data
          count++;
     }
     nature.population.push_back(tmpSolutions);
     /*nature.showGeneration(0); //show generation number 0
     nature.showGeneration(1);
     nature.showGeneration(2);
     nature.showGeneration(3);*/
     nature.showResume();
     Population bestSolution = nature.operators.elitism(nature.population.at(0));
     cout<<bestSolution.fitness<<endl;
     for(int iterations = 0; iterations < 1 ; iterations++){ //por cada generacion que desee iterar
          vector<Population> prev_generation = nature.population.at(iterations);
          
          vector<Population> new_generation = vector<Population>();
          new_generation.push_back(bestSolution);
          //first select
          //second mutate
          //third repare
          //exit condition if prev solution its fine
          //else elitism
          //Population bestSolution = nature.operators.elitism(new_generation);     
     }
     exit(100);
}
 /*         cout << "EHEH" << endl;
          for (int a = 10; a < (gen_mutate.generation.size()); a++) {
               Individual indTmp = gen_mutate.generation.at(a);
     struct timespec ts;

     // for(int i = 0; i < 1;i++){
     //first mutation generation
     double max_fitness = 0.0;
     int id = -1;
     for (int j = 1; j < 100; j++) {
          Population gen_mutate = Population();
          for (int i = 0; i < nature.population.at(j-1).generation.size(); i++) {
               clock_gettime(CLOCK_MONOTONIC, &ts);
               /* using nano-seconds instead of seconds 
               srand((time_t) ts.tv_nsec);
               double probability = ((double) rand() / (RAND_MAX));
               //cout<<"Going with prob: "<<probability<<endl;
               Individual indTmp = Individual(0);
               indTmp = nature.operators.mutate(nature.population.at(j - 1).generation.at(i),
                                                nature.agency.getFlights(), probability);
               gen_mutate.generation.push_back(indTmp);
               //must calculate price and fitness of population mutated
          }
          cout << gen_mutate.generation.size() << endl;
          nature.compressGeneration(&gen_mutate.generation);
          //TODO Repair generation
          nature.operators.getFitness(&gen_mutate, nature.agency.getFlights().size());
          if (gen_mutate.fitness > max_fitness) {
               max_fitness = gen_mutate.fitness;
               id = j;
          }
          //debemos reparar gen_mutate de acuerdo a toda la solucion
          vector<int> flights_taked = vector<int>();
          for (int z = 0; z < nature.agency.getFlights().size(); z++) {
               flights_taked.push_back(0);
          }
          nature.population.push_back(gen_mutate);
     }
     nature.showGeneration(id);
            exit(2);
          return 1;
               //iteramos en sus chromosomes
               vector<int> chromosome_tmp = indTmp.getChromosomes();
               for (int b = 0; b < chromosome_tmp.size(); b++) {
                    flights_taked.at(chromosome_tmp.at(b) - 1) += 1;
                    if (flights_taked.at(chromosome_tmp.at(b) - 1) > 1) {
                        andk:;
                         int id_conflicto = chromosome_tmp.at(b);
                         //cambiar por otra qe puede servir CAE

                         //TODO tomar repetidos y cambiar por otros que no se han usado
                         int changed = nature.operators.repare(&gen_mutate.generation.at(a), id_conflicto,
                                                                    nature.agency.getFlights());
                         nature.operators.getFitness(&gen_mutate, nature.agency.getFlights().size());
                         if (gen_mutate.fitness > max_fitness) {
                              max_fitness = gen_mutate.fitness;
                              id = j;
                         }
/*
                         if (changed == 0) {
                              cout << "Changed == 0" << endl;
                              for (int i = 0; i < flights_taked.size(); i++) {
                                   if (flights_taked.at(i) == 0) {
                                        int flag = 0;
                                        if(nature.agency.getFlights().at(i).aeropuerto_init.compare(BASE1) == 0 || nature.agency.getFlights().at(i).aeropuerto_init.compare(BASE2)==0)
                                        {
                                             flag = 1;
                                             gen_mutate.generation.at(a) = nature.getGreedyIndividual(i + 1);
                                             nature.operators.getFitness(&gen_mutate, nature.agency.getFlights().size());
                                             if (gen_mutate.fitness < max_fitness) {
                                                  max_fitness = gen_mutate.fitness;
                                                  id = j;
                                             }
                                             break;
                                        }
                                        if(gen_mutate.generation.at(a).getChromosomes().size()==0){
                                             flights_taked.at(i) = -1; //no es base
                                             if(flag == 0){
                                                  goto ghelp;
                                             }
                                        }else {
                                             flights_taked.at(chromosome_tmp.at(b) - 1) -= 1;
                                        }
                                        //actualizar los vuelos tomados
                                        for (int k = 0; k < gen_mutate.generation.at(a).getChromosomes().size(); k++) {
                                             flights_taked.at(
                                                     gen_mutate.generation.at(a).getChromosomes().at(k) - 1) += 1;
                                        }
                                        cout << "ouch" << endl;
                                        break;
                                   }
                              }
                              break;
                         }
                         cout << "chorm -> " << chromosome_tmp.at(b) << "id-> " << b << " individual-> " << a << endl;
                    }
                   ghelp:;
               }
          }

          flights_taked.clear();
          for (int z = 0; z < nature.agency.getFlights().size(); z++) {
               flights_taked.push_back(0);
          }

          for (int a = 10; a < (gen_mutate.generation.size()); a++) {
               Individual indTmp = gen_mutate.generation.at(a);
               //iteramos en sus chromosomes
               vector<int> chromosome_tmp = indTmp.getChromosomes();
               for (int b = 0; b < chromosome_tmp.size(); b++) {
                    flights_taked.at(chromosome_tmp.at(b) - 1) += 1;
               }
          }

           for (int z = 0; z < nature.agency.getFlights().size(); z++) {
               cout << flights_taked.at(z) << endl;
          }

          nature.population.push_back(gen_mutate);
     }
     cout<<max_fitness<<endl;
     nature.showGeneration(id);
     /*
     nature.showGeneration(0);
     nature.showGeneration(200);
     nature.showGeneration(400);
     nature.showGeneration(999);
*/
     /*data loaded in vector flights*/

     //nature.agency.resume(); //to show all table flights
     //nature.agency.showAirports();
     //test(); */

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
