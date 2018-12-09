#include "nature.hpp"
#include "operators.hpp"
#include<iostream>
#include <string.h>

using namespace std;

const double MIN_BETWEN_FLIGHT = 0.5;
const double MAX_BETWEN_FLIGHT = 4.0;
const double MAX_TIME_DUTY = 12.0;
const double MAX_TIME_FLIGHT = 8.0;
const string BASE1 = "IST";
const string BASE2 = "ANK";

bool Nature::exists(vector<int> usados, int position){
     bool used = false;
     for(int j = 0; j<usados.size();j++){
          if(usados.at(j) == agency.getFlights().at(position).id){
               //cout<<"usado"<<endl;
               used = true;
               break;
          }
     }
     return used;
}

//function to prove if its valid relation two flights
int compareHour (string hora_one,string hora_two){
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
                             minDif = min_init;
                             //minDif = 60-min_init;
                         }else{
                              minDif = stoi(min_inicio_two)  + 60 - stoi(min_termino_one);
                         }
                         hourDif = 0;
                    }else{
                         minDif = 0;
                         hourDif = 1;
                    }
          }else{
               if(min_init!=min_fin){
                    if(hourDif == 0 && min_init<min_fin){
                         //cout<<"zero"<<endl;
                         return 0;
                    }else{
                        minDif = min_init - min_fin;
                        double time_dif = (double)hourDif+(minDif/60.0);
                        if(time_dif>MIN_BETWEN_FLIGHT && time_dif < MAX_BETWEN_FLIGHT){
                            return 1;
                        }else{
                            return 0;
                        }
                    }
               }
               if(min_init == 0 &&  min_fin == 0){
                    minDif = 0;
               }else {
                    if(min_init != 0 && min_fin == 0 ){
                         minDif = -min_init;
                    }else{
                         if(min_init == 0 && min_fin != 0 ){
                         minDif = min_fin;
                         }else{
                              minDif = min_fin + (60 - min_init);
                         }
                    }
               }
          }
     } 
    // cout<<hourDif<<" ";
    // cout<<minDif<<endl;
	double time_dif = (double)hourDif+(minDif/60.0);
     if(time_dif>MIN_BETWEN_FLIGHT && time_dif < MAX_BETWEN_FLIGHT){
          return 1;
     }else{
          return 0;
     }
}

