#ifndef OPERATORS_H
#define OPERATORS_H
#include "individual.hpp"
#include "flight.hpp"
#include "population.hpp"
using namespace std;

class Operators{
public:
     Population mutate(Population, vector<Flight>,double);
     vector<Population> selectRouletteWheel(vector<Population>);
     void getFitness(Population*,int);
     int repare(Individual* , int , vector<Flight>);
     Population elitism(vector<Population>);
};

#endif