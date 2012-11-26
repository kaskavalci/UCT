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

	bool hc1();
	bool hc_worstsection();

	bool equalsh(Individual&, Individual&);
	bool equalss(Individual&, Individual&);

	void cross(Individual*, Individual*);
	int fitnessHCAL(int);
	int fitnessFCAL(int);
	int fitnessF1CAL(int);
	int fitnessF2CAL(int);
	int fitnessF3CAL(int);
	int dominates(const Individual *target);
	void buildtimetable();
	void printtable();
	void printlect();
	void printdekanlik();
	Chromosome* getChromosome();
	void updatefitness();

	void setConf(Common*& conf) {
		this->conf = conf;
	}

	int fitnessh;
	int fitnessh1;
	int fitnessf;
	int fitnessf2;

private:
	typedef struct {
		int id[HARD_FIT_N];
		int fitness[HARD_FIT_N];
		int total_fit;
	} s_worst_chrom_t;

	typedef struct {
		int fitness[HARD_FIT_N];
		int total_fit;
	} s_hard_fitness_t;

	int findcourse(int sem, int dy, int slt);
	int decode(int);
	bool fit_hconfmat(int i,int j);
	bool fit_hsameday(int i, int j);
	bool fit_hmidday(int i, int j);
	bool fit_sdepmeet(int i);
	bool fit_slabconf(int i, int j);
	void get_hardfit(list<int> &, s_worst_chrom_t &);
	void get_hardfit(list<int> &, s_hard_fitness_t&);


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
