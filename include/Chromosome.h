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

	void updatefitness(int);
	void calc_hardfit(const list<int> &, s_hard_fitness_t&, int);
	void calc_softfit(const list<int> &, s_soft_fitness_t&, int);
	void add(int section, int slot);
	void update(int section, int slot);
	void update_slot(int section);
	const int get_slot(int section) const;
	list<int> *get_longest_slot();
	list<int> *get_section_list(int slot);
	const s_hard_fitness_t& get_hardfit();
	const s_soft_fitness_t& get_softfit();
	int day[CHROML];
	int slot[CHROML];
	int chrom_length;
	int no_colors;
	//holds the sections assigned to that slot. Used to directly access sections for particular slot.
	list<int> slot_map[NCOL];
	friend class Fitness;

private:
	//our chromosome. Consist of all sections. Value represents the slot they are assigned to.
	int chrom[CHROML];
	Fitness *fit;
};

} /* namespace std */
#endif /* CHROMOSOME_H_ */
