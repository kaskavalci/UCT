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

/**
 * Depreciated. Mutation now takes place in Individual.
 * @param chrom
 */
Mutation::Mutation(Chromosome *chrom) {
	this->conf = Common::getConf();
	chromosome = chrom;
}

Mutation::~Mutation() {
	// TODO Auto-generated destructor stub
}

bool Mutation::mutate() {
	if (RND(1000000) < 10000 * conf->mutrate)
	{
		int pos, val;
		pos = RND(conf->ChromSize);
		val = RND(NCOL);
		//mutate only if no constraint given on that lecture
		if (!conf->courmat[pos].has_cons) {
			chromosome->update(pos, val);
			return true;
		}
	}
	return false;
}

bool Mutation::mutateg1() {
	int col1, col2, i, k;
	bool retval = false;
	for (k = 0; k < conf->ChromSize; k++) {
		if (RND(1000000) >= 10000 * conf->mutrate)
			continue;
		col1 = chromosome->get_slot(k);
		col2 = RND(chromosome->no_colors);
		for (i = 0; i < conf->ChromSize; i++) {
			if (conf->courmat[i].has_cons) continue;
			if (chromosome->get_slot(i) == col1) {
				chromosome->update(i, col2);
				retval = true;
			} else if (chromosome->get_slot(i) == col2) {
				chromosome->update(i, col1);
				retval = true;
			}
		}
	}
	return retval;
}

bool Mutation::mutateg3() {
	int i, k;
	int pos1, pos2;
	int val1, val2;
	bool retval = false;
	for (k = 0; k < conf->ChromSize; k++) {
		if (RND(1000000) >= 10000 * conf->mutrate)
			continue;
		pos1 = k;
		pos2 = RND(conf->ChromSize);
		while (pos1 == pos2) {
			pos2 = RND(conf->pop_size);
		}
		val1 = chromosome->get_slot(pos1);
		val2 = chromosome->get_slot(pos2);
		for (i = 0; i < conf->ChromSize; i++) {
			if (conf->courmat[i].has_cons) continue;
			if (chromosome->get_slot(i) == val1 && RND(100) < 50) {
				chromosome->update(i, val2);
				retval = true;
			}
		}
	}
	return retval;
}

bool Mutation::mutateg5() {
	int i, sel, k;
	int pos1, pos2, pos3;
	int val1, val2, val3;
	bool retval = false;
	for (k = 0; k < conf->ChromSize; k++) {
		if (RND(1000000) >= 10000 * conf->mutrate)
			continue;
		pos1 = k;
		pos2 = RND(conf->ChromSize);
		while (pos1 == pos2) {
			pos2 = RND(conf->pop_size);
		}
		pos3 = RND(conf->ChromSize);
		while (pos3 == pos2 || pos3 == pos1) {
			pos3 = RND(conf->pop_size);
		}
		val1 = chromosome->get_slot(pos1);
		val2 = chromosome->get_slot(pos2);
		val3 = chromosome->get_slot(pos3);
		for (i = 0; i < conf->ChromSize; i++) {
			if (conf->courmat[i].has_cons) continue;
			sel = RND(2);
			if (chromosome->get_slot(i) == val3 && sel == 0 && RND(100) < 50) {
				chromosome->update(i, val1);
				retval = true;
			} else if (chromosome->get_slot(i) == val3 && sel == 1 && RND(100) < 50) {
				chromosome->update(i, val2);
				retval = true;
			}
		}
	}
	return retval;
}
/*
 * mutates genes that does not have constraints.
 */
bool Mutation::mutate_all() {
	bool retval = false;
	retval |= mutate();
	retval |= mutateg1();
	retval |= mutateg3();
	retval |= mutateg5();
	return retval;
}

} /* namespace std */
