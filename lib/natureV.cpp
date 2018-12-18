#include "nature.hpp"
#include "operators.hpp"
#include "utils.cpp"
#include<iostream>
#include <string.h>
#include <algorithm>
using namespace std;


bool Nature::exists(vector<int> usados, int position){
	 bool used = false;
	 if(usados.at(position) > 0){
		  return true;
	 }else{
		  return false;
	 }
}


bool Nature::validFlight(vector<int> chromosomes, int position){
	 int id = chromosomes.at( chromosomes.size() - 1);
	 Flight last = agency.getFlights().at(id-1);
	 Flight actual = agency.getFlights().at(position);
	 if( compareHourV2(last.horaFin, actual.horaInicio) == 1){
		  return true;
	 }else{
		  return false;
	 }
}

bool exist_in_generation(vector<Individual> pre_individuals, vector<int> chromosomes){
	for(int i = 0; i < pre_individuals.size() ; i++){
		if(pre_individuals.at(i).getChromosomes().at(0) == chromosomes.at(0)){
			bool exist = true;
			if(pre_individuals.at(i).getChromosomes().size() == chromosomes.size()){
                goto equals;
			}else if(chromosomes.size() > pre_individuals.at(i).getChromosomes().size()){
                equals:;
			    for( int j = 1; j< pre_individuals.at(i).getChromosomes().size(); j++ ){
                    if(pre_individuals.at(i).getChromosomes().at(j) == chromosomes.at(j)){
                        exist = true;
                    }else{
                        exist = false;
                    }
                }
                if(exist == true){
                    return true;
                }
			} else{
                for( int j = 1; j< chromosomes.size(); j++ ){
                    if(pre_individuals.at(i).getChromosomes().at(j) == chromosomes.at(j)){
                        exist = true;
                    }else{
                        exist = false;
                    }
                }
                if(exist == true){
				 cout<<exist<<endl;
                    return true;
                }
            }
		}
	}
	return false;
}

//main function of librarie Nature

