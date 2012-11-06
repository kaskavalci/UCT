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

namespace std {

Individual::Individual(Common *conf) {
	int i, j, cnt = 0;

	this->conf = conf;
	chrom_length = CHROML;

	no_colors = NCOL;
	no_periods = 4;
	fitnessh = 0;
	fitnessh1 = 0;
	fitnessh2 = 0;
	fitnessf = 0;
	fitnessf1 = 0;
	fitnessf2 = 0;
	fitnessf3 = 0;
	errf = fopen("errors.txt", "a");

	for (i = 0; i < CHROML; i++) {
		day[i] = -1;
		slot[i] = -1;
		chrom[i] = RND(NCOL);
	}
	for (j = 0; j < CHROML; j++) {
		if (conf->courmat[j].has_constraint == 1) {
			chrom[j] = cnt % NCOL;
			cnt++;
		}
	}
	c2cnt = cnt;
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
				chrom[j] = chrom[i];
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
	for (i = 0; i < CHROML; i++) {
		day[i] = -1;
		slot[i] = -1;
		chrom[i] = RND(NCOL);
	}
	this->errf = source.errf;
	this->chrom_length = source.chrom_length;
	this->conf = source.conf;
	this->c2cnt = source.c2cnt;
	this->no_periods = source.no_periods;
	this->no_colors = source.no_colors;
	this->fitnessh = source.fitnessh;
	this->fitnessh1 = source.fitnessh1;
	this->fitnessh2 = source.fitnessh2;
	this->fitnessf = source.fitnessf;
	this->fitnessf1 = source.fitnessf1;
	this->fitnessf2 = source.fitnessf2;
	this->fitnessf3 = source.fitnessf3;
}

Individual &Individual::operator=(Individual &source) {
	int i, j, k;
	for (i = 0; i < CHROML; i++) {
		this->chrom[i] = source.chrom[i];
		this->day[i] = source.day[i];
		this->slot[i] = source.slot[i];
	}
	for (j = 0; j < 5; j++) {
		for (k = 0; k < 4; k++) {
			this->timetable1[j][k] = source.timetable1[j][k];
			this->timetable2[j][k] = source.timetable2[j][k];
		}
	}
	this->fitnessh = source.fitnessh;
	this->fitnessh1 = source.fitnessh1;
	this->fitnessh2 = source.fitnessh2;
	this->fitnessf = source.fitnessf;
	this->fitnessf1 = source.fitnessf1;
	this->fitnessf2 = source.fitnessf2;
	this->fitnessf3 = source.fitnessf3;
	return *this;
}

void Individual::mutateg5() {
	int i, sel, k;
	int pos1, pos2, pos3;
	int val1, val2, val3;
	for (k = 0; k < chrom_length; k++) {
		if (RND(1000000) > 1000000 * conf->mutg5rate)
			continue;
		pos1 = k;
		pos2 = RND(chrom_length);
		while (pos1 == pos2) {
			pos2 = RND(POPUL);
		}
		pos3 = RND(chrom_length);
		while (pos3 == pos2 || pos3 == pos1) {
			pos3 = RND(POPUL);
		}
		val1 = chrom[pos1];
		val2 = chrom[pos2];
		val3 = chrom[pos3];
		for (i = 0; i < chrom_length; i++) {
			sel = RND(2);
			if (chrom[i] == val3 && sel == 0 && RND(100) < 50) {
				chrom[i] = val1;
				day[i] = chrom[i] / no_periods;
				slot[i] = chrom[i] % no_periods;
			} else if (chrom[i] == val3 && sel == 1 && RND(100) < 50) {
				chrom[i] = val2;
				day[i] = chrom[i] / no_periods;
				slot[i] = chrom[i] % no_periods;
			}
		}
	}
}

void Individual::mutateg3() {
	int i, k;
	int pos1, pos2;
	int val1, val2;
	for (k = 0; k < chrom_length; k++) {
		if (RND(1000000) > 1000000 * conf->mutg3rate)
			continue;
		pos1 = k;
		pos2 = RND(chrom_length);
		while (pos1 == pos2) {
			pos2 = RND(POPUL);
		}
		val1 = chrom[pos1];
		val2 = chrom[pos2];
		for (i = 0; i < chrom_length; i++) {
			if (chrom[i] == val1 && RND(100) < 50) {
				chrom[i] = val2;
				day[i] = chrom[i] / no_periods;
				slot[i] = chrom[i] % no_periods;
			}
		}
	}
}
void Individual::mutateg1() {
	int col1, col2, i, k;
	for (k = 0; k < chrom_length; k++) {
		if (RND(1000000) > 1000000 * conf->mutg1rate)
			continue;
		col1 = chrom[k];
		col2 = RND(no_colors);
		for (i = 0; i < chrom_length; i++) {
			if (chrom[i] == col1) {
				chrom[i] = col2;
				day[i] = chrom[i] / no_periods;
				slot[i] = chrom[i] % no_periods;
			} else if (chrom[i] == col2) {
				chrom[i] = col1;
				day[i] = chrom[i] / no_periods;
				slot[i] = chrom[i] % no_periods;
			}
		}
	}
}

void Individual::mutatehc1(int pos, int val) {
	chrom[pos] = val;
	day[pos] = chrom[pos] / no_periods;
	slot[pos] = chrom[pos] % no_periods;
}
bool Individual::equalsh(Individual&in1, Individual&in2) {
	if (in1.fitnessh == in2.fitnessh\
 && in1.fitnessh1 == in2.fitnessh1 && in1.fitnessh2 == in2.fitnessh2)
		return true;
	return false;
}

void Individual::hc1() {
	int rndidx;
	int selcolor;
	Individual hcchild(*this);

	if (RND(1000) < 1000 * conf->hillrnd || conflv.size() == 0) {
		rndidx = RND(chrom_length);
	} else {
		rndidx = conflv[RND(conflv.size())];
	}

	for (selcolor = 0; selcolor < no_colors; selcolor++) {
		//mutate the child's gene for every color except for its own
		if (hcchild.chrom[rndidx] == selcolor)
			continue;
		hcchild.mutatehc1(rndidx, selcolor);
		hcchild.buildtimetable();

		if ((hcchild.fitnessh + hcchild.fitnessh1 + hcchild.fitnessh2
				< this->fitnessh + this->fitnessh1 + this->fitnessh2)
				&& (hcchild.fitnessf + hcchild.fitnessf1 + hcchild.fitnessf2 + hcchild.fitnessf3
						< this->fitnessf + this->fitnessf1 + this->fitnessf2 + this->fitnessf3)) {
			*this = hcchild;
		}
	}
	conflv.clear();
}

bool Individual::equalss(Individual&in1, Individual&in2) {
	if (in1.fitnessf == in2.fitnessf && in1.fitnessf1 == in2.fitnessf1\
 && in1.fitnessf2 == in2.fitnessf2
			&& in1.fitnessf3 == in2.fitnessf3)
		return true;
	return false;
}

void Individual::mutate() {
	int pos, val;
	pos = RND(CHROML);
	val = RND(NCOL);
	if (conf->courmat[pos].has_constraint != 1) {
		chrom[pos] = val;
		day[pos] = chrom[pos] / 4;
		slot[pos] = chrom[pos] % 4;
	}
}

void Individual::cross(Individual&p1, Individual&p2, Individual&ch1, Individual&ch2) {
	int pos, i;
	pos = RND(CHROML);
	for (i = 0; i < pos; i++) {
		ch1.chrom[i] = p1.chrom[i];
		ch2.chrom[i] = p2.chrom[i];
	}
	for (i = pos; i < CHROML; i++) {
		ch1.chrom[i] = p2.chrom[i];
		ch2.chrom[i] = p1.chrom[i];
	}
}

int Individual::dominates(Individual *target) {
	if (this->fitnessh <= target->fitnessh && this->fitnessh1 <= target->fitnessh1
			&& this->fitnessh2 <= target->fitnessh2 && this->fitnessf <= target->fitnessf
			&& this->fitnessf1 <= target->fitnessf1 && this->fitnessf2 <= target->fitnessf2
			&& this->fitnessf3 <= target->fitnessf3)
		return 1;
	if (target->fitnessh <= this->fitnessh && target->fitnessh1 <= this->fitnessh1
			&& target->fitnessh2 <= this->fitnessh2 && target->fitnessf <= this->fitnessf
			&& target->fitnessf1 <= this->fitnessf1 && target->fitnessf2 <= this->fitnessf2
			&& target->fitnessf3 <= this->fitnessf3)
		return 2;
	return 0;
}

int Individual::decode(int cidx) {
	int rslot = -1, rval = -1;
	if (conf->courmat[cidx].hours == 1) {
		switch (slot[cidx]) {
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
		switch (slot[cidx]) {
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
	rval = 10 * day[cidx] + rslot;
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
					switch (slot[m]) {
					case 0:
						lectmatrix[day[m]][2] = 1;
						break;
					case 1:
						lectmatrix[day[m]][3] = 1;
						break;
					case 2:
						lectmatrix[day[m]][4] = 1;
						break;
					case 3:
						lectmatrix[day[m]][9] = 1;
						break;
					}
				}
				if (conf->courmat[m].hours == 2) {
					switch (slot[m]) {
					case 0:
						lectmatrix[day[m]][0] = 1;
						lectmatrix[day[m]][1] = 1;
						break;
					case 1:
						lectmatrix[day[m]][2] = 1;
						lectmatrix[day[m]][3] = 1;
						break;
					case 2:
						lectmatrix[day[m]][5] = 1;
						lectmatrix[day[m]][6] = 1;
						break;
					case 3:
						lectmatrix[day[m]][7] = 1;
						lectmatrix[day[m]][8] = 1;
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
		if (((conf->cse[i] == 1 && day[i] == 3 && (slot[i] == 2) && conf->courmat[i].hours == 2)
				|| (conf->cse[i] == 1 && day[i] == 3 && (slot[i] == 2) && conf->courmat[i].hours == 1))
				&& conf->courmat[i].cname.substr(0, 8) != "cse211.L"
				&& conf->courmat[i].cname.substr(0, 8) != "cse112.L") {
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
			if ((conf->lab[i] == 1 && conf->lab[j] == 1)
					&& (((conf->courmat[i].cname.substr(0, 6) == "cse221"
							|| conf->courmat[i].cname.substr(0, 6) == "cse421"
							|| conf->courmat[i].cname.substr(0, 6) == "cse232")
							&& (conf->courmat[j].cname.substr(0, 6) == "cse221"
									|| conf->courmat[j].cname.substr(0, 6) == "cse421"
									|| conf->courmat[j].cname.substr(0, 6) == "cse232"))
							&& (day[i] == day[j] && slot[i] == slot[j]))) {
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
					switch (slot[m]) {
					case 0:
						lectmatrix[day[m]][2] = 1;
						break;
					case 1:
						lectmatrix[day[m]][3] = 1;
						break;
					case 2:
						lectmatrix[day[m]][4] = 1;
						break;
					case 3:
						lectmatrix[day[m]][9] = 1;
						break;
					}
				}
				if (conf->courmat[m].hours == 2) {
					switch (slot[m]) {
					case 0:
						lectmatrix[day[m]][0] = 1;
						lectmatrix[day[m]][1] = 1;
						break;
					case 1:
						lectmatrix[day[m]][2] = 1;
						lectmatrix[day[m]][3] = 1;
						break;
					case 2:
						lectmatrix[day[m]][5] = 1;
						lectmatrix[day[m]][6] = 1;
						break;
					case 3:
						lectmatrix[day[m]][7] = 1;
						lectmatrix[day[m]][8] = 1;
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
			if (conf->courmat[i].semid == m + 1 && conf->courmat[i].hours == 1 && slot[i] == 0)
				lunch[m][day[i]][0] = 1;
			if (conf->courmat[i].semid == m + 1 && conf->courmat[i].hours == 1 && slot[i] == 1)
				lunch[m][day[i]][1] = 1;
			if (conf->courmat[i].semid == m + 1 && conf->courmat[i].hours == 1 && slot[i] == 2)
				lunch[m][day[i]][2] = 1;
			if (conf->courmat[i].semid == m + 1 && conf->courmat[i].hours == 2 && slot[i] == 1) {
				lunch[m][day[i]][0] = 1;
				lunch[m][day[i]][1] = 1;
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
		if (conf->courmat[i].hours == 1 && slot[i] == 3) {
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
		if (conf->courmat[i].cname.size() > 8 && conf->lab[i] == 1 && slot[i] == 0
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
					&& conf->courmat[i].hours == conf->courmat[j].hours && day[i] == day[j]
					&& slot[i] == slot[j] && conf->cse[i] == 1 && conf->cse[j] == 1 && conf->lab[i] != 1
					&& conf->lab[j] != 1) {
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
					&& conf->courmat[i].hours == 1 && conf->courmat[j].hours == 2 && day[i] == day[j]
					&& (slot[i] == 0 || slot[i] == 1) && slot[j] == 1 && conf->cse[i] == 1\

					&& conf->cse[j] == 1) {
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
					&& conf->courmat[i].hours == 2 && conf->courmat[j].hours == 1 && day[i] == day[j]
					&& (slot[j] == 0 || slot[j] == 1) && slot[i] == 1 && conf->cse[i] == 1\

					&& conf->cse[j] == 1) {
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
				conf->lectures[lidx].cid1day = day[i];
				conf->lectures[lidx].cid1slot = slot[i];
			}
			if (conf->courmat[i].cname.at(7) == '0' && conf->courmat[i].hours == 2) {
				conf->lectures[lidx].cid2day = day[i];
				conf->lectures[lidx].cid2slot = slot[i];
			}
			if (conf->courmat[i].cname.at(7) == 'L' && conf->courmat[i].cname.at(8) == '1'
					&& conf->courmat[i].hours == 2) {
				conf->lectures[lidx].lab1day = day[i];
				conf->lectures[lidx].lab1slot = slot[i];
			}
			if (conf->courmat[i].cname.at(7) == 'L' && conf->courmat[i].cname.at(8) == '2'
					&& conf->courmat[i].hours == 2) {
				conf->lectures[lidx].lab2day = day[i];
				conf->lectures[lidx].lab2slot = slot[i];
			}
			if (conf->courmat[i].cname.at(7) == 'L' && conf->courmat[i].hours == 1) {
				conf->lectures[lidx].lab3day = day[i];
				conf->lectures[lidx].lab3slot = slot[i];
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
			if ((i != j && conf->confmat[i][j] == 1 && chrom[i] == chrom[j]
					&& conf->courmat[i].hours == conf->courmat[j].hours)
					|| (i != j && conf->confmat[i][j] == 1 && conf->courmat[i].hours != conf->courmat[j].hours
							&& day[i] == day[j]
							&& ((conf->courmat[i].hours == 1 && conf->courmat[j].hours == 2
									&& (slot[i] == 0 || slot[i] == 1) && slot[j] == 1)
									|| (conf->courmat[i].hours == 2 && conf->courmat[j].hours == 1
											&& (slot[j] == 0 || slot[j] == 1) && slot[i] == 1)))) {
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
			if (i != j && day[i] == day[j] && conf->cid[i] == conf->cid[j]
					&& conf->courmat[i].cname.substr(0, 8) != "cse211.L"
					&& conf->courmat[i].cname.substr(0, 8) != "cse112.L") {
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
			if (i != j && conf->courmat[i].semid == conf->courmat[j].semid && day[i] == day[j]
					&& ((conf->courmat[i].hours == 1 && conf->courmat[j].hours == 2
							&& (slot[i] == 0 || slot[i] == 1) && slot[j] == 1)
							|| ((conf->courmat[i].hours == 2 && conf->courmat[j].hours == 1
									&& ((slot[j] == 0 || slot[j] == 1) && slot[i] == 1))))) {
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
				conf->lectures[lidx].cid1day = day[i];
				conf->lectures[lidx].cid1slot = slot[i];
			}
			if (conf->courmat[i].cname.at(7) == '0' && conf->courmat[i].hours == 2
					&& conf->courmat[i].cname.at(8) == '1') {
				conf->lectures[lidx].cid2day = day[i];
				conf->lectures[lidx].cid2slot = slot[i];
			}
			if (conf->courmat[i].cname.at(7) == '0' && conf->courmat[i].hours == 1
					&& conf->courmat[i].cname.at(8) == '2') {
				conf->lectures[lidx].cid3day = day[i];
				conf->lectures[lidx].cid3slot = slot[i];
			}
			if (conf->courmat[i].cname.at(7) == '0' && conf->courmat[i].hours == 2
					&& conf->courmat[i].cname.at(8) == '2') {
				conf->lectures[lidx].cid4day = day[i];
				conf->lectures[lidx].cid4slot = slot[i];
			}
			if (conf->courmat[i].cname.at(7) == 'L' && conf->courmat[i].cname.at(8) == '1'
					&& conf->courmat[i].hours == 2) {
				conf->lectures[lidx].lab1day = day[i];
				conf->lectures[lidx].lab1slot = slot[i];
			}
			if (conf->courmat[i].cname.at(7) == 'L' && conf->courmat[i].cname.at(8) == '2'
					&& conf->courmat[i].hours == 2) {
				conf->lectures[lidx].lab2day = day[i];
				conf->lectures[lidx].lab2slot = slot[i];
			}
			if (conf->courmat[i].cname.at(7) == 'L' && conf->courmat[i].hours == 1) {
				conf->lectures[lidx].lab3day = day[i];
				conf->lectures[lidx].lab3slot = slot[i];
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
			day[i] = conf->courmat[i].c2day;
			slot[i] = conf->courmat[i].c2slot;
			chrom[i] = 4 * conf->courmat[i].c2day + conf->courmat[i].c2slot;
		} else {
			day[i] = chrom[i] / 4;
			slot[i] = chrom[i] % 4;
		}
	}
	fitnessHCAL(0);
	fitnessFCAL(0);
	fitnessF1CAL(0);
	fitnessF2CAL(0);
	fitnessF3CAL(0);
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
		if (conf->courmat[h].semid == sem && day[h] == dy && conf->courmat[h].hours == 2
				&& slot[h] == rslot2) {
			found = 1;
			break;
		} else if (conf->courmat[h].semid == sem && day[h] == dy && conf->courmat[h].hours == 1
				&& slot[h] == rslot1) {
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

} /* namespace std */
