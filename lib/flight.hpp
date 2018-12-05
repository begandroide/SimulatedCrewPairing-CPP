#ifndef FLIGHT_H
#define FLIGHT_H
#include <string>
using namespace std;

class Flight{
public:

    int  id;
    double timeFlight;
    std::string   aeropuerto_init; //o city
    std::string   aeropuerto_fin;  //o city
    std::string   horaInicio;
    std::string   horaFin;

    /*----------------------------------Constructor------------------------------ */
     Flight(int,double,std::string,std::string,std::string,std::string);
    
     /*---------------------------------Operators---------------------------------*/
     void calculateTimeFlight();
     /*---------------------------------Setters---------------------------------*/
     void setId(int);
     void setAir_init(std::string);
     void setAir_fin(std::string);
     void setHour_init(std::string);
     void setHour_fin(std::string);
     /*---------------------------------Getters---------------------------------*/
     int getId();
     std::string getAir_init();
     std::string getAir_fin();
     std::string getHour_init();
     std::string getHour_fin();
};

#endif