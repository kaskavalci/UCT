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

namespace std {

Fitness::Fitness(const Chromosome *chrom) {
	this->chromosome = chrom;
	conf = Common::getConf();
	for (int i = 0; i < HARD_FIT_N; ++i) {
		hard_fit.fitness[i] = 0;
	}
	for (int i = 0; i < SOFT_FIT_N; ++i) {
		soft_fit.fitness[i] = 0;
	}
}

Fitness::Fitness(const Fitness* source) {
	conf = Common::getConf();
	chromosome = NULL;
	hard_fit = source->hard_fit;
	soft_fit = source->soft_fit;
}

void Fitness::calc_hardfit(s_hard_fitness_t& fit, int print) {
	fit.total_fit = 0;
	h_confmat(fit, print);
	h_sameday(fit, print);
	h_midday(fit, print);
	for (int i = 0; i < HARD_FIT_N; ++i) {
		fit.total_fit += fit.fitness[i];
	}
}

void Fitness::calc_softfit(s_soft_fitness_t& fit, int print) {
	fit.total_fit = 0;
	s_depmet(fit, print);
	s_hwlab(fit, print);
	s_lecturer(fit, print);
	s_LTLconflict(fit, print);
	s_ConsecSem(fit, print);
	s_ConsecSemLab(fit, print);
	s_lunch(fit, print);
	s_eveningLecture(fit, print);
	s_morningLab(fit, print);
	for (int i = 0; i < SOFT_FIT_N; ++i) {
		fit.total_fit += fit.fitness[i];
	}
}
/*
 * todo: conf'u deðiþtirmeden yap
 */
void Fitness::init_labs(vector<Lecture>::iterator it1, vector<Lecture>::iterator it2) {
	if (it1->lab1day == it2->cid1day
			&& (it1->lab1slot == 1 && (it2->cid1slot == 0 || it2->cid1slot == 1)))
		it1->labs[0] = 1;
	if (it1->lab1day == it2->lab3day
			&& (it1->lab1slot == 1 && (it2->lab3slot == 0 || it2->lab3slot == 1)))
		it1->labs[0] = 1;
	if (it1->lab1day == it2->cid2day
			&& it1->lab1slot == it2->cid2slot)
		it1->labs[0] = 1;
	if (it1->lab1day == it2->lab1day
			&& it1->lab1slot == it2->lab1slot)
		it1->labs[0] = 1;
	if (it1->lab1day == it2->lab2day
			&& it1->lab1slot == it2->lab2slot)
		it1->labs[0] = 1;
	if (it1->lab2day == it2->cid1day
			&& (it1->lab2slot == 1 && (it2->cid1slot == 0 || it2->cid1slot == 1)))
		it1->labs[1] = 1;
	if (it1->lab2day == it2->lab3day
			&& (it1->lab2slot == 1 && (it2->lab3slot == 0 || it2->lab3slot == 1)))
		it1->labs[1] = 1;
	if (it1->lab2day == it2->cid2day
			&& it1->lab2slot == it2->cid2slot)
		it1->labs[1] = 1;
	if (it1->lab2day == it2->lab1day
			&& it1->lab2slot == it2->lab1slot)
		it1->labs[1] = 1;
	if (it1->lab2day == it2->lab2day
			&& it1->lab2slot == it2->lab2slot)
		it1->labs[1] = 1;
	if (it1->lab3day == it2->lab1day
			&& (it2->lab1slot == 1 && (it1->lab3slot == 0 || it1->lab3slot == 1)))
		it1->labs[2] = 1;
	if (it1->lab3day == it2->lab2day
			&& (it2->lab2slot == 1 && (it1->lab3slot == 0 || it1->lab3slot == 1)))
		it1->labs[2] = 1;
	if (it1->lab3day == it2->cid2day
			&& (it2->cid2slot == 1 && (it1->lab3slot == 0 || it1->lab3slot == 1)))
		it1->labs[2] = 1;
	if (it1->lab3day == it2->cid1day
			&& it1->lab3slot == it2->cid1slot)
		it1->labs[2] = 1;
	if (it1->lab3day == it2->lab3day
			&& it1->lab3slot == it2->lab3slot)
		it1->labs[2] = 1;
}

void Fitness::updatefitness(int print) {
	calc_hardfit(hard_fit, print);
	calc_softfit(soft_fit, print);
}

inline bool Fitness::fit_hconfmat(int i, int j) {
	return conf->confmat[i][j] == 1
			&& ((chromosome->get_slot(i) == chromosome->get_slot(j) && conf->courmat[i].hours == conf->courmat[j].hours)
					|| (conf->courmat[i].hours != conf->courmat[j].hours && chromosome->day[i] == chromosome->day[j]
							&& ((conf->courmat[i].hours == 1 && conf->courmat[j].hours == 2
									&& (chromosome->slot[i] == 0 || chromosome->slot[i] == 1)
									&& chromosome->slot[j] == 1)
									|| (conf->courmat[i].hours == 2 && conf->courmat[j].hours == 1
											&& (chromosome->slot[j] == 0 || chromosome->slot[j] == 1)
											&& chromosome->slot[i] == 1))));
}

inline bool Fitness::fit_hsameday(int i, int j) {
	return chromosome->day[i] == chromosome->day[j] && conf->cid[i] == conf->cid[j]
			&& conf->courmat[i].cname.substr(0, 8) != "cse211.L" && conf->courmat[i].cname.substr(0, 8) != "cse112.L";
}

inline bool Fitness::fit_hmidday(int i, int j) {
	return conf->courmat[i].semid == conf->courmat[j].semid && chromosome->day[i] == chromosome->day[j]
			&& ((conf->courmat[i].hours == 1 && conf->courmat[j].hours == 2
					&& (chromosome->slot[i] == 0 || chromosome->slot[i] == 1) && chromosome->slot[j] == 1)
					|| ((conf->courmat[i].hours == 2 && conf->courmat[j].hours == 1
							&& ((chromosome->slot[j] == 0 || chromosome->slot[j] == 1) && chromosome->slot[i] == 1))));
}

inline bool Fitness::fit_sdepmeet(int i) {
	return ((conf->cse[i] == 1 && chromosome->day[i] == 3 && (chromosome->slot[i] == 2) && conf->courmat[i].hours == 2)
			|| (conf->cse[i] == 1 && chromosome->day[i] == 3 && (chromosome->slot[i] == 2)
					&& conf->courmat[i].hours == 1)) && conf->courmat[i].cname.substr(0, 8) != "cse211.L"
			&& conf->courmat[i].cname.substr(0, 8) != "cse112.L";
}

inline bool Fitness::fit_sconssem1(int i, int j) {
	return conf->prereq[i][j] == 0 && i != j && conf->courmat[i].semid == conf->courmat[j].semid + 1
			&& conf->courmat[i].hours == conf->courmat[j].hours && chromosome->day[i] == chromosome->day[j]
			&& chromosome->slot[i] == chromosome->slot[j] && conf->cse[i] == 1 && conf->cse[j] == 1 && conf->lab[i] != 1
			&& conf->lab[j] != 1;
}

inline bool Fitness::fit_sconssem2(int i, int j) {
	return conf->prereq[i][j] == 0 && i != j && conf->courmat[i].semid == conf->courmat[j].semid + 1
			&& conf->courmat[i].hours == 1 && conf->courmat[j].hours == 2 && chromosome->day[i] == chromosome->day[j]
			&& (chromosome->slot[i] == 0 || chromosome->slot[i] == 1) && chromosome->slot[j] == 1 && conf->cse[i] == 1
			&& conf->cse[j] == 1;
}

inline bool Fitness::fit_sconssem3(int i, int j) {
	return conf->prereq[i][j] == 0 && i != j && conf->courmat[i].semid == conf->courmat[j].semid + 1
			&& conf->courmat[i].hours == 2 && conf->courmat[j].hours == 1 && chromosome->day[i] == chromosome->day[j]
			&& (chromosome->slot[j] == 0 || chromosome->slot[j] == 1) && chromosome->slot[i] == 1 && conf->cse[i] == 1
			&& conf->cse[j] == 1;
}

inline bool Fitness::fit_sconssemlab1(vector<Lecture>::iterator it1, vector<Lecture>::iterator it2) {
	return conf->prereq[it1->cormat_id][it2->cormat_id] == 0 && it1->lab1 != -1 && it1->lab2 == -1
			&& it1->lab3 == -1 && it1->labs[0] == 1;
}

inline bool Fitness::fit_sconssemlab2(vector<Lecture>::iterator it1, vector<Lecture>::iterator it2) {
	return conf->prereq[it1->cormat_id][it2->cormat_id] == 0 && it1->lab1 == -1 && it1->lab2 == -1
			&& it1->lab3 != -1 && it1->labs[2] == 1;
}

inline bool Fitness::fit_sconssemlab3(vector<Lecture>::iterator it1, vector<Lecture>::iterator it2) {
	return conf->prereq[it1->cormat_id][it2->cormat_id] == 0 && it1->lab1 != -1 && it1->lab2 != -1
			&& it1->lab3 == -1 && it1->labs[0] == 1 && it1->labs[1] == 1;
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
					&& (chromosome->day[i] == chromosome->day[j] && chromosome->slot[i] == chromosome->slot[j]));
}

