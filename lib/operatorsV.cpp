#include "operators.hpp"
#include "nature.hpp"
#include<iostream>
#include <cstring>
using namespace std;

const double MIN_BETWEN_FLIGHT = 0.5;
const double MAX_BETWEN_FLIGHT = 4.0;
const double MAX_TIME_DUTY = 12.0;
const double MAX_TIME_FLIGHT = 8.0;
const string BASE1 = "IST";
const string BASE2 = "ANK";


Population Operators::elitism(vector<Population> solutions){
	int minorFitness = 9999;
     int position_generation;
     for(int i = 0; i < solutions.size();i++){
          Population tmpSolution = solutions.at(i);
          if(tmpSolution.fitness < minorFitness) {
               minorFitness = tmpSolution.fitness;
               position_generation = i;
          } 
     }
     return solutions.at(position_generation);
};


void Operators::getFitness(Population* all_generation, int num_flights){
	//first: search flight no taked
	vector<int> taked = vector<int>();
	vector<int> taked_count = vector<int>();
	for(int i = 0; i < num_flights; i++){
		taked.push_back(0);
		taked_count.push_back(0);
	}
	
	vector<Individual>* generation =  &all_generation->generation;
	//price
	double price_gen = 0.0;
	//for each individual in generation
	for(int i = 0; i<generation->size();i++){
		Individual indTmp = generation->at(i);
		price_gen += indTmp.getPrice();
		vector<int> chromoTmp =  indTmp.getChromosomes();
		for(int j = 0; j < chromoTmp.size();j++){
			taked.at(chromoTmp.at(j)-1) = 1;
			taked_count.at(chromoTmp.at(j)-1) += 1;
		} 
	}

	int count_repited = 0;
	for(int i = 0 ; i < num_flights; i++){
	//	cout<<taked_count.at(i)<<endl;
		if(taked_count.at(i)>1){
			count_repited += 1; 
		}
	}
	//cout<<"repited -> "<<count_repited<<endl;

	double fitness = 0.0;
	fitness = price_gen + (count_repited)*100;
	//cout<<"fitness val -> "<<1/fitness<<endl;
	all_generation->fitness = fitness;
	all_generation->price = price_gen;
};

vector<Population> Operators::selectRouletteWheel(vector<Population> solutions){
     //obtain total fitness of solutions
     double totalFitness = 0.0;
     for(int i =0; i < solutions.size(); i++){
          totalFitness += solutions.at(i).fitness;
     }
     //cout<<"total fitnes -> "<<totalFitness<<endl;
     vector<double> relativesFitness = vector<double>();
     for(int i =0; i < solutions.size(); i++){
          double tmpRLF = solutions.at(i).fitness /totalFitness; 
          relativesFitness.push_back(tmpRLF);
     }
     //seleccionemos la mitad de la poblacion
     vector<Population> selecteds = vector<Population>();
     struct timespec ts;
     for(int i = 0; i < solutions.size()/2;i++){
		clock_gettime(CLOCK_MONOTONIC, &ts);
		srand((time_t) ts.tv_nsec);
          double probability = ((double) rand() / (RAND_MAX));
          //cout<<"probability -> "<<probability<<endl;
          double suma = 0.0;
          for(int j = 0; j < solutions.size();j++){
               suma += relativesFitness.at(j);
               //cout<<"suma: "<<suma<<endl;
               if(probability<suma){
                    //cout<<"suma excedio -> "<<suma<<endl;
                    //este debemos seleccinar
                    selecteds.push_back( solutions.at(j) );
                    break;
               }
          }
     }

/*     for(int i = 0; i < selecteds.size() ; i++){
          cout<<selecteds.at(i).fitness<<endl;
          vector<Individual> individuos = selecteds.at(i).generation;
          for(int j = 0; j< individuos.size(); j++){
               vector<int> cromosomas = individuos.at(j).getChromosomes();
               for (int k = 0; k <  cromosomas.size(); k++){
                    cout<<cromosomas.at(k)<<"--"; 
               }
               cout<<endl;
          }
     }
*/
     return selecteds;
}