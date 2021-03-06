#include<iostream>
#include <string.h>
using namespace std;

const double MIN_BETWEN_FLIGHT = 0.5;
const double MAX_BETWEN_FLIGHT = 4.0;
const double MAX_TIME_DUTY = 12.0;
const double MAX_TIME_FLIGHT = 8.0;
const string BASE1 = "IST";
const string BASE2 = "ANK";

//function to prove if its valid relation two flights

int check_base(string airport){
     if(airport.compare(BASE1) == 0 || airport.compare(BASE2)== 0 ){
          return 0;
     }else{
          return 1;
     }
}

int compareHourV2 (string hora_one,string hora_two){
    int hourDif, minDif;
    string hora_termino_one = string();
    string min_termino_one = string();
    string hora_inicio_two = string();
    string min_inicio_two = string();

    hora_termino_one = hora_one.substr(0,2);
    min_termino_one = hora_one.substr(3,5);
    hora_inicio_two = hora_two.substr(0,2);
    min_inicio_two = hora_two.substr(3,5);

    double hour_init = stoi(hora_inicio_two);
    double hour_fin = stoi(hora_termino_one);

    double min_init = stoi(min_inicio_two)/60.0;
    double min_fin = stoi(min_termino_one)/60.0;

    hour_init += min_init;
    hour_fin += min_fin;

    double time_dif = hour_init - hour_fin;
    if(time_dif>MIN_BETWEN_FLIGHT && time_dif < MAX_BETWEN_FLIGHT){
        return 1;
    }else{
        return 0;
    }
}

double getIdleTime (string hora_one,string hora_two){
    double hourDif;
	string hora_termino_one = string();
	string min_termino_one = string();
	string hora_inicio_two = string();
	string min_inicio_two = string();
     
	hora_termino_one = hora_one.substr(0,2);
	min_termino_one = hora_one.substr(3,5);
	hora_inicio_two = hora_two.substr(0,2);
	min_inicio_two = hora_two.substr(3,5);

	double hour_inicio = stoi(hora_inicio_two);
	double hour_fin =  stoi(hora_termino_one);
    double min_init = stoi(min_inicio_two)/60.0;
    double min_fin = stoi(min_termino_one)/60.0;

    if(hour_inicio == 0){
        hour_inicio =24;
    } else if(hour_inicio == 1){
        hour_inicio = 25;
    } else if(hour_inicio == 2){
        hour_inicio = 26;
    }

    if(hour_fin == 0){
        hour_fin =24;
    } else if(hour_fin == 1){
        hour_fin = 25;
    } else if(hour_fin == 2){
        hour_fin = 26;
    }
    hour_fin += min_fin;
    hour_inicio += min_init;
	double time_dif = hour_inicio - hour_fin;
	if(time_dif < 0) return 0;
	return time_dif;
}
