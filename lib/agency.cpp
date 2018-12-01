#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <cstdio>
#include <iostream>
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
    printf("%s\n",string_id_vuelo);
    char* string_ciudad_salida = getString(line,1);
    char* string_ciudad_llegada = getString(line,2);
    char* hora_salida = getString(line,3);
    char* hora_llegada = getString(line,4);

    char *ptr;
    int ret;
    ret = std::atoi(string_id_vuelo);
     cout << ret <<endl;
     Flight flight = Flight(ret,string_ciudad_salida,string_ciudad_llegada,hora_salida,hora_llegada);
    return flight;
}


void resume(vector<Flight> vuelos, int num_vuelos)
{
     int i=0;
     int count_airports = 0;
     char** airports = new char*[1];// malloc(1*sizeof(char*));
     char** fingerPrint = new char*[1];//malloc(1*sizeof(char*));

     cout << "|  id  | origen | llegada | hr_inicio | hr_fin |" << endl;

    while(i<num_vuelos){
         string out = string();
         out.append("|  ");
         out.append(to_string(vuelos.at(i).id));
         out.append("   |  ");
         out.append(vuelos.at(i).aeropuerto_init);
         out.append("    |  ");
         out.append(vuelos.at(i).aeropuerto_fin);
         out.append("   |  ");
         out.append(vuelos.at(i).horaInicio);
         out.append("    |  ");
         out.append(vuelos.at(i).horaFin);
         out.append("  |  ");
         
         cout <<  out<<endl;
          //cout << vuelos.at(i).aeropuerto_init + '~' +  <<endl;
         
         // printf("id -> %s\n",vuelos.at(i).aeropuerto_init);
        //char* aer_origin = vuelos->aeropuerto_init;
        if(i>0)
        {
            airports = &*fingerPrint;
        }
          ++i;
        /*
        if(count_airports == 0)
        {
            *airports = malloc(3*sizeof(char));
            *airports = strcpy(*airports,vuelos->aeropuerto_init);
            printf("una nueva -> %s\n",vuelos->aeropuerto_init);
            if(i==0)
            {
                *fingerPrint = &**airports;
            }
            //*airports = vuelos->aeropuerto_init;
            count_airports++;
        }else{
            int j = 0;
            int flag = 0;
            while(j<count_airports){
                if(strcmp(*airports,vuelos->aeropuerto_init) == 0)
                {
                    //son iguales
                    flag = 1;
                    //printf("Ya encontrado: %s -> %s\n",*airports, vuelos->aeropuerto_init);
                    break;
                }else{
                    flag = 0;
                    if(!((j+1)==count_airports))
                    {
                        ++airports;
                    }
                }
                j++;
            }
            if(flag==0){
                //airports = realloc(airports,(count_airports+1)*sizeof(char*));
                ++airports;
                *airports = malloc(3*sizeof(char));
                *airports = strcpy(*airports,vuelos->aeropuerto_init);
//                    airports = vuelos->aeropuerto_init;
                count_airports++;
                //no lo son
                printf("una nueva! -> %s\n",vuelos->aeropuerto_init);
            }
        }*/
        //airports -= (count_airports-1);
//        ++i;
    }

}


/*
 * main function of this module.
 * bgautier produce
 */
void getFlights(char const *argv){
     int num_vuelos = cuenta_lineas(argv);
     vector<Flight> flights;
    FILE* stream = fopen(argv, "r");
    char line[1024];
    fgets(line, 1024, stream); //nos saltamos la primera linea
    int cant_vuelos = 0;
    while (fgets(line, 1024, stream))
    {
        char* tmp = strdup(line);
        flights.push_back( getLine(tmp));
        cant_vuelos++;
        free(tmp);
    }
    printf("Vuelos cargados!\n");     
    resume(flights,num_vuelos);
}
