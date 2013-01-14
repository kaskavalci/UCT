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
	Population(int seed);
	virtual ~Population();

	void crossover();
	void mutation();
	void hillclimber();
	void startGA();
	double getduration();
	void print_footer(double duration, int iteration);

	const string& getPrinterFname() const {
		return printer_fname;
	}

	void setPrinterFname(const string& printerFname) {
		printer_fname = printerFname;
	}

	const Individual* getInd(int idx) const {
		return pop[idx];
	}

	const vector<Individual*>& getpareto() const;

	const Individual* getBestInd() const {
		return pop[pareto_bestID];
	}

	int getParetoBestId() const {
		return pareto_bestID;
	}

	Common *conf;

private:
	bool add_to_population(Individual*);
	bool add_to_pareto(int);
	void tournament(Individual**);
	int calc_crowd(int, const vector<int>&);
	bool crowd_condition(vector<vector<int> >,Individual*,Individual*);
	inline void update_pareto(int);
	inline void update_pareto(int, int);
	inline void update_bestInd(int);

	time_t start;
	vector<bool> inpf3;
	vector<int> paretof;
	vector<int> population;

	vector<Individual*> pop;
	int crossel1;
	int crossel2;
	Printer *printer;
	string printer_fname;

	int stats[STAT_LEN];
	int pareto_bestID, pareto_minHFit, pareto_minTFit;
	int seed;
};

} /* namespace std */
#endif /* POPULATION_H_ */
