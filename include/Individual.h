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
	//virtual ~Individual();

	void clear_conflv();
	void mutate();
	void mutateg1();
	void mutateg3();
	void mutateg5();
	void mutatehg1();
	void mutatehg3();
	void mutatehg5();
	void hc1();
	void hc122();
	void hc2();
	void hc1g();
	void hc12g();
	void hc2g();
	void mutatehc1(int, int);
	bool equalsh(Individual&, Individual&);
	bool equalss(Individual&, Individual&);
	void mutate2(int, int);
	void cross(Individual&, Individual&, Individual&, Individual&);
	void cross2(Individual&, Individual&, Individual&, Individual&);
	void cross3(Individual&, Individual&, Individual&, Individual&);
	int fitnessHCAL(int);
	int fitnessFCAL(int);
	int fitnessF1CAL(int);
	int fitnessF2CAL(int);
	int fitnessF3CAL(int);
	int decode(int);
	void copyindividual(Individual&, Individual&);
	int dominates(Individual&, Individual&);
	void buildtimetable();
	void buildtimetableold();
	void printtable();
	void printlect();
	void printtt();
	void printdekanlik();
	void printjava();
	void writeind();
	void writetime();
	void readtime();
	void readind();
	int getc2idx(int);

	int fitnessh;
	int fitnessh1;
	int fitnessh2;
	int fitnessf;
	int fitnessf1;
	int fitnessf2;
	int fitnessf3;

private:
	int findcourse(int sem, int dy, int slt);
	int chrom_length;
	int *chrom;
	int *day;
	int *slot;
	int **timetable1;
	int **timetable2;
	int no_colors;
	int no_periods;
	int c2cnt;
	vector<int> conflv;
	vector<int> conflvh;
	vector<int> conflvf;
	FILE*errf;


	Common *conf;
};

} /* namespace std */
#endif /* INDIVIDUAL_H_ */