inline void Fitness::h_confmat(s_hard_fitness_t& fit, int print) {
	int j;
	fit.fitness[fit_hConfmat] = 0;
	for (int i = 0; i < CHROML; ++i) {
		fit.fitnessBySect[i][fit_hConfmat] = 0;
		for (j = i + 1; j < CHROML; ++j) {
			if (fit_hconfmat(i, j)) {
				fit.fitness[fit_hConfmat]++;
				fit.fitnessBySect[i][fit_hConfmat]++;
				if (print == 1) {
					cout << "Hard 1 Confmat " << conf->courmat[i].cname << " " << conf->courmat[j].cname << endl;
				}
			}
		}
	}
}

inline void Fitness::h_sameday(s_hard_fitness_t& fit, int print) {
	int j;
	fit.fitness[fit_hSameDay] = 0;
	for (int i = 0; i < CHROML; ++i) {
		fit.fitnessBySect[i][fit_hSameDay] = 0;
		for (j = i + 1; j < CHROML; ++j) {
			if (fit_hsameday(i, j)) {
				fit.fitness[fit_hSameDay]++;
				fit.fitnessBySect[i][fit_hSameDay]++;
				if (print == 1) {
					cout << "Hard 2 Same Day " << conf->courmat[i].cname << " " << conf->courmat[j].cname << endl;
				}
			}
		}
	}
}

