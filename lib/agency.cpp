#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <cstdio>
#include<iostream>
#include<sstream>
#include <vector>
#include "flight.hpp"
using namespace std;

int cuenta_lineas(char const * file){
    FILE* stream = fopen(file, "r");
    int count = 0;
    char line[1024];
    if(stream){
	   fgets(line,1024,stream); //primera linea
	   while(fgets(line,1024,stream))
	   {
		  count++;
	   }
    }
    fclose(stream);
    return count;
}
char* getString(char *line, int after_comas){
    char* string_id_vuelo = new char();//malloc(sizeof(char)*3);
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

Flight getLine(char* line)
{
    char* string_id_vuelo = getString(line,0); //cero comas delante
    char* string_ciudad_salida = getString(line,1);
    char* string_ciudad_llegada = getString(line,2);
    char* hora_salida = getString(line,3);
    char* hora_llegada = getString(line,4);

    char *ptr;
    int ret;
    ret = std::atoi(string_id_vuelo);
	Flight flight = Flight(ret,string_ciudad_salida,string_ciudad_llegada,hora_salida,hora_llegada);
    return flight;
}

void resume(vector<Flight> vuelos, int num_vuelos,bool show_table)
{
	int i=0;
	int count_airports = 0;
	char** airports = new char*[num_vuelos];
	char** fingerPrint = new char*[num_vuelos];

	if(show_table) cout << "|  id  | origen | llegada | hr_inicio | hr_fin |" << endl;
    while(i<num_vuelos){
	    //if want show enterely table
	    if(show_table)
	    {
			ostringstream os;
			os << "|  " << to_string(vuelos.at(i).id) << "   |  " << vuelos.at(i).aeropuerto_init << "  |     " << vuelos.at(i).aeropuerto_fin << " |   " << vuelos.at(i).horaInicio << " |    " << vuelos.at(i).aeropuerto_fin;
			cout << os.str() << endl;
			os.clear();         
	    }
	    /*to return begining of array of *char*/
	    if(i>0){
		  airports = &*fingerPrint;
	    }
	    if(count_airports == 0)
	    {
			*airports = new char[1];
			*airports = strcpy(*airports,vuelos.at(i).aeropuerto_init.c_str());
	//	  	printf("una nueva -> %s\n",vuelos.at(i).aeropuerto_init.c_str());
			if(i==0)
			{
				*fingerPrint = &**airports;
			}
		  	count_airports++;
	    }else{
			int j = 0;
		  	int flag = 0;
		  	while(j<count_airports){
				if(vuelos.at(i).aeropuerto_init.compare(*airports) == 0)
				{
					//son iguales
					flag = 1;
					break;
				}else{
					flag = 0;
					if(!(     (j+1)==count_airports    ))
					{
					++airports;
					}
				}
				j++;
			}
		  	if(flag==0){
				++airports;
				*airports = new char[3];
				*airports = strcpy(*airports,vuelos.at(i).aeropuerto_init.c_str());
				count_airports++;
	//			cout <<((string)"una nueva! ->").append(vuelos.at(i).aeropuerto_init)<<endl;
		}
	   }
	++i;
    }
    //show airports;
    int z = 0;
    airports -= (count_airports-1);
    while(z<(count_airports)){
	    cout<<*airports++<<endl;
	    z++;
    }
}

/*
 * main function of this module.
 * bgautier produce
 */
void getFlights(char const *argv, vector<Flight>* pointer){
	int num_vuelos = cuenta_lineas(argv);
	//vector<Flight> flights;
    FILE* stream = fopen(argv, "r");
    char line[1024];
    fgets(line, 1024, stream); //nos saltamos la primera linea
    int cant_vuelos = 0;
    while (fgets(line, 1024, stream))
    {
	   char* tmp = strdup(line);
	   (*pointer).push_back( getLine(tmp));
	   cant_vuelos++;
	   free(tmp);
    }
    printf("Vuelos satisfactoriamente cargados!\n");
    fclose(stream);     
}
