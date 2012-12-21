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
#include "Printer.h"

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
	void tournament(Individual**);
	int calc_crowd(int, const vector<int>&);
	void hillclimbmix2();
	bool foundinpar(int);
	inline void print_fitness(ostream &, Individual*);
	bool crowd_condition(vector<vector<int> >, vector<vector<int> >,Individual*,Individual*);
	inline void update_pareto(int);
	inline void update_pareto(int, int);
	inline void update_bestInd(int);

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
	Printer *printer;

	int stats[STAT_LEN];
	int pareto_bestID, pareto_minHFit;
};

} /* namespace std */
#endif /* POPULATION_H_ */
