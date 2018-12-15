#include "nature.hpp"
#include "operators.hpp"
#include "utils.cpp"
#include<iostream>
#include <string.h>


bool Nature::exists(vector<int> usados, int position){
	 bool used = false;
	 if(usados.at(position) > 3){
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
		if(usados->at(id_flight_start-1) > 3){ //vuelo usado cmo max 2 veces
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
		for(int a = 0; a < 4; a++) { //for each chromosome setted to 7 given a max duty time of 12 hours
			bool found = false;
			for (int i = 0; i < agency.getFlights().size(); i++) {
				if (airport_llegada.compare(agency.getFlights().at(i).aeropuerto_init) == 0) {
					// prove if the currect fligth its used for current duty
					//cout<< agency.getFlights().at(i).id<<endl;
                    bool used = exists(*usados, i); //if exist

                    bool valid = validFlight(new_chromosomes, i);   //if take order to rules of time
                    //si no fue usado lo usamos

                    if (used != true && valid) {
                        vector<int> tmp2 = new_chromosomes;
                        tmp2.push_back(i+1);
                        cout << "base" << endl;
                        bool exists = exist_in_generation(prev_individuals,tmp2);
                        cout << "base2" << endl;
                        if(!exists){
                            found = true;
                            usados->at(agency.getFlights().at(i).id - 1) += 1;
                            cpy_used.at(agency.getFlights().at(i).id - 1) += 1;
                            new_chromosomes.push_back(agency.getFlights().at(i).id);

							string hora_prev, hora_now;
							hora_prev = agency.getFlights().at(new_chromosomes.at(new_chromosomes.size() - 2) - 1).horaFin;
							hora_now = agency.getFlights().at(
									new_chromosomes.at(new_chromosomes.size() - 1) - 1).horaInicio;

							double tmp = getIdleTime(hora_prev, hora_now);

							size += tmp +
									agency.getFlights().at(new_chromosomes.at(new_chromosomes.size() - 1) - 1).timeFlight;
							time += agency.getFlights().at(new_chromosomes.at(new_chromosomes.size() - 1) - 1).timeFlight;
							price += 0.75 * tmp +
									 agency.getFlights().at(new_chromosomes.at(new_chromosomes.size() - 1) - 1).timeFlight;

							if (check_base(agency.getFlights().at(i).aeropuerto_fin) == 0) goto help;

							//cout<<"chromosme added"<<endl;
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
				size -= getIdleTime(agency.getFlights().at(new_chromosomes.at(new_chromosomes.size()-2)-1).horaFin,agency.getFlights().at(new_chromosomes.at(new_chromosomes.size()-1)-1).horaInicio);
				size -= agency.getFlights().at(new_chromosomes.at(new_chromosomes.size()-1)-1).timeFlight;
				time -= agency.getFlights().at(new_chromosomes.at(new_chromosomes.size()-1)-1).timeFlight;
				price -= 0.75*getIdleTime(agency.getFlights().at(new_chromosomes.at(new_chromosomes.size()-2)-1).horaFin,agency.getFlights().at(new_chromosomes.at(new_chromosomes.size()-1)-1).horaInicio) +
						agency.getFlights().at(new_chromosomes.at(new_chromosomes.size()-1)-1).timeFlight;
				usados->at(new_chromosomes.at(new_chromosomes.size()-1)-1)--;
				new_chromosomes.pop_back();
			}
		}

		int flag2 = 0;
		if( agency.getFlights().at(new_chromosomes.at(new_chromosomes.size()-1)-1).aeropuerto_fin.compare(BASE1)==0 || agency.getFlights().at(new_chromosomes.at(new_chromosomes.size()-1)-1).aeropuerto_fin.compare(BASE2)==0 ){
		 flag2=1;
		}
		if(flag2==0){
			while(agency.getFlights().at(new_chromosomes.at(new_chromosomes.size()-1)-1).aeropuerto_fin.compare(BASE1)==0 || agency.getFlights().at(new_chromosomes.at(new_chromosomes.size()-1)-1).aeropuerto_fin.compare(BASE2)==0 ){
				 //nos quedamos en z
				 size -= getIdleTime(agency.getFlights().at(new_chromosomes.at(new_chromosomes.size()-2)-1).horaFin,agency.getFlights().at(new_chromosomes.at(new_chromosomes.size()-1)-1).horaInicio);
				 size -= agency.getFlights().at(new_chromosomes.at(new_chromosomes.size()-1)-1).timeFlight;time -= agency.getFlights().at(new_chromosomes.at(new_chromosomes.size()-1)-1).timeFlight;
				 price -= 0.75*getIdleTime(agency.getFlights().at(new_chromosomes.at(new_chromosomes.size()-2)-1).horaFin,agency.getFlights().at(new_chromosomes.at(new_chromosomes.size()-1)-1).horaInicio) +
						  agency.getFlights().at(new_chromosomes.at(new_chromosomes.size()-1)-1).timeFlight;
				 usados->at(new_chromosomes.at(new_chromosomes.size()-1)-1)--;
				 new_chromosomes.pop_back();
			}
		}

		if(check_base( agency.getFlights().at(new_chromosomes.at(new_chromosomes.size()-1)-1).aeropuerto_fin) != 0){
		 return {};
		}
		output.push_back(Individual(size, time,price,0,new_chromosomes));
		return output;
	}else{
		return {};
	}
}


vector<Individual> Nature::getGreedyIndividual(int id_flight_start,vector<int>* usados,int repare=0){
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
		if(usados->at(id_flight_start-1) > 6){ //vuelo usado cmo max 2 veces
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
		for(int a = 0; a < 4; a++) { //for each chromosome setted to 7 given a max duty time of 12 hours
			bool found = false;
			for (int i = 0; i < agency.getFlights().size(); i++) {
				if (airport_llegada.compare(agency.getFlights().at(i).aeropuerto_init) == 0) {
					// prove if the currect fligth its used for current duty
					//cout<< agency.getFlights().at(i).id<<endl;
					bool used = exists(*usados, i); //if exist

					bool valid = validFlight(new_chromosomes, i);   //if take order to rules of time

					//si no fue usado lo usamos

					if (used != true && valid) {
						found = true;
						cout << "base" << endl;
						usados->at(agency.getFlights().at(i).id - 1) += 1;
						cpy_used.at(agency.getFlights().at(i).id - 1) += 1;
						new_chromosomes.push_back(agency.getFlights().at(i).id);

						string hora_prev, hora_now;
						hora_prev = agency.getFlights().at(new_chromosomes.at(new_chromosomes.size() - 2) - 1).horaFin;
						hora_now = agency.getFlights().at(
								new_chromosomes.at(new_chromosomes.size() - 1) - 1).horaInicio;

						double tmp = getIdleTime(hora_prev, hora_now);

						size += tmp +
								agency.getFlights().at(new_chromosomes.at(new_chromosomes.size() - 1) - 1).timeFlight;
						time += agency.getFlights().at(new_chromosomes.at(new_chromosomes.size() - 1) - 1).timeFlight;
						price += 0.75 * tmp +
								 agency.getFlights().at(new_chromosomes.at(new_chromosomes.size() - 1) - 1).timeFlight;

						if (check_base(agency.getFlights().at(i).aeropuerto_fin) == 0) goto help;

						//cout<<"chromosme added"<<endl;
						airport_llegada.clear();
						airport_llegada = agency.getFlights().at(i).aeropuerto_fin;
						break;
					}
				}
			}
			if(repare==1){
				if(found == false){
					return {};
				}
			}
		}
		help:;
		if(size > MAX_TIME_DUTY){
			while(size>MAX_TIME_DUTY){
				size -= getIdleTime(agency.getFlights().at(new_chromosomes.at(new_chromosomes.size()-2)-1).horaFin,agency.getFlights().at(new_chromosomes.at(new_chromosomes.size()-1)-1).horaInicio);
				size -= agency.getFlights().at(new_chromosomes.at(new_chromosomes.size()-1)-1).timeFlight;
				time -= agency.getFlights().at(new_chromosomes.at(new_chromosomes.size()-1)-1).timeFlight;
				price -= 0.75*getIdleTime(agency.getFlights().at(new_chromosomes.at(new_chromosomes.size()-2)-1).horaFin,agency.getFlights().at(new_chromosomes.at(new_chromosomes.size()-1)-1).horaInicio) +
						 agency.getFlights().at(new_chromosomes.at(new_chromosomes.size()-1)-1).timeFlight;
				usados->at(new_chromosomes.at(new_chromosomes.size()-1)-1)--;
				new_chromosomes.pop_back();
			}
		}

		int flag2 = 0;
		if( agency.getFlights().at(new_chromosomes.at(new_chromosomes.size()-1)-1).aeropuerto_fin.compare(BASE1)==0 || agency.getFlights().at(new_chromosomes.at(new_chromosomes.size()-1)-1).aeropuerto_fin.compare(BASE2)==0 ){
			flag2=1;
		}
		if(flag2==0){
			while(agency.getFlights().at(new_chromosomes.at(new_chromosomes.size()-1)-1).aeropuerto_fin.compare(BASE1)==0 || agency.getFlights().at(new_chromosomes.at(new_chromosomes.size()-1)-1).aeropuerto_fin.compare(BASE2)==0 ){
				//nos quedamos en z
				size -= getIdleTime(agency.getFlights().at(new_chromosomes.at(new_chromosomes.size()-2)-1).horaFin,agency.getFlights().at(new_chromosomes.at(new_chromosomes.size()-1)-1).horaInicio);
				size -= agency.getFlights().at(new_chromosomes.at(new_chromosomes.size()-1)-1).timeFlight;time -= agency.getFlights().at(new_chromosomes.at(new_chromosomes.size()-1)-1).timeFlight;
				price -= 0.75*getIdleTime(agency.getFlights().at(new_chromosomes.at(new_chromosomes.size()-2)-1).horaFin,agency.getFlights().at(new_chromosomes.at(new_chromosomes.size()-1)-1).horaInicio) +
						 agency.getFlights().at(new_chromosomes.at(new_chromosomes.size()-1)-1).timeFlight;
				usados->at(new_chromosomes.at(new_chromosomes.size()-1)-1)--;
				new_chromosomes.pop_back();
			}
		}

		if(check_base( agency.getFlights().at(new_chromosomes.at(new_chromosomes.size()-1)-1).aeropuerto_fin) != 0){
			return {};
		}

		output.push_back(Individual(size, time,price,0,new_chromosomes));
		if(repare == 3){
			vector<Individual> indTmp2 = getGreedyIndividual(id_flight_start,&cpy_used,1);
			if(indTmp2.empty() == false){
				for(int i = 0; i < indTmp2.size();i++){
					output.push_back(indTmp2.at(i));
				}
			}
			cout<<"analisis"<<endl;
		}
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
						if (sizeTmp + size > MAX_TIME_DUTY) {
							cout << "no sirve" << endl;
						} else if (timeTmp + time > MAX_TIME_FLIGHT) {
							cout << "no sirve" << endl;
							break;
						} else {
							//unir
							cout << "unir" << endl;
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
							cout << time + timeTmp << endl;
							cout << size + sizeTmp << endl;
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

}

bool existNotUsed(vector<int> useds){
	for(int i = 0; i < useds.size(); i++){
		if(useds.at(i) == 0) return true;
	}
	return false;
}

void Nature::makePopulation(int numGeneration, int numIndividuals){
	Population new_gen = Population();
	vector<Individual>* individues = new vector<Individual>;

	vector<int> usados = vector<int>(); //usados en blanco
	for(int i = 0 ; i<agency.getFlights().size();i++) usados.push_back(0);

	struct timespec ts;

	bool flag = true;
	while(flag == true){
		anta:;
		clock_gettime(CLOCK_MONOTONIC, &ts);
		srand((time_t) ts.tv_nsec);
		int probability = ( std::rand() % (  agency.getFlights().size() + 1  ));
		if(probability == 0 || probability > agency.getFlights().size()) goto anta;
		cout<<probability<<endl;
		vector<Individual> indi = getGreedyIndividual(*individues,probability,&usados,0);
		if(indi.size() != 0){
			for(int j = 0; j < indi.size();j++){
                compressGeneration(&indi);
                if(!exist_in_generation(*individues,indi.at(j).getChromosomes())){
                    individues->push_back(indi.at(j)); //push individue
                }
			}
		}
		updateUsed(&usados,*individues);
		flag = existNotUsed(usados);
	}
	compressGeneration(individues);
    compressGeneration(individues);
	updateUsed(&usados,*individues);
	//TODO getGreedyIndividual vector of individues availables from one base. -> generacion por columnas no me sirve!
	new_gen.generation = *individues;
	operators.getFitness(&new_gen,agency.getFlights().size());
	population.push_back(new_gen);
/*

	 for(int i = 0;i<agency.getFlights().size()-1;i++){
		  /*if(individues->size() == numIndividuals){
			   break;
		  }
		  clock_gettime(CLOCK_MONOTONIC, &ts);
		  srand((time_t) ts.tv_nsec);
		  ant:;
		  int probability = ( std::rand() % ( agency.getFlights().size() ) );
		  if(probability == 0) goto ant;
		  while(probability > agency.getFlights().size()){
			clock_gettime(CLOCK_MONOTONIC, &ts);
			srand((time_t) ts.tv_nsec);
			probability = ( rand() % 100);
		  }
		  //vector<Individual> indi = getGreedyIndividual(i+1,&usados,0);
		  vector<Individual> indi = getGreedyIndividual(probability,&usados,0);
		  if(indi.size() > 0){
			  for(int j = 0; j < indi.size();j++){
				  individues->push_back(indi.at(j)); //push individue
			  }
		  }
	 }
	 //process of selection

	 updateUsed(&usados,*individues);
	 compressGeneration(individues);
	 updateUsed(&usados,*individues);
	 //TODO getGreedyIndividual vector of individues availables from one base. -> generacion por columnas no me sirve!
	 new_gen.generation = *individues;
	 operators.getFitness(&new_gen,agency.getFlights().size());
	 population.push_back(new_gen);

   /*  struct timespec ts;
	 vector<Individual> new_gen_mutata = vector<Individual>();
	 for(int i = 0; i < new_gen.generation.size();i++){
		 clock_gettime(CLOCK_MONOTONIC, &ts);
		 srand((time_t) ts.tv_nsec);
		 double probability = ((double) rand() / (RAND_MAX));
		 Individual indTmp = Individual(0);
		 indTmp = operators.mutate(&population.at(0).generation.at(i),
								   agency.getFlights(), probability);
		new_gen_mutata.push_back(indTmp);
	 }
	 Population popu = Population();
	 popu.generation = new_gen_mutata;
	 population.push_back(popu);
	*/
//	 showGeneration(0);
  //  showGeneration(1);
//	exit(2);

};

void Nature::showGeneration(int number_gen){
	 cout<<"o---------------------------------------------------------------------o"<<endl;
	 cout<<"o---------------------------------------------------------------------o"<<endl;
	 cout<<"o--------------------Showing Generation Number "<<number_gen<<" ---------------------o"<<endl;
	 cout<<"o---------------------------------------------------------------------o"<<endl;
	 cout<<"o---------------------------------------------------------------------o"<<endl;
	 for(int z = 0; z < population.at(number_gen).generation.size();z++){
		  cout<<"<~~~~~~~~~~~~~~~~~~~~~~~~individual number "<<to_string(z)<<" ~~~~~~~~~~~~~~~~~~~~~~~~~>"<<endl;
		  for(int x = 0; x < population.at(number_gen).generation.at(z).getChromosomes().size();x++){
			   cout<<population.at(number_gen).generation.at(z).getChromosomes().at(x)<<endl;
		  }
		  cout<<" - - > TIME_FLY: in hours  :  "<<population.at(number_gen).generation.at(z).getTime()<<endl;
		  cout<<" - - > PRICE: in dolars $ "<<population.at(number_gen).generation.at(z).getPrice()<<endl;
		  cout<<" - - > SIZE: in hours      "<<population.at(number_gen).generation.at(z).getSize()<<endl;
	 }
	 cout<<" - - > Price Pairing: $  "<<     population.at(number_gen).price<<endl;
	 cout<<" - - > fitness value :  "<<population.at(number_gen).fitness<<endl;
} 