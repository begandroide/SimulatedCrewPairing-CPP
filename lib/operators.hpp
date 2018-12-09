#ifndef OPERATORS_H
#define OPERATORS_H
#include "individual.hpp"
#include "flight.hpp"
#include "population.hpp"
using namespace std;

class Operators{
public:
     void mutate(Individual*, vector<Flight>*,double);
     void selectRoulette(vector<Individual>*);
     void getFitness(Population*,int);
};

#endif