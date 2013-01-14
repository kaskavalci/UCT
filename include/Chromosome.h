/*
 * Chromosome.h
 *
 *  Created on: 06 Kas 2012
 *      Author: HalilCan
 */

#ifndef CHROMOSOME_H_
#define CHROMOSOME_H_

#include "Constants.h"
#include "Fitness.h"
#include <list>

namespace std {
class Chromosome {
public:
	Chromosome(int chrom_length, int no_colors);
	Chromosome(const Chromosome*);
	virtual ~Chromosome();
	friend class Fitness;

	void add(int section, int slot);
	void update(int section, int slot);
	list<int> *get_longest_slot();
	list<int> *get_section_list(int slot);

	inline int get_day(int section) const {
		return day->at(section);
	}
	inline int get_period(int section) const {
		return slot->at(section);
	}
	inline int get_slot(int section) const{
		return chrom->at(section);
	}

	const inline fitness_t& getFit() const {
		return fit->getFit();
	}
	inline void calcFit(int print, fitness_t &fitness, int type) const {
		fit->calcFit(print,fitness, type);
	}
	inline void updatefitness(int p) const {
		fit->updateFitness(p);
	}

	int chrom_length;
	int no_colors;
	//holds the sections assigned to that slot. Used to directly access sections for particular slot.

	Fitness *fit;
private:
	//our chromosome. Consist of all sections. Value represents the slot they are assigned to.
	list<int> slot_map[NCOL];
	vector <int> *chrom, *day, *slot;

};

} /* namespace std */
#endif /* CHROMOSOME_H_ */
