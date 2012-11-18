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

namespace std {

class Individual {
public:
	Individual(Common *conf);
	Individual& operator= (const Individual &source);
	Individual(const Individual&);
	virtual ~Individual();

	void hc1();
	void hc_amap();

	bool equalsh(Individual&, Individual&);
	bool equalss(Individual&, Individual&);

	void cross(Individual&, Individual&);
	int fitnessHCAL(int);
	int fitnessFCAL(int);
	int fitnessF1CAL(int);
	int fitnessF2CAL(int);
	int fitnessF3CAL(int);
	int dominates(Individual *target);
	void buildtimetable();
	void printtable();
	void printlect();
	void printdekanlik();
	Chromosome* getChromosome();

	int fitnessh;
	int fitnessh1;
	int fitnessf;
	int fitnessf2;

private:
	int findcourse(int sem, int dy, int slt);
	int decode(int);
	void updatefitness();

	Chromosome *chromosome;
	int chrom_length;
	int timetable1[5][4], timetable2[5][4];
	int no_colors;
	int no_periods;
	vector<int> conflv;
	FILE*errf;


	Common *conf;
};

} /* namespace std */
#endif /* INDIVIDUAL_H_ */