vector<Individual> Nature::getGreedyIndividual(vector<Individual> prev_individuals,int id_flight_start,vector<int>* usados,int repare=0){
	vector<Individual> output = vector<Individual>();
	Flight new_flight = agency.getFlights().at(id_flight_start-1);
	vector<int> cpy_used = *usados;
	if(check_base(new_flight.aeropuerto_init) == 0) {
		string airport_llegada = new_flight.aeropuerto_fin;

		vector<int> new_chromosomes = vector<int>();
		new_chromosomes.push_back(id_flight_start);

		double size = 0.0;
		double price = 0.0;
		double time = 0.0;

		//buscar un proximo vuelo para hacer duties
		if(usados->at(id_flight_start-1) > 0){ //vuelo usado cmo max 1 vez (set packing)
			new_chromosomes.clear();
			return {};
		}else{
		   usados->at(id_flight_start-1) += 1;
		}

		size = new_flight.timeFlight;
		time = new_flight.timeFlight;
		price = new_flight.timeFlight;

		// look for next available from arrived airport
		// take order to time and rules.
		//  we suposed 15 chromosomes max in duty
		for(int a = 0; a < 5; a++) 
		{ //for each chromosome setted to 7 given a max duty time of 12 hours
			bool found = false;
			for (int i = 0; i < agency.getFlights().size(); i++) 
			{
				if (airport_llegada.compare(agency.getFlights().at(i).aeropuerto_init) == 0) 
				{
					// prove if the currect fligth its used for current duty
					//cout<< agency.getFlights().at(i).id<<endl;
					bool used = exists(*usados, i); //if exist

					bool valid = validFlight(new_chromosomes, i);   //if take order to rules of time
					//si no fue usado lo usamos

					if (used != true && valid) 
					{
						vector<int> tmp2 = new_chromosomes;
						tmp2.push_back(i+1);
						bool exists = exist_in_generation(prev_individuals,tmp2);
						if(!exists)
						{
							found = true;
							usados->at(agency.getFlights().at(i).id - 1) += 1;
							cpy_used.at(agency.getFlights().at(i).id - 1) += 1;
							new_chromosomes.push_back(agency.getFlights().at(i).id);
							
							string hora_prev, hora_now;
							
							hora_prev = agency.getFlights().at(new_chromosomes.at(new_chromosomes.size() - 2) - 1).horaFin;
							hora_now = agency.getFlights().at(new_chromosomes.at(new_chromosomes.size() - 1) - 1).horaInicio;
							//update Costs 
							double tmpIdle = getIdleTime(hora_prev, hora_now);
							double tmpTime = agency.getFlights().at(new_chromosomes.at(new_chromosomes.size() - 1) - 1).timeFlight;
							size += tmpIdle + tmpTime;
							time += tmpTime;
							price += 0.75 * tmpIdle + tmpTime;

							//if (check_base(agency.getFlights().at(i).aeropuerto_fin) == 0) goto help;

							airport_llegada.clear();
							airport_llegada = agency.getFlights().at(i).aeropuerto_fin;
							break;
						}
					}
				}
			}
		}
		help:;
		if(size > MAX_TIME_DUTY){
			while(size>MAX_TIME_DUTY){
				double idle_tiny = getIdleTime(
													agency.getFlights().at(new_chromosomes.at(new_chromosomes.size()-2)-1).horaFin ,
													agency.getFlights().at(new_chromosomes.at(new_chromosomes.size()-1)-1).horaInicio
											); 
				double time_tiny = agency.getFlights().at(new_chromosomes.at(new_chromosomes.size()-1)-1).timeFlight;
				size -= idle_tiny + time_tiny;
				time -= time_tiny;
				price -= 0.75*idle_tiny + time_tiny;
				usados->at(new_chromosomes.at(new_chromosomes.size()-1)-1)--;
				new_chromosomes.pop_back();
			}
		}

		int flag2 = 0;
		string last_airport = agency.getFlights().at(new_chromosomes.at(new_chromosomes.size()-1)-1).aeropuerto_fin;
		if( check_base(last_airport) == 0 ){
			flag2 = 1;
		}
		if(flag2==0){
			while( check_base(last_airport) == 1){
				 //nos quedamos en z
				 if(new_chromosomes.size() > 1){
					double tmpIdle = getIdleTime(
														agency.getFlights().at(new_chromosomes.at(new_chromosomes.size()-2)-1).horaFin ,
														agency.getFlights().at(new_chromosomes.at(new_chromosomes.size()-1)-1).horaInicio
												); 
					double tmpTime = agency.getFlights().at(new_chromosomes.at(new_chromosomes.size()-1)-1).timeFlight; 
					size -= tmpIdle;
					size -= tmpTime;
					time -= tmpTime;
					price -= 0.75*tmpIdle + tmpTime;
					usados->at(new_chromosomes.at(new_chromosomes.size()-1)-1)--;
					new_chromosomes.pop_back();
					last_airport =  agency.getFlights().at(new_chromosomes.at(new_chromosomes.size()-1)-1).aeropuerto_fin;
				 }else{
					 return {};
				 }
			}
		}
		output.push_back(Individual(size, time,price,0,new_chromosomes));
		return output;
	}else{
		return {};
	}
}



void updateUsed(vector<int>* used, vector<Individual> individues){
	int num_flights = used->size();
	used->clear();
	for(int i = 0 ; i<num_flights;i++) used->push_back(0);
	for(int a = 0; a < individues.size();a++){ //for each individual
		Individual tmpIndividual = individues.at(a);
		for(int b = 0; b<tmpIndividual.getChromosomes().size();b++){
			used->at(tmpIndividual.getChromosomes().at(b)-1) +=1;
		}
	}
}

