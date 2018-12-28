#include <vector>
#include <stdio.h>
#include "../headers/individual.hpp"
using namespace std;

Individual::Individual(double _size)
{
     size = _size;
     fitness = 0;
     price = 0;
}


Individual::Individual(double _size,std::vector<int> _chromosomes)
{
     size = _size;
     price = 0;
     fitness = 0;
     chromosomes = _chromosomes;
}

Individual::Individual(double _size, float _price, float _fitness, std::vector<int> _chromosomes)
{
     size = _size;
     price = _price;
     fitness = _fitness;
     chromosomes = _chromosomes;
}

Individual::Individual(double _size, double _time, float _price, float _fitness, std::vector<int> _chromosomes)
{
     size = _size;
     time = _time;
     price = _price;
     fitness = _fitness;
     chromosomes = _chromosomes;
}

void Individual::setSize(double _size)
{
     size = _size;
};


void Individual::setPrice(float _price)
{
     price = _price;
};

void Individual::setTime(double _time)
{
     time = _time;
};


void Individual::setFitness(float _fitness)
{
     fitness = _fitness;
};

void Individual::setChromosome(std::vector<int> _chromosomes)
{
     chromosomes =    _chromosomes;
};

double Individual::getSize()
{
     return size;
}

double Individual::getTime(){
     return time;
}

float Individual::getPrice()
{
     return price;
}

std::vector<int> Individual::getChromosomes()
{
     return chromosomes;
};

float Individual::getFitness()
{
     return fitness;
};

//void mutate(float);
