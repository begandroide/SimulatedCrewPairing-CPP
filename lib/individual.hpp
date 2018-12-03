#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H
#include <vector>
using namespace std;
/*
 *  Class Individual its the same that a duty. 
 *  Duty is s representation of a set of fligths
 *  Attributes:
 *      size: number of flights that compound the duty
 *      price: value of duty, depends of time of flights and duration inactive of fligth
 *      chromosomes: vector contains id's of flights that compound the duty
 *      fitness:  value of capacity of the individual respect of the problem
 */
class Individual{
private:
    int size;
    float price;
    vector<int> chromosomes; //vuelos que lo componen
    float fitness;
public:
    Individual(int);
    Individual(int,vector<int>);
    Individual(int , float , float , std::vector<int>); 
    
    /*---------------------------------Operators---------------------------------*/
    //static Individual* getGreedyIndividual(int); made by nature
    static Individual* getCopy(Individual*);
    static void calculatePrice();
    //static void calculateFitness(); made by nature
    static void calculateSize();
    

    /*---------------------------------Setters---------------------------------*/
    void setSize(int);
    void setPrice(float);
    void setFitness(float);
    void setChromosome(vector<int>);
    void mutate(float);
    
    /*---------------------------------Getters---------------------------------*/
    int getSize();
    float getPrice();
    vector<int> getChromosomes();
    float getFitness();
};
#endif