inline void Fitness::h_midday(s_hard_fitness_t& fit, int print) {
	int j;
	fit.fitness[fit_hMidHour] = 0;
	for (int i = 0; i < CHROML; ++i) {
		fit.fitnessBySect[i][fit_hMidHour] = 0;
		for (j = i + 1; j < CHROML; ++j) {
			if (fit_hmidday(i, j)) {
				fit.fitness[fit_hMidHour]++;
				fit.fitnessBySect[i][fit_hMidHour]++;
				if (print == 1) {
					cout << "Hard 3 Hours 11-13 " << conf->courmat[i].cname << " " << conf->courmat[j].cname << endl;
				}
			}
		}
	}
}

inline void Fitness::s_depmet(s_soft_fitness_t& fit, int print) {
	fit.fitness[fit_sDepMeeting] = 0;
	for (int i = 0; i < CHROML; ++i) {
		fit.fitnessBySect[i][fit_sDepMeeting] = 0;
		if (fit_sdepmeet(i)) {
			fit.fitness[fit_sDepMeeting]++;
			fit.fitnessBySect[i][fit_sDepMeeting]++;
			if (print == 1) {
				cout << "Soft 15 Departmental Meeting " << conf->courmat[i].cname << endl;
			}
		}
	}
}

inline void Fitness::s_hwlab(s_soft_fitness_t& fit, int print) {
	fit.fitness[fit_shardlab] = 0;
	int i, j;
	for (i = 0; i < CHROML; ++i) {
		fit.fitnessBySect[i][fit_shardlab] = 0;
		for (j = i + 1; j < CHROML; ++j) {
			if (fit_slabconf(i, j)) {
				fit.fitness[fit_shardlab]++;
				fit.fitnessBySect[i][fit_shardlab]++;
				if (print == 1) {
					cout << "Soft 16 Hardware Labs " << conf->courmat[i].cname << " " << conf->courmat[j].cname << endl;
				}
			}
		}
	}
}
//TODO: enhance algorithm. too many loops
inline void Fitness::s_lecturer(s_soft_fitness_t& fit, int print) {
	int i, n;
	int lectmatrix[5][10];
	int morlect[5];

	fit.fitness[fit_slectConsWork] = 0;
	fit.fitness[fit_sLectEmptyHour] = 0;
	fit.fitness[fit_sLectOverWork] = 0;
	fit.fitness[fit_sLectNoFreeDay] = 0;
	fit.fitness[fit_sLectMorningCourse] = 0;

	for (size_t h = 0; h < conf->lecturers.size(); h++) {
		for (i = 0; i < 5; i++) {
			morlect[i] = -1;
			for (n = 0; n < 10; n++)
				lectmatrix[i][n] = -1;
		}
		for (i = 0; i < CHROML; ++i) {
			fit.fitnessBySect[i][fit_slectConsWork] = 0;
			fit.fitnessBySect[i][fit_sLectEmptyHour] = 0;
			fit.fitnessBySect[i][fit_sLectOverWork] = 0;
			fit.fitnessBySect[i][fit_sLectNoFreeDay] = 0;
			fit.fitnessBySect[i][fit_sLectMorningCourse] = 0;

			if (conf->lecturers[h].compare(conf->courmat[i].lname) == 0) {
				if (conf->courmat[i].hours == 1) {
					switch (chromosome->slot[i]) {
					case 0:
						lectmatrix[chromosome->day[i]][2] = i;
						break;
					case 1:
						lectmatrix[chromosome->day[i]][3] = i;
						break;
					case 2:
						lectmatrix[chromosome->day[i]][4] = i;
						break;
					case 3:
						lectmatrix[chromosome->day[i]][9] = i;
						break;
					}
				} else if (conf->courmat[i].hours == 2) {
					switch (chromosome->slot[i]) {
					case 0:
						lectmatrix[chromosome->day[i]][0] = i;
						lectmatrix[chromosome->day[i]][1] = i;
						break;
					case 1:
						lectmatrix[chromosome->day[i]][2] = i;
						lectmatrix[chromosome->day[i]][3] = i;
						break;
					case 2:
						lectmatrix[chromosome->day[i]][5] = i;
						lectmatrix[chromosome->day[i]][6] = i;
						break;
					case 3:
						lectmatrix[chromosome->day[i]][7] = i;
						lectmatrix[chromosome->day[i]][8] = i;
						break;
					}
				}
			}
		}
		int fday, fday2 = 0, strcnt, stpcnt, dayov;
		bool foundr;
		for (i = 0; i < 5; i++) {
			fday = -1;
			strcnt = 0;
			stpcnt = 0;
			foundr = false;
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
					fit.fitness[fit_slectConsWork]++;
					fit.fitnessBySect[lectmatrix[i][n + 2]][fit_slectConsWork]++;
					if (print == 1) {
						cout << "Soft 7 Lecturer 4 cons Work Hour " << conf->lecturers[h] << endl;
					}
				}
				if (n + 1 < 10 && lectmatrix[i][n] != -1 && lectmatrix[i][n + 1] == -1 && !foundr) {
					strcnt = n + 1;
					foundr = true;
				}
				if (n + 1 < 10 && lectmatrix[i][n] == -1 && lectmatrix[i][n + 1] != -1) {
					stpcnt = n + 1;
				}
			}
			if (stpcnt - strcnt > 3 && foundr) {
				fit.fitness[fit_sLectEmptyHour]++;
				fit.fitnessBySect[lectmatrix[i][stpcnt]][fit_sLectEmptyHour]++;
				if (print == 1) {
					cout << "Soft 8 Lecturer 4 cons Empty Hour " << conf->lecturers[h] << endl;
				}
			}
			if (dayov > 4) {
				fit.fitness[fit_sLectOverWork]++;
				fit.fitnessBySect[fday][fit_sLectOverWork]++;
				if (print == 1) {
					cout << "Soft 9 Lecturer More than 4 Hours per day  " << conf->lecturers[h] << endl;
				}
			}
			if (fday == -1)
				fday2 = 1;
		}
		if (fday2 == 0) {
			fit.fitness[fit_sLectNoFreeDay]++;
			//choose a random day, and favor evening courses for fitness.
			int rndDay = RND(5);
			for (i = 9; i >= 0; --i) {
				if (lectmatrix[rndDay][i] != -1) break;
			}
			fit.fitnessBySect[i][fit_sLectNoFreeDay]++;
			if (print == 1) {
				cout << "Soft 10 Lecturer No free Day  " << conf->lecturers[h] << endl;
			}
		}
		//count number of morning lectures a lecturer has. stop if we have 2 or more morning lectures.
		//favor the first day we encounter morning lecture.
		int nummor = 0;
		for (i = 0; i < 5; i++) {
			if (morlect[i] != -1) {
				if (++nummor > 1) break;
			}
		}
		if (nummor > 1) {
			fit.fitness[fit_sLectMorningCourse]++;
			fit.fitnessBySect[i][fit_sLectMorningCourse]++;
			if (print == 1) {
				cout << "Soft 12 Lecturer More than 2 morning courses  " << conf->lecturers[h] << endl;
			}
		}
	}
}

