/*
 * Population.h
 *
 *  Created on: 21 Eki 2012
 *      Author: HalilCan
 */

#ifndef POPULATION_H_
#define POPULATION_H_

#include "Common.h"
#include "Individual.h"
#include <vector>

namespace std {

class Population {
public:
	Population(Common *conf);
	virtual ~Population();
	void print();
	void crossmut3();
	void hillclimbmix2();
	void init();
	int findmin();
	int findmax();
	void initpareto();
	bool foundinpar(int);
	void nextgen(Individual[]);
	void nextgenold(Individual&);
	void selection2(Individual&, Individual&);
	void selection3(Individual&, Individual&);
	void selection(Individual&, Individual&);
	void run(int seed);

private:
	time_t start;
	double getduration();
	int inpf3[POPUL];
	vector<int> paretof;
	Common *conf;
	vector<Individual> pop, ch;
	int crossel1;
	int crossel2;
};

} /* namespace std */
#endif /* POPULATION_H_ */
