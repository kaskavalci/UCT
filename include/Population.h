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

	void run(int seed);

private:
	double getduration();
	void crossover();
	void mutation();
	void add_new_individual(Individual&);
	void selection(Individual&, Individual&);
	void hillclimbmix2();
	void initpareto();
	bool foundinpar(int);
	void selection_old(Individual&, Individual&);

	time_t start;
	bool inpf3[POPUL];
	vector<int> paretof;
	Common *conf;
	vector<Individual> pop;
	int crossel1;
	int crossel2;
};

} /* namespace std */
#endif /* POPULATION_H_ */
