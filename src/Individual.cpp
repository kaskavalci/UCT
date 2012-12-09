/*
 * Individual.cpp
 *
 *  Created on: 21 Eki 2012
 *      Author: HalilCan
 */

#include "Individual.h"
#include "Common.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "FileReader.h"
#include <algorithm>
#include <vector>

namespace std {

Individual::Individual(Common *conf) {
	int i, j, cnt = 0;

	this->conf = conf;
	chrom_length = CHROML;
	no_periods = 4;
	chromosome = new Chromosome(chrom_length, NCOL);

	for (i = 0; i < CHROML; i++) {
		if (conf->courmat[i].has_constraint == 1) {
			chromosome->add(i, cnt % NCOL);
			cnt++;
		} else {
			chromosome->add(i, RND(NCOL));
		}
	}
	for (i = 0; i < 5; i++) {
		for (j = 0; j < 4; j++) {
			timetable1[i][j] = -1;
			timetable2[i][j] = -1;
		}
	}
	for (i = 0; i < CHROML; i++) {
		for (j = 0; j < CHROML; j++) {
			if (conf->courmat[i].has_constraint == 1 && conf->courmat[j].has_constraint == 1
					&& conf->courmat[i].hours == conf->courmat[j].hours
					&& conf->courmat[i].c2day == conf->courmat[j].c2day
					&& conf->courmat[i].c2slot == conf->courmat[j].c2slot) {
				//assign i's slot to j's.
				chromosome->update(j, chromosome->get_slot(i));
			}
		}
	}
	buildtimetable();
	chromosome->updatefitness(0);
}

Individual::Individual() {
	int i, j;
	chrom_length = CHROML;
	no_periods = 4;
	chromosome = new Chromosome(chrom_length, NCOL);

	conf = Common::getConf();

	for (i = 0; i < 5; i++) {
		for (j = 0; j < 4; j++) {
			timetable1[i][j] = -1;
			timetable2[i][j] = -1;
		}
	}
}

Individual::~Individual() {
	delete (chromosome);
}

Individual::Individual(const Individual& source) {
	int i, j;
	for (i = 0; i < 5; i++) {
		for (j = 0; j < 4; j++) {
			timetable1[i][j] = source.timetable1[i][j];
			timetable2[i][j] = source.timetable2[i][j];
		}
	}

	this->chromosome = new Chromosome(source.chromosome);
	this->chrom_length = source.chrom_length;
	this->conf = source.conf;
	this->no_periods = source.no_periods;
}

Individual &Individual::operator=(const Individual &source) {
	int i, j;
	for (i = 0; i < 5; i++) {
		for (j = 0; j < 4; j++) {
			timetable1[i][j] = source.timetable1[i][j];
			timetable2[i][j] = source.timetable2[i][j];
		}
	}
	if (this->chromosome)
		delete (this->chromosome);
	this->chromosome = new Chromosome(source.chromosome);
	this->chrom_length = source.chrom_length;
	this->conf = source.conf;
	this->no_periods = source.no_periods;
	return *this;
}
//TODO chance hc1 for new fitness class
/*
 bool Individual::hc1() {
 int rndidx;
 int selcolor;
 Individual hcchild(*this);
 bool finalchange = false;

 if (RND(1000) < 1000 * conf->hillrnd) {
 do {
 rndidx = RND(chrom_length);
 } while (conf->courmat[rndidx].has_constraint == 1);
 } else {
 return false;
 //rndidx = conflv[RND(conflv.size())];
 }

 for (selcolor = 0; selcolor < NCOL; selcolor++) {
 //mutate the child's gene rndidx for every color except for its own
 if (hcchild.chromosome->get_slot(rndidx) == selcolor)
 continue;
 hcchild.chromosome->update(rndidx, selcolor);
 hcchild.buildtimetable();
 hcchild.updatefitness();

 if ((hcchild.fitnessh + hcchild.fitnessh1 < this->fitnessh + this->fitnessh1)
 && (hcchild.fitnessf + hcchild.fitnessf2 < this->fitnessf + this->fitnessf2)) {
 *this = hcchild;
 return true;
 }
 }
 //conflv.clear();
 return finalchange;
 }
 */

void Individual::cross(const Individual& p1, const Individual &p2) {
	bool checked[CHROML] = { };
	list<int> *src_list, *candidate_list1, *candidate_list2;
	for (int i = 0; i < NCOL; ++i) {
		candidate_list1 = p1.chromosome->get_section_list(i);
		candidate_list2 = p2.chromosome->get_section_list(i);
		if (candidate_list1->size() > candidate_list2->size()) {
			src_list = candidate_list1;
		} else {
			src_list = candidate_list2;
		}
		list<int>::const_iterator ite = src_list->end();
		for (list<int>::const_iterator it = src_list->begin(); it != ite; it++) {
			if (checked[*it] == false) {
				this->chromosome->add(*it, i);
				checked[*it] = true;
			}
		}
	}
	Chromosome *chrom;
	//we copied the largest sets from p1 and p2. but there might have chromosomes haven't copied yet. Randomly inherit from parents.
	for (int i = 0; i < CHROML; ++i) {
		if (checked[i] == false) {
			switch (RND(2)) {
			case 0:
				chrom = p1.chromosome;
				break;
			case 1:
				chrom = p2.chromosome;
				break;
			default:
				chrom = p1.chromosome;
				break;
			}
			this->chromosome->add(i, chrom->get_slot(i));
		}
	}
}

/*
 * dominates: return D_TRUE if "this" Individual dominates (has superior fitness) target.
 * 			return D_FALSE if target dominates "this"
 * 			return D_NO_DOMINATION if there is no domination.
 */
int Individual::dominates(const Individual *target) {
	enum {
		soft, hard
	};
	bool target_dominates[2] = { true, true }, this_dominates[2] = { true, true };
	int tot_this, tot_targ;
	for (vector<vector<int> >::const_iterator group = conf->hardgroup.begin(); group != conf->hardgroup.end();
			++group) {
		tot_this = tot_targ = 0;
		for (vector<int>::const_iterator el = group->begin(); el != group->end(); el++) {
			tot_this += this->getHardFit().fitness[*el];
			tot_targ += target->getHardFit().fitness[*el];
		}
		this_dominates[hard] &= (tot_this < tot_targ);
		target_dominates[hard] &= (tot_targ <= tot_this);
	}
	for (vector<vector<int> >::const_iterator group = conf->softgroup.begin(); group != conf->softgroup.end();
			++group) {
		tot_this = tot_targ = 0;
		for (vector<int>::const_iterator el = group->begin(); el != group->end(); el++) {
			tot_this += this->getSoftFit().fitness[*el];
			tot_targ += target->getSoftFit().fitness[*el];
		}
		this_dominates[soft] &= (tot_this < tot_targ);
		target_dominates[soft] &= (tot_targ <= tot_this);
	}
	//target is dominated by this, in soft and hard constraints
	if (this_dominates[hard] && this_dominates[soft]) {
		return D_TRUE;
	} else if (target_dominates[hard] && target_dominates[soft]) {
		return D_FALSE;
	} else if (this_dominates[hard]) {
		return D_NO_HARDDOMINATION;
	} else
		return D_NO_DOMINATION;
}

void Individual::printlect() {
	size_t h;
	for (h = 0; h < conf->lecturers.size(); h++) {
		int m, n;
		int lectmatrix[5][10];
		for (m = 0; m < 5; m++) {
			for (n = 0; n < 10; n++)
				lectmatrix[m][n] = 0;
		}
		for (m = 0; m < CHROML; m++) {
			if (conf->lecturers[h] == conf->courmat[m].lname) {
				if (conf->courmat[m].hours == 1) {
					switch (chromosome->slot[m]) {
					case 0:
						lectmatrix[chromosome->day[m]][2] = 1;
						break;
					case 1:
						lectmatrix[chromosome->day[m]][3] = 1;
						break;
					case 2:
						lectmatrix[chromosome->day[m]][4] = 1;
						break;
					case 3:
						lectmatrix[chromosome->day[m]][9] = 1;
						break;
					}
				}
				if (conf->courmat[m].hours == 2) {
					switch (chromosome->slot[m]) {
					case 0:
						lectmatrix[chromosome->day[m]][0] = 1;
						lectmatrix[chromosome->day[m]][1] = 1;
						break;
					case 1:
						lectmatrix[chromosome->day[m]][2] = 1;
						lectmatrix[chromosome->day[m]][3] = 1;
						break;
					case 2:
						lectmatrix[chromosome->day[m]][5] = 1;
						lectmatrix[chromosome->day[m]][6] = 1;
						break;
					case 3:
						lectmatrix[chromosome->day[m]][7] = 1;
						lectmatrix[chromosome->day[m]][8] = 1;
						break;
					}
				}
			}
		}
		for (m = 0; m < 5; m++) {
			for (n = 0; n < 10; n++) {
				if (lectmatrix[m][n] == 1)
					printf(" %-9s ", conf->lecturers[h].data());
				else
					printf(" OOOOO0000 ");
			}
			printf("\n");
		}
		printf("\n");
	}
}

void Individual::printdekanlik() {
	size_t i;
	string sched2, sched1;
	int lidx;
	for (i = 0; i < CHROML; i++) {
		lidx = conf->findlecture(i);
		if (lidx != -1) {
			if (conf->courmat[i].cname.at(7) == '0' && conf->courmat[i].hours == 1
					&& conf->courmat[i].cname.at(8) == '1') {
				conf->lectures[lidx].cid1day = chromosome->day[i];
				conf->lectures[lidx].cid1slot = chromosome->slot[i];
			}
			if (conf->courmat[i].cname.at(7) == '0' && conf->courmat[i].hours == 2
					&& conf->courmat[i].cname.at(8) == '1') {
				conf->lectures[lidx].cid2day = chromosome->day[i];
				conf->lectures[lidx].cid2slot = chromosome->slot[i];
			}
			if (conf->courmat[i].cname.at(7) == '0' && conf->courmat[i].hours == 1
					&& conf->courmat[i].cname.at(8) == '2') {
				conf->lectures[lidx].cid3day = chromosome->day[i];
				conf->lectures[lidx].cid3slot = chromosome->slot[i];
			}
			if (conf->courmat[i].cname.at(7) == '0' && conf->courmat[i].hours == 2
					&& conf->courmat[i].cname.at(8) == '2') {
				conf->lectures[lidx].cid4day = chromosome->day[i];
				conf->lectures[lidx].cid4slot = chromosome->slot[i];
			}
			if (conf->courmat[i].cname.at(7) == 'L' && conf->courmat[i].cname.at(8) == '1'
					&& conf->courmat[i].hours == 2) {
				conf->lectures[lidx].lab1day = chromosome->day[i];
				conf->lectures[lidx].lab1slot = chromosome->slot[i];
			}
			if (conf->courmat[i].cname.at(7) == 'L' && conf->courmat[i].cname.at(8) == '2'
					&& conf->courmat[i].hours == 2) {
				conf->lectures[lidx].lab2day = chromosome->day[i];
				conf->lectures[lidx].lab2slot = chromosome->slot[i];
			}
			if (conf->courmat[i].cname.at(7) == 'L' && conf->courmat[i].hours == 1) {
				conf->lectures[lidx].lab3day = chromosome->day[i];
				conf->lectures[lidx].lab3slot = chromosome->slot[i];
			}
		}
	}
	printf("\n");
	for (i = 0; i < conf->lectures.size(); i++) {
		sched2.clear();
		switch (conf->lectures[i].cid2day) {
		case 0:
			sched2 = "M";
			break;
		case 1:
			sched2 = "T";
			break;
		case 2:
			sched2 = "W";
			break;
		case 3:
			sched2 = "R";
			break;
		case 4:
			sched2 = "F";
			break;
		}
		switch (conf->lectures[i].cid2slot) {
		case 0:
			sched2.append("12");
			break;
		case 1:
			sched2.append("34");
			break;
		case 2:
			sched2.append("67");
			break;
		case 3:
			sched2.append("89");
			break;
		}
		sched1.clear();
		switch (conf->lectures[i].cid1day) {
		case 0:
			sched1 = "M";
			break;
		case 1:
			sched1 = "T";
			break;
		case 2:
			sched1 = "W";
			break;
		case 3:
			sched1 = "R";
			break;
		case 4:
			sched1 = "F";
			break;
		}
		switch (conf->lectures[i].cid1slot) {
		case 0:
			sched1.append("3");
			break;
		case 1:
			sched1.append("4");
			break;
		case 2:
			sched1.append("5");
			break;
		case 3:
			sched1.append("X");
			break;
		}
		if (!sched1.empty()) {
			sched2.append("-");
			sched2.append(sched1);
		}
		if (!sched2.empty() && !conf->courmat[conf->lectures[i].cid2].cname.empty())
			printf("%-12s %-12s  %-12s %-12c %-12s \n", conf->courmat[conf->lectures[i].cid2].lname.data(),
					conf->courmat[conf->lectures[i].cid2].cname.data(), conf->lectures[i].lectname.data(),
					conf->courmat[conf->lectures[i].cid2].cname.at(8), sched2.data());
	}
	for (i = 0; i < conf->lectures.size(); i++) {
		sched2.clear();
		switch (conf->lectures[i].cid4day) {
		case 0:
			sched2 = "M";
			break;
		case 1:
			sched2 = "T";
			break;
		case 2:
			sched2 = "W";
			break;
		case 3:
			sched2 = "R";
			break;
		case 4:
			sched2 = "F";
			break;
		}
		switch (conf->lectures[i].cid4slot) {
		case 0:
			sched2.append("12");
			break;
		case 1:
			sched2.append("34");
			break;
		case 2:
			sched2.append("67");
			break;
		case 3:
			sched2.append("89");
			break;
		}
		sched1.clear();
		switch (conf->lectures[i].cid3day) {
		case 0:
			sched1 = "M";
			break;
		case 1:
			sched1 = "T";
			break;
		case 2:
			sched1 = "W";
			break;
		case 3:
			sched1 = "R";
			break;
		case 4:
			sched1 = "F";
			break;
		}
		switch (conf->lectures[i].cid3slot) {
		case 0:
			sched1.append("3");
			break;
		case 1:
			sched1.append("4");
			break;
		case 2:
			sched1.append("5");
			break;
		case 3:
			sched1.append("X");
			break;
		}
		if (!sched1.empty()) {
			sched2.append("-");
			sched2.append(sched1);
		}
		if (!sched2.empty() && !conf->courmat[conf->lectures[i].cid4].cname.empty())
			printf("%-12s %-12s  %-12s %-12c %-12s \n", conf->courmat[conf->lectures[i].cid4].lname.data(),
					conf->courmat[conf->lectures[i].cid4].cname.data(), conf->lectures[i].lectname.data(),
					conf->courmat[conf->lectures[i].cid4].cname.at(8), sched2.data());
	}
	for (i = 0; i < conf->lectures.size(); i++) {
		sched2.clear();
		switch (conf->lectures[i].lab1day) {
		case 0:
			sched2 = "M";
			break;
		case 1:
			sched2 = "T";
			break;
		case 2:
			sched2 = "W";
			break;
		case 3:
			sched2 = "R";
			break;
		case 4:
			sched2 = "F";
			break;
		}
		switch (conf->lectures[i].lab1slot) {
		case 0:
			sched2.append("12");
			break;
		case 1:
			sched2.append("34");
			break;
		case 2:
			sched2.append("67");
			break;
		case 3:
			sched2.append("89");
			break;
		}
		if (!sched2.empty() && !conf->courmat[conf->lectures[i].lab1].cname.empty())
			printf("%-12s %-12s  %-12s %-12c %-12s \n", conf->courmat[conf->lectures[i].lab1].lname.data(),
					conf->courmat[conf->lectures[i].lab1].cname.data(), conf->lectures[i].lectname.data(),
					conf->courmat[conf->lectures[i].lab1].cname.at(8), sched2.data());
		sched2.clear();
		switch (conf->lectures[i].lab2day) {
		case 0:
			sched2 = "M";
			break;
		case 1:
			sched2 = "T";
			break;
		case 2:
			sched2 = "W";
			break;
		case 3:
			sched2 = "R";
			break;
		case 4:
			sched2 = "F";
			break;
		}
		switch (conf->lectures[i].lab2slot) {
		case 0:
			sched2.append("12");
			break;
		case 1:
			sched2.append("34");
			break;
		case 2:
			sched2.append("67");
			break;
		case 3:
			sched2.append("89");
			break;
		}
		if (!sched2.empty() && !conf->courmat[conf->lectures[i].lab2].cname.empty())
			printf("%-12s %-12s  %-12s %-12c %-12s \n", conf->courmat[conf->lectures[i].lab2].lname.data(),
					conf->courmat[conf->lectures[i].lab2].cname.data(), conf->lectures[i].lectname.data(),
					conf->courmat[conf->lectures[i].lab2].cname.at(8), sched2.data());
		sched2.clear();
		switch (conf->lectures[i].lab3day) {
		case 0:
			sched2 = "M";
			break;
		case 1:
			sched2 = "T";
			break;
		case 2:
			sched2 = "W";
			break;
		case 3:
			sched2 = "R";
			break;
		case 4:
			sched2 = "F";
			break;
		}
		switch (conf->lectures[i].lab3slot) {
		case 0:
			sched2.append("3");
			break;
		case 1:
			sched2.append("4");
			break;
		case 2:
			sched2.append("5");
			break;
		case 3:
			sched2.append("X");
			break;
		}
		if (!sched2.empty() && !conf->courmat[conf->lectures[i].lab3].cname.empty())
			printf("%-12s %-12s  %-12s %-12c %-12s \n", conf->courmat[conf->lectures[i].lab3].lname.data(),
					conf->courmat[conf->lectures[i].lab3].cname.data(), conf->lectures[i].lectname.data(),
					conf->courmat[conf->lectures[i].lab3].cname.at(8), sched2.data());
	}
}

void Individual::buildtimetable() {
	int i;

	for (i = 0; i < CHROML; i++) {
		if (conf->courmat[i].has_constraint == 1) {
			chromosome->update(i, 4 * conf->courmat[i].c2day + conf->courmat[i].c2slot);
			//update method updates day and slot but we have to overwrite it for this occasion.
			chromosome->day[i] = conf->courmat[i].c2day;
			chromosome->slot[i] = conf->courmat[i].c2slot;
		} else {
			chromosome->update_slot(i);
		}
	}
}

void Individual::calc_hardfit(s_hard_fitness_t& fit, int print) {
	chromosome->calc_hardfit(fit, print);
}

void Individual::calc_softfit(s_soft_fitness_t& fit, int print) {
	chromosome->calc_softfit(fit, print);
}

const s_hard_fitness_t& Individual::getHardFit() const {
	return chromosome->get_hardfit();
}

const s_soft_fitness_t& Individual::getSoftFit() const {
	return chromosome->get_softfit();
}

int Individual::findcourse(int sem, int dy, int slt) {
	size_t h;
	int found, rslot1, rslot2;
	switch (slt) {
	case 0:
		rslot1 = -1;
		rslot2 = 0;
		break;
	case 1:
		rslot1 = -1;
		rslot2 = 0;
		break;
	case 2:
		rslot1 = 0;
		rslot2 = 1;
		break;
	case 3:
		rslot1 = 1;
		rslot2 = 1;
		break;
	case 4:
		rslot1 = 2;
		rslot2 = -1;
		break;
	case 5:
		rslot1 = -1;
		rslot2 = 2;
		break;
	case 6:
		rslot1 = -1;
		rslot2 = 2;
		break;
	case 7:
		rslot1 = -1;
		rslot2 = 3;
		break;
	case 8:
		rslot1 = -1;
		rslot2 = 3;
		break;
	case 9:
		rslot1 = 3;
		rslot2 = -1;
		break;
	default:
		cerr << "invalid slot for Individual::findcourse.";
		exit(EXIT_FAILURE);
		break;
	}
	found = 0;
	for (h = 0; h < CHROML; h++) {
		if (conf->courmat[h].semid == sem && chromosome->day[h] == dy && conf->courmat[h].hours == 2
				&& chromosome->slot[h] == rslot2) {
			found = 1;
			break;
		} else if (conf->courmat[h].semid == sem && chromosome->day[h] == dy && conf->courmat[h].hours == 1
				&& chromosome->slot[h] == rslot1) {
			found = 1;
			break;
		}
	}
	if (found == 1)
		return h;
	else
		return -1;
}

void Individual::printtable() {
	int i, j, k, ret;
	for (i = 0; i < 8; i++) {
		printf("SEM  %d  \n", i + 1);
		printf("       MON            TUE            WED            THR            FRI     \n");
		for (j = 0; j < 10; j++) {
			printf("%-2d : ", j + 9);
			for (k = 0; k < 5; k++) {
				ret = findcourse(i + 1, k, j);
				if (ret != -1)
					printf("%-11s\t", conf->courmat[ret].cname.data());
				else
					printf("xxxxxxxxxxx\t");
			}
			printf("\n");
		}
		printf("\n");
	}
}

//TODO update hard and soft fitness structs
void Individual::updatefitness(int val) {
	chromosome->updatefitness(val);
}

bool Individual::hc_worstsection(int level) {
	Individual subject(*this);
	s_hard_fitness_t old_hfit, target_newHFit;
	s_soft_fitness_t old_sfit, target_newSFit;
	bool finalchange = false;
	int oldSlotHFit[NCOL] = { }, oldSlotSFit[NCOL] = { };
	vector<sorted_list_t> sortedListHard, sortedListSoft;

	//get fitnesses first
	if (level == hc_hard || level == hc_both) {
		calc_hardfit(old_hfit, 0);
	}
	if (level == hc_soft || level == hc_both) {
		calc_softfit(old_sfit, 0);
	}

	//iterate within all sections
	for (int k = 0; k < NCOL; ++k) {
		list<int>::const_iterator it, ite = subject.chromosome->get_section_list(k)->end();
		//in given slot, iterate through it and calculate the worst section.
		for (it = subject.chromosome->get_section_list(k)->begin(); it != ite; ++it) {
			int dummy = 0;
			//TODO: maybe multi objective?
			for (int i = 0; i < HARD_FIT_N; ++i) {
				dummy += old_hfit.fitnessBySect[*it][i];
			}
			oldSlotHFit[k] += dummy;
			sortedListHard.push_back(sorted_list_t(dummy, k, *it));

			dummy = 0;
			for (int i = 0; i < SOFT_FIT_N; ++i) {
				dummy += old_sfit.fitnessBySect[*it][i];
			}
			oldSlotSFit[k] += dummy;
			sortedListSoft.push_back(sorted_list_t(dummy, k, *it));
		}
	}
	sort(sortedListHard.begin(), sortedListHard.end(), compare());
	sort(sortedListSoft.begin(), sortedListSoft.end(), compare());

	vector<sorted_list_t>::reverse_iterator ritHard, ritSoft;
	ritHard = sortedListHard.rbegin();
	ritSoft = sortedListSoft.rbegin();

	for (int i = 0; i < conf->hc_max_ind && ritHard != sortedListHard.rend(); ++i, ++ritHard) {
		if (ritHard->max == 0) {
			//yay! perfect individual. no need for hill climbing!
			return false;
		}
		//TODO: for soft

		int maxdiff = 0, maxid = -1;
		for (int selcolor = 0; selcolor < NCOL; selcolor++) {
			//mutate the child's gene in the longest_slot list for every color except for its own
			if (selcolor == ritHard->targetSlot) {
				continue;
			}
			//update the gene to selcolor, if we have any "worst" color.
			subject.chromosome->update(ritHard->targetSect, selcolor);
			//get the new fitness for comparison
			if (level == hc_hard) {
				subject.calc_hardfit(target_newHFit, 0);
			} else if (level == hc_soft) {
				calc_softfit(target_newSFit, 0);
			} else if (level == hc_both) {
				subject.calc_hardfit(target_newHFit, 0);
				calc_softfit(target_newSFit, 0);
			}
/*
			dummy = 0;
			for (list<int>::const_iterator it = subject.chromosome->get_section_list(selcolor)->begin();
					it != subject.chromosome->get_section_list(selcolor)->end(); ++it) {
				//TODO: maybe multi objective?
				for (int i = 0; i < HARD_FIT_N; ++i) {
					dummy += old_hfit.fitnessBySect[*it][i];
				}
			}
*/
			if ((old_hfit.total_fit - target_newHFit.total_fit) > maxdiff) {
				maxdiff = old_hfit.total_fit - target_newHFit.total_fit;
				maxid = selcolor;
			}
			/*
			if ((oldSlotHFit[selcolor] - target_newHFit.total_fit) + (oldSlotSFit[selcolor] - target_newSFit.total_fit)
					> maxdiff) {
				maxdiff = (old_hfit.total_fit - target_newHFit.total_fit)
						+ (old_sfit.total_fit - target_newSFit.total_fit);
				maxid = selcolor;
			}*/
			//un-stage changes
			subject.chromosome->update(ritHard->targetSect, ritHard->targetSlot);
		}

		//if difference is larger than 0, then we found a better slot for that section.
		if (maxdiff > 0 && maxid != -1) {
			chromosome->update(ritHard->targetSect, maxid);
			buildtimetable();
			updatefitness(0);
			finalchange = true;
		}
		//todo: updatefitness'ý deðiþtir. modüler fitness tut, herþeyi baþtan hesaplama.
	}
	return finalchange;
}

Chromosome* Individual::getChromosome() {
	return chromosome;
}

} /* namespace std */
