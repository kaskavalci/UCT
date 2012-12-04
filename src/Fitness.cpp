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

void Fitness::calc_hardfit(const list<int>& list, s_hard_fitness_t& fit, int print) {
	fit.total_fit = 0;
	h_confmat(list, fit, print);
	h_sameday(list, fit, print);
	h_midday(list, fit, print);
	for (int i = 0; i < HARD_FIT_N; ++i) {
		fit.total_fit += fit.fitness[i];
	}
}

void Fitness::calc_softfit(const list<int>& list, s_soft_fitness_t& fit, int print) {
	fit.total_fit = 0;
	s_depmet(list, fit, print);
	s_hwlab(list, fit, print);
	s_lecturer(list, fit, print);
	s_LTLconflict(list, fit, print);
	s_ConsecSem(list, fit, print);
	//s_ConsecSemLab(list, fit, print);
	s_lunch(list, fit, print);
	s_eveningLecture(list, fit, print);
	s_morningLab(list, fit, print);
	for (int i = 0; i < SOFT_FIT_N; ++i) {
		fit.total_fit += fit.fitness[i];
	}
}
/*
 * todo: conf'u deðiþtirmeden yap
 */
void Fitness::init_labs(int i, int j) {
	if (conf->lectures[i].lab1day == conf->lectures[j].cid1day
			&& (conf->lectures[i].lab1slot == 1 && (conf->lectures[j].cid1slot == 0 || conf->lectures[j].cid1slot == 1)))
		conf->lectures[i].labs[0] = 1;
	if (conf->lectures[i].lab1day == conf->lectures[j].lab3day
			&& (conf->lectures[i].lab1slot == 1 && (conf->lectures[j].lab3slot == 0 || conf->lectures[j].lab3slot == 1)))
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
			&& (conf->lectures[i].lab2slot == 1 && (conf->lectures[j].cid1slot == 0 || conf->lectures[j].cid1slot == 1)))
		conf->lectures[i].labs[1] = 1;
	if (conf->lectures[i].lab2day == conf->lectures[j].lab3day
			&& (conf->lectures[i].lab2slot == 1 && (conf->lectures[j].lab3slot == 0 || conf->lectures[j].lab3slot == 1)))
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
			&& (conf->lectures[j].lab1slot == 1 && (conf->lectures[i].lab3slot == 0 || conf->lectures[i].lab3slot == 1)))
		conf->lectures[i].labs[2] = 1;
	if (conf->lectures[i].lab3day == conf->lectures[j].lab2day
			&& (conf->lectures[j].lab2slot == 1 && (conf->lectures[i].lab3slot == 0 || conf->lectures[i].lab3slot == 1)))
		conf->lectures[i].labs[2] = 1;
	if (conf->lectures[i].lab3day == conf->lectures[j].cid2day
			&& (conf->lectures[j].cid2slot == 1 && (conf->lectures[i].lab3slot == 0 || conf->lectures[i].lab3slot == 1)))
		conf->lectures[i].labs[2] = 1;
	if (conf->lectures[i].lab3day == conf->lectures[j].cid1day
			&& conf->lectures[i].lab3slot == conf->lectures[j].cid1slot)
		conf->lectures[i].labs[2] = 1;
	if (conf->lectures[i].lab3day == conf->lectures[j].lab3day
			&& conf->lectures[i].lab3slot == conf->lectures[j].lab3slot)
		conf->lectures[i].labs[2] = 1;
}

