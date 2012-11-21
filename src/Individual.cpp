/*
 * Individual.cpp
 *
 *  Created on: 21 Eki 2012
 *      Author: HalilCan
 */

#include "Individual.h"
#include "Common.h"
#include "Errnode.h"
#include <stdlib.h>
#include <stdio.h>
#include "FileReader.h"
#include <set>

namespace std {

Individual::Individual(Common *conf) {
	int i, j, cnt = 0;

	this->conf = conf;
	chrom_length = CHROML;
	no_colors = NCOL;
	no_periods = 4;
	chromosome = new Chromosome(chrom_length, no_colors);

	fitnessh = 0;
	fitnessh1 = 0;
	fitnessf = 0;
	fitnessf2 = 0;
	errf = fopen("errors.txt", "a");

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
			if ((conf->courmat[i].has_constraint == 1 && conf->courmat[i].hours == conf->courmat[j].hours)
					&& (conf->courmat[j].has_constraint == 1)
					&& (conf->courmat[i].c2day == conf->courmat[j].c2day
							&& conf->courmat[i].c2slot == conf->courmat[j].c2slot)) {
				//assign i's slot to j's.
				chromosome->update(j, chromosome->get_slot(i));
			}
		}
	}
	fitnessHCAL(0);
	fitnessFCAL(0);
	fitnessF1CAL(0);
	fitnessF2CAL(0);
	fitnessF3CAL(0);
}

Individual::~Individual() {
	fclose(errf);
}

Individual::Individual(const Individual& source) {
	int i;
	for (i = 0; i < 5; i++) {
		for (int k = 0; k < 4; k++) {
			this->timetable1[i][k] = source.timetable1[i][k];
			this->timetable2[i][k] = source.timetable2[i][k];
		}
	}
	this->chromosome = source.chromosome;
	this->errf = source.errf;
	this->chrom_length = source.chrom_length;
	this->conf = source.conf;
	this->no_periods = source.no_periods;
	this->no_colors = source.no_colors;
	this->fitnessh = source.fitnessh;
	this->fitnessh1 = source.fitnessh1;
	this->fitnessf = source.fitnessf;
	this->fitnessf2 = source.fitnessf2;
}

Individual &Individual::operator=(const Individual &source) {
	int j, k;
	for (j = 0; j < 5; j++) {
		for (k = 0; k < 4; k++) {
			this->timetable1[j][k] = source.timetable1[j][k];
			this->timetable2[j][k] = source.timetable2[j][k];
		}
	}
	this->chromosome = source.chromosome;
	this->fitnessh = source.fitnessh;
	this->fitnessh1 = source.fitnessh1;
	this->fitnessf = source.fitnessf;
	this->fitnessf2 = source.fitnessf2;
	return *this;
}

bool Individual::equalsh(Individual&in1, Individual&in2) {
	if (in1.fitnessh == in2.fitnessh && in1.fitnessh1 == in2.fitnessh1)
		return true;
	return false;
}

//do hill climb operation for the longest slot of gene. Longest slot means the slot with most lectures.
void Individual::hc_longestslot() {
	Individual subject(*this);
	list<int> longest_slot(*subject.chromosome->get_longest_slot());
	list<int>::iterator it;
	for (it = longest_slot.begin(); it != longest_slot.end(); ++it) {
		if (conf->courmat[*it].has_constraint == 1 || RND(1000) > 1000 * conf->hillrnd) {
			//no hill climbing for courses which has day constraints.
			continue;
		}
		for (int selcolor = 0; selcolor < no_colors; selcolor++) {
			if (subject.chromosome->get_slot(*it) == selcolor) {
				//mutate the child's gene in the longest_slot list for every color except for its own
				continue;
			}
			subject.chromosome->update(*it, selcolor);
			subject.buildtimetable();
			subject.updatefitness();

			//if there is an improvement on subject's gene (both hard and soft), then update that value for
			//real gene of the individual.
			if ((subject.fitnessh + subject.fitnessh1 < this->fitnessh + this->fitnessh1)
					&& (subject.fitnessf + subject.fitnessf2 < this->fitnessf + this->fitnessf2)) {
				chromosome->update(*it, selcolor);
				updatefitness();
			}
		}
	}
}

void Individual::hc1() {
	int rndidx;
	int selcolor;
	Individual hcchild(*this);

	if (RND(1000) < 1000 * conf->hillrnd || conflv.size() == 0) {
		do {
			rndidx = RND(chrom_length);
		} while (conf->courmat[rndidx].has_constraint == 1);
	} else {
		rndidx = conflv[RND(conflv.size())];
	}

	for (selcolor = 0; selcolor < no_colors; selcolor++) {
		//mutate the child's gene rndidx for every color except for its own
		//todo: error prone
		if (hcchild.chromosome->get_slot(rndidx) == selcolor)
			continue;
		hcchild.chromosome->update(rndidx, selcolor);
		hcchild.buildtimetable();
		hcchild.updatefitness();

		if ((hcchild.fitnessh + hcchild.fitnessh1 < this->fitnessh + this->fitnessh1)
				&& (hcchild.fitnessf + hcchild.fitnessf2 < this->fitnessf + this->fitnessf2)) {
			*this = hcchild;
		}
	}
	conflv.clear();
}

