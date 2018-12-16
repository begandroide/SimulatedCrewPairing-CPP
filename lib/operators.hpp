#ifndef OPERATORS_H
#define OPERATORS_H
#include "individual.hpp"
#include "flight.hpp"
#include "population.hpp"


using namespace std;

class Operators{
public:
     Individual mutate(Individual, vector<Flight>,double);
     void selectRoulette(vector<Individual>*);
     void getFitness(Population*,int);
     int repare(Individual* , int , vector<Flight>);
};

#endif