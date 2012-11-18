/*
 * Chromosome.h
 *
 *  Created on: 06 Kas 2012
 *      Author: HalilCan
 */

#ifndef CHROMOSOME_H_
#define CHROMOSOME_H_

#include "Constants.h"
#include <list>

namespace std {

class Chromosome {
public:
	Chromosome(int chrom_length, int no_colors);
	virtual ~Chromosome();
	void add(int section, int slot);
	void update(int section, int slot);
	void update_slot(int section);
	int get_slot(int section);
	list<int> *get_longest_slot();
	list<int> *get_section_list(int slot);
	int day[CHROML];
	int slot[CHROML];
	int chrom_length;
	int no_colors;
	list<int> slot_map[NCOL];
private:
	//our chromosome. Consist of all sections. Value represents the slot they are assigned to.
	int chrom[CHROML];
	//holds the sections assigned to that slot. Used to directly access sections for particular slot.
};

} /* namespace std */
#endif /* CHROMOSOME_H_ */
