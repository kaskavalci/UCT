/*
 * Fitness.cpp
 *
 *  Created on: 30 Kas 2012
 *      Author: HalilCan
 */

#include "Fitness.h"
#include "Errnode.h"
#include "Chromosome.h"
#include <vector>
#include <set>
#include <iostream>
#include <stdlib.h>

#define INC_HARD(i, x)	fit.fitness[x]++; \
				fit.hard_fit++; \
				fit.fitnessBySect[i][fit_mHard]++; \
				fit.fitnessBySect[i][fit_mGeneTotal]++;

#define INC_SOFT(i, x) fit.fitness[x]++; \
				fit.soft_fit++; \
				fit.fitnessBySect[i][fit_mSoft]++; \
				fit.fitnessBySect[i][fit_mGeneTotal]++;

namespace std {

Fitness::Fitness(const Chromosome *chrom) {
	this->chromosome = chrom;
	conf = Common::getConf();
	fill(fitness.fitness, fitness.fitness + TOT_FIT_N, 0);
}

Fitness::Fitness(const Fitness* source) {
	conf = Common::getConf();
	chromosome = NULL;
	fitness = source->fitness;
}
/**
 * Calculates hard and soft fitness value. Warning: Large overhead.
 * @param print 0 does not print out the fitness calculation results, 1 prints.
 * @param fit fitness value to be filled. default is Chromosome's fitness.
 */
void Fitness::calcFit(int print, fitness_t& fit, int type) {
	fit.hard_fit = 0;
	fit.soft_fit = 0;
	//init chrom IDs in fitnessBySect struct. it is to be used while sorting
	for (int i = 0; i < CHROML; ++i) {
		fit.fitnessBySect[i].fill(0);
		fit.fitnessBySect[i][fit_mGeneID] = i;
	}
	fill(fit.fitness, fit.fitness + TOT_FIT_N, 0);

	//hard fitness calculation
	if (type == hc_hard || type == hc_both) {
		h_confmat(fit, print);
		h_sameday(fit, print);
		h_midday(fit, print);
	}
	//soft fitness calculation
	if (type == hc_soft || type == hc_both) {
		s_depmet(fit, print);
		s_hwlab(fit, print);
		s_lecturer(fit, print);
		s_LTLconflict(fit, print);
		s_ConsecSem(fit, print);
		s_ConsecSemLab(fit, print);
		s_lunch(fit, print);
		s_eveningLecture(fit, print);
		s_morningLab(fit, print);
	}
	fit.total_fit = fit.hard_fit + fit.soft_fit;
}

void Fitness::init_labs(vector<Lecture>::const_iterator it1, vector<Lecture>::const_iterator it2) {
	fill_n(&labs[0], 3, false);
	if (it1->lab[lect_lab1][lect_labday] == it2->cid1day
			&& (it1->lab[lect_lab1][lect_labslot] == 1 && (it2->cid1slot == 0 || it2->cid1slot == 1)))
		labs[0] = true;
	if (it1->lab[lect_lab1][lect_labday] == it2->lab[lect_lab3][lect_labday]
			&& (it1->lab[lect_lab1][lect_labslot] == 1
					&& (it2->lab[lect_lab3][lect_labslot] == 0 || it2->lab[lect_lab3][lect_labslot] == 1)))
		labs[0] = true;
	if (it1->lab[lect_lab1][lect_labday] == it2->cid2day && it1->lab[lect_lab1][lect_labslot] == it2->cid2slot)
		labs[0] = true;
	if (it1->lab[lect_lab1][lect_labday] == it2->lab[lect_lab1][lect_labday]
			&& it1->lab[lect_lab1][lect_labslot] == it2->lab[lect_lab1][lect_labslot])
		labs[0] = true;
	if (it1->lab[lect_lab1][lect_labday] == it2->lab[lect_lab2][lect_labday]
			&& it1->lab[lect_lab1][lect_labslot] == it2->lab[lect_lab2][lect_labslot])
		labs[0] = true;
	if (it1->lab[lect_lab2][lect_labday] == it2->cid1day
			&& (it1->lab[lect_lab2][lect_labslot] == 1 && (it2->cid1slot == 0 || it2->cid1slot == 1)))
		labs[1] = true;
	if (it1->lab[lect_lab2][lect_labday] == it2->lab[lect_lab3][lect_labday]
			&& (it1->lab[lect_lab2][lect_labslot] == 1
					&& (it2->lab[lect_lab3][lect_labslot] == 0 || it2->lab[lect_lab3][lect_labslot] == 1)))
		labs[1] = true;
	if (it1->lab[lect_lab2][lect_labday] == it2->cid2day && it1->lab[lect_lab2][lect_labslot] == it2->cid2slot)
		labs[1] = true;
	if (it1->lab[lect_lab2][lect_labday] == it2->lab[lect_lab1][lect_labday]
			&& it1->lab[lect_lab2][lect_labslot] == it2->lab[lect_lab1][lect_labslot])
		labs[1] = true;
	if (it1->lab[lect_lab2][lect_labday] == it2->lab[lect_lab2][lect_labday]
			&& it1->lab[lect_lab2][lect_labslot] == it2->lab[lect_lab2][lect_labslot])
		labs[1] = true;
	if (it1->lab[lect_lab3][lect_labday] == it2->lab[lect_lab1][lect_labday]
			&& (it2->lab[lect_lab1][lect_labslot] == 1
					&& (it1->lab[lect_lab3][lect_labslot] == 0 || it1->lab[lect_lab3][lect_labslot] == 1)))
		labs[2] = true;
	if (it1->lab[lect_lab3][lect_labday] == it2->lab[lect_lab2][lect_labday]
			&& (it2->lab[lect_lab2][lect_labslot] == 1
					&& (it1->lab[lect_lab3][lect_labslot] == 0 || it1->lab[lect_lab3][lect_labslot] == 1)))
		labs[2] = true;
	if (it1->lab[lect_lab3][lect_labday] == it2->cid2day
			&& (it2->cid2slot == 1 && (it1->lab[lect_lab3][lect_labslot] == 0 || it1->lab[lect_lab3][lect_labslot] == 1)))
		labs[2] = true;
	if (it1->lab[lect_lab3][lect_labday] == it2->cid1day && it1->lab[lect_lab3][lect_labslot] == it2->cid1slot)
		labs[2] = true;
	if (it1->lab[lect_lab3][lect_labday] == it2->lab[lect_lab3][lect_labday]
			&& it1->lab[lect_lab3][lect_labslot] == it2->lab[lect_lab3][lect_labslot])
		labs[2] = true;
}

inline bool Fitness::fit_hconfmat(int i, int j) {
	return conf->courmat[i].semid == conf->courmat[j].semid
			&& ((chromosome->get_slot(i) == chromosome->get_slot(j) && conf->courmat[i].hours == conf->courmat[j].hours)
					|| (conf->courmat[i].hours != conf->courmat[j].hours
							&& chromosome->get_day(i) == chromosome->get_day(j)
							&& ((conf->courmat[i].hours == 1 && conf->courmat[j].hours == 2
									&& (chromosome->get_slot(i) == 0 || chromosome->get_slot(i) == 1)
									&& chromosome->get_slot(j) == 1)
									|| (conf->courmat[i].hours == 2 && conf->courmat[j].hours == 1
											&& (chromosome->get_slot(j) == 0 || chromosome->get_slot(j) == 1)
											&& chromosome->get_slot(i) == 1))));
}
/*
 * TODO: ability to add special constraints. as in here, substr.
 * course's uniqueID corresponds to lecturer. Because one section can only be assigned to one lecturer.
 * if we check uniqueIDs we also check if the course is given by the same lecturer.
 */
inline bool Fitness::fit_hsameday(int i, int j) {
	return chromosome->get_day(i) == chromosome->get_day(j) && conf->courmat[i].uniqueID == conf->courmat[j].uniqueID
			&& conf->courmat[i].cname.substr(0, 8) != "cse211.L" && conf->courmat[i].cname.substr(0, 8) != "cse112.L";
}

inline bool Fitness::fit_hmidday(int i, int j) {
	return conf->courmat[i].semid == conf->courmat[j].semid && chromosome->get_day(i) == chromosome->get_day(j)
			&& ((conf->courmat[i].hours == 1 && conf->courmat[j].hours == 2
					&& (chromosome->get_slot(i) == 0 || chromosome->get_slot(i) == 1) && chromosome->get_slot(j) == 1)
					|| ((conf->courmat[i].hours == 2 && conf->courmat[j].hours == 1
							&& ((chromosome->get_slot(j) == 0 || chromosome->get_slot(j) == 1)
									&& chromosome->get_slot(i) == 1))));
}

inline bool Fitness::fit_sdepmeet(int i) {
	return ((conf->cse[i] == 1 && chromosome->get_day(i) == 3 && (chromosome->get_slot(i) == 2)
			&& conf->courmat[i].hours == 2)
			|| (conf->cse[i] == 1 && chromosome->get_day(i) == 3 && (chromosome->get_slot(i) == 2)
					&& conf->courmat[i].hours == 1)) && conf->courmat[i].cname.substr(0, 8) != "cse211.L"
			&& conf->courmat[i].cname.substr(0, 8) != "cse112.L";
}

inline bool Fitness::fit_sconssem1(int i, int j) {
	return conf->prereq[i][j] == 0 && i != j && conf->courmat[i].semid == conf->courmat[j].semid + 1
			&& conf->courmat[i].hours == conf->courmat[j].hours && chromosome->get_day(i) == chromosome->get_day(j)
			&& chromosome->get_slot(i) == chromosome->get_slot(j) && conf->cse[i] == 1 && conf->cse[j] == 1
			&& conf->lab[i] != 1 && conf->lab[j] != 1;
}

inline bool Fitness::fit_sconssem2(int i, int j) {
	return conf->prereq[i][j] == 0 && i != j && conf->courmat[i].semid == conf->courmat[j].semid + 1
			&& conf->courmat[i].hours == 1 && conf->courmat[j].hours == 2
			&& chromosome->get_day(i) == chromosome->get_day(j)
			&& (chromosome->get_slot(i) == 0 || chromosome->get_slot(i) == 1) && chromosome->get_slot(j) == 1
			&& conf->cse[i] == 1 && conf->cse[j] == 1;
}

inline bool Fitness::fit_sconssem3(int i, int j) {
	return conf->prereq[i][j] == 0 && i != j && conf->courmat[i].semid == conf->courmat[j].semid + 1
			&& conf->courmat[i].hours == 2 && conf->courmat[j].hours == 1
			&& chromosome->get_day(i) == chromosome->get_day(j)
			&& (chromosome->get_slot(j) == 0 || chromosome->get_slot(j) == 1) && chromosome->get_slot(i) == 1
			&& conf->cse[i] == 1 && conf->cse[j] == 1;
}

inline bool Fitness::fit_sconssemlab1(int it1, int it2) {
	return conf->prereq[it1][it2] == 0 && conf->lectures[it1].lab[lect_lab1][lect_idx] != -1
			&& conf->lectures[it1].lab[lect_lab2][lect_idx] == -1 && conf->lectures[it1].lab[lect_lab3][lect_idx] == -1
			&& labs[0];
}

inline bool Fitness::fit_sconssemlab2(int it1, int it2) {
	return conf->prereq[it1][it2] == 0 && conf->lectures[it1].lab[lect_lab1][lect_idx] == -1
			&& conf->lectures[it1].lab[lect_lab2][lect_idx] == -1 && conf->lectures[it1].lab[lect_lab3][lect_idx] != -1
			&& labs[2];
}

inline bool Fitness::fit_sconssemlab3(int it1, int it2) {
	return conf->prereq[it1][it2] == 0 && conf->lectures[it1].lab[lect_lab1][lect_idx] != -1
			&& conf->lectures[it1].lab[lect_lab2][lect_idx] != -1 && conf->lectures[it1].lab[lect_lab3][lect_idx] == -1
			&& labs[0] && labs[1];
}

/*
 * apparently, we are OK with lunch conflict when it is on
 * 1. semester: Thursday.
 * 2. semester: Wednesday, Thursday, Friday.
 */
inline bool Fitness::fit_signoredlunchconflict(int semester, int day) {
	return !(semester == 0 && day == 3) && !(semester == 1 && day == 2) && !(semester == 1 && day == 3)
			&& !(semester == 1 && day == 4);
}

inline bool Fitness::fit_slabconf(int i, int j) {
	return (conf->lab[i] == 1 && conf->lab[j] == 1)
			&& (((conf->courmat[i].cname.substr(0, 6) == "cse221" || conf->courmat[i].cname.substr(0, 6) == "cse421"
					|| conf->courmat[i].cname.substr(0, 6) == "cse232")
					&& (conf->courmat[j].cname.substr(0, 6) == "cse221"
							|| conf->courmat[j].cname.substr(0, 6) == "cse421"
							|| conf->courmat[j].cname.substr(0, 6) == "cse232"))
					&& (chromosome->get_day(i) == chromosome->get_day(j)
							&& chromosome->get_slot(i) == chromosome->get_slot(j)));
}

inline void Fitness::h_confmat(fitness_t& fit, int print) {
	int j;
	for (int i = 0; i < CHROML; ++i) {
		for (j = i + 1; j < CHROML; ++j) {
			if (fit_hconfmat(i, j)) {
				INC_HARD(i, fit_hConfmat)
				if (print == 1) {
					cout << "Hard 1 Confmat " << conf->courmat[i].cname << " " << conf->courmat[j].cname << endl;
				}
			}
		}
	}
}

inline void Fitness::h_sameday(fitness_t& fit, int print) {
	int j;
	for (int i = 0; i < CHROML; ++i) {
		for (j = i + 1; j < CHROML; ++j) {
			if (fit_hsameday(i, j)) {
				INC_HARD(i, fit_hSameDay)
				if (print == 1) {
					cout << "Hard 2 Same Day " << conf->courmat[i].cname << " " << conf->courmat[j].cname << endl;
				}
			}
		}
	}
}

inline void Fitness::h_midday(fitness_t& fit, int print) {
	int j;
	for (int i = 0; i < CHROML; ++i) {
		for (j = i + 1; j < CHROML; ++j) {
			if (fit_hmidday(i, j)) {
				INC_HARD(i, fit_hMidHour)
				if (print == 1) {
					cout << "Hard 3 Hours 11-13 " << conf->courmat[i].cname << " " << conf->courmat[j].cname << endl;
				}
			}
		}
	}
}

inline void Fitness::s_depmet(fitness_t& fit, int print) {
	for (int i = 0; i < CHROML; ++i) {
		if (fit_sdepmeet(i)) {
			INC_SOFT(i, fit_sDepMeeting)
			if (print == 1) {
				cout << "Soft 15 Departmental Meeting " << conf->courmat[i].cname << endl;
			}
		}
	}
}

inline void Fitness::s_hwlab(fitness_t& fit, int print) {
	int i, j;
	for (i = 0; i < CHROML; ++i) {
		for (j = i + 1; j < CHROML; ++j) {
			if (fit_slabconf(i, j)) {
				INC_SOFT(i, fit_shardlab)
				if (print == 1) {
					cout << "Soft 16 Hardware Labs " << conf->courmat[i].cname << " " << conf->courmat[j].cname << endl;
				}
			}
		}
	}
}

inline void Fitness::s_lecturer(fitness_t& fit, int print) {
	int i, n;
	int lectmatrix[5][10];
	int morlect[5];

	for (size_t h = 0; h < conf->lecturers.size(); h++) {
		fill_n(&morlect[0], 5, -1);
		fill_n(&lectmatrix[0][0], 5 * 10, -1);
		//iterate all courses that Lecturer has
		for (vector<int>::iterator idx = conf->lecturers[h].myCourses.begin();
				idx != conf->lecturers[h].myCourses.end(); ++idx) {
			if (conf->lecturers[h].myCourses.size() > 1) {
				cout << "";
			}
			if (conf->courmat[*idx].hours == 1) {
				switch (chromosome->get_period(*idx)) {
				case 0:
					lectmatrix[chromosome->get_day(*idx)][2] = *idx;
					break;
				case 1:
					lectmatrix[chromosome->get_day(*idx)][3] = *idx;
					break;
				case 2:
					lectmatrix[chromosome->get_day(*idx)][4] = *idx;
					break;
				case 3:
					lectmatrix[chromosome->get_day(*idx)][9] = *idx;
					break;
				}
			} else if (conf->courmat[*idx].hours == 2) {
				switch (chromosome->get_period(*idx)) {
				case 0:
					lectmatrix[chromosome->get_day(*idx)][0] = *idx;
					lectmatrix[chromosome->get_day(*idx)][1] = *idx;
					break;
				case 1:
					lectmatrix[chromosome->get_day(*idx)][2] = *idx;
					lectmatrix[chromosome->get_day(*idx)][3] = *idx;
					break;
				case 2:
					lectmatrix[chromosome->get_day(*idx)][5] = *idx;
					lectmatrix[chromosome->get_day(*idx)][6] = *idx;
					break;
				case 3:
					lectmatrix[chromosome->get_day(*idx)][7] = *idx;
					lectmatrix[chromosome->get_day(*idx)][8] = *idx;
					break;
				}
			}
		}
		int fday, fday2 = 0, dayov;
		for (i = 0; i < 5; i++) {
			fday = -1;
			dayov = 0;
			if (lectmatrix[i][0] != -1)
				morlect[i] = lectmatrix[i][0];
			for (n = 0; n < 10; n++) {
				if (lectmatrix[i][n] != -1) {
					fday = lectmatrix[i][n];
					dayov++;
				}
				if (n + 3 < 10 && lectmatrix[i][n] != -1 && lectmatrix[i][n + 1] != -1 && lectmatrix[i][n + 2] != -1
						&& lectmatrix[i][n + 3] != -1) {
					INC_SOFT(lectmatrix[i][n + 2], fit_slectConsWork)
					if (print == 1) {
						cout << "Soft 7 Lecturer 4 cons Work Hour " << conf->lecturers[h].name << endl;
					}
				}
			}
			if (dayov > 4) {
				INC_SOFT(fday, fit_sLectOverWork)
				if (print == 1) {
					cout << "Soft 9 Lecturer More than 4 Hours per day  " << conf->lecturers[h].name << endl;
				}
			}
			if (fday == -1)
				fday2 = 1;
		}
		if (fday2 == 0) {
			//choose a random day, and favor evening courses for fitness.
			int rndDay = RND(5);
			for (i = 9; i >= 0; --i) {
				if (lectmatrix[rndDay][i] != -1)
					break;
			}
			INC_SOFT(i, fit_sLectNoFreeDay)

			if (print == 1) {
				cout << "Soft 10 Lecturer No free Day  " << conf->lecturers[h].name << endl;
			}
		}
		//count number of morning lectures a lecturer has. stop if we have 2 or more morning lectures.
		//favor the first day we encounter morning lecture.
		int nummor = 0;
		for (i = 0; i < 5; i++) {
			if (morlect[i] != -1) {
				if (++nummor > 1)
					break;
			}
		}
		if (nummor > 1) {
			INC_SOFT(i, fit_sLectMorningCourse)
			if (print == 1) {
				cout << "Soft 12 Lecturer More than 2 morning courses  " << conf->lecturers[h].name << endl;
			}
		}
	}
}

inline void Fitness::s_LTLconflict(fitness_t& fit, int print) {
	vector<int> cmax(conf->labcourses.size(), -1);
	vector<int> cmin(conf->labcourses.size(), 100);
	vector<int> lab1(conf->labcourses.size(), -1);
	vector<int> lab2(conf->labcourses.size(), -1);
	int decv = -1;
	for (int i = 0; i < CHROML; i++) {
		if (conf->labid[i] != -1) {
			decv = decode(i);
			if (conf->labid[i] > (int) conf->labcourses.size()) {
				cerr << "SEGFAULT";
			}
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
	for (size_t i = 0; i < conf->labcourses.size(); i++) {
		if ((lab1[i] < cmax[i] && lab1[i] > cmin[i]) || (lab2[i] < cmax[i] && lab2[i] > cmin[i])) {
			INC_SOFT(conf->labcourses[i].idx, fit_sLTLconflict)
			if (print == 1) {
				cout << "Soft 11 L T L conflict  " << conf->labcourses[i].course_name << " " << i << endl;
			}
		}
		if ((lab1[i] < cmin[i]) && (lab2[i] > cmin[i])) {
			INC_SOFT(conf->labcourses[i].idx, fit_sLTLconflict)
			if (print == 1) {
				cout << "Soft 11 L T L conflict  " << conf->labcourses[i].course_name << " " << i << endl;
			}
		}
		if ((lab1[i] > cmax[i]) && (lab2[i] < cmax[i])) {
			INC_SOFT(conf->labcourses[i].idx, fit_sLTLconflict)
			if (print == 1) {
				cout << "Soft 11 L T L conflict  " << conf->labcourses[i].course_name << " " << i << endl;
			}
		}
	}
}
/*
 * function that checks if 2 courses that are in consecutive semesters conflicts.
 * for countable fitness structure, only the first lecture will be used.
 * this is not correct but we can choose only ONE
 */
inline void Fitness::s_ConsecSem(fitness_t& fit, int print) {
	int i, j;
	for (i = 0; i < CHROML; i++) {
		for (j = 0; j < CHROML; ++j) {
			if (fit_sconssem1(i, j)) {
				INC_SOFT(i, fit_sConsecSemester)
				if (print == 1) {
					cout << "Soft 4 Prev Sem Next Sem " << conf->courmat[i].cname << " " << conf->courmat[j].cname
							<< endl;
				}
			}
			if (fit_sconssem2(i, j)) {
				INC_SOFT(i, fit_sConsecSemester)
				if (print == 1) {
					cout << "Soft 4 Prev Sem Next Sem " << conf->courmat[i].cname << " " << conf->courmat[j].cname
							<< endl;
				}
			}
			if (fit_sconssem3(i, j)) {
				INC_SOFT(i, fit_sConsecSemester)
				if (print == 1) {
					cout << "Soft 4 Prev Sem Next Sem " << conf->courmat[i].cname << " " << conf->courmat[j].cname
							<< endl;
				}
			}
		}
	}
}

inline void Fitness::s_lunch(fitness_t& fit, int print) {
	short int lunch[8][5][3];
	fill_n(&lunch[0][0][0], 8 * 5 * 3, -1);
	for (int i = 0; i < CHROML; i++) {
		//only if it's 1 hour course and in the first 3rd slot
		if (conf->courmat[i].hours == 1 && chromosome->get_slot(i) > 0 && chromosome->get_slot(i) < 3) {
			lunch[conf->courmat[i].semid - 1][chromosome->get_day(i)][chromosome->get_slot(i)] = i;
		} else if (conf->courmat[i].hours == 2 && chromosome->get_slot(i) == 1) {
			lunch[conf->courmat[i].semid - 1][chromosome->get_day(i)][0] = i;
			lunch[conf->courmat[i].semid - 1][chromosome->get_day(i)][1] = i;
		}
	}
	for (int semester = 0; semester < 8; semester++) {
		for (int day = 0; day < 5; day++) {
			if (lunch[semester][day][0] != -1 && lunch[semester][day][1] != -1 && lunch[semester][day][2] != -1
					&& fit_signoredlunchconflict(semester, day)) {
				//only count the courses on noon.
				INC_SOFT(lunch[semester][day][1], fit_sNoLunch)
				if (print == 1) {
					cout << "Soft 14 No Lunch Hour conflict. semester:" << semester << ", day:" << day + 1 << endl;
				}
			}
		}
	}
}

inline void Fitness::s_eveningLecture(fitness_t& fit, int print) {
	for (int i = 0; i < CHROML; i++) {
		if (conf->courmat[i].hours == 1 && chromosome->get_slot(i) == 3) {
			INC_SOFT(i, fit_sEveningLecture)
			if (print == 1) {
				cout << "Soft 5 Evening Lecture " << conf->courmat[i].cname << endl;
			}
		}
	}
}

inline void Fitness::s_morningLab(fitness_t& fit, int print) {
	for (int i = 0; i < CHROML; i++) {
		if (conf->courmat[i].cname.size() > 8 && conf->lab[i] == 1 && chromosome->get_slot(i) == 0
				&& conf->courmat[i].hours == 2) {
			INC_SOFT(i, fit_sMorningLab)
			if (print == 1) {
				cout << "Soft 6 Mourning Lab " << conf->courmat[i].cname << endl;
			}
		}
	}
}

inline void Fitness::s_ConsecSemLab(fitness_t& fit, int print) {
	int i;
	for (size_t h = 0; h < conf->lectures.size(); ++h) {
		for (set<int>::iterator it = conf->lectures[h].courses.begin(); it != conf->lectures[h].courses.end(); ++it) {
			if (!conf->courmat[*it].isLab && conf->courmat[*it].hours == 1) {
				conf->lectures[h].cid1day = chromosome->get_day(*it);
				conf->lectures[h].cid1slot = chromosome->get_slot(*it);
			}
			if (!conf->courmat[*it].isLab && conf->courmat[*it].hours == 2) {
				conf->lectures[h].cid2day = chromosome->get_day(*it);
				conf->lectures[h].cid2slot = chromosome->get_slot(*it);
			}
			if (conf->courmat[*it].isLab && conf->courmat[*it].cname.at(8) == '1' && conf->courmat[*it].hours == 2) {
				conf->lectures[h].lab[lect_lab1][lect_labday] = chromosome->get_day(*it);
				conf->lectures[h].lab[lect_lab1][lect_labslot] = chromosome->get_slot(*it);
			}
			if (conf->courmat[*it].isLab && conf->courmat[*it].cname.at(8) == '2' && conf->courmat[*it].hours == 2) {
				conf->lectures[h].lab[lect_lab2][lect_labday] = chromosome->get_day(*it);
				conf->lectures[h].lab[lect_lab2][lect_labslot] = chromosome->get_slot(*it);
			}
			if (conf->courmat[*it].isLab && conf->courmat[*it].hours == 1) {
				conf->lectures[h].lab[lect_lab3][lect_labday] = chromosome->get_day(*it);
				conf->lectures[h].lab[lect_lab3][lect_labslot] = chromosome->get_slot(*it);
			}
		}
	}
	int sem1, sem2;
	vector<Lecture>::iterator ite = conf->lectures.end();
	for (vector<Lecture>::iterator it1 = conf->lectures.begin(); it1 != ite; ++it1) {
		for (vector<Lecture>::iterator it2 = conf->lectures.begin(); it2 != ite; ++it2) {
			for (i = 0; i < lect_LABNUM; ++i) {
				if (it1->lab[i][lect_idx] != -1)
					break;
			}
			//lecture doesn't have lab. no need for further checks.
			if (i == lect_LABNUM) {
				continue;
			}
			sem1 = conf->courmat[*it1->courses.begin()].semid;
			sem2 = conf->courmat[*it2->courses.begin()].semid;
			if (sem1 == sem2 + 1 || sem2 == sem1 + 1) {
				init_labs(it1, it2);
				if (fit_sconssemlab1(it1 - conf->lectures.begin(), it2 - conf->lectures.begin())) {
					INC_SOFT(it1->lab[i][lect_idx], fit_sConsecSemLab)
					if (print == 1) {
						cout << "Soft 4 Prev Sem Next Sem LABS" << it1->lectname << " " << it2->lectname << endl;
					}
				}
				if (fit_sconssemlab2(it1 - conf->lectures.begin(), it2 - conf->lectures.begin())) {
					INC_SOFT(it1->lab[i][lect_idx], fit_sConsecSemLab)
					if (print == 1) {
						cout << "Soft 4 Prev Sem Next Sem LABS" << it1->lectname << " " << it2->lectname << endl;
					}
				}
				if (fit_sconssemlab3(it1 - conf->lectures.begin(), it2 - conf->lectures.begin())) {
					INC_SOFT(it1->lab[i][lect_idx], fit_sConsecSemLab)
					if (print == 1) {
						cout << "Soft 4 Prev Sem Next Sem LABS" << it1->lectname << " " << it2->lectname << endl;
					}
				}
			}
		}
	}
}

void Fitness::updateFitness(int print) {
	calcFit(print, fitness, hc_both);
}

int Fitness::decode(int cidx) {
	int rslot = -1, rval = -1;
	if (conf->courmat[cidx].hours == 1) {
		switch (chromosome->get_slot(cidx)) {
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
		switch (chromosome->get_slot(cidx)) {
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
	rval = 10 * chromosome->get_day(cidx) + rslot;
	return rval;
}

Fitness::~Fitness() {
	// TODO Auto-generated destructor stub
}

} /* namespace std */
