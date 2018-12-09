#include "operators.hpp"
#include<iostream>
#include <cstring>
using namespace std;

double getIdleTime_two (string hora_one,string hora_two){
     int hourDif, minDif;
	string hora_termino_one = string();
	string min_termino_one = string();
	string hora_inicio_two = string();
	string min_inicio_two = string();
     
	hora_termino_one = hora_one.substr(0,2);
	min_termino_one = hora_one.substr(3,5);
	hora_inicio_two = hora_two.substr(0,2);
	min_inicio_two = hora_two.substr(3,5);

     hourDif = stoi(hora_inicio_two) - stoi(hora_termino_one);
     if (hourDif < 0) {
          return 0;   
     }else{
          int min_init = stoi(min_inicio_two);
          int min_fin = stoi(min_termino_one);     
          if(hourDif == 1){
                    if(min_init != min_fin){
                         if(min_fin == 0){
                              minDif = 60-min_init;
                         }else{
                              minDif = stoi(min_inicio_two)  + 60 - stoi(min_termino_one);
                         }
                         hourDif = 0;
                    }else{
                         minDif = 0;
                         hourDif = 1;
                    }
          }else{
              if(hourDif > 1){
                  if(min_init == 0 && min_fin != 0){
                      hourDif--;
                      minDif = (60-min_fin);
                      return (double)hourDif+(minDif/60.0);
                  }
              }
               if(min_init == 0 &&  min_fin == 0){
                    minDif = 0;
               }else {
                    if(min_init != 0 && min_fin == 0 ){
                         minDif = min_init;
                    }else{
                         if(min_init == 0 && min_fin != 0 ){
                         minDif = min_fin;
                         }else{
                              minDif = min_fin + (60 - min_init);
                              //cout<<minDif<<endl;
                         }
                    }
               }
          }
     } 
    // cout<<hourDif<<endl;
    // cout<<minDif<<endl;

	double time_dif = (double)hourDif+(minDif/60.0);
     //cout<<to_string(time_dif)<<endl;
     return time_dif;
}

