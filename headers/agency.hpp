#ifndef AGENCY_H
#define AGENCY_H
#include <vector>
#include <string>
#include "flight.hpp"

using namespace std;

class Agency{
private:
     vector<Flight> flights;
     vector<string> airports;
     vector<string> bases;
public:
    /*---------------------------------Utils---------------------------------*/
     int cuenta_lineas(FILE *);
     char* getString(char *, int);
     Flight getLine(char* );
     void resume();
     void loadFlights(char const *);
     void showAirports();
    /*---------------------------------Getters---------------------------------*/
     vector<Flight> getFlights();
     vector<string> getAirports();
     vector<string> getBases();
    /*---------------------------------Setters---------------------------------*/
    void setFlights(vector<Flight>);
    void setAirports(vector<string>);
    void setBases(vector<string>);
};

#endif