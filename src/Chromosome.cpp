/*
 * Chromosome.cpp
 *
 *  Created on: 06 Kas 2012
 *      Author: HalilCan
 */

#include "Chromosome.h"

namespace std {

Chromosome::Chromosome(int chrom_length, int no_colors) {
	this->chrom_length = chrom_length;
	this->no_colors = no_colors;
}

Chromosome::~Chromosome() {
	// TODO Auto-generated destructor stub
}

void Chromosome::add(int section, int val) {
	chrom[section] = val;
	slot_map[val].push_back(section);
	day[section] = -1;
	slot[section] = -1;
}

void Chromosome::update(int section, int val) {
	//remove destination slot from slot_map.
	slot_map[chrom[section]].remove(section);
	chrom[section] = val;
	//add the slot to appropriate position
	slot_map[val].push_back(section);
	day[section] = val / PERIODS;
	slot[section] = val % PERIODS;
}

void Chromosome::update_slot(int section) {
	day[section] = chrom[section] / PERIODS;
	slot[section] = chrom[section] % PERIODS;
}

int Chromosome::get_slot(int section) {
	return chrom[section];
}

list<int> *Chromosome::get_longest_slot() {
	list<int> *thelist = &slot_map[0];
	for (int i = 1; i < NCOL; ++i) {
		if (slot_map[i].size() > thelist->size()) {
			thelist = &slot_map[i];
		}
	}
	return thelist;
}

list<int> *Chromosome::get_section_list(int slot) {
	return &slot_map[slot];
}

} /* namespace std */