bool Individual::equalss(Individual&in1, Individual&in2) {
	if (in1.fitnessf == in2.fitnessf && in1.fitnessf2 == in2.fitnessf2)
		return true;
	return false;
}

void Individual::cross(Individual&p1, Individual&p2) {
	bool checked[CHROML] = { };
	Chromosome *chrom;
	for (int i = 0; i < NCOL; ++i) {
		if (p1.chromosome->slot_map[i].size() > p2.chromosome->slot_map[i].size()) {
			chrom = p1.chromosome;
		} else {
			chrom = p2.chromosome;
		}
		list<int>::iterator ite = chrom->slot_map[i].end();
		for (list<int>::iterator it = chrom->slot_map[i].begin(); it != ite; it++) {
			if (checked[*it] == false) {
				this->chromosome->add(*it, i);
				checked[*it] = true;
			}
		}
	}
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
			}
			this->chromosome->add(i, chrom->get_slot(i));
		}
	}
}

/*
 * dominates: return D_TRUE if "this" Individual dominates (has superior fitness) target.
 * 			return D_FALSE if target dominates "this"
 * 			return D_IN_RANGE if there is no domination and 2 Individual are in their crowding distance
 * 			return D_OUT_RANGE if there is no domination and 2 Individual are NOT in their crowding distance
 */
int Individual::dominates(Individual *target) {
	if (this->fitnessh <= target->fitnessh && this->fitnessh1 <= target->fitnessh1
			&& this->fitnessf <= target->fitnessf && this->fitnessf2 <= target->fitnessf2)
		return D_TRUE;
	if (target->fitnessh <= this->fitnessh && target->fitnessh1 <= this->fitnessh1
			&& target->fitnessf <= this->fitnessf && target->fitnessf2 <= this->fitnessf2)
		return D_FALSE;
	//todo: daha düzenli yaz.
	//represent fitness values as multi-dimensional rectangle.
	int my_x1 = this->fitnessh - conf->crowding_dist, my_x2 = this->fitnessh + conf->crowding_dist;
	int my_y1 = this->fitnessh1 - conf->crowding_dist, my_y2 = this->fitnessh1 + conf->crowding_dist;
	int my_z1 = this->fitnessf - conf->crowding_dist, my_z2 = this->fitnessf + conf->crowding_dist;
	int my_w1 = this->fitnessf2 - conf->crowding_dist, my_w2 = this->fitnessf2 + conf->crowding_dist;

	int tr_x1 = target->fitnessh - conf->crowding_dist, tr_x2 = target->fitnessh + conf->crowding_dist;
	int tr_y1 = target->fitnessh1 - conf->crowding_dist, tr_y2 = target->fitnessh1 + conf->crowding_dist;
	int tr_z1 = target->fitnessf - conf->crowding_dist, tr_z2 = target->fitnessf + conf->crowding_dist;
	int tr_w1 = target->fitnessf2 - conf->crowding_dist, tr_w2 = target->fitnessf2 + conf->crowding_dist;
	//overlap condition
	if (my_x1 < tr_x2 && my_x2 > tr_x1 && my_y1 < tr_y2 && my_y2 > tr_y1 && my_z1 < tr_z1 && my_z2 > tr_z1
			&& my_w1 < tr_w1 && my_w2 > tr_w1) {
		return D_IN_RANGE;
	} else
		return D_OUT_RANGE;
}

Chromosome* Individual::getChromosome() {
	return chromosome;
}

