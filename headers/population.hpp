#ifndef POPULATION_H
#define POPULATION_H
#include <vector>
#include "individual.hpp"
using namespace std;

class Population{
public:
     vector<Individual> generation;
     double price;
     double fitness;
};

#endif