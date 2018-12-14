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

double getIdleTime_two (string hora_one,string hora_two){

    if(hora_two.compare("")==0){
        cout<<"HELP"<<endl;
    }

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

Individual Operators::mutate(Individual* individual, vector<Flight> flights,double prob_mutation){
	cout<<"mutate with prob -> "<<prob_mutation*100<<endl;
	//prefieres explorar o explotar---- depende para la probabilidad
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
	
	//get values of input individual to compare.
	double timeIndividual_init = individual->getTime();
	double priceIndividual_init = individual->getPrice();
	double sizeIndividual_init = individual->getSize();
	int number_cobert_fligths_init = individual->getChromosomes().size();

	//make a copy of the individual to modify and compare if its better or not.
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
/*		cout<<"DELETE"<<endl;
	cout<<flight_delete->id<<endl;
	cout<<flight_delete->aeropuerto_init<<endl;
	cout<<flight_delete->aeropuerto_fin<<endl;
	cout<<flight_delete->horaInicio<<endl;
	cout<<flight_delete->horaFin<<endl; 
	*/
	double new_price = soon_individual->getPrice();
	double new_size = soon_individual->getSize();
	double new_time = soon_individual->getTime();
	
	if(val_rand == 1){
		//	cout<<"~"<<endl;
			//getting flight and idle time
			if(flight_after->horaInicio.compare("")==0){
			    cout<<"ouch"<<endl;
			}
			double idle = getIdleTime_two(flight_delete->horaFin,flight_after->horaInicio); 
		//	cout<<flight_delete->timeFlight<<endl;
		//	cout<<idle<<endl;
			new_size -= (idle +  flight_delete->timeFlight);
			new_price -= (0.75*idle + flight_delete->timeFlight);
			new_time -= flight_delete->timeFlight;

		/*	cout<<"AFTER"<<endl;
			cout<<flight_after->id<<endl;
			cout<<flight_after->aeropuerto_init<<endl;
			cout<<flight_after->aeropuerto_fin<<endl;
			cout<<flight_after->horaInicio<<endl;
			cout<<flight_after->horaFin<<endl;
			cout<<"New Vals"<<endl;
			cout<<new_size<<endl;
			cout<<new_price<<endl;
			cout<<new_time<<endl;
		*/	//updated values of new_size,price & time
			// go for other fly that must be better
			//soon_individual->getChromosomes().at(val_rand-1) = nuevo;
			//solo nos preocupamos que sea antes del "AFTER"
			//debe obedecer:
			// 		- Empezar en una base, terminar en la ciudad de inicio del AFTER
			// 		- Y antes del tiempo de AFTER, respetando reglas.
			string after_airport =  flight_after->aeropuerto_init;
			string after_hour = flight_after->horaInicio;
			for(int i = 0; i < flights.size();i++){
				//si el vuelo actual termina en donde comienza el proximo vuelo
				if(flights.at(i).id != flight_delete->id){
					if(flights.at(i).aeropuerto_fin.compare( after_airport ) == 0){
						//si el vuelo es una base
						if(flights.at(i).aeropuerto_init.compare(BASE1) == 0 ||flights.at(i).aeropuerto_init.compare(BASE2) == 0 ){
						    if(after_hour.compare("") == 0){
						        cout<<"ouch"<<endl;
						    }
							double timeDif = getIdleTime_two(flights.at(i).horaInicio,after_hour);
							//si respeta las reglas de la empresa 30 min entre vuelos, max 4 horas
							if( timeDif > MIN_BETWEN_FLIGHT && timeDif<MAX_BETWEN_FLIGHT){
								//podemos cambiar el vuelo. actualizar precio, tamaño y tiempo!
								double timeFlight = flights.at(i).timeFlight;
								double tmpSize = new_size + timeFlight + timeDif;
								double tmpTime = new_time + timeFlight;
								if(tmpSize > MAX_TIME_DUTY || tmpTime > MAX_TIME_FLIGHT){
									goto cnt;
								} 
								double tmpPrice = new_price + timeFlight + 0.75*(timeDif);
			/*					cout<<"MEJOR? -- comparando "<< flights.at(i).id <<endl;
								cout<< tmpSize <<endl;
								cout<< tmpTime <<endl;
								cout<< tmpPrice <<endl;
			*/					//si es mejor lo cambiamos
								//mejor es si la diferencia entre size y time es menor;
								//tambien es mejor si el precio es menor (definitivamente)
								if(prob_mutation*100<60){
									if( (tmpSize - tmpTime) < (sizeIndividual_init - timeIndividual_init) || tmpPrice < priceIndividual_init ){
										//mejor
										vector<int> chromos = soon_individual->getChromosomes();//.at(val_rand-1) = flights.at(i).id;
										chromos.at(val_rand-1) = flights.at(i).id;
										soon_individual->setChromosome(chromos);
										soon_individual->setPrice( tmpPrice );
										soon_individual->setTime( tmpTime );
										soon_individual->setSize( tmpSize );
										cout<<"MUTED------------------"<<endl;
										return *soon_individual;
									}else{
										cout<<"desechado"<<endl;
										return *individual;
									}
								}else{
									//muto igual

									vector<int> chromos = soon_individual->getChromosomes();//.at(val_rand-1) = flights.at(i).id;
									chromos.at(val_rand-1) = flights.at(i).id;
									soon_individual->setChromosome(chromos);
									//soon_individual->getChromosomes().at(val_rand-1) = flights.at(i).id;
									soon_individual->setPrice( tmpPrice );
									soon_individual->setTime( tmpTime );
									soon_individual->setSize( tmpSize );
									cout<<"MUTED------------------"<<endl;
									return *soon_individual;
								}
							}
						}
					}
					cnt:;
				}
			}
		}else if(val_rand == soon_individual->getChromosomes().size()-1){
			/*cout<<"~"<<endl;
			cout<<"PREV"<<endl;	
			cout<<fligth_prev->id<<endl;
			cout<<fligth_prev->aeropuerto_init<<endl;
			cout<<fligth_prev->aeropuerto_fin<<endl;
			cout<<fligth_prev->horaInicio<<endl;
			cout<<fligth_prev->horaFin<<endl;*/
			//getting flight and idle time
			if(flight_delete->horaInicio.compare("")==0){
			    cout<<"ouch"<<endl;
			}
			double idle = getIdleTime_two(fligth_prev->horaFin,flight_delete->horaInicio); 
		//	cout<<flight_delete->timeFlight<<endl;
		//	cout<<idle<<endl;
			new_size -= (idle + flight_delete->timeFlight);
			new_price -= (0.75*idle + flight_delete->timeFlight);
			new_time -= flight_delete->timeFlight;
			/*cout<<"New Vals"<<endl;
			cout<<new_size<<endl;
			cout<<new_price<<endl;
			cout<<new_time<<endl;*/

			string prev_airport =  fligth_prev->aeropuerto_fin;
			string prev_hour = fligth_prev->horaFin;
			for(int i = 0; i < flights.size();i++){
				//si el vuelo actual empieza en donde termina el vuelo anterior
				if(flights.at(i).id != flight_delete->id){
					if(flights.at(i).aeropuerto_init.compare( prev_airport ) == 0){
						//si el vuelo es una base
						if(flights.at(i).aeropuerto_fin.compare(BASE1) == 0 ||flights.at(i).aeropuerto_fin.compare(BASE2) == 0 ){
						    if(flights.at(i).horaInicio.compare("")==0){
						        cout<<"ouch"<<endl;
						    }
							double timeDif = getIdleTime_two(prev_hour, flights.at(i).horaInicio);
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
							/* cout<<"MEJOR? -- comparando "<< flights.at(i).id <<endl;
								cout<< tmpSize <<endl;
								cout<< tmpTime <<endl;
								cout<< tmpPrice <<endl;*/
								//si es mejor lo cambiamos
								//mejor es si la diferencia entre size y time es menor;
								//tambien es mejor si el precio es menor (definitivamente)
								if(prob_mutation*100<60){
									if( (tmpSize - tmpTime) < (sizeIndividual_init - timeIndividual_init) || tmpPrice < priceIndividual_init ){
										//mejor

										vector<int> chromos = soon_individual->getChromosomes();//.at(val_rand-1) = flights.at(i).id;
										chromos.at(val_rand-1) = flights.at(i).id;
										soon_individual->setChromosome(chromos);
//										soon_individual->getChromosomes().at(val_rand-1) = flights.at(i).id;
										soon_individual->setPrice( tmpPrice );
										soon_individual->setTime( tmpTime );
										soon_individual->setSize( tmpSize );
										cout<<"MUTED------------------"<<endl;
										return *soon_individual;
									}else{
										cout<<"desechado"<<endl;
										return *individual;
									}
								}else{
									//muto igual

									vector<int> chromos = soon_individual->getChromosomes();//.at(val_rand-1) = flights.at(i).id;
									chromos.at(val_rand-1) = flights.at(i).id;
									soon_individual->setChromosome(chromos);
									//									soon_individual->getChromosomes().at(val_rand-1) = flights.at(i).id;
									soon_individual->setPrice( tmpPrice );
									soon_individual->setTime( tmpTime );
									soon_individual->setSize( tmpSize );
									cout<<"MUTED------------------"<<endl;
									return *soon_individual;
								}
							}
						}
					}
					cnt_skip:;
				}	
			}
		}else{
		/*	cout<<"PREV"<<endl;
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
			cout<<getIdleTime_two(flight_delete->horaFin,flight_after->horaInicio)<<endl;*/
		    if(flight_delete->horaInicio.compare("")==0){
		        cout<<"ouch"<<endl;
		    }
			double idle_prev = getIdleTime_two(fligth_prev->horaFin,flight_delete->horaInicio);
            if(flight_after->horaInicio.compare("")==0){
                cout<<"ouch"<<endl;
            }
		    double idle_after = getIdleTime_two(flight_delete->horaFin,flight_after->horaInicio);
			new_size -= idle_prev+idle_after+flight_delete->timeFlight;
			new_price -= (0.75*(idle_prev+idle_after) + flight_delete->timeFlight);
			new_time -= flight_delete->timeFlight;
		/*	cout<<"New Vals"<<endl;
			cout<<new_size<<endl;
			cout<<new_price<<endl;
			cout<<new_time<<endl;*/

			string prev_airport =  fligth_prev->aeropuerto_fin;
			string prev_hour = fligth_prev->horaFin;
			string after_airport = flight_after->aeropuerto_init;
			string after_hour = flight_after->horaInicio;

			for(int i = 0; i < flights.size();i++){
				if(flights.at(i).id != flight_delete->id){
				//si el vuelo actual empieza en donde termina el vuelo anterior
				// Y si termina donde comienza el siguiente AFTERvuelo
					if(flights.at(i).aeropuerto_init.compare( prev_airport ) == 0 && flights.at(i).aeropuerto_fin.compare( after_airport ) == 0 ){
						//si el vuelo es una base no importa
                        if(flights.at(i).horaInicio.compare("")==0){
                            cout<<"ouch"<<endl;
                        }
						double timeDif_prev = getIdleTime_two(prev_hour, flights.at(i).horaInicio);
                        if(after_hour.compare("")==0){
                            cout<<"ouch"<<endl;
                        }
                        double timeDif_after = getIdleTime_two( flights.at(i).horaFin,after_hour);
						
						//si respeta las reglas de la empresa 30 min entre vuelos, max 4 horas
						if( (timeDif_prev > MIN_BETWEN_FLIGHT && timeDif_after > MIN_BETWEN_FLIGHT) && (timeDif_prev < MAX_BETWEN_FLIGHT && timeDif_after < MAX_BETWEN_FLIGHT) ){
						//	cout<<"cpuCOU"<<endl;
					//		cout<<"time-Dif-prev"<<timeDif_prev<<endl;
					//		cout<<"time-Dif-after"<<timeDif_after<<endl;
							//podemos cambiar el vuelo. actualizar precio, tamaño y tiempo!
							double timeFlight = flights.at(i).timeFlight;
							double tmpSize = new_size + timeFlight + timeDif_prev + timeDif_after;
							double tmpTime = new_time + timeFlight;
							if(tmpSize > MAX_TIME_DUTY || tmpTime > MAX_TIME_FLIGHT){
								goto cnt_skip2;
							} 
							double tmpPrice = new_price + timeFlight + 0.75*(timeDif_prev+timeDif_after);
					//		cout<<"MEJOR? -- comparando "<< flights.at(i).id <<endl;
					//		cout<< tmpSize <<endl;
					//		cout<< tmpTime <<endl;
					//		cout<< tmpPrice <<endl;
							//si es mejor lo cambiamos
							//mejor es si la diferencia entre size y time es menor;
							//tambien es mejor si el precio es menor (definitivamente)
							if(prob_mutation*100<60){
								if( (tmpSize - tmpTime) < (sizeIndividual_init - timeIndividual_init) || tmpPrice < priceIndividual_init ){
									//mejor

									vector<int> chromos = soon_individual->getChromosomes();//.at(val_rand-1) = flights.at(i).id;
									chromos.at(val_rand-1) = flights.at(i).id;
									soon_individual->setChromosome(chromos);
//									soon_individual->getChromosomes().at(val_rand-1) = flights.at(i).id;
									soon_individual->setPrice( tmpPrice );
									soon_individual->setTime( tmpTime );
									soon_individual->setSize( tmpSize );
									cout<<"MUTED------------------"<<endl;
									return *soon_individual;
								}else{
									cout<<"desechado"<<endl;
								}
							}else{

								vector<int> chromos = soon_individual->getChromosomes();//.at(val_rand-1) = flights.at(i).id;
								chromos.at(val_rand-1) = flights.at(i).id;
								soon_individual->setChromosome(chromos);
//								soon_individual->getChromosomes().at(val_rand-1) = flights.at(i).id;
								soon_individual->setPrice( tmpPrice );
								soon_individual->setTime( tmpTime );
								soon_individual->setSize( tmpSize );
								cout<<"MUTED------------------"<<endl;
								return *soon_individual;
							}
						}
						
					}
					cnt_skip2:;
				}	
			}
		}
	return *soon_individual;
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

//TODO -> ELITISM move the best individue of a gen.

/*
void Operators::selectRoulette(vector<Individual>*);
*/

int Operators::repare(Individual* individual, int gen, vector<Flight> flights){
	int val_rand = 0;
	for(int z = 0; z < individual->getChromosomes().size();z++){
		if(individual->getChromosomes().at(z) == gen){
			val_rand =  z;
		}
	}
	printf ("reparing gen: %d in position %d\n", gen ,val_rand);
	//get values of input individual to compare.
	double timeIndividual_init = individual->getTime();
	double priceIndividual_init = individual->getPrice();
	double sizeIndividual_init = individual->getSize();
	int number_cobert_fligths_init = individual->getChromosomes().size();

	//make a copy of the individual to modify and compare if its better or not.
	Individual* soon_individual = (Individual*)malloc(sizeof(Individual));
	soon_individual = (Individual*) memcpy(soon_individual,individual,sizeof(Individual));
	//soon individual to know if the created individual its best that the father. in terms of price and size
	Flight* fligth_prev = (Flight*)malloc(sizeof(Flight));
	Flight* flight_delete = (Flight*)malloc(sizeof(Flight));
	Flight* flight_after = (Flight*)malloc(sizeof(Flight));
	flight_delete = &flights.at(individual->getChromosomes().at(val_rand)-1);
	if(val_rand == 0){
		flight_after = &flights.at(individual->getChromosomes().at(val_rand+1)-1);
	}else if(val_rand == soon_individual->getChromosomes().size()-1){
		fligth_prev = &flights.at(individual->getChromosomes().at(val_rand-1)-1);
	}else{
		fligth_prev = &flights.at(individual->getChromosomes().at(val_rand-1)-1);
		flight_after = &flights.at(individual->getChromosomes().at(val_rand+1)-1);
	}
	int changed = 0;
	double new_price = soon_individual->getPrice();
	double new_size = soon_individual->getSize();
	double new_time = soon_individual->getTime();

	if(val_rand == 0){
		//getting flight and idle time
        if(flight_after->horaInicio.compare("")==0){
            cout<<"ouch"<<endl;
        }
		double idle = getIdleTime_two(flight_delete->horaFin,flight_after->horaInicio);
		new_size -= (idle +  flight_delete->timeFlight);
		new_price -= (0.75*idle + flight_delete->timeFlight);
		new_time -= flight_delete->timeFlight;

        //updated values of new_size,price & time
		// go for other fly that must be better
		//soon_individual->getChromosomes().at(val_rand-1) = nuevo;
		//solo nos preocupamos que sea antes del "AFTER"
		//debe obedecer:
		// 		- Empezar en una base, terminar en la ciudad de inicio del AFTER
		// 		- Y antes del tiempo de AFTER, respetando reglas.
		string after_airport =  flight_after->aeropuerto_init;
		string after_hour = flight_after->horaInicio;
		for(int i = 0; i < flights.size();i++){
			//si el vuelo actual termina en donde comienza el proximo vuelo
			if(flights.at(i).id != flight_delete->id){
				if(flights.at(i).aeropuerto_fin.compare( after_airport ) == 0){
					//si el vuelo es una base
					if(flights.at(i).aeropuerto_init.compare(BASE1) == 0 ||flights.at(i).aeropuerto_init.compare(BASE2) == 0 ){
					    if(after_hour.compare("")==0){
					        cout<<"ouch"<<endl;
					    }
						double timeDif = getIdleTime_two(flights.at(i).horaInicio,after_hour);
						//si respeta las reglas de la empresa 30 min entre vuelos, max 4 horas
						if( timeDif > MIN_BETWEN_FLIGHT && timeDif<MAX_BETWEN_FLIGHT){
							//podemos cambiar el vuelo. actualizar precio, tamaño y tiempo!
							double timeFlight = flights.at(i).timeFlight;
							double tmpSize = new_size + timeFlight + timeDif;
							double tmpTime = new_time + timeFlight;
							if(tmpSize > MAX_TIME_DUTY || tmpTime > MAX_TIME_FLIGHT){
								goto cnt;
							}
							double tmpPrice = new_price + timeFlight + 0.75*(timeDif);
							/*					cout<<"MEJOR? -- comparando "<< flights.at(i).id <<endl;
                                                cout<< tmpSize <<endl;
                                                cout<< tmpTime <<endl;
                                                cout<< tmpPrice <<endl;
                            */					//si es mejor lo cambiamos
							//mejor es si la diferencia entre size y time es menor;
							//tambien es mejor si el precio es menor (definitivamente)
							vector<int> chromos = soon_individual->getChromosomes();//.at(val_rand-1) = flights.at(i).id;
							chromos.at(val_rand) = flights.at(i).id;
							individual->setChromosome(chromos);
							individual->setPrice( tmpPrice );
							individual->setTime( tmpTime );
							individual->setSize( tmpSize );
							cout<<"MUTED------------------"<<endl;
							changed = 1;
							goto bye;
							}else{
								cout<<"desechado"<<endl;
							}
						}
					}
				cnt:;
			}
			cntk:;
		}
	}else if(val_rand == soon_individual->getChromosomes().size()-1){
		//getting flight and idle time
        if(flight_delete->horaInicio.compare("")==0){
            cout<<"ouch"<<endl;
        }
		double idle = getIdleTime_two(fligth_prev->horaFin,flight_delete->horaInicio);
		//	cout<<flight_delete->timeFlight<<endl;
		//	cout<<idle<<endl;
		new_size -= (idle + flight_delete->timeFlight);
		new_price -= (0.75*idle + flight_delete->timeFlight);
		new_time -= flight_delete->timeFlight;
		/*cout<<"New Vals"<<endl;
        cout<<new_size<<endl;
        cout<<new_price<<endl;
        cout<<new_time<<endl;*/

		string prev_airport =  fligth_prev->aeropuerto_fin;
		string prev_hour = fligth_prev->horaFin;
		for(int i = 0; i < flights.size();i++){
			//si el vuelo actual empieza en donde termina el vuelo anterior
			if(flights.at(i).id != flight_delete->id){
				if(flights.at(i).aeropuerto_init.compare( prev_airport ) == 0){
					//si el vuelo es una base
					if(flights.at(i).aeropuerto_fin.compare(BASE1) == 0 ||flights.at(i).aeropuerto_fin.compare(BASE2) == 0 ){
                        if(flights.at(i).horaInicio.compare("")==0){
                            cout<<"ouch"<<endl;
                        }
						double timeDif = getIdleTime_two(prev_hour, flights.at(i).horaInicio);
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
							//si es mejor lo cambiamos
							//mejor es si la diferencia entre size y time es menor;
							//tambien es mejor si el precio es menor (definitivamente)
							if( (tmpSize - tmpTime) < (sizeIndividual_init - timeIndividual_init) || tmpPrice < priceIndividual_init ){
								//mejor

								vector<int> chromos = soon_individual->getChromosomes();//.at(val_rand-1) = flights.at(i).id;
								chromos.at(val_rand) = flights.at(i).id;
								individual->setChromosome(chromos);
//										soon_individual->getChromosomes().at(val_rand-1) = flights.at(i).id;
								individual->setPrice( tmpPrice );
								individual->setTime( tmpTime );
								individual->setSize( tmpSize );
								cout<<"MUTED------------------"<<endl;
								changed = 1;
								goto bye;
							}else{
								cout<<"desechado"<<endl;
							}
						}
					}
				}
				cnt_skip:;
			}
		}
	}else{
        if(flight_delete->horaInicio.compare("")==0){
            cout<<"ouch"<<endl;
        }
		double idle_prev = getIdleTime_two(fligth_prev->horaFin,flight_delete->horaInicio);
        if(flight_after->horaInicio.compare("")==0){
            cout<<"ouch"<<endl;
        }
		double idle_after = getIdleTime_two(flight_delete->horaFin,flight_after->horaInicio);
		new_size -= idle_prev+idle_after+flight_delete->timeFlight;
		new_price -= (0.75*(idle_prev+idle_after) + flight_delete->timeFlight);
		new_time -= flight_delete->timeFlight;
		string prev_airport =  fligth_prev->aeropuerto_fin;
		string prev_hour = fligth_prev->horaFin;
		string after_airport = flight_after->aeropuerto_init;
		string after_hour = flight_after->horaInicio;

		for(int i = 0; i < flights.size();i++){
			if(flights.at(i).id != flight_delete->id){
				//si el vuelo actual empieza en donde termina el vuelo anterior
				// Y si termina donde comienza el siguiente AFTERvuelo
				if(flights.at(i).aeropuerto_init.compare( prev_airport ) == 0 && flights.at(i).aeropuerto_fin.compare( after_airport ) == 0 ){
					//si el vuelo es una base no importa
                    if(flights.at(i).horaInicio.compare("")==0){
                        cout<<"ouch"<<endl;
                    }
					double timeDif_prev = getIdleTime_two(prev_hour, flights.at(i).horaInicio);
                    if(after_hour.compare("")==0){
                        cout<<"ouch"<<endl;
                    }
                    double timeDif_after = getIdleTime_two( flights.at(i).horaFin,after_hour);

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

						//si es mejor lo cambiamos
						//mejor es si la diferencia entre size y time es menor;
						//tambien es mejor si el precio es menor (definitivamente)
						if( (tmpSize - tmpTime) < (sizeIndividual_init - timeIndividual_init) || tmpPrice < priceIndividual_init ){
							//mejor

							vector<int> chromos = soon_individual->getChromosomes();//.at(val_rand-1) = flights.at(i).id;
							chromos.at(val_rand) = flights.at(i).id;
							individual->setChromosome(chromos);
//									soon_individual->getChromosomes().at(val_rand-1) = flights.at(i).id;
							individual->setPrice( tmpPrice );
							individual->setTime( tmpTime );
							individual->setSize( tmpSize );
							cout<<"MUTED------------------"<<endl;
							changed = 1;
							goto bye;
						}
					}

				}
				cnt_skip2:;
			}
		}
	}
	bye:;
	return changed;
}