int Individual::decode(int cidx) {
	int rslot = -1, rval = -1;
	if (conf->courmat[cidx].hours == 1) {
		switch (chromosome->slot[cidx]) {
		case 0:
			rslot = 2;
			break;
		case 1:
			rslot = 3;
			break;
		case 2:
			rslot = 4;
			break;
		case 3:
			rslot = 9;
			break;
		}
	} else {
		switch (chromosome->slot[cidx]) {
		case 0:
			rslot = 0;
			break;
		case 1:
			rslot = 2;
			break;
		case 2:
			rslot = 5;
			break;
		case 3:
			rslot = 7;
			break;
		}
	}
	rval = 10 * chromosome->day[cidx] + rslot;
	return rval;
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

int Individual::fitnessFCAL(int prnt) {
	int j, cnt;
	cnt = 0;
	vector<Errnode> errv;
	Errnode tmperr;
	size_t i, h;
	for (i = 0; i < CHROML; i++) {
		//Department meeting conflict
		if (fit_sdepmeet(i)) {
			cnt = cnt + 1;
			if (prnt == 1) {
				tmperr.desc = " 15 Departmental Meeting     ";
				tmperr.s1 = conf->courmat[i].cname;
				tmperr.s2 = conf->courmat[i].cname;
				errv.push_back(tmperr);
			}
			if (prnt == 2) {
				conflv.push_back(i);
				conflv.push_back(j);
			}
		}
		//Hardware labs conflict
		for (j = i + 1; j < CHROML; j++) {
			if (fit_slabconf(i, j)) {
				cnt = cnt + 1;
				if (prnt == 1) {
					tmperr.desc = " 16 Hardware Labs      ";
					tmperr.s1 = conf->courmat[i].cname;
					tmperr.s2 = conf->courmat[j].cname;
					errv.push_back(tmperr);
				}
				if (prnt == 2) {
					conflv.push_back(i);
					conflv.push_back(j);
				}
			}
		}
	}
	for (h = 0; h < conf->lecturers.size(); h++) {
		int m, n;
		int lectmatrix[5][10];
		int morlect[5];
		for (m = 0; m < 5; m++) {
			morlect[m] = 0;
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
		int fday, fday2 = 0;
		int strcnt = 0, stpcnt = 0, foundr = 0;
		int dayov;
		for (m = 0; m < 5; m++) {
			fday = 0;
			strcnt = 0;
			stpcnt = 0;
			foundr = 0;
			dayov = 0;
			if (lectmatrix[m][0] == 1)
				morlect[m] = 1;
			for (n = 0; n < 10; n++) {
				if (lectmatrix[m][n] == 1) {
					fday = 1;
					dayov++;
				}
				if (n + 3 < 10 && lectmatrix[m][n] == 1 && lectmatrix[m][n + 1] == 1
						&& lectmatrix[m][n + 2] == 1 && lectmatrix[m][n + 3] == 1) {
					cnt = cnt + 1;
					if (prnt == 1) {
						tmperr.desc = " 7 Lecturer 4 cons Work Hour     ";
						tmperr.s1 = conf->lecturers[h];
						tmperr.s2 = " ";
						errv.push_back(tmperr);
					}
				}
				if (n + 1 < 10 && lectmatrix[m][n] == 1 && lectmatrix[m][n + 1] == 0 && foundr == 0) {
					strcnt = n + 1;
					foundr = 1;
				}
				if (n + 1 < 10 && lectmatrix[m][n] == 0 && lectmatrix[m][n + 1] == 1) {
					stpcnt = n + 1;
				}
			}
			if (stpcnt - strcnt > 3 && foundr != 0) {
				cnt = cnt + 1;
				if (prnt == 1) {
					tmperr.desc = " 8 Lecturer 4 cons Empty Hour     ";
					tmperr.s1 = conf->lecturers[h];
					tmperr.s2 = " ";
					errv.push_back(tmperr);
				}
			}
			if (dayov > 4) {
				cnt = cnt + 1;
				if (prnt == 1) {
					tmperr.desc = " 9 Lecturer More than 4 Hours per day       ";
					tmperr.s1 = conf->lecturers[h];
					tmperr.s2 = " ";
					errv.push_back(tmperr);
				}
			}
			if (fday == 0)
				fday2 = 1;
		}
		if (fday2 == 0) {
			cnt = cnt + 1;
			if (prnt == 1) {
				tmperr.desc = " 10 Lecturer No free Day      ";
				tmperr.s1 = conf->lecturers[h];
				tmperr.s2 = " ";
				errv.push_back(tmperr);
			}
		}
		int p, nummor = 0;
		for (p = 0; p < 5; p++) {
			nummor = nummor + morlect[p];
		}
		if (nummor > 1) {
			cnt = cnt + 1;
			if (prnt == 1) {
				tmperr.desc = " 12 Lecturer More than 2 morning courses      ";
				tmperr.s1 = conf->lecturers[h];
				tmperr.s2 = " ";
				errv.push_back(tmperr);
			}
		}
	}
	if (prnt == 1)
		for (i = 0; i < errv.size(); i++) {
			printf("Soft %s ", errv[i].desc.data());
			printf("%s ", errv[i].s1.data());
			printf("%s ", errv[i].s2.data());
			printf("%d \n", errv[i].n1);
			fprintf(errf, "Soft %s ", errv[i].desc.data());
			fprintf(errf, "%s ", errv[i].s1.data());
			fprintf(errf, "%s ", errv[i].s2.data());
			fprintf(errf, "%d \n", errv[i].n1);
		}
	fitnessf = cnt;
	return 0;
}
int Individual::fitnessF3CAL(int prnt) {
	int cnt = 0;
	size_t i, j;
	vector<Errnode> errv;
	Errnode tmperr;
	int lunch[8][5][3];
	int k, m;
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 5; j++) {
			for (k = 0; k < 3; k++) {
				lunch[i][j][k] = 0;
			}
		}
	}
	for (m = 0; m < 8; m++) {
		for (i = 0; i < CHROML; i++) {
			if (conf->courmat[i].semid == m + 1 && conf->courmat[i].hours == 1 && chromosome->slot[i] == 0)
				lunch[m][chromosome->day[i]][0] = 1;
			if (conf->courmat[i].semid == m + 1 && conf->courmat[i].hours == 1 && chromosome->slot[i] == 1)
				lunch[m][chromosome->day[i]][1] = 1;
			if (conf->courmat[i].semid == m + 1 && conf->courmat[i].hours == 1 && chromosome->slot[i] == 2)
				lunch[m][chromosome->day[i]][2] = 1;
			if (conf->courmat[i].semid == m + 1 && conf->courmat[i].hours == 2 && chromosome->slot[i] == 1) {
				lunch[m][chromosome->day[i]][0] = 1;
				lunch[m][chromosome->day[i]][1] = 1;
			}
		}
	}
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 5; j++) {
			if (lunch[i][j][0] == 1 && lunch[i][j][1] == 1 && lunch[i][j][2] == 1 && !(i == 0 && j == 3)
					&& !(i == 1 && j == 2) && !(i == 1 && j == 3) && !(i == 1 && j == 4)) {
				cnt = cnt + 1;
				if (prnt == 1) {
					tmperr.desc = " 14 No Lunch Hour  conflict       ";
					tmperr.s1 = " ";
					tmperr.s2 = " ";
					tmperr.n1 = i * 8 + j;
					errv.push_back(tmperr);
				}
			}
		}
	}
	for (i = 0; i < CHROML; i++) {
		if (conf->courmat[i].hours == 1 && chromosome->slot[i] == 3) {
			cnt = cnt + 1;
			if (prnt == 1) {
				tmperr.desc = " 5 Evening Lecture    ";
				tmperr.s1 = conf->courmat[i].cname;
				tmperr.s2 = " ";
				errv.push_back(tmperr);
			}
			if (prnt == 2) {
				conflv.push_back(i);
			}
		}
		if (conf->courmat[i].cname.size() > 8 && conf->lab[i] == 1 && chromosome->slot[i] == 0
				&& conf->courmat[i].hours == 2) {
			cnt = cnt + 1;
			if (prnt == 1) {
				tmperr.desc = " 6 Mourning Lab     ";
				tmperr.s1 = conf->courmat[i].cname;
				tmperr.s2 = " ";
				errv.push_back(tmperr);
			}
			if (prnt == 2) {
				conflv.push_back(i);
			}
		}
	}
	if (prnt == 1)
		for (i = 0; i < errv.size(); i++) {
			printf("Soft3 %s ", errv[i].desc.data());
			printf("%s ", errv[i].s1.data());
			printf("%s ", errv[i].s2.data());
			printf("%d \n", errv[i].n1);
			fprintf(errf, "Soft3 %s ", errv[i].desc.data());
			fprintf(errf, "%s ", errv[i].s1.data());
			fprintf(errf, "%s ", errv[i].s2.data());
			fprintf(errf, "%d \n", errv[i].n1);
		}
	fclose(errf);
	fitnessf2 += cnt;
	return fitnessf2;
}
int Individual::fitnessF1CAL(int prnt) {
	int cnt = 0;
	size_t i;
	vector<Errnode> errv;
	Errnode tmperr;
	int cmax[conf->labcourses.size()];
	int cmin[conf->labcourses.size()];
	int lab1[conf->labcourses.size()];
	int lab2[conf->labcourses.size()];
	int decv = -1;
	for (i = 0; i < conf->labcourses.size(); i++) {
		cmax[i] = -1;
		cmin[i] = 100;
		lab1[i] = -1;
		lab2[i] = -1;
	}
	for (i = 0; i < CHROML; i++) {
		if (conf->labid[i] != -1) {
			decv = decode(i);
			if (decv > cmax[conf->labid[i]] && conf->courmat[i].cname.size() > 8 && conf->lab[i] == 0)
				cmax[conf->labid[i]] = decv;
			if (decv < cmin[conf->labid[i]] && conf->courmat[i].cname.size() > 8 && conf->lab[i] == 0)
				cmin[conf->labid[i]] = decv;
			if (lab1[conf->labid[i]] == -1 && conf->courmat[i].cname.size() > 8 && conf->lab[i] == 1)
				lab1[conf->labid[i]] = decv;
			if (lab1[conf->labid[i]] != -1 && conf->courmat[i].cname.size() > 8 && conf->lab[i] == 1)
				lab2[conf->labid[i]] = decv;
		}
	}
	for (i = 0; i < conf->labcourses.size(); i++) {
		if ((lab1[i] < cmax[i] && lab1[i] > cmin[i]) || (lab2[i] < cmax[i] && lab2[i] > cmin[i])) {
			cnt = cnt + 1;
			if (prnt == 1) {
				tmperr.desc = " 11 L T L conflict       ";
				tmperr.s1 = conf->labcourses[i];
				tmperr.s2 = " ";
				tmperr.n1 = i;
				errv.push_back(tmperr);
			}
		}
		if ((lab1[i] < cmin[i]) && (lab2[i] > cmin[i])) {
			cnt = cnt + 1;
			if (prnt == 1) {
				tmperr.desc = " 11 L T L conflict       ";
				tmperr.s1 = conf->labcourses[i];
				tmperr.s2 = " ";
				tmperr.n1 = i;
				errv.push_back(tmperr);
			}
		}
		if ((lab1[i] > cmax[i]) && (lab2[i] < cmax[i])) {
			cnt = cnt + 1;
			if (prnt == 1) {
				tmperr.desc = " 11 L T L conflict       ";
				tmperr.s1 = conf->labcourses[i];
				tmperr.s2 = " ";
				tmperr.n1 = i;
				errv.push_back(tmperr);
			}
		}
	}
	if (prnt == 1)
		for (i = 0; i < errv.size(); i++) {
			printf("Soft1 %s ", errv[i].desc.data());
			printf(" %s ", errv[i].s1.data());
			printf(" %s ", errv[i].s2.data());
			printf(" %d \n", errv[i].n1);
			fprintf(errf, "Soft1 %s ", errv[i].desc.data());
			fprintf(errf, " %s ", errv[i].s1.data());
			fprintf(errf, " %s ", errv[i].s2.data());
			fprintf(errf, " %d \n", errv[i].n1);
		}
	fitnessf += cnt;
	return fitnessf;
}
int Individual::fitnessF2CAL(int prnt) {
	int cnt = 0;
	size_t i, j;
	vector<Errnode> errv;
	Errnode tmperr;
	for (i = 0; i < CHROML; i++) {
		for (j = 0; j < CHROML; j++) {
			if (conf->prereq[i][j] == 0 && i != j && conf->courmat[i].semid == conf->courmat[j].semid + 1
					&& conf->courmat[i].hours == conf->courmat[j].hours
					&& chromosome->day[i] == chromosome->day[j] && chromosome->slot[i] == chromosome->slot[j]
					&& conf->cse[i] == 1 && conf->cse[j] == 1 && conf->lab[i] != 1 && conf->lab[j] != 1) {
				cnt = cnt + 1;
				if (prnt == 1) {
					tmperr.desc = " 4 Prev Sem Next Sem    ";
					tmperr.s1 = conf->courmat[i].cname;
					tmperr.s2 = conf->courmat[j].cname;
					errv.push_back(tmperr);
				}
				if (prnt == 2) {
					conflv.push_back(i);
					conflv.push_back(j);
				}
			}
			if (conf->prereq[i][j] == 0 && i != j && conf->courmat[i].semid == conf->courmat[j].semid + 1
					&& conf->courmat[i].hours == 1 && conf->courmat[j].hours == 2
					&& chromosome->day[i] == chromosome->day[j]
					&& (chromosome->slot[i] == 0 || chromosome->slot[i] == 1) && chromosome->slot[j] == 1
					&& conf->cse[i] == 1 && conf->cse[j] == 1) {
				cnt = cnt + 1;
				if (prnt == 1) {
					tmperr.desc = " 4 Prev Sem Next Sem    ";
					tmperr.s1 = conf->courmat[i].cname;
					tmperr.s2 = conf->courmat[j].cname;
					errv.push_back(tmperr);
				}
				if (prnt == 2) {
					conflv.push_back(i);
					conflv.push_back(j);
				}
			}
			if (conf->prereq[i][j] == 0 && i != j && conf->courmat[i].semid == conf->courmat[j].semid + 1
					&& conf->courmat[i].hours == 2 && conf->courmat[j].hours == 1
					&& chromosome->day[i] == chromosome->day[j]
					&& (chromosome->slot[j] == 0 || chromosome->slot[j] == 1) && chromosome->slot[i] == 1
					&& conf->cse[i] == 1 && conf->cse[j] == 1) {
				cnt = cnt + 1;
				if (prnt == 1) {
					tmperr.desc = " 4 Prev Sem Next Sem    ";
					tmperr.s1 = conf->courmat[i].cname;
					tmperr.s2 = conf->courmat[j].cname;
					errv.push_back(tmperr);
				}
				if (prnt == 2) {
					conflv.push_back(i);
					conflv.push_back(j);
				}
			}
		}
	}
	int lidx;
	for (i = 0; i < CHROML; i++) {
		lidx = conf->findlecture(i);
		if (lidx != -1) {
			if (conf->courmat[i].cname.at(7) == '0' && conf->courmat[i].hours == 1) {
				conf->lectures[lidx].cid1day = chromosome->day[i];
				conf->lectures[lidx].cid1slot = chromosome->slot[i];
			}
			if (conf->courmat[i].cname.at(7) == '0' && conf->courmat[i].hours == 2) {
				conf->lectures[lidx].cid2day = chromosome->day[i];
				conf->lectures[lidx].cid2slot = chromosome->slot[i];
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
	for (i = 0; i < conf->lectures.size(); i++) {
		for (j = 0; j < conf->lectures.size(); j++) {
			conf->lectures[i].labs[0] = -1;
			conf->lectures[i].labs[1] = -1;
			conf->lectures[i].labs[2] = -1;
			if (conf->lectures[i].semid == conf->lectures[j].semid + 1
					|| conf->lectures[j].semid == conf->lectures[i].semid + 1) {
				if (conf->lectures[i].lab1day == conf->lectures[j].cid1day
						&& (conf->lectures[i].lab1slot == 1
								&& (conf->lectures[j].cid1slot == 0 || conf->lectures[j].cid1slot == 1)))
					conf->lectures[i].labs[0] = 1;
				if (conf->lectures[i].lab1day == conf->lectures[j].lab3day
						&& (conf->lectures[i].lab1slot == 1
								&& (conf->lectures[j].lab3slot == 0 || conf->lectures[j].lab3slot == 1)))
					conf->lectures[i].labs[0] = 1;
				if (conf->lectures[i].lab1day == conf->lectures[j].cid2day
						&& conf->lectures[i].lab1slot == conf->lectures[j].cid2slot)
					conf->lectures[i].labs[0] = 1;
				if (conf->lectures[i].lab1day == conf->lectures[j].lab1day
						&& conf->lectures[i].lab1slot == conf->lectures[j].lab1slot)
					conf->lectures[i].labs[0] = 1;
				if (conf->lectures[i].lab1day == conf->lectures[j].lab2day
						&& conf->lectures[i].lab1slot == conf->lectures[j].lab2slot)
					conf->lectures[i].labs[0] = 1;
				if (conf->lectures[i].lab2day == conf->lectures[j].cid1day
						&& (conf->lectures[i].lab2slot == 1
								&& (conf->lectures[j].cid1slot == 0 || conf->lectures[j].cid1slot == 1)))
					conf->lectures[i].labs[1] = 1;
				if (conf->lectures[i].lab2day == conf->lectures[j].lab3day
						&& (conf->lectures[i].lab2slot == 1
								&& (conf->lectures[j].lab3slot == 0 || conf->lectures[j].lab3slot == 1)))
					conf->lectures[i].labs[1] = 1;
				if (conf->lectures[i].lab2day == conf->lectures[j].cid2day
						&& conf->lectures[i].lab2slot == conf->lectures[j].cid2slot)
					conf->lectures[i].labs[1] = 1;
				if (conf->lectures[i].lab2day == conf->lectures[j].lab1day
						&& conf->lectures[i].lab2slot == conf->lectures[j].lab1slot)
					conf->lectures[i].labs[1] = 1;
				if (conf->lectures[i].lab2day == conf->lectures[j].lab2day
						&& conf->lectures[i].lab2slot == conf->lectures[j].lab2slot)
					conf->lectures[i].labs[1] = 1;
				if (conf->lectures[i].lab3day == conf->lectures[j].lab1day
						&& (conf->lectures[j].lab1slot == 1
								&& (conf->lectures[i].lab3slot == 0 || conf->lectures[i].lab3slot == 1)))
					conf->lectures[i].labs[2] = 1;
				if (conf->lectures[i].lab3day == conf->lectures[j].lab2day
						&& (conf->lectures[j].lab2slot == 1\

								&& (conf->lectures[i].lab3slot == 0 || conf->lectures[i].lab3slot == 1)))
					conf->lectures[i].labs[2] = 1;
				if (conf->lectures[i].lab3day == conf->lectures[j].cid2day
						&& (conf->lectures[j].cid2slot == 1\

								&& (conf->lectures[i].lab3slot == 0 || conf->lectures[i].lab3slot == 1)))
					conf->lectures[i].labs[2] = 1;
				if (conf->lectures[i].lab3day == conf->lectures[j].cid1day
						&& conf->lectures[i].lab3slot == conf->lectures[j].cid1slot)
					conf->lectures[i].labs[2] = 1;
				if (conf->lectures[i].lab3day == conf->lectures[j].lab3day
						&& conf->lectures[i].lab3slot == conf->lectures[j].lab3slot)
					conf->lectures[i].labs[2] = 1;
				if (conf->prereq[i][j] == 0 && conf->lectures[i].lab1 != -1 && conf->lectures[i].lab2 == -1
						&& conf->lectures[i].lab3 == -1\
 && conf->lectures[i].labs[0] == 1) {
					cnt = cnt + 1;
					if (prnt == 1) {
						tmperr.desc = " 4 Prev Sem Next Sem   LABS ";
						tmperr.s1 = conf->lectures[i].lectname;
						tmperr.s2 = conf->lectures[j].lectname;
						errv.push_back(tmperr);
					}
				}
				if (conf->prereq[i][j] == 0 && conf->lectures[i].lab1 == -1 && conf->lectures[i].lab2 == -1
						&& conf->lectures[i].lab3 != -1\
 && conf->lectures[i].labs[2] == 1) {
					cnt = cnt + 1;
					if (prnt == 1) {
						tmperr.desc = " 4 Prev Sem Next Sem   LABS ";
						tmperr.s1 = conf->lectures[i].lectname;
						tmperr.s2 = conf->lectures[j].lectname;
						errv.push_back(tmperr);
					}
				}
				if (conf->prereq[i][j] == 0 && conf->lectures[i].lab1 != -1 && conf->lectures[i].lab2 != -1
						&& conf->lectures[i].lab3 == -1\
 && conf->lectures[i].labs[0] == 1
						&& conf->lectures[i].labs[1] == 1) {
					cnt = cnt + 1;
					if (prnt == 1) {
						tmperr.desc = " 4 Prev Sem Next Sem   LABS ";
						tmperr.s1 = conf->lectures[i].lectname;
						tmperr.s2 = conf->lectures[j].lectname;
						errv.push_back(tmperr);
					}
				}
			}
		}
	}
	if (prnt == 1)
		for (i = 0; i < errv.size(); i++) {
			printf("Soft2 %s ", errv[i].desc.data());
			printf("%s ", errv[i].s1.data());
			printf("%s ", errv[i].s2.data());
			printf("%d \n", errv[i].n1);
			fprintf(errf, "Soft2 %s ", errv[i].desc.data());
			fprintf(errf, "%s ", errv[i].s1.data());
			fprintf(errf, "%s ", errv[i].s2.data());
			fprintf(errf, "%d \n", errv[i].n1);
		}
	fitnessf2 = cnt;
	return 0;
}
int Individual::fitnessHCAL(int prnt) {
	int cnt = 0, cnt1 = 0;
	size_t i, j;
	vector<Errnode> errv;
	Errnode tmperr;
	for (i = 0; i < CHROML; i++) {
		for (j = i + 1; j < CHROML; j++) {
			if (fit_hconfmat(i, j)) {
				cnt = cnt + 1;
				if (prnt == 1) {
					tmperr.desc = " 1 Confmat ";
					tmperr.s1 = conf->courmat[i].cname;
					tmperr.s2 = conf->courmat[j].cname;
					errv.push_back(tmperr);
				}
				if (prnt == 2) {
					conflv.push_back(i);
					conflv.push_back(j);
				}
			}
			if (fit_hsameday(i, j)) {
				cnt = cnt + 1;
				if (prnt == 1) {
					tmperr.desc = " 2 Same Day ";
					tmperr.s1 = conf->courmat[i].cname;
					tmperr.s2 = conf->courmat[j].cname;
					errv.push_back(tmperr);
				}
				if (prnt == 2) {
					conflv.push_back(i);
					conflv.push_back(j);
				}
			}
			if (fit_hmidday(i, j)) {
				cnt1 = cnt1 + 1;
				if (prnt == 1) {
					tmperr.desc = " 3 Hours 11-13   ";
					tmperr.s1 = conf->courmat[i].cname;
					tmperr.s2 = conf->courmat[j].cname;
					errv.push_back(tmperr);
				}
				if (prnt == 2) {
					conflv.push_back(i);
					conflv.push_back(j);
				}
			}
		}
	}
	if (prnt == 1)
		for (i = 0; i < errv.size(); i++) {
			printf("Hard %s ", errv[i].desc.data());
			printf("%s ", errv[i].s1.data());
			printf("%s ", errv[i].s2.data());
			printf("%d \n", errv[i].n1);
			fprintf(errf, "Hard %s ", errv[i].desc.data());
			fprintf(errf, "%s ", errv[i].s1.data());
			fprintf(errf, "%s ", errv[i].s2.data());
			fprintf(errf, "%d \n", errv[i].n1);
		}
	fitnessh = cnt;
	fitnessh1 = cnt1;
	return cnt + cnt1;
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

void Individual::updatefitness() {
	fitnessHCAL(0);
	fitnessFCAL(0);
	fitnessF1CAL(0);
	fitnessF2CAL(0);
	fitnessF3CAL(0);
}

bool Individual::fit_hconfmat(int i, int j) {
	return conf->confmat[i][j] == 1
			&& ((chromosome->get_slot(i) == chromosome->get_slot(j)
					&& conf->courmat[i].hours == conf->courmat[j].hours)
					|| (conf->courmat[i].hours != conf->courmat[j].hours
							&& chromosome->day[i] == chromosome->day[j]
							&& ((conf->courmat[i].hours == 1 && conf->courmat[j].hours == 2
									&& (chromosome->slot[i] == 0 || chromosome->slot[i] == 1)
									&& chromosome->slot[j] == 1)
									|| (conf->courmat[i].hours == 2 && conf->courmat[j].hours == 1
											&& (chromosome->slot[j] == 0 || chromosome->slot[j] == 1)
											&& chromosome->slot[i] == 1))));
}

bool Individual::fit_hsameday(int i, int j) {
	return chromosome->day[i] == chromosome->day[j] && conf->cid[i] == conf->cid[j]
			&& conf->courmat[i].cname.substr(0, 8) != "cse211.L"
			&& conf->courmat[i].cname.substr(0, 8) != "cse112.L";
}

bool Individual::fit_hmidday(int i, int j) {
	return conf->courmat[i].semid == conf->courmat[j].semid && chromosome->day[i] == chromosome->day[j]
			&& ((conf->courmat[i].hours == 1 && conf->courmat[j].hours == 2
					&& (chromosome->slot[i] == 0 || chromosome->slot[i] == 1) && chromosome->slot[j] == 1)
					|| ((conf->courmat[i].hours == 2 && conf->courmat[j].hours == 1
							&& ((chromosome->slot[j] == 0 || chromosome->slot[j] == 1)
									&& chromosome->slot[i] == 1))));
}

bool Individual::fit_sdepmeet(int i) {
	return ((conf->cse[i] == 1 && chromosome->day[i] == 3 && (chromosome->slot[i] == 2)
			&& conf->courmat[i].hours == 2)
			|| (conf->cse[i] == 1 && chromosome->day[i] == 3 && (chromosome->slot[i] == 2)
					&& conf->courmat[i].hours == 1)) && conf->courmat[i].cname.substr(0, 8) != "cse211.L"
			&& conf->courmat[i].cname.substr(0, 8) != "cse112.L";
}

void Individual::hc_worstsection() {
	Individual subject(*this);
	s_hard_fitness_t oldfit, newfit, rndfit;
	int rnd_slot = 0;
	int max_attempt = conf->hc_max_ind; //no infinite loop if we have perfect individual.
	int maxdiff, maxid;
	bool somechanges;

	bool visitlist[CHROML] = {};

	for (int i = 0; i < conf->hc_max_ind; ++i) {
		//try to find a slot that has conflicts.
		while(max_attempt > 0) {
			rnd_slot = RND(NCOL);
			get_hardfit(*subject.chromosome->get_section_list(rnd_slot), rndfit);
			if (rndfit.total_fit > 0) break;

			max_attempt--;
		}
		if (!max_attempt) {
			//yay! perfect individual. no need for hill climbing!
			return;
		}

		int sectid, maxtry = 3;
		do {
			list<int>::iterator it = chromosome->slot_map[rnd_slot].begin();
			advance(it, RND(chromosome->slot_map[rnd_slot].size()));
			sectid = *it;
			maxtry--;
		} while (visitlist[sectid] && maxtry > 0);
		//if we haven't found any non visited member on that section, proceed to another random section
		if (!maxtry) continue;

		maxdiff = 0;
		maxid = -1;
		for (int selcolor = 0; selcolor < no_colors; selcolor++) {
			//mutate the child's gene in the longest_slot list for every color except for its own
			if (selcolor == rnd_slot) {
				continue;
			}
			//restore old fitness of selcolor
			get_hardfit(*subject.chromosome->get_section_list(selcolor), oldfit);
			//update the gene to selcolor, if we have any "worst" color.
			subject.chromosome->update(sectid, selcolor);
			//get the new fitness for comparison
			get_hardfit(*subject.chromosome->get_section_list(selcolor), newfit);

			if ((oldfit.fitness[i] - newfit.fitness[i]) > maxdiff) {
				maxdiff = oldfit.total_fit - newfit.total_fit;
				maxid = selcolor;
			}
			//un-stage changes
			subject.chromosome->update(sectid, rnd_slot);
		}

		somechanges = false;
		//if difference is larger than 0, then we found a better slot for that section.
		if (maxdiff > 0) {
			chromosome->update(sectid, maxid);
			somechanges = true;
		}
		//todo: updatefitness'ý deðiþtir. modüler fitness tut, herþeyi baþtan hesaplama.
		if (somechanges) {
			buildtimetable();
			updatefitness();
		}
	}

}

bool Individual::fit_slabconf(int i, int j) {
	return (conf->lab[i] == 1 && conf->lab[j] == 1)
			&& (((conf->courmat[i].cname.substr(0, 6) == "cse221"
					|| conf->courmat[i].cname.substr(0, 6) == "cse421"
					|| conf->courmat[i].cname.substr(0, 6) == "cse232")
					&& (conf->courmat[j].cname.substr(0, 6) == "cse221"
							|| conf->courmat[j].cname.substr(0, 6) == "cse421"
							|| conf->courmat[j].cname.substr(0, 6) == "cse232"))
					&& (chromosome->day[i] == chromosome->day[j] && chromosome->slot[i] == chromosome->slot[j]));
}
/*
 * takes a list of slots (which contain course ID that is assigned to slot, for more info refer to Chromosome.cpp::slot_map)
 * and find the worst gene in it. Also compute fitness of the slot.
 */
void Individual::get_hardfit(list<int> &list, s_worst_chrom_t &worst_chrom) {
	int temp_fit[HARD_FIT_N] = { };
	for (int i = 0; i < HARD_FIT_N; ++i) {
		worst_chrom.fitness[i] = 0;
		worst_chrom.id[i] = -1;
	}
	std::list<int>::iterator it1e = list.end();
	for (std::list<int>::iterator it1 = list.begin(); it1 != it1e; ++it1) {
		//don't calculate fitness for courses that has constraint. We cannot change them, so focus on
		//the ones that we can change.
		if (conf->courmat[*it1].has_constraint == 1) {
			continue;
		}
		for (std::list<int>::iterator it2 = list.begin(); it2 != it1e; ++it2) {
			if (fit_hconfmat(*it1, *it2)) {
				temp_fit[0]++;
			}
			if (fit_hsameday(*it1, *it2)) {
				temp_fit[1]++;
			}
			if (fit_hmidday(*it1, *it2)) {
				temp_fit[2]++;
			}
		}
		for (int i = 0; i < HARD_FIT_N; ++i) {
			if (temp_fit[i] > worst_chrom.fitness[i]) {
				worst_chrom.fitness[i] = temp_fit[i];
				worst_chrom.id[i] = *it1;
			}
		}
		for (int i = 0; i < HARD_FIT_N; ++i) {
			temp_fit[i] = 0;
		}
	}
}

void Individual::get_hardfit(list<int>& list, s_hard_fitness_t& fit) {
	int i;
	for (i = 0; i < HARD_FIT_N; ++i) {
		fit.fitness[i] = 0;
	}
	fit.total_fit = 0;
	std::list<int>::iterator ite = list.end();
	std::set<int> visit_set;
	for (std::list<int>::iterator it = list.begin(); it != ite; ++it) {
		for (i = 0; i < POPUL; ++i) {
			if (*it == i || visit_set.find(i) != visit_set.end())
				continue;
			if (fit_hconfmat(*it, i)) {
				fit.fitness[0]++;
			}
			if (fit_hsameday(*it, i)) {
				fit.fitness[1]++;
			}
			if (fit_hmidday(*it, i)) {
				fit.fitness[2]++;
			}
		}
		visit_set.insert(*it);
	}
	for (int i = 0; i < HARD_FIT_N; ++i) {
		fit.total_fit += fit.fitness[i];
	}
}

} /* namespace std */
