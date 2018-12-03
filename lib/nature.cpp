#include "nature.hpp"
#include<iostream>
using namespace std;

const double MIN_BETWEN_FLIGHT = 0.5;
const double MAX_BETWEN_FLIGHT = 4.0;
const double MAX_TIME_DUTY = 12.0;
const double MAX_TIME_FLIGHT = 0.5;

Individual Nature::getGreedyIndividual(int id_flight_start){
     int size;
     float price, fitness;
     vector<int> chromosomes;
     chromosomes.push_back(id_flight_start);
     string airport_llegada = agency.getFlights().at(id_flight_start-1).aeropuerto_fin;
     cout<<airport_llegada<<endl;
     //buscamos el siguiente disponible desde la ciudad de 
     //llegada a una hora acorde; respetando las reglas.
     for(int i = 0; i < agency.getFlights().size()-1;i++){
          if( airport_llegada.compare( agency.getFlights().at(i).aeropuerto_init ) == 0)
          {
               cout<<"yes"<<endl;
          }
     }
     return NULL;
};



void Nature::makePopulation(int numGeneration, int numIndividuals){
     vector<Individual>* individues = new vector<Individual>;
     for(int i = 0;i<numIndividuals;i++){
          individues->push_back(getGreedyIndividual(1));
     }
     population.push_back(*individues);
};