void Nature::compressGeneration(vector<Individual>* generation) {
	//por cada conjunto de vuelos ( por cada individuo ) buscar otro conjunto de vuelo que los conecte repetando las reglas
	vector<Individual> new_generation = vector<Individual>();
	vector<int> used = vector<int>();
	for (int i = 0; i < generation->size(); i++) {
		used.push_back(0);
	}
	new_generation;
	for (int i = 0; i < generation->size(); i++) {
		if (used.at(i) != 1) {
			vector<int> pivoteChromosomes = vector<int>();
			double size = generation->at(i).getSize();
			double time = generation->at(i).getTime();
			double price = generation->at(i).getPrice();
			pivoteChromosomes = generation->at(i).getChromosomes();
			//vamos a buscar otro individuo de la generacion que nos sirva
			for (int j = 0; j < generation->size(); j++) {
				vector<int> tmpChromosomes = vector<int>();
				tmpChromosomes = generation->at(j).getChromosomes();
				if (tmpChromosomes.at(0) != pivoteChromosomes.at(0)) //si no son iguales o si j!=i
				{
					//si el aerop. de fin del primero es igual al inicio del segundo
					string aero_fin_first = agency.getFlights().at(
							pivoteChromosomes.at(pivoteChromosomes.size() - 1) - 1).aeropuerto_fin;
					string aero_begin_last = agency.getFlights().at(tmpChromosomes.at(0) - 1).aeropuerto_init;
					bool valid = validFlight(pivoteChromosomes, tmpChromosomes.at(0)-1);
					if (aero_fin_first.compare(aero_begin_last) == 0 && valid && used.at(j) != 1) {
						// checkear si respeta reglas de tiempo
						double idleTmp = getIdleTime(
								agency.getFlights().at(pivoteChromosomes.at(pivoteChromosomes.size() - 1) - 1).horaFin,
								agency.getFlights().at(tmpChromosomes.at(0) - 1).horaInicio);
						double sizeTmp = generation->at(j).getSize() + idleTmp;
						double timeTmp = generation->at(j).getTime();
						double priceTmp = generation->at(j).getPrice() + 0.75 * idleTmp;
						if (sizeTmp + size > MAX_TIME_DUTY || timeTmp + time > MAX_TIME_FLIGHT) {
							break;
						} else {
							//unir
							//cout << "unir" << endl;
							vector<int> newChromosome = vector<int>();
							for (int i = 0; i < pivoteChromosomes.size(); i++) {
								newChromosome.push_back(pivoteChromosomes.at(i));
							}
							for (int i = 0; i < tmpChromosomes.size(); i++) {
								newChromosome.push_back(tmpChromosomes.at(i));
							}
							sizeTmp += size;
							priceTmp += price;
							timeTmp += time;
							used.at(i) = 1;
							used.at(j) = 1;
							new_generation.push_back(Individual(sizeTmp, timeTmp, priceTmp, 0, newChromosome));
							break;
						}
					}
				}
			}
		}
	}
	for(int i = 0; i < used.size();i++){
		if(used.at(i) == 0){
			new_generation.push_back(generation->at(i));
		}
	}
	generation->clear();
	*generation = new_generation;
}

void Nature::repare(vector<int> *usados, vector<Individual> *individuals) {
	vector<int> ids_flights_notUseds = vector<int>();
	for(int i = 0; i < usados->size(); i++) if(usados->at(i) == 0) ids_flights_notUseds.push_back(i+1);
	//cout<<ids_flights_notUseds.size()<<endl;
	for(int i = 0; i < ids_flights_notUseds.size();i++)
	{
		//cout<<ids_flights_notUseds.at(i)<<endl;
		Flight objectiveFlight = agency.getFlights().at(ids_flights_notUseds.at(i)-1);
		string airport_inicio = objectiveFlight.aeropuerto_init;
		string airport_fin = objectiveFlight.aeropuerto_fin;
		string hora_inicio = objectiveFlight.horaInicio;
		string hora_fin = objectiveFlight.horaFin;
		
		vector<int>  chromosomes = vector<int>();
		chromosomes.push_back(ids_flights_notUseds.at(i));

		//tres casos;
		if(check_base(airport_inicio) == 0){ // airport  inicio es base, buscamos un vuelo que comienze en airport fin
			usados->at( ids_flights_notUseds.at(i) -1) += 1;
			for(int j = 0; j < agency.getFlights().size(); j++){
				//un vuelo que comienze en airport_fin
				if(agency.getFlights().at(j).aeropuerto_init.compare(airport_fin) == 0){
					if(check_base(agency.getFlights().at(j).aeropuerto_fin) == 0){
						//es una base podemos finalizar el duty.

						bool valid =  validFlight(chromosomes,j);
						if(valid){
							chromosomes.push_back( agency.getFlights().at(j).id );
							usados->at( j ) += 1;
							break;
						}
					}
				}
			}/*
			for(int k = 0; k < chromosomes.size(); k++ ){
				cout <<chromosomes.at(k) << " ";
			}*/
			//cout<<endl;
		}else if(check_base(airport_fin) == 0){
			//cout<<"preparacion"<<endl;
			usados->at( ids_flights_notUseds.at(i) -1) += 1;
			for(int j = 0; j < agency.getFlights().size(); j++){
				//un vuelo que comienze en airport_fin
				if(agency.getFlights().at(j).aeropuerto_fin.compare(airport_inicio) == 0){
					if(check_base(agency.getFlights().at(j).aeropuerto_init) == 0){
						//es una base podemos finalizar el duty.
						//solo si respeta reglas
						int lastChrom = chromosomes.at(chromosomes.size()-1);
						chromosomes.pop_back();
						chromosomes.push_back(j+1);
						bool valid =  validFlight(chromosomes,ids_flights_notUseds.at(i)-1);
						chromosomes.pop_back();
						chromosomes.push_back(lastChrom);
						if(valid){
							chromosomes.push_back( agency.getFlights().at(j).id );
							usados->at( j) += 1;
							break;
						}
					}
				}
			}
			std::reverse(chromosomes.begin(),chromosomes.end());
			/*for(int k = 0; k < chromosomes.size(); k++ ){
				cout <<chromosomes.at(k) << "-";
			}
			cout<<endl;*/
		}

		//calcular precio del cromosoma creado. ya esta validado idletime
		double time = 0.0;
		double size = 0.0;
		double price = 0.0;
		for(int k = 0; k < chromosomes.size(); k++ ){
//			cout <<chromosomes.at(k) << " ";
			Flight tmpFlight = agency.getFlights().at(chromosomes.at(k)-1);
			time += tmpFlight.timeFlight;
			price += tmpFlight.timeFlight;
			size += tmpFlight.timeFlight;
			if(k != 0){
				string hourPrev = agency.getFlights().at(chromosomes.at(k-1) -1).horaFin;
				string hourRecent = tmpFlight.horaInicio;
				double tmpIdle = getIdleTime(hourPrev,hourRecent);
				//cout<<tmpIdle<<endl;
				price += 0.75*tmpIdle;
				size += tmpIdle;
			}
		}
//		cout<<endl;
		if(!exist_in_generation(*individuals,chromosomes))  individuals->push_back( Individual(size, time,price,0,chromosomes));
		/*cout<<"size ->"<<size<<endl;
		cout<<"price ->"<<price<<endl;
		cout<<"time ->"<<time<<endl;
*/
	}
	//exit(299); 
}

