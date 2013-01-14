/*
 * Individual.h
 *
 *  Created on: 21 Eki 2012
 *      Author: HalilCan
 */

#ifndef INDIVIDUAL_H_
#define INDIVIDUAL_H_

#include "Common.h"
#include "Course.h"
#include "Chromosome.h"
#include "Mutation.h"
#include <list>

namespace std {

class Individual {
public:
	Individual(Common *conf);
	Individual();
	Individual& operator=(const Individual &source);
	Individual(const Individual&);
	virtual ~Individual();

	bool hc_worstsection();
	bool mutate_all();

	void cross(const Individual&, const Individual&);
	domination_t dominates(const Individual *target);
	void printtable();
	void printlect();
	void printdekanlik();
	Chromosome* getChromosome();
	void updatefitness(int);

	/*
	 * does not re-calculate the fitness. only gets the pre-calculated fitness
	 * to process it. calling functions cannot change the fitness!
	 */
	const inline fitness_t& getFit() const {
		return chromosome->fit->getFit();
	}

	const Chromosome* getConstChromosome() const {
		return chromosome;
	}

private:
	int findcourse(int sem, int dy, int slt);
	void calcFit(fitness_t&, int,int);
	Chromosome *chromosome;
	Mutation *mutator;
	int chrom_length;
	int no_periods;
	Common *conf;
};

} /* namespace std */
#endif /* INDIVIDUAL_H_ */
