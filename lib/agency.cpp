#include <string.h>
#include<iostream>
#include<sstream>
#include <vector>
#include "flight.hpp"
#include "agency.hpp"
#include <algorithm>
using namespace std;

double getDifferenceTime(char* hora_salida, char* hora_llegada){
	int hourDif, minDif;
	char* hora_init = (char*)malloc(2*sizeof(char));
	char *hora_fin = (char*)malloc(2*sizeof(char));
	char *min_init = (char*)malloc(2*sizeof(char));
	char *min_fin = (char*)malloc(2*sizeof(char));
	hora_init = strncpy(hora_init,hora_salida,2);
	hora_fin = (char*)memcpy(hora_fin,hora_llegada,2);
	min_init = strncpy(min_init,3+hora_salida,2);
	min_fin = strncpy(min_fin,3+hora_llegada,2);
	//printf("%s --",min_fin);
	//printf("%s\n",min_init);
	hourDif = stoi(hora_fin) - stoi(hora_init);
	minDif = stoi(min_fin);
	//si el vuelo termina a las 00
	(stoi(hora_fin) == 0) ? hourDif = 24 - stoi(hora_init) : hourDif = stoi(hora_fin) - stoi(hora_init); 
	//cout<<to_string(minDif)<<endl;
	delete hora_init;
	delete hora_fin;
	delete min_init;
	delete min_fin;
	//cout<<(double)hourDif+minDif/60.0<<endl;
	return (double)hourDif+(minDif/60.0);
}

int Agency::cuenta_lineas(FILE* stream){
    int count = 0;
    char line[1024];
    if(stream){
	   fgets(line,1024,stream); //primera linea
	   while(fgets(line,1024,stream))
	   {
		  count++;
	   }
    	}
	fseek(stream, 0, SEEK_SET);
	return count;
}

char* Agency::getString(char *line, int after_comas){
    char* string_id_vuelo = new char();
    int comas_pasadas = 0;
    while(*line != '\r'){
	   if(*line != ',' && comas_pasadas == after_comas){
		  strncat(string_id_vuelo,line, sizeof(char));
	   }else if(*line == ','){
		  comas_pasadas++;
	   }
	   if(*line == '\0') return string_id_vuelo;
	   ++line;
	   if(comas_pasadas > after_comas) break;
    }
    return  string_id_vuelo;
}

Flight Agency::getLine(char* line)
{
    	char* string_id_vuelo = getString(line,0); //cero comas delante
    	char* string_ciudad_salida = getString(line,1);
    	char* string_ciudad_llegada = getString(line,2);
    	char* hora_salida = getString(line,3);
    	char* hora_llegada = getString(line,4);
	double timeFlight = getDifferenceTime(hora_salida,hora_llegada);
	//cout <<timeFlight<<endl;
    	int ret;
    	ret = std::atoi(string_id_vuelo);
	Flight flight = Flight(ret,timeFlight,string_ciudad_salida,string_ciudad_llegada,hora_salida,hora_llegada);
    	return flight;
}

/*
 * main function of this module.
 * bgautier produce
 */
void Agency::loadFlights(char const *argv){
	FILE* stream = fopen(argv, "r");
	int num_vuelos = cuenta_lineas(stream);
	char line[1024];
    fgets(line, 1024, stream); //nos saltamos la primera linea
    while (fgets(line, 1024, stream))
    {
		char* tmp = strdup(line);
	   	flights.push_back( getLine(tmp));
		if(!(std::find(airports.begin(), airports.end(), flights.at(flights.size()-1).aeropuerto_init) != airports.end()))
		 {
			/* airports contains x */
			airports.push_back(flights.at(flights.size()-1).aeropuerto_init);
		}
	   	free(tmp);
    }
    printf("Vuelos satisfactoriamente cargados!\n");
    fclose(stream);     
}

void Agency::resume()
{
	int i=0;
	cout << "|  id  | origen | llegada | hr_inicio | hr_fin |" << endl;
    	while(i<flights.size()){
		//if want show enterely table
	    	ostringstream os;
		os << "|  " << to_string(flights.at(i).id) << "   |  " << flights.at(i).aeropuerto_init << "  |     " << flights.at(i).aeropuerto_fin << " |   " << flights.at(i).horaInicio << " |    " << flights.at(i).aeropuerto_fin;
		cout << os.str() << endl;
		os.clear();         
		++i;
    	}
}

vector<Flight> Agency::getFlights(){
	return flights;
}

vector<string> Agency::getAirports(){
	return airports;
}

void Agency::showAirports(){
	for ( int i = 0; i<airports.size();i++){
		printf("aeropuerto -> %s\n",airports.at(i).c_str());
	}
}

vector<string> Agency::getBases(){
	return bases;
}