double getIdleTime (string hora_one,string hora_two){
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

bool Nature::validFlight(vector<int> chromosomes, int position){
     int id = chromosomes.at( chromosomes.size() - 1);
     Flight last = agency.getFlights().at(id-1);
     Flight actual = agency.getFlights().at(position);
     if( compareHour(last.horaFin, actual.horaInicio) == 1){
          return true;
     }else{
          return false;
     }
}

/*
TODO
* If time is available, calculate size, time and price in paralell.
*/
Individual Nature::getGreedyIndividual(int id_flight_start){
     double size;
     float price, fitness;
     vector<int> chromosomes;
     //push chromosome parameter to vector chromosomes
     int flag = 0;
     if(agency.getFlights().at(id_flight_start-1).aeropuerto_init.compare(BASE1) == 0 || agency.getFlights().at(id_flight_start-1).aeropuerto_init.compare(BASE2) == 0 )
     {
          flag =1;
     };
     if(flag == 0) {
          chromosomes.clear();
          return Individual(size, chromosomes);
     }
     chromosomes.push_back(id_flight_start);
     
     //airport llegada, because the first fligth of duty its in param of func
     string airport_llegada = agency.getFlights().at(id_flight_start-1).aeropuerto_fin;
     
     //usados, vector to know ids used
     vector<int> usados = vector<int>();
     usados.push_back(id_flight_start);

     // look for next available from arrived airport 
     // take order to time and rules.
     //  we suposed 15 chromosomes max in duty
     for(int a = 0; a <= 7; a++){ //for each chromosome setted to 7 given a max duty time of 12 hours
          for(int i = 0; i < agency.getFlights().size()-1;i++){
               if( airport_llegada.compare( agency.getFlights().at(i).aeropuerto_init ) == 0)
               {
                    // prove if the currect fligth its used for current duty
                    //cout<< agency.getFlights().at(i).id<<endl;
                    bool used = exists(usados,i); //if exist
                    bool valid = validFlight(chromosomes,i);   //if take order to rules of time
                    //si no fue usado lo usamos
               
                    if(used!=true  && valid){                     
                         usados.push_back(agency.getFlights().at(i).id);
                         chromosomes.push_back(agency.getFlights().at(i).id);
                         //cout<<"chromosme added"<<endl;
                         airport_llegada.clear();
                         airport_llegada = agency.getFlights().at(i).aeropuerto_fin;
                         break;
                    }
               }
          }
     }
     
     //calculateSize (OK)
     size = agency.getFlights().at(chromosomes.at(0)).timeFlight;
     for(int z = 0; z < chromosomes.size();z++){
          if(z>0){
               size += agency.getFlights().at(chromosomes.at(z)-1).timeFlight;
               size += getIdleTime(agency.getFlights().at(chromosomes.at(z-1)-1).horaFin,agency.getFlights().at(chromosomes.at(z)-1).horaInicio);
          }
     }

    if(size > MAX_TIME_DUTY){
        while(size>MAX_TIME_DUTY){
            //nos quedamos en z
            size -= getIdleTime(agency.getFlights().at(chromosomes.at(chromosomes.size()-2)-1).horaFin,agency.getFlights().at(chromosomes.at(chromosomes.size()-1)-1).horaInicio);
            size -= agency.getFlights().at(chromosomes.at(chromosomes.size()-1)-1).timeFlight;
            chromosomes.pop_back();
        }
    }

     int flag2 = 0;
     if( agency.getFlights().at(chromosomes.at(chromosomes.size()-1)).aeropuerto_fin.compare(BASE1)==0 || agency.getFlights().at(chromosomes.at(chromosomes.size()-1)).aeropuerto_fin.compare(BASE2)==0 ){
          flag2=1;
     }
     if(flag2==0){
         while(agency.getFlights().at(chromosomes.at(chromosomes.size()-1)).aeropuerto_fin.compare(BASE1)==0 || agency.getFlights().at(chromosomes.at(chromosomes.size()-1)).aeropuerto_fin.compare(BASE2)==0 ){
             //nos quedamos en z
             size -= getIdleTime(agency.getFlights().at(chromosomes.at(chromosomes.size()-2)-1).horaFin,agency.getFlights().at(chromosomes.at(chromosomes.size()-1)-1).horaInicio);
             size -= agency.getFlights().at(chromosomes.at(chromosomes.size()-1)-1).timeFlight;
             chromosomes.pop_back();
         }
     }

     // calculate time (OK) 
     double time = 0.0;
     for(int z = 0; z < chromosomes.size();z++){
          time += agency.getFlights().at(chromosomes.at(z)-1).timeFlight;
     }

     /*
     TODO 
      * (WARNING) active this depending of the instance input
      * 3 to instance 1,
      * 5 to instance 2,
      * 6 to instance 3,
      * time can vary depending of objectives of bussines and data study
      */
     if(time > MAX_TIME_FLIGHT || time<6){
          if(time<6){
              chromosomes.clear();
              return Individual(size, chromosomes);
          }else{
              while(agency.getFlights().at(chromosomes.at(chromosomes.size()-1)).aeropuerto_fin.compare(BASE1)==0 || agency.getFlights().at(chromosomes.at(chromosomes.size()-1)).aeropuerto_fin.compare(BASE2)==0 ){
                  //nos quedamos en z
                  if(chromosomes.size()<3){
                      return Individual(size,chromosomes);
                  }
                  size -= getIdleTime(agency.getFlights().at(chromosomes.at(chromosomes.size()-2)-1).horaFin,agency.getFlights().at(chromosomes.at(chromosomes.size()-1)-1).horaInicio);
                  size -= agency.getFlights().at(chromosomes.at(chromosomes.size()-1)-1).timeFlight;
                  time -= agency.getFlights().at(chromosomes.at(chromosomes.size()-1)-1).timeFlight;
                  chromosomes.pop_back();
              }
          }
     }


     
     // calculate price to this duty (OK)
     price = agency.getFlights().at(chromosomes.at(0)).timeFlight;
     for(int z = 0; z < chromosomes.size();z++){
          if(z>0){
               price += agency.getFlights().at(chromosomes.at(z)-1).timeFlight;
               price += 0.75*getIdleTime(agency.getFlights().at(chromosomes.at(z-1)-1).horaFin,agency.getFlights().at(chromosomes.at(z)-1).horaInicio);
          }
     }

     return Individual(size, time,price,0,chromosomes);
};

void selection_natural(vector<Individual>* individues){
     int MAX_POP = 23;

     if(individues->size()>20){
          while(individues->size()>20){
               double maxTime = 0;
               int id_borrar = 0;
                double difTmp = 0.0;
               for(int i = 0; i<individues->size();i++){
                    cout<<"~"<<endl;
                    cout<<"Price"<<individues->at(i).getPrice()<<endl;
                    cout<<"Size"<<individues->at(i).getSize()<<endl;
                    cout<<"Time"<<individues->at(i).getTime()<<endl;
                    difTmp = individues->at(i).getSize() - individues->at(i).getTime();
                    if(difTmp > 4 ){
                        cout<<difTmp<<endl;
                         if(maxTime < difTmp){
                             cout<<"maximo->"<<endl;
                              maxTime = difTmp;
                              id_borrar = i;
                         }
                    }
               }
               /*
               cout<<"voy"<<endl;
               cout<<id_borrar<<endl; //id del duty mas molestoso, esto es, el que tiene mas tiempo de no vuelo.
               cout<<individues->at(id_borrar).getSize()<<endl;
               cout<<individues->at(id_borrar).getTime()<<endl;
               */
               individues->erase(individues->begin() + id_borrar);
          }
     }
};


//main function of librarie Nature
void Nature::makePopulation(int numGeneration, int numIndividuals){
     Population new_gen = Population();
     vector<Individual>* individues = new vector<Individual>;
     int discover = 1;
     for(int i = 0;i<agency.getFlights().size()-1;i++){
          if(individues->size() == numIndividuals){
               break;
          }
          Individual indi = getGreedyIndividual(i+1);
          if(indi.getChromosomes().size() > 0){
               individues->push_back(indi); //push individue
          }
     }
     //process of selection
     selection_natural(individues);

     new_gen.generation = *individues;
     operators.getFitness(&new_gen,agency.getFlights().size());
     population.push_back(new_gen);
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
} 