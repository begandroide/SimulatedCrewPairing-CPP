#ifndef OPERATORS_H
#define OPERATORS_H
#include "individual.hpp"
#include "flight.hpp"
using namespace std;

class Operators{
public:
     void mutate(Individual*, vector<Flight>*,double);
     void selectRoulette(vector<Individual>*);
     void getFitness(vector<Individual>*,int);
};

#endif