inline void Fitness::s_LTLconflict(s_soft_fitness_t& fit, int print) {
	vector<int> cmax(conf->labcourses.size(), -1);
	vector<int> cmin(conf->labcourses.size(), 100);
	vector<int> lab1(conf->labcourses.size(), -1);
	vector<int> lab2(conf->labcourses.size(), -1);
	int decv = -1;
	fit.fitness[fit_sLTLconflict] = 0;
	for (int i = 0; i < CHROML; i++) {
		fit.fitnessBySect[i][fit_sLTLconflict] = 0;
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
			fit.fitness[fit_sLTLconflict]++;
			fit.fitnessBySect[conf->labcourses[i].idx][fit_sLTLconflict]++;
			if (print == 1) {
				cout << "Soft 11 L T L conflict  " << conf->labcourses[i].course_name << " " << i << endl;
			}
		}
		if ((lab1[i] < cmin[i]) && (lab2[i] > cmin[i])) {
			fit.fitness[fit_sLTLconflict]++;
			fit.fitnessBySect[conf->labcourses[i].idx][fit_sLTLconflict]++;
			if (print == 1) {
				cout << "Soft 11 L T L conflict  " << conf->labcourses[i].course_name << " " << i << endl;
			}
		}
		if ((lab1[i] > cmax[i]) && (lab2[i] < cmax[i])) {
			fit.fitness[fit_sLTLconflict]++;
			fit.fitnessBySect[conf->labcourses[i].idx][fit_sLTLconflict]++;
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
inline void Fitness::s_ConsecSem(s_soft_fitness_t& fit, int print) {
	int i, j;
	fit.fitness[fit_sConsecSemester] = 0;
	for (i = 0; i < CHROML; i++) {
		fit.fitnessBySect[i][fit_sConsecSemester] = 0;
		for (j = 0; j < CHROML; ++j) {
			if (fit_sconssem1(i, j)) {
				fit.fitness[fit_sConsecSemester]++;
				fit.fitnessBySect[i][fit_sConsecSemester]++;
				if (print == 1) {
					cout << "Soft 4 Prev Sem Next Sem " << conf->courmat[i].cname << " " << conf->courmat[j].cname
							<< endl;
				}
			}
			if (fit_sconssem2(i, j)) {
				fit.fitness[fit_sConsecSemester]++;
				fit.fitnessBySect[i][fit_sConsecSemester]++;
				if (print == 1) {
					cout << "Soft 4 Prev Sem Next Sem " << conf->courmat[i].cname << " " << conf->courmat[j].cname
							<< endl;
				}
			}
			if (fit_sconssem3(i, j)) {
				fit.fitness[fit_sConsecSemester]++;
				fit.fitnessBySect[i][fit_sConsecSemester]++;
				if (print == 1) {
					cout << "Soft 4 Prev Sem Next Sem " << conf->courmat[i].cname << " "
							<< conf->courmat[j].cname << endl;
				}
			}
		}
	}
}

inline void Fitness::s_lunch(s_soft_fitness_t& fit, int print) {
	short int lunch[8][5][3];
	fill_n(&lunch[0][0][0], 8 * 5 * 3, -1);
	fit.fitness[fit_sNoLunch] = 0;
	for (int i = 0; i < CHROML; i++) {
		fit.fitnessBySect[i][fit_sNoLunch] = 0;
		//only if it's 1 hour course and in the first 3rd slot
		if (conf->courmat[i].hours == 1 && chromosome->slot[i] > 0 && chromosome->slot[i] < 3) {
			lunch[conf->courmat[i].semid - 1][chromosome->day[i]][chromosome->slot[i]] = i;
		} else if (conf->courmat[i].hours == 2 && chromosome->slot[i] == 1) {
			lunch[conf->courmat[i].semid - 1][chromosome->day[i]][0] = i;
			lunch[conf->courmat[i].semid - 1][chromosome->day[i]][1] = i;
		}
	}
	for (int semester = 0; semester < 8; semester++) {
		for (int day = 0; day < 5; day++) {
			if (lunch[semester][day][0] != -1 && lunch[semester][day][1] != -1 && lunch[semester][day][2] != -1
					&& fit_signoredlunchconflict(semester, day)) {
				fit.fitness[fit_sNoLunch]++;
				//only count the courses on noon.
				fit.fitnessBySect[lunch[semester][day][1]][fit_sNoLunch]++;
				if (print == 1) {
					cout << "Soft 14 No Lunch Hour conflict. semester:" << semester << ", day:" << day + 1 << endl;
				}
			}
		}
	}
}

inline void Fitness::s_eveningLecture(s_soft_fitness_t& fit, int print) {
	fit.fitness[fit_sEveningLecture] = 0;
	for (int i = 0; i < CHROML; i++) {
		if (conf->courmat[i].hours == 1 && chromosome->slot[i] == 3) {
			fit.fitness[fit_sEveningLecture]++;
			fit.fitnessBySect[i][fit_sEveningLecture] = 1;
			if (print == 1) {
				cout << "Soft 5 Evening Lecture " << conf->courmat[i].cname << endl;
			}
		}
		else {
			fit.fitnessBySect[i][fit_sEveningLecture] = 0;
		}
	}
}

inline void Fitness::s_morningLab(s_soft_fitness_t& fit, int print) {
	fit.fitness[fit_sMorningLab] = 0;
	for (int i = 0; i < CHROML; i++) {
		if (conf->courmat[i].cname.size() > 8 && conf->lab[i] == 1 && chromosome->slot[i] == 0
				&& conf->courmat[i].hours == 2) {
			fit.fitness[fit_sMorningLab]++;
			fit.fitnessBySect[i][fit_sMorningLab] = 1;
			if (print == 1) {
				cout << "Soft 6 Mourning Lab " << conf->courmat[i].cname << endl;
			}
		}
		else {
			fit.fitnessBySect[i][fit_sMorningLab] = 0;
		}
	}
}

inline void Fitness::s_ConsecSemLab(s_soft_fitness_t& fit, int print) {
	int lidx, i;
	fit.fitness[fit_sConsecSemLab] = 0;
	for (i = 0; i < CHROML; i++) {
		fit.fitnessBySect[i][fit_sConsecSemLab] = 0;
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
	vector<Lecture>::iterator ite = conf->lectures.end();
	for (vector<Lecture>::iterator it1 = conf->lectures.begin(); it1 != ite; ++it1) {
		for (vector<Lecture>::iterator it2 = conf->lectures.begin(); it2 != ite; ++it2) {
			it1->labs[0] = -1;
			it1->labs[1] = -1;
			it1->labs[2] = -1;
			if (it1->semid == it2->semid + 1
					|| it2->semid == it1->semid + 1) {
				init_labs(it1, it2);
				if (fit_sconssemlab1(it1, it2)) {
					fit.fitness[fit_sConsecSemLab]++;
					fit.fitnessBySect[it1->cormat_id][fit_sConsecSemLab]++;
					if (print == 1) {
						cout << "Soft 4 Prev Sem Next Sem LABS" << it1->lectname << " "
								<< it2->lectname << endl;
					}
				}
				if (fit_sconssemlab2(it1, it2)) {
					fit.fitness[fit_sConsecSemLab]++;
					fit.fitnessBySect[it1->cormat_id][fit_sConsecSemLab]++;
					if (print == 1) {
						cout << "Soft 4 Prev Sem Next Sem LABS" << it1->lectname << " "
								<< it2->lectname << endl;
					}
				}
				if (fit_sconssemlab3(it1, it2)) {
					fit.fitness[fit_sConsecSemLab]++;
					fit.fitnessBySect[it1->cormat_id][fit_sConsecSemLab]++;
					if (print == 1) {
						cout << "Soft 4 Prev Sem Next Sem LABS" << it1->lectname << " "
								<< it2->lectname << endl;
					}
				}
			}
		}
	}
}

int Fitness::decode(int cidx) {
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

Fitness::~Fitness() {
	// TODO Auto-generated destructor stub
}

} /* namespace std */
