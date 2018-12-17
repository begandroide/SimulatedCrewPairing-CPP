#include "operators.hpp"
#include "nature.hpp"
#include<iostream>
#include <string.h>
using namespace std;

const double MIN_BETWEN_FLIGHT = 0.5;
const double MAX_BETWEN_FLIGHT = 4.0;
const double MAX_TIME_DUTY = 12.0;
const double MAX_TIME_FLIGHT = 8.0;
const string BASE1 = "IST";
const string BASE2 = "ANK";


int check_base_operator(string airport){
     if(airport.compare(BASE1) == 0 || airport.compare(BASE2)== 0 ){
          return 0;
     }else{
          return 1;
     }
}

double getIdleTime_operator(string hora_one,string hora_two){
    double hourDif;
	string hora_termino_one = string();
	string min_termino_one = string();
	string hora_inicio_two = string();
	string min_inicio_two = string();
     
	hora_termino_one = hora_one.substr(0,2);
	min_termino_one = hora_one.substr(3,5);
	hora_inicio_two = hora_two.substr(0,2);
	min_inicio_two = hora_two.substr(3,5);

	double hour_inicio = stoi(hora_inicio_two);
	double hour_fin =  stoi(hora_termino_one);
    double min_init = stoi(min_inicio_two)/60.0;
    double min_fin = stoi(min_termino_one)/60.0;

    if(hour_inicio == 0){
        hour_inicio =24;
    } else if(hour_inicio == 1){
        hour_inicio = 25;
    } else if(hour_inicio == 2){
        hour_inicio = 26;
    }

    if(hour_fin == 0){
        hour_fin =24;
    } else if(hour_fin == 1){
        hour_fin = 25;
    } else if(hour_fin == 2){
        hour_fin = 26;
    }
    hour_fin += min_fin;
    hour_inicio += min_init;
	double time_dif = hour_inicio - hour_fin;
	if(time_dif < 0) return 0;
	return time_dif;
}


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
     return selecteds;
     /*
     for(int i = 0; i < selecteds.size() ; i++){
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
}

Individual Operators::mutate(Population individual, vector<Flight> flights,double prob_mutation){
	cout<<"mutate with prob -> "<<prob_mutation*100<<endl;
	//prefieres explorar o explotar---- depende para la probabilidad
	int num_flights = flights.size();

	//get random position of duty in vector individual 
     int numDuties = individual.generation.size();
     //cout<<numDuties<<endl;
	struct timespec ts;
randem:;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	srand((time_t)ts.tv_nsec);
	int val_rand =  rand()%(numDuties);
	while(val_rand>=individual.generation.size() ) goto randem;
	printf ("Random 2 fast seeded: %d\n", val_rand);
     //val_rand has the position of duty to mutate.      
     Individual dutyObjective =  individual.generation.at(val_rand);
     if(dutyObjective.getChromosomes().size() == 1){
		goto randem;
	}
randemize:;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	srand((time_t)ts.tv_nsec);
	val_rand =  rand()%(dutyObjective.getChromosomes().size());
	while(val_rand>=dutyObjective.getChromosomes().size() ) goto randemize;
	printf ("Random 2 fast seeded: %d\n", val_rand);
     //cout<<dutyObjective.getChromosomes().at(val_rand)<<endl;

     cout<<"nos llego"<<endl;
     for(int i = 0; i < dutyObjective.getChromosomes().size();i++){
          cout<<dutyObjective.getChromosomes().at(i)<<" - ";
     }
     cout<<endl;

     Individual* mutatedIndividual = (Individual*)malloc(sizeof(Individual));
     mutatedIndividual = (Individual*) memcpy(mutatedIndividual,&dutyObjective,sizeof(Individual));
	
     Flight* fligth_prev = (Flight*)malloc(sizeof(Flight));
	Flight* fligth_delete = (Flight*)malloc(sizeof(Flight));
	Flight* fligth_after = (Flight*)malloc(sizeof(Flight));

     fligth_delete = &flights.at(dutyObjective.getChromosomes().at(val_rand) -1);
	double new_time = dutyObjective.getTime();
	double new_size = dutyObjective.getSize();
     double new_price = dutyObjective.getPrice();
	//get values of input individual to compare.
	double timeIndividual_init = new_time;
	double sizeIndividual_init = new_size;
	double priceIndividual_init = new_price;

     if(val_rand == 0){
          //first flight leg
          //cout<<"principio"<<endl;
          fligth_after = &flights.at(dutyObjective.getChromosomes().at(val_rand + 1 )-1);
          //cout<<"flight init -> "<<fligth_delete->id<<endl;cout<<"flight fin -> "<<fligth_after->id<<endl;
          double idle = getIdleTime_operator(fligth_delete->horaFin, fligth_after->horaInicio);
          
          new_size -= (idle +  fligth_delete->timeFlight);
          new_price -= (0.75*idle + fligth_delete->timeFlight);
          new_time -= fligth_delete->timeFlight;
          
          /*updated values of new_size,price & time
               go for other fly that must be better
               soon_individual->getChromosomes().at(val_rand-1) = nuevo;
               solo nos preocupamos que sea antes del "AFTER"
               debe obedecer:
                	- Empezar en una base, terminar en la ciudad de inicio del AFTER
                    - Y antes del tiempo de AFTER, respetando reglas.*/
          string after_airport =  fligth_after->aeropuerto_init;
		string after_hour = fligth_after->horaInicio;
		for(int i = 0; i < flights.size();i++){
          //si el vuelo actual termina en donde comienza el proximo vuelo
			if(flights.at(i).id != fligth_delete->id){
				if(flights.at(i).aeropuerto_fin.compare( after_airport ) == 0){
					//si el vuelo es una base
                         if(check_base_operator(flights.at(i).aeropuerto_init) == 0){
                              double timeDif = getIdleTime_operator(flights.at(i).horaFin,after_hour);
						//si respeta las reglas de la empresa 30 min entre vuelos, max 4 horas
                              if( timeDif >= MIN_BETWEN_FLIGHT && timeDif<=MAX_BETWEN_FLIGHT){
                                   //podemos cambiar el vuelo. actualizar precio, tamaño y tiempo!
                                   double timeFlight = flights.at(i).timeFlight;
                                   
                                   double tmpSize = new_size + timeFlight + timeDif;
                                   double tmpTime = new_time + timeFlight;
                                   if(tmpSize > MAX_TIME_DUTY || tmpTime > MAX_TIME_FLIGHT){
                                        goto cnt;
                                   }
							double tmpPrice = new_price + timeFlight + 0.75*(timeDif);
                                   if(  tmpPrice < priceIndividual_init ){
                                        //mejor
                                        vector<int> chromos = mutatedIndividual->getChromosomes();//.at(val_rand-1) = flights.at(i).id;
                                        chromos.at(val_rand) = flights.at(i).id;
                                        mutatedIndividual->setChromosome(chromos);
                                        mutatedIndividual->setPrice( tmpPrice );
                                        mutatedIndividual->setTime( tmpTime );
                                        mutatedIndividual->setSize( tmpSize );
                                        cout<<"MUTED------------------"<<endl;

                                        cout<<"Lo dejamos"<<endl;
                                        for(int i = 0; i < mutatedIndividual->getChromosomes().size();i++){
                                             cout<<mutatedIndividual->getChromosomes().at(i)<<" - ";
                                        }
                                        cout<<endl;
                                        return *mutatedIndividual;
                                   }else{
                                        //cout<<"desechado"<<endl;
                                        goto cnt;
                                        //return dutyObjective;
                                   }
                              }
                         }
                    }
               }
               cnt:;
          }
			
     }else if(val_rand == dutyObjective.getChromosomes().size() - 1){
          //last flight leg
          //cout<<"final"<<endl;cout<<"flight prev -> "<<fligth_prev->id<<endl;cout<<"flight fin -> "<<fligth_delete->id<<endl;
          fligth_prev = &flights.at(dutyObjective.getChromosomes().at(val_rand - 1 )-1);
          double idle = getIdleTime_operator(fligth_prev->horaFin, fligth_delete->horaInicio);
          //cout<<idle<<endl;
          new_size -= (idle +  fligth_delete->timeFlight);
          new_price -= (0.75*idle + fligth_delete->timeFlight);
          new_time -= fligth_delete->timeFlight;

          string prev_airport =  fligth_prev->aeropuerto_fin;
          string prev_hour = fligth_prev->horaFin;
          for(int i = 0; i < flights.size();i++){
               //si el vuelo actual empieza en donde termina el vuelo anterior
               if(flights.at(i).id != fligth_delete->id){
                    if(flights.at(i).aeropuerto_init.compare( prev_airport ) == 0){
                         //si el vuelo es una base
                         if(check_base_operator( flights.at(i).aeropuerto_fin)  == 0 ){
                              double timeDif = getIdleTime_operator(prev_hour, flights.at(i).horaInicio);
                              //si respeta las reglas de la empresa 30 min entre vuelos, max 4 horas
                              if( timeDif > MIN_BETWEN_FLIGHT && timeDif<MAX_BETWEN_FLIGHT){
                                   //podemos cambiar el vuelo. actualizar precio, tamaño y tiempo!
                                   double timeFlight = flights.at(i).timeFlight;
                                   double tmpSize = new_size + timeFlight + timeDif;
                                   double tmpTime = new_time + timeFlight;
                                   if(tmpSize > MAX_TIME_DUTY || tmpTime > MAX_TIME_FLIGHT){
                                        goto cnt_skip;
                                   } 
                                   double tmpPrice = new_price + timeFlight + 0.75*(timeDif);
                                   if(  tmpPrice < priceIndividual_init ){
                                        vector<int> chromos = mutatedIndividual->getChromosomes();//.at(val_rand-1) = flights.at(i).id;
                                        chromos.at(val_rand) = flights.at(i).id;
                                        mutatedIndividual->setChromosome(chromos);
                                        mutatedIndividual->setPrice( tmpPrice );
                                        mutatedIndividual->setTime( tmpTime );
                                        mutatedIndividual->setSize( tmpSize );
                                        cout<<"MUTED------------------"<<endl;

                                        cout<<"Lo dejamos"<<endl;
                                        for(int i = 0; i < mutatedIndividual->getChromosomes().size();i++){
                                             cout<<mutatedIndividual->getChromosomes().at(i)<<" - ";
                                        }
                                        cout<<endl;
                                        return *mutatedIndividual;
                                   }else{
                                        //cout<<"desechado"<<endl;
                                        goto cnt_skip;
                                   }
                              }
                         }
                    }
               }
               cnt_skip:;
          }
     }else{
          //cout<<"intermedio"<<endl;cout<<"flight prev -> "<<fligth_prev->id<<endl;cout<<"flight inter -> "<<fligth_delete->id<<endl;
          //cout<<"flight after -> "<<fligth_after->id<<endl;
          fligth_prev = &flights.at(dutyObjective.getChromosomes().at(val_rand - 1 )-1);
          fligth_after = &flights.at(dutyObjective.getChromosomes().at(val_rand + 1 )-1);

          double idle_prev = getIdleTime_operator(fligth_prev->horaFin,fligth_delete->horaInicio);
          double idle_after = getIdleTime_operator(fligth_delete->horaFin,fligth_after->horaInicio);
          new_size -= idle_prev+idle_after+fligth_delete->timeFlight;
          new_price -= (0.75*(idle_prev+idle_after) + fligth_delete->timeFlight);
          new_time -= fligth_delete->timeFlight;
          
          string prev_airport =  fligth_prev->aeropuerto_fin;
          string prev_hour = fligth_prev->horaFin;
          string after_airport = fligth_after->aeropuerto_init;
          string after_hour = fligth_after->horaInicio;
          
          for(int i = 0; i < flights.size();i++){
			if(flights.at(i).id  != fligth_delete->id){
				//si el vuelo actual empieza en donde termina el vuelo anterior
				// Y si termina donde comienza el siguiente AFTERvuelo
				if(flights.at(i).aeropuerto_init.compare( prev_airport ) == 0 && flights.at(i).aeropuerto_fin.compare( after_airport ) == 0 ){
					//si el vuelo es una base no importa
                         double timeDif_prev = getIdleTime_operator(prev_hour, flights.at(i).horaInicio);
                         double timeDif_after = getIdleTime_operator( flights.at(i).horaFin,after_hour);
					//si respeta las reglas de la empresa 30 min entre vuelos, max 4 horas
					if( (timeDif_prev > MIN_BETWEN_FLIGHT && timeDif_after > MIN_BETWEN_FLIGHT) && (timeDif_prev < MAX_BETWEN_FLIGHT && timeDif_after < MAX_BETWEN_FLIGHT) ){
                              //podemos cambiar el vuelo. actualizar precio, tamaño y tiempo!
                              double timeFlight = flights.at(i).timeFlight;
                              double tmpSize = new_size + timeFlight + timeDif_prev + timeDif_after;
                              double tmpTime = new_time + timeFlight;
                              if(tmpSize > MAX_TIME_DUTY || tmpTime > MAX_TIME_FLIGHT){
                                   goto cnt_skip2;
                              }
                              double tmpPrice = new_price + timeFlight + 0.75*(timeDif_prev+timeDif_after);
                              if(  tmpPrice < priceIndividual_init ){
                                   //mejor

                                   vector<int> chromos = mutatedIndividual->getChromosomes();//.at(val_rand-1) = flights.at(i).id;
                                   chromos.at(val_rand) = flights.at(i).id;
                                   mutatedIndividual->setChromosome(chromos);
                                   mutatedIndividual->setPrice( tmpPrice );
                                   mutatedIndividual->setTime( tmpTime );
                                   mutatedIndividual->setSize( tmpSize );
                                   cout<<"MUTED------------------"<<endl;

                                   cout<<"Lo dejamos"<<endl;
                                   for(int i = 0; i < mutatedIndividual->getChromosomes().size();i++){
                                        cout<<mutatedIndividual->getChromosomes().at(i)<<" - ";
                                   }
                                   cout<<endl;
                                   return *mutatedIndividual;
                              }else{
                                   //cout<<"desechado"<<endl;
                                   goto cnt_skip2;
                              }
                         }
                    }
               }
               cnt_skip2:;
          }
     }
     goto randem;
};