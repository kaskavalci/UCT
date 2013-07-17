/*
 * IFitness.h
 *
 *  Created on: 17 Tem 2013
 *      Author: HalilCan
 */

#ifndef IFITNESS_H_
#define IFITNESS_H_

#include "Common.h"

namespace std {
class Chromosome;
class IFitness {
public:
	virtual ~IFitness() {};

	virtual void calcFit(fitness_t&, int type = hc_both, int print = 0) = 0;

	void updateFitness(int val) {
		calcFit(*fitness, hc_both, val);
	}

	const inline fitness_t& getFit() const {
		return *fitness;
	}

	void setChromosome(const Chromosome* chromosome) {
		this->chromosome = chromosome;
	}

protected:
	const Chromosome *chromosome;
	Common *conf;
	fitness_t *fitness;
};
}

#endif /* IFITNESS_H_ */
