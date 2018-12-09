#include "operators.hpp"
#include<iostream>
using namespace std;

/*
void Operators::mutate(Individual*, vector<Flight>*,double);
void Operators::selectRoulette(vector<Individual>*);
*/
void Operators::getFitness(Population* all_generation, int num_flights){
	//first: search flight no taked
	vector<int> taked = vector<int>();
	vector<int> taked_count = vector<int>();
	for(int i = 0; i < num_flights; i++){
		taked.push_back(0);
		taked_count.push_back(0);
	}

	vector<Individual>* generation =  &all_generation->generation;
	//price
	double price_gen = 0.0;
	//for each individual in generation
	for(int i = 0; i<generation->size();i++){
		Individual indTmp = generation->at(i);
		price_gen += indTmp.getPrice();
		vector<int> chromoTmp =  indTmp.getChromosomes();
		for(int j = 0; j < chromoTmp.size();j++){
			taked.at(chromoTmp.at(j)-1) = 1;
			taked_count.at(chromoTmp.at(j)-1) += 1;
		} 
	}
	cout<<"price total ->"<<price_gen<<endl;

	int no_taken = 0;
	for(int i = 0; i < num_flights; i++){
		//cout<<taked.at(i)<<endl;
		if(taked.at(i) == 1){
			no_taken++;
		}
	}
	cout<<no_taken<<endl;

	int count_repited = 0;
	for(int i ; i < num_flights; i++){
		cout<<taked_count.at(i)<<endl;
		if(taked_count.at(i)>1){
			count_repited += taked_count.at(i) -1; 
		}
	}
	cout<<"repited -> "<<count_repited<<endl;

	double fitness = 0.0;
	fitness = price_gen + (no_taken)*10 +(count_repited)*10;
	cout<<"fitness val -> "<<1/fitness<<endl;
	all_generation->fitness = 1/fitness;
	all_generation->price = price_gen;
};