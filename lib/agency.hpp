#ifndef AGENCY_H
#define AGENCY_H

int cuenta_lineas(char const *);
char* getString(char *, int);
Flight getLine(char* );
void resume(vector<Flight>,int,bool=false);
void getFlights(char const *,vector<Flight>*);

#endif