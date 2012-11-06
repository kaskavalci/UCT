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

namespace std {

class Individual {
public:
	Individual(Common *conf);
	Individual& operator= (Individual &target);
	Individual(const Individual&);
	virtual ~Individual();

	void mutate();
	void mutateg1();
	void mutateg3();
	void mutateg5();
	void hc1();

	bool equalsh(Individual&, Individual&);
	bool equalss(Individual&, Individual&);

	void cross(Individual&, Individual&, Individual&, Individual&);
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

	int fitnessh;
	int fitnessh1;
	int fitnessh2;
	int fitnessf;
	int fitnessf1;
	int fitnessf2;
	int fitnessf3;

private:
	int findcourse(int sem, int dy, int slt);
	int decode(int);
	void mutatehc1(int, int);

	int chrom_length;
	int chrom[CHROML];
	int day[CHROML];
	int slot[CHROML];
	int timetable1[5][4], timetable2[5][4];
	int no_colors;
	int no_periods;
	int c2cnt;
	vector<int> conflv;
	FILE*errf;


	Common *conf;
};

} /* namespace std */
#endif /* INDIVIDUAL_H_ */
