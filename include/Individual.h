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
#include <list>

namespace std {

class Individual {
public:
	Individual(Common *conf);
	Individual();
	Individual& operator= (const Individual &source);
	Individual(const Individual&);
	virtual ~Individual();

	//bool hc1();
	bool hc_worstsection(int);

	void cross(Individual*, Individual*);
	int dominates(const Individual *target);
	void buildtimetable();
	void printtable();
	void printlect();
	void printdekanlik();
	Chromosome* getChromosome();
	void updatefitness(int);

	void calc_hardfit(const list<int> &, s_hard_fitness_t&, int);
	void calc_softfit(const list<int> &, s_soft_fitness_t&, int);
	const s_hard_fitness_t& getHardFit() const;
	const s_soft_fitness_t& getSoftFit() const;

private:
	int findcourse(int sem, int dy, int slt);

	Chromosome *chromosome;
	int chrom_length;
	int timetable1[5][4], timetable2[5][4];
	int no_colors;
	int no_periods;

	Common *conf;
};

} /* namespace std */
#endif /* INDIVIDUAL_H_ */
