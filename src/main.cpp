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
     //nature.showResume();
     Population bestSolution;

     double minFitness = 9999;
     Population bestALL; 
     for(int iterations = 0; iterations < 1000 ; iterations++){ //por cada generacion que desee iterar
          //bestSolution = nature.operators.elitism(nature.population.at(iterations));
          //cout<<bestSolution.fitness<<endl;
          //if(bestSolution.fitness < minFitness) minFitness = bestSolution.fitness;
          
          vector<Population> prev_generation = nature.population.at(iterations);
          
          //vector<Population> new_generation = vector<Population>();
          //new_generation.push_back(bestSolution);
          //first select
          vector<Population> victimsToTransform = nature.operators.selectRouletteWheel(prev_generation);
          
          vector<int> positionCambiadas = vector<int>();
          for(int j = 0; j < prev_generation.size(); j++){
               positionCambiadas.push_back(0);
          }

          for(int j = 0 ; j < victimsToTransform.size(); j++){
               Population populationlMutated = nature.operators.mutate(victimsToTransform.at(j),nature.agency.getFlights(), 0.07);
               //TODO {DONE} REPARE: go to pairing and search for duties more tiny, check if exist flight leg that can be changed for crash (choques)
               nature.repareSolution(&populationlMutated);
               //TODO DELETE {DONE} DUTIES COPIEDS
               nature.deleteDuplicate(&populationlMutated);
               //TODO reemplazar los 10 peores (mayor fitness) elitista evita que el mejor se pierda
               // encontrar el peor, cambiarlo por uno, marcar la posicion y buscar el peor sacando la posicion cubierta ya

               double max = 0.0;
               int position_cambio = -1;

               for(int i = 0; i < prev_generation.size(); i++ ){
                    if( prev_generation.at(i).fitness > max ){
                         if(positionCambiadas.at(i)  == 0){
                              max = prev_generation.at(i).fitness;
                              position_cambio = i;
                         }
                    } 
               }
               positionCambiadas.at(position_cambio) = 1;
               nature.operators.getFitness(&populationlMutated,nature.agency.getFlights().size());
               //cout<<prev_generation.at(position_cambio).fitness<<endl;
               prev_generation.at(position_cambio) = populationlMutated;
           //    cout<<prev_generation.at(position_cambio).fitness<<endl;
          }

          bestSolution = nature.operators.elitism(prev_generation);
         // cout<<bestSolution.fitness<<endl;
          if(bestSolution.fitness < minFitness)
          {
               minFitness = bestSolution.fitness;
               bestALL = bestSolution;
          } 
          nature.population.push_back(prev_generation);
     }
     //nature.showResume();
     cout<<"Minimum price $" <<bestALL.price<<endl;
     //cout<<minFitness<<endl;
     cout<<"Duties:"<<endl;
     for(int a = 0; a < bestALL.generation.size(); a++){
          for(int b = 0; b<bestALL.generation.at(a).getChromosomes().size(); b++){
               cout<<bestALL.generation.at(a).getChromosomes().at(b)<<"--";
          }
          cout<<" || ";
     }
     cout<<endl;
     
     exit(100);
}
