/*
 * Mutation.cpp
 *
 *  Created on: 10 Kas 2012
 *      Author: HalilCan
 */

#include "Mutation.h"
#include "Constants.h"
#include <stdlib.h>

namespace std {

Mutation::Mutation(Common *conf) {
	this->conf = conf;
	chromosome = NULL;
}

Mutation::~Mutation() {
	// TODO Auto-generated destructor stub
}

void Mutation::mutate() {
	int pos, val;
	pos = RND(CHROML);
	val = RND(NCOL);
	if (conf->courmat[pos].has_constraint != 1) {
		chromosome->update(pos, val);
	}
}

void Mutation::mutateg1() {
	int col1, col2, i, k;
	for (k = 0; k < chromosome->chrom_length; k++) {
		if (RND(1000000) > 1000000 * conf->mutg1rate)
			continue;
		col1 = chromosome->get_slot(k);
		col2 = RND(chromosome->no_colors);
		for (i = 0; i < chromosome->chrom_length; i++) {
			if (chromosome->get_slot(i) == col1) {
				chromosome->update(i, col2);
			} else if (chromosome->get_slot(i) == col2) {
				chromosome->update(i, col1);
			}
		}
	}
}

void Mutation::mutateg3() {
	int i, k;
	int pos1, pos2;
	int val1, val2;
	for (k = 0; k < chromosome->chrom_length; k++) {
		if (RND(1000000) > 1000000 * conf->mutg3rate)
			continue;
		pos1 = k;
		pos2 = RND(chromosome->chrom_length);
		while (pos1 == pos2) {
			pos2 = RND(POPUL);
		}
		val1 = chromosome->get_slot(pos1);
		val2 = chromosome->get_slot(pos2);
		for (i = 0; i < chromosome->chrom_length; i++) {
			if (chromosome->get_slot(i) == val1 && RND(100) < 50) {
				chromosome->update(i, val2);
			}
		}
	}
}

void Mutation::mutateg5() {
	int i, sel, k;
	int pos1, pos2, pos3;
	int val1, val2, val3;
	for (k = 0; k < chromosome->chrom_length; k++) {
		if (RND(1000000) > 1000000 * conf->mutg5rate)
			continue;
		pos1 = k;
		pos2 = RND(chromosome->chrom_length);
		while (pos1 == pos2) {
			pos2 = RND(POPUL);
		}
		pos3 = RND(chromosome->chrom_length);
		while (pos3 == pos2 || pos3 == pos1) {
			pos3 = RND(POPUL);
		}
		val1 = chromosome->get_slot(pos1);
		val2 = chromosome->get_slot(pos2);
		val3 = chromosome->get_slot(pos3);
		for (i = 0; i < chromosome->chrom_length; i++) {
			sel = RND(2);
			if (chromosome->get_slot(i) == val3 && sel == 0 && RND(100) < 50) {
				chromosome->update(i, val1);
			} else if (chromosome->get_slot(i) == val3 && sel == 1 && RND(100) < 50) {
				chromosome->update(i, val2);
			}
		}
	}
}

void Mutation::mutate_all() {
	mutate();
	mutateg1();
	mutateg3();
	mutateg5();
}

void Mutation::setChromosome(Chromosome* chromosome) {
	this->chromosome = chromosome;
}

} /* namespace std */
