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

Chromosome::Chromosome(Chromosome* source) {
	int i;
	this->chrom_length = source->chrom_length;
	this->no_colors = source->no_colors;
	fit = new Fitness(this);

	for (i = 0; i < NCOL; ++i) {
		this->slot_map[i] = source->slot_map[i];
	}
	for (i = 0; i < CHROML; ++i) {
		this->day[i] = source->day[i];
		this->slot[i] = source->slot[i];
		this->chrom[i] = source->chrom[i];
	}
}

Chromosome::~Chromosome() {
	delete fit;
}

void Chromosome::add(int section, int val) {
	chrom[section] = val;
	slot_map[val].push_back(section);
	day[section] = val / PERIODS;
	slot[section] = val % PERIODS;
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

const int Chromosome::get_slot(int section) const{
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

void Chromosome::updatefitness(int val) {
	fit->updatefitness(val);
}

void Chromosome::calc_hardfit(const std::list<int>& list, s_hard_fitness_t& fitness, int print) {
	fit->calc_hardfit(list, fitness, print);
}

void Chromosome::calc_softfit(const list<int>& list, s_soft_fitness_t& fitness, int print) {
	fit->calc_softfit(list, fitness, print);

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
