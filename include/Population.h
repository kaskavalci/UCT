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
	bool add_to_population(Individual*);
	bool add_to_pareto(int);
	void selection(Individual*, Individual*);
	void tournament(Individual*);
	int calc_nitch(int);
	int calc_crowd(int);
	void hillclimbmix2();
	bool foundinpar(int);
	void selection_old(Individual&, Individual&);
	void print_stat();
	void print_fit(int);
	int get_smallest_hfit();

	bool updatePareto;

	int fit_table[POPUL][4];
	time_t start;
	bool inpf3[POPUL];
	vector<int> paretof;
	Common *conf;
	vector<Individual*> pop;
	int crossel1;
	int crossel2;

	int cross_suc, cross_fail, add_suc, add_fail, hc_suc, hc_fail, par_suc, par_fail, mut_suc, mut_fail;
};

} /* namespace std */
#endif /* POPULATION_H_ */
