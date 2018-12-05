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

    /*---------------------------------Operators---------------------------------*/
    bool exists(vector<int>,int);
    bool validFlight(vector<int>,int);
     /*
     * Function to poblate, depends of version from generation.
     * param1: version of generation
     * param2: numbers of population
     */
     void makePopulation(int , int=23);
     /*
     * Function to create a individual by greedy
     * param: to know number of iterations; idea of generate a solution with
     * different begin flight
     */
     Individual getGreedyIndividual(int);
     
     /*
     * Function depends of hours of flights, hours of no flights (idle time),
     * and price (*1 to flights and *0.75 to idle).
     */
     static float calculateFitness(Individual);
};

#endif