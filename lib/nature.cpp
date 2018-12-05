#include "nature.hpp"
#include<iostream>
#include <string.h>
using namespace std;

const double MIN_BETWEN_FLIGHT = 0.5;
const double MAX_BETWEN_FLIGHT = 4.0;
const double MAX_TIME_DUTY = 12.0;
const double MAX_TIME_FLIGHT = 0.5;

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
                         }
                    }
               }
          }
     } 
    // cout<<hourDif<<endl;
    // cout<<minDif<<endl;

	double time_dif = (double)hourDif+(minDif/60.0);
     if(time_dif>MIN_BETWEN_FLIGHT && time_dif < MAX_BETWEN_FLIGHT){
          /*cout<<"time dif->";
          cout<<to_string(time_dif)<<endl;
          cout<<"----------------...------------"<<endl;*/
          return 1;
     }else{
          return 0;
     }
}


bool Nature::validFlight(vector<int> chromosomes, int position){
     int id = chromosomes.at( chromosomes.size() - 1);
     //cout<<id<<endl;
     Flight last = agency.getFlights().at(id-1);
     Flight actual = agency.getFlights().at(position);
    // cout<<last.horaInicio<<endl;
     if( compareHour(last.horaFin, actual.horaInicio) == 1){
          return true;
     }else{
          return false;
     }
}


Individual Nature::getGreedyIndividual(int id_flight_start){
     int size;
     float price, fitness;
     vector<int> chromosomes;
     chromosomes.push_back(id_flight_start);
     //airport llegada ya que el primer vuelo del duty viene del parametro de la func.
     string airport_llegada = agency.getFlights().at(id_flight_start-1).aeropuerto_fin;
    // cout<<airport_llegada<<endl;
     
     //buscamos el siguiente disponible desde la ciudad de 
     //llegada a una hora acorde; respetando las reglas.
     
     vector<int> usados = vector<int>();
     usados.push_back(id_flight_start);

     bool init = true;
     bool fin = false;
     //supongamos 8 cromosomas máximo en el duty
     for(int a = 0; a <= 13; a++){ //for each chromosome
          for(int i = 0; i < agency.getFlights().size();i++){
               if( airport_llegada.compare( agency.getFlights().at(i).aeropuerto_init ) == 0)
               {
                    //comprobar si está en la lista de utilizados por este duty
                    //cout<<"por -> ";
                    //cout<< agency.getFlights().at(i).id<<endl;
                    bool used = exists(usados,i);
                    bool valid = validFlight(chromosomes,i);   
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

     for(int z = 0; z < chromosomes.size();z++){
          cout<<chromosomes.at(z)<<endl;
     }
     return NULL;
};



void Nature::makePopulation(int numGeneration, int numIndividuals){
     vector<Individual>* individues = new vector<Individual>;
     int discover = 1;
     for(int i = 0;i<numIndividuals;i++){
          cout<<"-----individual number -> "+to_string(i+1)<<endl;
          if(discover+10>agency.getFlights().size()){
               discover = (discover/(3*i))+1;
          }
          individues->push_back(getGreedyIndividual(discover));
          discover = 3*discover;
     }
     population.push_back(*individues);
};
