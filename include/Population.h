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
	Population();
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
	int calc_crowd(int, const vector<int>&);
	void hillclimbmix2();
	bool foundinpar(int);
	void print_stat();
	void print_fitness(ostream &, Individual*);
	void print_header(ostream &);
	bool crowd_condition(vector<vector<int> >, Individual*,Individual*);

	bool updatePareto;

	int fit_table[POPUL][4];
	time_t start;
	bool inpf3[POPUL];
	vector<int> paretof;
	vector<int> population;
	Common *conf;
	vector<Individual*> pop;
	int crossel1;
	int crossel2;

	int pareto_bestID, pareto_minHFit, pareto_minSFit;
	int cross_suc, cross_fail, add_suc, add_fail, hc_suc, hc_fail, par_suc, par_fail, mut_suc, mut_fail;
};

} /* namespace std */
#endif /* POPULATION_H_ */