void Operators::mutate(Individual* individual, vector<Flight> flights,double prob_mutation){
	cout<<"mutate with prob -> "<<prob_mutation*100<<endl;
	//prefieres explorar o explotar---- depende para la probabilidad
	if(prob_mutation*100<50){
		//explotar tipo hill climbing
		int num_flights = flights.size();

		//get random position of vector chromosomes
		struct timespec ts;
		clock_gettime(CLOCK_MONOTONIC, &ts);
		/* using nano-seconds instead of seconds */
		srand((time_t)ts.tv_nsec);
		int val_rand =  rand()%10;
		printf ("Random fast seeded: %d\n", val_rand);
		while(val_rand>=individual->getChromosomes().size() || val_rand == 0 ){
			clock_gettime(CLOCK_MONOTONIC, &ts);
			/* using nano-seconds instead of seconds */
			srand((time_t)ts.tv_nsec);
			val_rand = rand()%10;
			printf ("Random 2 fast seeded: %d\n", val_rand);	
		}
		//val_rand has the position of chromosome to mutate. if not its best, go for other not used previously
		
		Individual* soon_individual = (Individual*)malloc(sizeof(Individual)); 
		soon_individual = (Individual*) memcpy(soon_individual,individual,sizeof(Individual));
		//soon individual to know if the created individual its best that the father. in terms of price and size
		Flight* fligth_prev = (Flight*)malloc(sizeof(Flight));
		Flight* flight_delete = (Flight*)malloc(sizeof(Flight));
		Flight* flight_after = (Flight*)malloc(sizeof(Flight));
		flight_delete = &flights.at(individual->getChromosomes().at(val_rand-1)-1);
		if(val_rand == 1){
			flight_after = &flights.at(individual->getChromosomes().at(val_rand)-1);	
		}else if(val_rand == soon_individual->getChromosomes().size()-1){
			fligth_prev = &flights.at(individual->getChromosomes().at(val_rand-2)-1);
		}else{
			fligth_prev = &flights.at(individual->getChromosomes().at(val_rand-2)-1);
			flight_after = &flights.at(individual->getChromosomes().at(val_rand)-1);
		}
		cout<<"DELETE"<<endl;
		cout<<flight_delete->id<<endl;
		cout<<flight_delete->aeropuerto_init<<endl;
		cout<<flight_delete->aeropuerto_fin<<endl;
		cout<<flight_delete->horaInicio<<endl;
		cout<<flight_delete->horaFin<<endl; 
		
		double new_price = soon_individual->getPrice();
		double new_size = soon_individual->getSize();
		double new_time = soon_individual->getTime();
		
		if(val_rand == 1){
				cout<<"~"<<endl;
				//getting flight and idle time 
				double idle = getIdleTime_two(flight_delete->horaFin,flight_after->horaInicio); 
				cout<<flight_delete->timeFlight<<endl;
				cout<<idle<<endl;
				new_size -= idle;
				new_price -= (0.75*idle + flight_delete->timeFlight);
				new_time -= flight_delete->timeFlight;

				cout<<"AFTER"<<endl;
				cout<<flight_after->id<<endl;
				cout<<flight_after->aeropuerto_init<<endl;
				cout<<flight_after->aeropuerto_fin<<endl;
				cout<<flight_after->horaInicio<<endl;
				cout<<flight_after->horaFin<<endl;
			}else if(val_rand == soon_individual->getChromosomes().size()-1){
				cout<<"~"<<endl;
				cout<<"PREV"<<endl;	
				cout<<fligth_prev->id<<endl;
				cout<<fligth_prev->aeropuerto_init<<endl;
				cout<<fligth_prev->aeropuerto_fin<<endl;
				cout<<fligth_prev->horaInicio<<endl;
				cout<<fligth_prev->horaFin<<endl;
				//getting flight and idle time 
				double idle = getIdleTime_two(fligth_prev->horaFin,flight_delete->horaInicio); 
				cout<<flight_delete->timeFlight<<endl;
				cout<<idle<<endl;
				new_size -= idle;
				new_price -= (0.75*idle + flight_delete->timeFlight);
				new_time -= flight_delete->timeFlight;

			}else{
				cout<<"PREV"<<endl;
				cout<<fligth_prev->id<<endl;
				cout<<fligth_prev->aeropuerto_init<<endl;
				cout<<fligth_prev->aeropuerto_fin<<endl;
				cout<<fligth_prev->horaInicio<<endl;
				cout<<fligth_prev->horaFin<<endl;
				cout<<"AFTER"<<endl;
				cout<<flight_after->id<<endl;
				cout<<flight_after->aeropuerto_init<<endl;
				cout<<flight_after->aeropuerto_fin<<endl;
				cout<<flight_after->horaInicio<<endl;
				cout<<flight_after->horaFin<<endl;
				cout<<"~"<<endl;
				//getting flight and idle time 
				cout<<flight_delete->timeFlight<<endl;
				cout<<getIdleTime_two(fligth_prev->horaFin,flight_delete->horaInicio)<<endl;
				cout<<getIdleTime_two(flight_delete->horaFin,flight_after->horaInicio)<<endl;

			}
	}else{
		//explorar sin importar si la solucion es mejor o no.
	}
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
	//cout<<"price total ->"<<price_gen<<endl;

	int no_taken = 0;
	for(int i = 0; i < num_flights; i++){
		//cout<<taked.at(i)<<endl;
		if(taked.at(i) == 1){
			no_taken++;
		}
	}
	//cout<<no_taken<<endl;

	int count_repited = 0;
	for(int i ; i < num_flights; i++){
	//	cout<<taked_count.at(i)<<endl;
		if(taked_count.at(i)>1){
			count_repited += taked_count.at(i) -1; 
		}
	}
	//cout<<"repited -> "<<count_repited<<endl;

	double fitness = 0.0;
	fitness = price_gen + (no_taken)*10 +(count_repited)*10;
	//cout<<"fitness val -> "<<1/fitness<<endl;
	all_generation->fitness = 1/fitness;
	all_generation->price = price_gen;
};

/*
void Operators::selectRoulette(vector<Individual>*);
*/