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
//#define SORT_MULTIOBJ

int HC_whichFit;

namespace std {

bool compare(const FitnessRow& a, const FitnessRow& b) {
#ifdef SORT_MULTIOBJ
		int tot_p1 = 0, tot_p2 = 0;
		bool result = true;
		//iterate all groups that are defined by user.
		for (; group != ite; ++group) {
			tot_p1 = tot_p2 = 0;
			//at each group, iterate fitness values and add them
			for (vector<int>::const_iterator el = group->begin(); el != group->end(); el++) {
				//target - source, higher values in result indicates more dominance
				tot_p1 += a[*el];
				tot_p2 += b[*el];
			}
			result &= tot_p1 > tot_p2;
		}

	return result;
#else
	switch (HC_whichFit) {
		case hc_soft:
			return a[fit_mSoft] > b[fit_mSoft];
		case hc_hard:
			return a[fit_mHard] > b[fit_mHard];
		default:
			return a[fit_mGeneTotal] > b[fit_mGeneTotal];
	}
#endif
}

Individual::Individual(Common *conf) {
	int i;
	this->conf = conf;
	chrom_length = conf->ChromSize;
	no_periods = 4;
	chromosome = new Chromosome(chrom_length, NCOL);

	for (i = 0; i < chrom_length; i++) {
		if (conf->courmat[i].has_constraint == 1) {
			chromosome->add(i, 4 * conf->courmat[i].c2day + conf->courmat[i].c2slot);
		} else {
			chromosome->add(i, RND(NCOL));
		}
	}
	chromosome->updatefitness(0);
	mutator = new Mutation(chromosome);
}

Individual::Individual() {
	conf = Common::getConf();

	chrom_length = conf->ChromSize;
	no_periods = 4;
	chromosome = new Chromosome(chrom_length, NCOL);
	mutator = new Mutation(chromosome);
}

Individual::~Individual() {
	delete (chromosome);
	delete mutator;
}

Individual::Individual(const Individual& source) {
	this->chromosome = new Chromosome(source.chromosome);
	this->mutator = new Mutation(this->chromosome);
	this->chrom_length = source.chrom_length;
	this->conf = source.conf;
	this->no_periods = source.no_periods;
}

Individual &Individual::operator=(const Individual &source) {
	if (this->chromosome)
		delete (this->chromosome);
	this->chromosome = new Chromosome(source.chromosome);
	this->mutator = new Mutation(this->chromosome);
	this->chrom_length = source.chrom_length;
	this->conf = source.conf;
	this->no_periods = source.no_periods;
	return *this;
}

void Individual::cross(const Individual& p1, const Individual &p2) {
	vector <bool> checked(chrom_length, false);
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
	for (int i = 0; i < chrom_length; ++i) {
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
 * dominates:
 * 			Function also returns list of fitnesses dominated by calling function.
 * 			fitness values in result array, higher values indicate calling Individual dominates more
 * 			return D_TRUE if "this" Individual dominates (has superior fitness) target.
 * 			return D_FALSE if target dominates "this"
 * 			return D_ONLYHARD_DOMINATION if this dominates only in hard fitness
 * 			return D_NO_DOMINATION if there is no domination.
 */
domination_t Individual::dominates(const Individual *target) {
	domination_t result;
	bool target_dominates = true, this_dominates = true;
	int tot_this, tot_targ;
	uint8_t pos = 0, fitsource, fittarget;
	//first hard groups

	//iterate all groups that are defined by user.
	for (auto group = conf->groups.begin(); group != conf->groups.end(); ++group, ++pos) {
		tot_this = tot_targ = 0;
		//at each group, iterate fitness values and add them
		for (vector<int>::const_iterator el = group->begin(); el != group->end(); el++) {
			fitsource = this->getFit().fitness[*el];
			fittarget = target->getFit().fitness[*el];
			//target - source, higher values in result indicates more dominance
			result.fit[pos] = fittarget - fitsource;
			tot_this += fitsource;
			tot_targ += fittarget;
		}
		this_dominates &= (tot_this < tot_targ);
		target_dominates &= (tot_targ <= tot_this);
	}
	//target is dominated by this, in soft and hard constraints
	if (this_dominates) {
		result.type = D_TRUE;
	} else if (target_dominates) {
		result.type = D_FALSE;
	} else {
		result.type = D_NO_DOMINATION;
	}
	return result;
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
		for (m = 0; m < chrom_length; m++) {
			if (conf->lecturers[h].name == conf->courmat[m].lname) {
				if (conf->courmat[m].hours == 1) {
					switch (chromosome->get_period(m)) {
					case 0:
						lectmatrix[chromosome->get_day(m)][2] = 1;
						break;
					case 1:
						lectmatrix[chromosome->get_day(m)][3] = 1;
						break;
					case 2:
						lectmatrix[chromosome->get_day(m)][4] = 1;
						break;
					case 3:
						lectmatrix[chromosome->get_day(m)][9] = 1;
						break;
					}
				}
				if (conf->courmat[m].hours == 2) {
					switch (chromosome->get_period(m)) {
					case 0:
						lectmatrix[chromosome->get_day(m)][0] = 1;
						lectmatrix[chromosome->get_day(m)][1] = 1;
						break;
					case 1:
						lectmatrix[chromosome->get_day(m)][2] = 1;
						lectmatrix[chromosome->get_day(m)][3] = 1;
						break;
					case 2:
						lectmatrix[chromosome->get_day(m)][5] = 1;
						lectmatrix[chromosome->get_day(m)][6] = 1;
						break;
					case 3:
						lectmatrix[chromosome->get_day(m)][7] = 1;
						lectmatrix[chromosome->get_day(m)][8] = 1;
						break;
					}
				}
			}
		}
		for (m = 0; m < 5; m++) {
			for (n = 0; n < 10; n++) {
				if (lectmatrix[m][n] == 1)
					printf(" %-9s ", conf->lecturers[h].name.c_str());
				else
					printf(" OOOOO0000 ");
			}
			printf("\n");
		}
		printf("\n");
	}
}

bool Individual::mutate_all() {
	return mutator->mutate_all();
}

/*
 void Individual::printdekanlik() {
 size_t i;
 string sched2, sched1;
 int lidx;
 for (i = 0; i < chrom_length; i++) {
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
 */

void inline Individual::calcFit(fitness_t & fit, int print, int type) {
	chromosome->fit->calcFit(print, fit, type);
}

int Individual::findcourse(int sem, int dy, int slt) {
	int h;
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
	for (h = 0; h < chrom_length; h++) {
		if (conf->courmat[h].semid == sem && chromosome->get_day(h) == dy && conf->courmat[h].hours == 2
				&& chromosome->get_slot(h) == rslot2) {
			found = 1;
			break;
		} else if (conf->courmat[h].semid == sem && chromosome->get_day(h) == dy && conf->courmat[h].hours == 1
				&& chromosome->get_slot(h) == rslot1) {
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

void Individual::updatefitness(int val) {
	chromosome->fit->updateFitness(val);
}

bool Individual::hc_worstsection() {
	fitness_t oldFit(chrom_length), newFit(chrom_length);
	bool finalchange = false;
	int oldSlot, oldFitSpec, newFitSpec, oldHardFit;

	//get fitnesses first
	oldFit = getFit();
	oldHardFit = getFit().hard_fit;
	switch (HC_whichFit) {
	case hc_hard:
		oldFitSpec = oldFit.hard_fit;
		break;
	case hc_soft:
		oldFitSpec = oldFit.soft_fit;
		break;
	default:
		oldFitSpec = oldFit.total_fit;
		break;
	}

	//sort the list to find worst gene in it.
	sort(oldFit.fitnessBySect.begin(), oldFit.fitnessBySect.end(), compare);
	auto it = oldFit.fitnessBySect.begin(), ite = oldFit.fitnessBySect.end();
	for (int i = 0; i < conf->hc_gene_count && it != ite; ++i, ++it) {
		if (it->at(fit_mGeneTotal) == 0) {
			//yay! perfect individual. no need for hill climbing!
			return false;
		}
		//no HC for courses that have constraints. they have fixed hours.
		if (conf->courmat[it->at(fit_mGeneID)].has_constraint == 1) {
			continue;
		}
		//restore old slot of gene, to reduce function calls.
		oldSlot = chromosome->get_slot(it->at(fit_mGeneID));
		int maxdiff = 0, bestSectionID = -1;
		for (int selcolor = 0; selcolor < NCOL; selcolor++) {
			//mutate the child's gene in the longest_slot list for every color except for its own
			if (selcolor == oldSlot) {
				continue;
			}
			//update the gene to selcolor
			chromosome->update(it->at(fit_mGeneID), selcolor);
			//find out new fitness value for that
			calcFit(newFit, 0, HC_whichFit);
			switch (HC_whichFit) {
			case hc_hard:
				newFitSpec = newFit.hard_fit;
				break;
			case hc_soft:
				newFitSpec = newFit.soft_fit;
				break;
			default:
				newFitSpec = newFit.total_fit;
				break;
			}
			//compare the enhancements, if there any. we need to check total_fit for that since
			//reducing particular gene's or groups fitness might increase the overall fitness.
			if ((oldHardFit >= newFit.hard_fit) && (oldFitSpec - newFitSpec) > maxdiff) {
				maxdiff = oldFitSpec - newFitSpec;
				bestSectionID = selcolor;
			}
		}
		//un-stage changes
		chromosome->update(it->at(fit_mGeneID), oldSlot);

		//if difference is larger than 0, then we found a better slot for that section.
		if (maxdiff > 0 && bestSectionID != -1) {
			int old = getFit().hard_fit;
			chromosome->update(it->at(fit_mGeneID), bestSectionID);
			updatefitness(0);
			//todo: debugý sil
			if (old < getFit().hard_fit) {
				cerr << "HC fails. old:" << old << "pre-calc old:" << oldHardFit << "new:" << getFit().hard_fit << endl;
			}
			//update new fitness for internal use
			oldHardFit = getFit().hard_fit;
			switch (HC_whichFit) {
			case hc_hard:
				oldFitSpec = getFit().hard_fit;
				break;
			case hc_soft:
				oldFitSpec = getFit().soft_fit;
				break;
			default:
				oldFitSpec = getFit().total_fit;
				break;
			}
			finalchange = true;
		} else {
			//because we calculated fitness with changed slots. let's restore old fitness back.
			updatefitness(0);
		}
		//todo: updatefitness'ý deðiþtir. modüler fitness tut, herþeyi baþtan hesaplama.
	}
	return finalchange;
}

Chromosome* Individual::getChromosome() {
	return chromosome;
}

} /* namespace std */