void Fitness::updatefitness(int print) {
	calc_hardfit(Common::getChrom(), hard_fit, print);
	calc_softfit(Common::getChrom(), soft_fit, print);
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

inline bool Fitness::fit_sconssemlab1(int i, int j) {
	return conf->prereq[i][j] == 0 && conf->lectures[i].lab1 != -1 && conf->lectures[i].lab2 == -1
			&& conf->lectures[i].lab3 == -1 && conf->lectures[i].labs[0] == 1;
}

inline bool Fitness::fit_sconssemlab2(int i, int j) {
	return conf->prereq[i][j] == 0 && conf->lectures[i].lab1 == -1 && conf->lectures[i].lab2 == -1
			&& conf->lectures[i].lab3 != -1 && conf->lectures[i].labs[2] == 1;
}

inline bool Fitness::fit_sconssemlab3(int i, int j) {
	return conf->prereq[i][j] == 0 && conf->lectures[i].lab1 != -1 && conf->lectures[i].lab2 != -1
			&& conf->lectures[i].lab3 == -1 && conf->lectures[i].labs[0] == 1 && conf->lectures[i].labs[1] == 1;
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

inline void Fitness::h_confmat(const list<int>& list, s_hard_fitness_t& fit, int print) {
	std::set<int> visit_set;
	int i;
	fit.fitness[fit_hConfmat] = 0;
	std::list<int>::const_iterator ite = list.end();
	for (std::list<int>::const_iterator it = list.begin(); it != ite; ++it) {
		for (i = 0; i < CHROML; ++i) {
			if (*it == i || visit_set.find(i) != visit_set.end())
				continue;
			if (fit_hconfmat(*it, i)) {
				fit.fitness[fit_hConfmat]++;
				if (print == 1) {
					cout << "Hard 1 Confmat " << conf->courmat[*it].cname << " " << conf->courmat[i].cname << endl;
				}
			}
		}
		visit_set.insert(*it);
	}
}

inline void Fitness::h_sameday(const list<int>& list, s_hard_fitness_t& fit, int print) {
	std::set<int> visit_set;
	int i;
	fit.fitness[fit_hSameDay] = 0;
	std::list<int>::const_iterator ite = list.end();
	for (std::list<int>::const_iterator it = list.begin(); it != ite; ++it) {
		for (i = 0; i < CHROML; ++i) {
			if (*it == i || visit_set.find(i) != visit_set.end())
				continue;
			if (fit_hsameday(*it, i)) {
				fit.fitness[fit_hSameDay]++;
				if (print == 1) {
					cout << "Hard 2 Same Day " << conf->courmat[*it].cname << " " << conf->courmat[i].cname << endl;
				}
			}
		}
		visit_set.insert(*it);
	}
}

inline void Fitness::h_midday(const list<int>& list, s_hard_fitness_t& fit, int print) {
	std::set<int> visit_set;
	int i;
	fit.fitness[fit_hMidHour] = 0;
	std::list<int>::const_iterator ite = list.end();
	for (std::list<int>::const_iterator it = list.begin(); it != ite; ++it) {
		for (i = 0; i < CHROML; ++i) {
			if (*it == i || visit_set.find(i) != visit_set.end())
				continue;
			if (fit_hmidday(*it, i)) {
				fit.fitness[fit_hMidHour]++;
				if (print == 1) {
					cout << "Hard 3 Hours 11-13 " << conf->courmat[*it].cname << " " << conf->courmat[i].cname << endl;
				}
			}
		}
		visit_set.insert(*it);
	}
}

inline void Fitness::s_depmet(const list<int>& list, s_soft_fitness_t& fit, int print) {
	fit.fitness[fit_sDepMeeting] = 0;
	std::list<int>::const_iterator ite = list.end();
	for (std::list<int>::const_iterator it = list.begin(); it != ite; ++it) {
		if (fit_sdepmeet(*it)) {
			fit.fitness[fit_sDepMeeting]++;
			if (print == 1) {
				cout << "Soft 15 Departmental Meeting " << conf->courmat[*it].cname << endl;
			}
		}
	}
}

inline void Fitness::s_hwlab(const list<int>& list, s_soft_fitness_t& fit, int print) {
	fit.fitness[fit_shardlab] = 0;
	std::list<int>::const_iterator ite = list.end();
	std::set<int> visit_set;
	int i;
	for (std::list<int>::const_iterator it = list.begin(); it != ite; ++it) {
		for (i = 0; i < CHROML; ++i) {
			if (*it == i || visit_set.find(i) != visit_set.end())
				continue;
			if (fit_slabconf(*it, i)) {
				fit.fitness[fit_shardlab]++;
				if (print == 1) {
					cout << "Soft 16 Hardware Labs " << conf->courmat[*it].cname << " " << conf->courmat[i].cname << endl;
				}
			}
		}
		visit_set.insert(*it);
	}
}

inline void Fitness::s_lecturer(const list<int>& list, s_soft_fitness_t& fit, int print) {
	int i,n;
	bool lectmatrix[5][10];
	int morlect[5];
	std::list<int>::const_iterator ite = list.end();

	fit.fitness[fit_slectConsWork] = 0;
	fit.fitness[fit_sLectEmptyHour] = 0;
	fit.fitness[fit_sLectOverWork] = 0;
	fit.fitness[fit_sLectNoFreeDay] = 0;
	fit.fitness[fit_sLectMorningCourse] = 0;

	for (size_t h = 0; h < conf->lecturers.size(); h++) {
		for (i = 0; i < 5; i++) {
			morlect[i] = 0;
			for (n = 0; n < 10; n++)
				lectmatrix[i][n] = false;
		}
		for (std::list<int>::const_iterator it = list.begin(); it != ite; ++it) {
			if (conf->lecturers[h] == conf->courmat[*it].lname) {
				if (conf->courmat[*it].hours == 1) {
					switch (chromosome->slot[*it]) {
					case 0:
						lectmatrix[chromosome->day[*it]][2] = true;
						break;
					case 1:
						lectmatrix[chromosome->day[*it]][3] = true;
						break;
					case 2:
						lectmatrix[chromosome->day[*it]][4] = true;
						break;
					case 3:
						lectmatrix[chromosome->day[*it]][9] = true;
						break;
					}
				} else if (conf->courmat[*it].hours == 2) {
					switch (chromosome->slot[*it]) {
					case 0:
						lectmatrix[chromosome->day[*it]][0] = true;
						lectmatrix[chromosome->day[i]][1] = true;
						break;
					case 1:
						lectmatrix[chromosome->day[*it]][2] = true;
						lectmatrix[chromosome->day[*it]][3] = true;
						break;
					case 2:
						lectmatrix[chromosome->day[*it]][5] = true;
						lectmatrix[chromosome->day[*it]][6] = true;
						break;
					case 3:
						lectmatrix[chromosome->day[*it]][7] = true;
						lectmatrix[chromosome->day[*it]][8] = true;
						break;
					}
				}
			}
		}
		int fday, fday2 = 0;
		int strcnt = 0, stpcnt = 0, foundr = 0;
		int dayov;
		for (i = 0; i < 5; i++) {
			fday = 0;
			strcnt = 0;
			stpcnt = 0;
			foundr = 0;
			dayov = 0;
			if (lectmatrix[i][0])
				morlect[i] = 1;
			for (n = 0; n < 10; n++) {
				if (lectmatrix[i][n]) {
					fday = 1;
					dayov++;
				}
				if (n + 3 < 10 && lectmatrix[i][n] && lectmatrix[i][n + 1] && lectmatrix[i][n + 2]
						&& lectmatrix[i][n + 3]) {
					fit.fitness[fit_slectConsWork]++;
					if (print == 1) {
						cout << "Soft 7 Lecturer 4 cons Work Hour " << conf->lecturers[h] << endl;
					}
				}
				if (n + 1 < 10 && lectmatrix[i][n] && !lectmatrix[i][n + 1] && foundr == 0) {
					strcnt = n + 1;
					foundr = 1;
				}
				if (n + 1 < 10 && !lectmatrix[i][n] && lectmatrix[i][n + 1]) {
					stpcnt = n + 1;
				}
			}
			if (stpcnt - strcnt > 3 && foundr != 0) {
				fit.fitness[fit_sLectEmptyHour]++;
				if (print == 1) {
					cout << "Soft 8 Lecturer 4 cons Empty Hour " << conf->lecturers[h] << endl;
				}
			}
			if (dayov > 4) {
				fit.fitness[fit_sLectOverWork]++;
				if (print == 1) {
					cout << "Soft 9 Lecturer More than 4 Hours per day  " << conf->lecturers[h] << endl;
				}
			}
			if (fday == 0)
				fday2 = 1;
		}
		if (fday2 == 0) {
			fit.fitness[fit_sLectNoFreeDay]++;
			if (print == 1) {
				cout << "Soft 10 Lecturer No free Day  " << conf->lecturers[h] << endl;
			}
		}
		int p, nummor = 0;
		for (p = 0; p < 5; p++) {
			nummor = nummor + morlect[p];
		}
		if (nummor > 1) {
			fit.fitness[fit_sLectMorningCourse]++;
			if (print == 1) {
				cout << "Soft 12 Lecturer More than 2 morning courses  " << conf->lecturers[h] << endl;
			}
		}
	}
}

inline void Fitness::s_LTLconflict(const list<int>& list, s_soft_fitness_t& fit, int print) {
	vector<int> cmax(conf->labcourses.size(), -1);
	vector<int> cmin(conf->labcourses.size(), 100);
	vector<int> lab1(conf->labcourses.size(), -1);
	vector<int> lab2(conf->labcourses.size(), -1);
	int decv = -1;
	fit.fitness[fit_sLTLconflict] = 0;
	std::list<int>::const_iterator ite = list.end();
	for (std::list<int>::const_iterator it = list.begin(); it != ite; ++it) {
		if (conf->labid[*it] != -1) {
			decv = decode(*it);
			if (conf->labid[*it] > conf->labcourses.size()) {
				cerr << "SEGFAULT";
			}
			if (decv > cmax[conf->labid[*it]] && conf->courmat[*it].cname.size() > 8 && conf->lab[*it] == 0)
				cmax[conf->labid[*it]] = decv;
			if (decv < cmin[conf->labid[*it]] && conf->courmat[*it].cname.size() > 8 && conf->lab[*it] == 0)
				cmin[conf->labid[*it]] = decv;
			if (lab1[conf->labid[*it]] == -1 && conf->courmat[*it].cname.size() > 8 && conf->lab[*it] == 1)
				lab1[conf->labid[*it]] = decv;
			if (lab1[conf->labid[*it]] != -1 && conf->courmat[*it].cname.size() > 8 && conf->lab[*it] == 1)
				lab2[conf->labid[*it]] = decv;
		}
	}
	for (size_t i = 0; i < conf->labcourses.size(); i++) {
		if ((lab1[i] < cmax[i] && lab1[i] > cmin[i]) || (lab2[i] < cmax[i] && lab2[i] > cmin[i])) {
			fit.fitness[fit_sLTLconflict]++;
			if (print == 1) {
				cout << "Soft 11 L T L conflict  " << conf->labcourses[i] << " " << i << endl;
			}
		}
		if ((lab1[i] < cmin[i]) && (lab2[i] > cmin[i])) {
			fit.fitness[fit_sLTLconflict]++;
			if (print == 1) {
				cout << "Soft 11 L T L conflict  " << conf->labcourses[i] << " " << i << endl;
			}
		}
		if ((lab1[i] > cmax[i]) && (lab2[i] < cmax[i])) {
			fit.fitness[fit_sLTLconflict]++;
			if (print == 1) {
				cout << "Soft 11 L T L conflict  " << conf->labcourses[i] << " " << i << endl;
			}
		}
	}
}

inline void Fitness::s_ConsecSem(const list<int>& list, s_soft_fitness_t& fit, int print) {
	int i;
	std::set<int> visit_set;
	std::list<int>::const_iterator ite = list.end();
	fit.fitness[fit_sConsecSemester] = 0;
	for (std::list<int>::const_iterator it = list.begin(); it !=ite; ++it) {
		for (i = 0; i < CHROML; ++i) {
			if (*it == i || visit_set.find(i) != visit_set.end())
				continue;
			if (fit_sconssem1(*it, i)) {
				fit.fitness[fit_sConsecSemester]++;
				if (print == 1) {
					cout << "Soft 4 Prev Sem Next Sem " << conf->courmat[*it].cname << " " << conf->courmat[i].cname << endl;
				}
			}
			if (fit_sconssem2(*it, i)) {
				fit.fitness[fit_sConsecSemester]++;
				if (print == 1) {
					cout << "Soft 4 Prev Sem Next Sem " << conf->courmat[*it].cname << " " << conf->courmat[i].cname << endl;
				}
			}
			if (fit_sconssem3(*it, i)) {
				fit.fitness[fit_sConsecSemester]++;
				if (print == 1) {
					cout << "Soft 4 Prev Sem Next Sem " << conf->courmat[*it].cname << " " << conf->courmat[i].cname << endl;
				}
			}
		}
		visit_set.insert(*it);
	}
}

inline void Fitness::s_lunch(const list<int>& list, s_soft_fitness_t& fit, int print) {
	bool lunch[8][5][3] = { };
	fit.fitness[fit_sNoLunch] = 0;
	std::list<int>::const_iterator ite = list.end();
	for (std::list<int>::const_iterator it = list.begin(); it !=ite; ++it) {
		//only if it's 1 hour course and in the first 3rd slot
		if (conf->courmat[*it].hours == 1 && chromosome->slot[*it] > 0 && chromosome->slot[*it] < 3) {
			lunch[conf->courmat[*it].semid - 1][chromosome->day[*it]][chromosome->slot[*it]] = true;
		} else if (conf->courmat[*it].hours == 2 && chromosome->slot[*it] == 1) {
			lunch[conf->courmat[*it].semid - 1][chromosome->day[*it]][0] = true;
			lunch[conf->courmat[*it].semid - 1][chromosome->day[*it]][1] = true;
		}
	}
	for (int semester = 0; semester < 8; semester++) {
		for (int day = 0; day < 5; day++) {
			if (lunch[semester][day][0] && lunch[semester][day][1] && lunch[semester][day][2]
					&& fit_signoredlunchconflict(semester, day)) {
				fit.fitness[fit_sNoLunch]++;
				if (print == 1) {
					cout << "Soft 14 No Lunch Hour  conflict " << semester * 8 + day << endl;
				}
			}
		}
	}
}

inline void Fitness::s_eveningLecture(const list<int>& list, s_soft_fitness_t& fit, int print) {
	fit.fitness[fit_sEveningLecture] = 0;
	std::list<int>::const_iterator ite = list.end();
	for (std::list<int>::const_iterator it = list.begin(); it !=ite; ++it) {
		if (conf->courmat[*it].hours == 1 && chromosome->slot[*it] == 3) {
			fit.fitness[fit_sEveningLecture]++;
			if (print == 1) {
				cout << "Soft 5 Evening Lecture " << conf->courmat[*it].cname << endl;
			}
		}
	}
}

inline void Fitness::s_morningLab(const list<int>& list, s_soft_fitness_t& fit, int print) {
	fit.fitness[fit_sMorningLab] = 0;
	std::list<int>::const_iterator ite = list.end();
	for (std::list<int>::const_iterator it = list.begin(); it !=ite; ++it) {
		if (conf->courmat[*it].cname.size() > 8 && conf->lab[*it] == 1 && chromosome->slot[*it] == 0
				&& conf->courmat[*it].hours == 2) {
			fit.fitness[fit_sMorningLab]++;
			if (print == 1) {
				cout << "Soft 6 Mourning Lab " << conf->courmat[*it].cname << endl;
			}
		}
	}
}
//TODO: liste için ayýrma iþlemi
inline void Fitness::s_ConsecSemLab(const list<int>& list, s_soft_fitness_t& fit, int print) {
	int lidx, i;
	fit.fitness[fit_sConsecSemLab] = 0;
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

	for (size_t i = 0; i < conf->lectures.size(); i++) {
			for (size_t j = 0; j < conf->lectures.size(); j++) {
				conf->lectures[i].labs[0] = -1;
				conf->lectures[i].labs[1] = -1;
				conf->lectures[i].labs[2] = -1;
				if (conf->lectures[i].semid == conf->lectures[j].semid + 1
						|| conf->lectures[j].semid == conf->lectures[i].semid + 1) {
					init_labs(i, j);
					if (fit_sconssemlab1(i, j)) {
						fit.fitness[fit_sConsecSemLab]++;
						if (print == 1) {
							cout << "Soft 4 Prev Sem Next Sem LABS" << conf->lectures[i].lectname << " " << conf->lectures[j].lectname << endl;
						}
					}
					if (fit_sconssemlab2(i, j)) {
						fit.fitness[fit_sConsecSemLab]++;
						if (print == 1) {
							cout << "Soft 4 Prev Sem Next Sem LABS" << conf->lectures[i].lectname << " " << conf->lectures[j].lectname << endl;
						}
					}
					if (fit_sconssemlab3(i, j)) {
						fit.fitness[fit_sConsecSemLab]++;
						if (print == 1) {
							cout << "Soft 4 Prev Sem Next Sem LABS" << conf->lectures[i].lectname << " " << conf->lectures[j].lectname << endl;
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