bool existNotUsed(vector<int> useds){
	for(int i = 0; i < useds.size(); i++){
		if(useds.at(i) == 0) return true;
	}
	return false;
}

Population Nature::makePopulation(int numGeneration, int numIndividuals){
	Population new_gen = Population();
	vector<Individual>* individues = new vector<Individual>;

	vector<int> usados = vector<int>(); //usados en blanco
	for(int i = 0 ; i<agency.getFlights().size();i++) usados.push_back(0);

	struct timespec ts;

	bool flag = true;
	int count = 0;
	while(count < 100){
		anta:;
		clock_gettime(CLOCK_MONOTONIC, &ts);
		srand((time_t) ts.tv_nsec);
		int probability = ( std::rand() % (  agency.getFlights().size() + 1  ));
		if(probability == 0 || probability > agency.getFlights().size()) goto anta;
		//cout<<probability<<endl;
		vector<Individual> indi = getGreedyIndividual(*individues,probability,&usados,0);
		if(indi.size() != 0){
			for(int j = 0; j < indi.size();j++){
                compressGeneration(&indi);
                if(!exist_in_generation(*individues,indi.at(j).getChromosomes())){
                    individues->push_back(indi.at(j)); //push individue
                }
			}
		}
		count++;
		updateUsed(&usados,*individues);
		flag = existNotUsed(usados);
	}

	for(int i = 0; i  < usados.size();i++){
		printf("%d ",usados.at(i));
	}
	cout<<endl;
	compressGeneration(individues);
	updateUsed(&usados,*individues);
	//TODO repair solution, must have all flights although has repited fligths
	repare(&usados,individues);
	for(int i = 0; i  < usados.size();i++){
		printf("%d ",usados.at(i));
	}
	cout<<endl;
	cout<<"----------------------------"<<endl;
	new_gen.generation = *individues;
	operators.getFitness(&new_gen,agency.getFlights().size());
	return new_gen;
};

