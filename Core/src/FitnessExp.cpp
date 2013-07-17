/*
 * FitnessExp.cpp
 *
 *  Created on: 17 Tem 2013
 *      Author: HalilCan
 */

#include "FitnessExp.h"

#include <Common.h>
#include <Constants.h>
#include <IFitness.h>
#include <algorithm>
#include <array>
#include <new>
#include <vector>

namespace std {

FitnessExp::FitnessExp(const Chromosome* chrom) {
	this->chromosome = chrom;
	conf = Common::getConf();
	fitness = new fitness_t(conf->ChromSize);
	fill(fitness->fitness, fitness->fitness + TOT_FIT_N, 0);
}

FitnessExp::FitnessExp(const IFitness* source) {
	conf = Common::getConf();
	fitness = new fitness_t(conf->ChromSize);
	chromosome = NULL;
	*fitness = source->getFit();
}

FitnessExp::~FitnessExp() {
	delete fitness;
}

void FitnessExp::calcFit(fitness_t& fit, int type, int print) {
	fit.hard_fit = 0;
	fit.soft_fit = 0;
	//init chrom IDs in fitnessBySect struct. it is to be used while sorting
	for (int i = 0; i < conf->ChromSize; ++i) {
		fit.fitnessBySect[i].fill(0);
		fit.fitnessBySect[i][fit_mGeneID] = i;
	}
	fill(fit.fitness, fit.fitness + TOT_FIT_N, 0);

	//hard fitness calculation
	if (type == hc_hard || type == hc_both) {
		course_conflict(fit);
	}
	//soft fitness calculation
	if (type == hc_soft || type == hc_both) {

	}
	fit.total_fit = fit.hard_fit + fit.soft_fit;
}

void std::FitnessExp::course_conflict(fitness_t& fit) {

}

} /* namespace std */
