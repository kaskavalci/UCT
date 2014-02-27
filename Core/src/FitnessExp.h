/*
 * FitnessExp.h
 *
 *  Created on: 17 Tem 2013
 *      Author: HalilCan
 */

#ifndef FITNESSEXP_H_
#define FITNESSEXP_H_

#include "IFitness.h"

namespace std {
class FitnessExp : public IFitness {
public:
	FitnessExp(const Chromosome *chromosome);
	FitnessExp(const IFitness*);
	virtual ~FitnessExp();

	void calcFit(fitness_t& fit, int type = hc_both, int print = 0);

private:
	Common *conf;
	void course_conflict(fitness_t& fit);
	void consecutive_semester_conflict(fitness_t& fit);
	void lecturer_checks(fitness_t& fit);
	bool do_overlap(int, int);
};

}

#endif /* FITNESSEXP_H_ */
