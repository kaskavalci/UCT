/*
 * Chromosome.cpp
 *
 *  Created on: 06 Kas 2012
 *      Author: HalilCan
 */

#include "Chromosome.h"
#include <iostream>
#include "FitnessExp.h"

namespace std {

Chromosome::Chromosome(int chrom_length) {
	this->chrom_length = chrom_length;
	chrom = new vector<int>(chrom_length, -1);
	day = new vector<int>(chrom_length, -1);
	slot = new vector<int>(chrom_length, -1);
	fit = new FitnessExp(this);
}

Chromosome::Chromosome(const Chromosome* source) {
	int i;
	this->chrom_length = source->chrom_length;
	this->fit = new FitnessExp(source->fit);
	this->fit->setChromosome(this);

	for (i = 0; i < NCOL; ++i) {
		this->slot_map[i] = source->slot_map[i];
	}
	this->chrom = new vector<int>(*source->chrom);
	this->day = new vector<int>(*source->day);
	this->slot = new vector<int>(*source->slot);
}

Chromosome::~Chromosome() {
	delete fit;
	delete chrom;
	delete day;
	delete slot;
}

void Chromosome::add(int section, int val) {
	chrom->at(section) = val;
	day->at(section) = val / PERIODS;
	slot->at(section) = val % PERIODS;
	slot_map[val].push_back(section);
}
/*
 * update given section to given slot.
 */
void Chromosome::update(int section, int _slot) {
	//remove destination slot from slot_map.
	slot_map[chrom->at(section)].remove(section);
	chrom->at(section) = _slot;
	day->at(section) = _slot / PERIODS;
	slot->at(section) = _slot % PERIODS;
	//add the slot to appropriate position
	slot_map[_slot].push_back(section);
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
