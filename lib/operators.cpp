#include "operators.hpp"
#include<iostream>
using namespace std;

/*
void Operators::mutate(Individual*, vector<Flight>*,double);
void Operators::selectRoulette(vector<Individual>*);
*/
void Operators::getFitness(vector<Individual>* generation, int num_flights){
	//first: search flight no taked
	vector<int> taked = vector<int>();
	vector<int> taked_count = vector<int>();
	for(int i = 0; i < num_flights; i++){
		taked.push_back(0);
		taked_count.push_back(0);
	}

	//for each individual in generation
	for(int i = 0; i<generation->size();i++){
		Individual indTmp = generation->at(i);
		vector<int> chromoTmp =  indTmp.getChromosomes();
		for(int j = 0; j < chromoTmp.size();j++){
			taked.at(chromoTmp.at(j)-1) = 1;
			taked_count.at(chromoTmp.at(j)-1) += 1;
		} 
	}
	
	int no_taken = 0;
	for(int i = 0; i < num_flights; i++){
		cout<<taked.at(i)<<endl;
		if(taked.at(i) == 1){
			no_taken++;
		}
	}
	cout<<no_taken<<endl;


	for(int i = 0; i < num_flights; i++){
		cout<<taked_count.at(i)<<endl;
	}
};