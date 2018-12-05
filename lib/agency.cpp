#include <string.h>
#include<iostream>
#include<sstream>
#include <vector>
#include "flight.hpp"
#include "agency.hpp"
#include <algorithm>
using namespace std;

double getDifferenceTime(string hora_one, string hora_two){
	int hourDif, minDif;
	string hora_termino_one = string();
	string min_termino_one = string();
	string hora_inicio_two = string();
	string min_inicio_two = string();
     
	hora_termino_one = hora_one.substr(0,2);
	min_termino_one = hora_one.substr(3,5);
	hora_inicio_two = hora_two.substr(0,2);
	min_inicio_two = hora_two.substr(3,5);


//     hourDif =  stoi(hora_inicio_two)-stoi(hora_termino_one);
	(stoi(hora_inicio_two) == 0) ? hourDif= 24 - stoi(hora_termino_one) : hourDif =  stoi(hora_inicio_two)-stoi(hora_termino_one) ;
	int min_init = stoi(min_termino_one);
	int min_fin = stoi(min_inicio_two);     
	if(hourDif == 1){
			if(min_init != min_fin){
					minDif = (60-min_init)  + min_fin;
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
		if(hourDif > 1){
			hourDif -= 1;
		}
	} 
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
	//cout<<"timee calculated ->";
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