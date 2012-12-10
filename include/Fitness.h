/*
 * Fitness.h
 *
 *  Created on: 30 Kas 2012
 *      Author: HalilCan
 */

#ifndef FITNESS_H_
#define FITNESS_H_

#include "Common.h"
#include <list>

namespace std {
class Chromosome;
class Fitness {
public:
	Fitness(const Chromosome *chromosome);
	Fitness(const Fitness*);
	virtual ~Fitness();
	void calc_hardfit(s_hard_fitness_t&, int);
	void calc_softfit(s_soft_fitness_t&, int);
	void updatefitness(int);

	const s_hard_fitness_t& getHardFit() const {
		return hard_fit;
	}

	const s_soft_fitness_t& getSoftFit() const {
		return soft_fit;
	}

	void setChromosome(const Chromosome* chromosome) {
		this->chromosome = chromosome;
	}

private:
	const Chromosome *chromosome;
	Common *conf;
	s_hard_fitness_t hard_fit;
	s_soft_fitness_t soft_fit;
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

	inline void h_confmat(s_hard_fitness_t&, int);
	inline void h_sameday(s_hard_fitness_t&, int);
	inline void h_midday(s_hard_fitness_t&, int);
	inline void s_depmet(s_soft_fitness_t&, int);
	inline void s_hwlab(s_soft_fitness_t&, int);
	inline void s_lecturer(s_soft_fitness_t&, int);
	inline void s_LTLconflict(s_soft_fitness_t&, int);
	inline void s_ConsecSemLab(s_soft_fitness_t&, int);
	inline void s_ConsecSem(s_soft_fitness_t&, int);
	inline void s_lunch(s_soft_fitness_t&, int);
	inline void s_eveningLecture(s_soft_fitness_t&, int);
	inline void s_morningLab(s_soft_fitness_t&, int);

	void init_labs(vector<Lecture>::const_iterator, vector<Lecture>::const_iterator);
	int decode(int);
};

} /* namespace std */
#endif /* FITNESS_H_ */
