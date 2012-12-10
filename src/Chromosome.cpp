/*
 * Chromosome.cpp
 *
 *  Created on: 06 Kas 2012
 *      Author: HalilCan
 */

#include "Chromosome.h"
#include <iostream>

namespace std {

Chromosome::Chromosome(int chrom_length, int no_colors) {
	this->chrom_length = chrom_length;
	this->no_colors = no_colors;
	fit = new Fitness(this);
}

Chromosome::Chromosome(const Chromosome* source) {
	int i;
	this->chrom_length = source->chrom_length;
	this->no_colors = source->no_colors;
	this->fit = new Fitness(source->fit);
	this->fit->setChromosome(this);

	for (i = 0; i < NCOL; ++i) {
		this->slot_map[i] = source->slot_map[i];
	}
	for (i = 0; i < CHROML; ++i) {
		this->chrom[i] = source->chrom[i];
		this->day[i] = source->day[i];
		this->slot[i] = source->slot[i];
	}
}

Chromosome::~Chromosome() {
	delete fit;
}

void Chromosome::add(int section, int val) {
	chrom[section] = val;
	day[section] = val / PERIODS;
	slot[section] = val % PERIODS;
	slot_map[val].push_back(section);
}
/*
 * update given section to given slot.
 */
void Chromosome::update(int section, int _slot) {
	//remove destination slot from slot_map.
	slot_map[chrom[section]].remove(section);
	chrom[section] = _slot;
	day[section] = _slot / PERIODS;
	slot[section] = _slot % PERIODS;
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

void Chromosome::updatefitness(int val) {
	fit->updatefitness(val);
}

void Chromosome::calc_hardfit(s_hard_fitness_t& fitness, int print) {
	fit->calc_hardfit(fitness, print);
}

void Chromosome::calc_softfit(s_soft_fitness_t& fitness, int print) {
	fit->calc_softfit(fitness, print);

}

list<int> *Chromosome::get_section_list(int slot) {
	return &slot_map[slot];
}

const s_hard_fitness_t& Chromosome::get_hardfit() {
	return fit->getHardFit();
}

const s_soft_fitness_t& Chromosome::get_softfit() {
	return fit->getSoftFit();
}

} /* namespace std */
