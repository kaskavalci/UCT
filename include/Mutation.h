/*
 * Mutation.h
 *
 *  Created on: 10 Kas 2012
 *      Author: HalilCan
 */

#ifndef MUTATION_H_
#define MUTATION_H_

#include "Chromosome.h"
#include "Common.h"

namespace std {

class Mutation {
public:
	Mutation(Chromosome *);
	virtual ~Mutation();
	bool mutate();
	bool mutateg1();
	bool mutateg3();
	bool mutateg5();
	bool mutate_all();

private:
	Chromosome *chromosome;
	Common *conf;
};

} /* namespace std */
#endif /* MUTATION_H_ */