void Nature::showGeneration(int number_gen){
	 cout<<"o---------------------------------------------------------------------o"<<endl;
	 cout<<"o---------------------------------------------------------------------o"<<endl;
	 cout<<"o--------------------Showing Generation Number "<<number_gen<<" ---------------------o"<<endl;
	 cout<<"o---------------------------------------------------------------------o"<<endl;
	 cout<<"o---------------------------------------------------------------------o"<<endl;

	vector<Population> allSolutions =  population.at(number_gen);
	for( int j = 0; j < allSolutions.size(); j++){ //por cada solucion de la generacion
		cout<<"solution -> "<<j<<" in generation"<<number_gen<<endl;
		Population one_solution = allSolutions.at(j);
		for(int z = 0; z < one_solution.generation.size();z++){
			cout <<"Duty -> "<<z<<" <------> ";
			for(int x = 0; x < one_solution.generation.at(z).getChromosomes().size();x++){
				cout<<one_solution.generation.at(z).getChromosomes().at(x)<<"-";
			}
			cout<<"  --- {price -> "<<one_solution.generation.at(z).getPrice() <<"}"<<endl;
			cout<<endl;
		}
		cout<<"Caracteristics of pairing -----------------------------o"<<endl;
		cout<<" - - > Price Pairing: $  "<<     one_solution.price<<endl;
		cout<<" - - > fitness value :  "<<		one_solution.fitness<<endl;
	}
};

void Nature::showResume(){
	 cout<<"o---------------------------------------------------------------------o"<<endl;
	 cout<<"o---------------------------------------------------------------------o"<<endl;
	 cout<<"o--------------------Showing all population---------------------o"<<endl;
	 cout<<"o---------------------------------------------------------------------o"<<endl;
	 cout<<"o---------------------------------------------------------------------o"<<endl;

	for(int i = 0; i < population.size(); i++){
		vector<Population> tmpPopulation =  population.at(i);
		for( int j = 0; j < tmpPopulation.size(); j++){
			Population one_solution = tmpPopulation.at(j);
			for(int z = 0; z < one_solution.generation.size();z++){
				for(int x = 0; x < one_solution.generation.at(z).getChromosomes().size();x++){
					cout<<one_solution.generation.at(z).getChromosomes().at(x)<<" ";
				}
				cout<<"||";
			}
			cout<<"price -> "<<tmpPopulation.at(j).price<< "fitness -> "<<tmpPopulation.at(j).fitness<<endl;
			cout<<endl;
		}
	}
} 

void Nature::repareSolution(Population* pairing){
	vector<int> useds = vector<int>();
	for(int i = 0; i < agency.getFlights().size();i++){
		useds.push_back(0);
	}

	for(int i = 0; i < pairing->generation.size(); i++){
		for(int a = 0; a < pairing->generation.at(i).getChromosomes().size();a++){
			cout<< pairing->generation.at(i).getChromosomes().at(a)<<"--";
			useds.at(pairing->generation.at(i).getChromosomes().at(a) -1) +=1;
		}
		cout<<" || ";
	}
	cout<<endl;

	for(int i = 0; i < agency.getFlights().size();i++){
		cout<<useds.at(i)<<" ";
	}
	cout<<endl;	
	repare(&useds, &pairing->generation);
	useds.clear();
	for(int i = 0; i < agency.getFlights().size();i++){
		useds.push_back(0);
	}
	for(int i = 0; i < pairing->generation.size(); i++){
		for(int a = 0; a < pairing->generation.at(i).getChromosomes().size();a++){
			cout<< pairing->generation.at(i).getChromosomes().at(a)<<"--";
			useds.at(pairing->generation.at(i).getChromosomes().at(a) -1) +=1;
		}
		cout<<" || ";
	}
	cout<<endl;

	for(int i = 0; i < agency.getFlights().size();i++){
		cout<<useds.at(i)<<" ";
	}
}

bool compareCromosomes(vector<int> cromosomasOne, vector<int> cromosomasTwo){
	if(cromosomasOne.size() != cromosomasTwo.size()){
		return false;
	}else{
		for(int i = 0; i < cromosomasOne.size(); i++){
			if(cromosomasOne.at(i) != cromosomasTwo.at(i)){
				return false;
			}
		}
	}
	return true;
}

void Nature::deleteDuplicate(Population* pairing){
	vector<Individual> individues = pairing->generation;
	vector<int> position_delete = vector<int>();
	for(int i = 0; i < individues.size(); i++){ //por cada individuo o duty
		vector<int> cromosomasPivote =  individues.at(i).getChromosomes();
		for(int j = 0; j < individues.size();j++){
			if(i!=j){
				if(compareCromosomes(cromosomasPivote, individues.at(j).getChromosomes())){
					//delete one of two
					position_delete.push_back(j);
				}				
			}
		}
	}
	for(int i = 0; i < position_delete.size(); i++){
		if(i > 0){
			pairing->generation.erase( pairing->generation.begin() +  position_delete.at(i) - 1);	
		}
		pairing->generation.erase( pairing->generation.begin() +  position_delete.at(i));
	}
}