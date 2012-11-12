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
	void crossmut3();
	void hillclimbmix2();
	void initpareto();
	bool foundinpar(int);
	void nextgen(Individual[]);
	void selection(Individual&, Individual&);

	time_t start;
	int inpf3[POPUL];
	vector<int> paretof;
	Common *conf;
	vector<Individual> pop, ch;
	int crossel1;
	int crossel2;
};

} /* namespace std */
#endif /* POPULATION_H_ */
