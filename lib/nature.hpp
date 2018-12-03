#ifndef NATURE_H
#define NATURE_H
#include <vector>
#include "agency.hpp"
#include "individual.hpp"
using namespace std;

class Nature{
public:
     Agency agency;
     vector<vector<Individual>> population; // vectors of vectors to generations.
     static Individual* getGreedyIndividual(int);
};

#endif