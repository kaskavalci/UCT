/*
 * Fitness.h
 *
 *  Created on: 30 Kas 2012
 *      Author: HalilCan
 */

#ifndef FITNESS_H_
#define FITNESS_H_

#include "Common.h"
#include "IFitness.h"
#include <list>

namespace std {
class Chromosome;
class Fitness : public IFitness {
public:
	Fitness(const Chromosome *chromosome);
	Fitness(const IFitness*);
	virtual ~Fitness();
	void calcFit(fitness_t&, int type = hc_both, int print = 0);

private:
	bool labs[3];

	inline bool fit_hconfmat(int i,int j);
	inline bool fit_hsameday(int i, int j);
	inline bool fit_hmidday(int i, int j);
	inline bool fit_sdepmeet(int i);
	inline bool fit_slabconf(int i, int j);
	inline bool fit_sconssem1(int i, int j);
	inline bool fit_sconssem2(int i, int j);
	inline bool fit_sconssem3(int i, int j);
	inline bool fit_sconssemlab1(int, int);
	inline bool fit_sconssemlab2(int, int);
	inline bool fit_sconssemlab3(int, int);
	inline bool fit_signoredlunchconflict(int i, int j);

	inline void h_confmat(fitness_t&, int);
	inline void h_sameday(fitness_t&, int);
	inline void h_midday(fitness_t&, int);
	inline void s_depmet(fitness_t&, int);
	inline void s_hwlab(fitness_t&, int);
	inline void s_lecturer(fitness_t&, int);
	inline void s_LTLconflict(fitness_t&, int);
	inline void s_ConsecSemLab(fitness_t&, int);
	inline void s_ConsecSem(fitness_t&, int);
	inline void s_lunch(fitness_t&, int);
	inline void s_eveningLecture(fitness_t&, int);
	inline void s_morningLab(fitness_t&, int);

	void init_labs(vector<Lecture>::const_iterator, vector<Lecture>::const_iterator);
	int decode(int);
};

} /* namespace std */
#endif /* FITNESS_H_ */
