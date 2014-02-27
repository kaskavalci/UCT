/*
 * FitnessExp.cpp
 *
 *  Created on: 17 Tem 2013
 *      Author: HalilCan
 */

#include "FitnessExp.h"

#include <Chromosome.h>
#include <Common.h>
#include <Constants.h>
#include <Course.h>
#include <IFitness.h>
#include <Lecturer.h>
#include <algorithm>
#include <array>
#include <cstddef>
#include <new>
#include <vector>

#define INC_HARD_2_ARGS(column, i, j)	fit.fitness[column]++; \
				fit.hard_fit++; \
				if (!conf->courmat[i].has_cons) { INC_FITBYSECT(i, fit_mHard); } \
				if (!conf->courmat[j].has_cons) { INC_FITBYSECT(j, fit_mHard); }

#define INC_HARD_1_ARGS(column, i)	fit.fitness[column]++; \
				fit.hard_fit++; \
				if (!conf->courmat[i].has_cons) { INC_FITBYSECT(i, fit_mHard); }

#define INC_SOFT_2_ARGS(column, i, j) fit.fitness[column]++; \
				fit.soft_fit++; \
				if (!conf->courmat[i].has_cons) { INC_FITBYSECT(i, fit_mSoft); } \
				if (!conf->courmat[j].has_cons) { INC_FITBYSECT(j, fit_mSoft); }

#define INC_SOFT_1_ARGS(column, i) fit.fitness[column]++; \
				fit.soft_fit++; \
				if (!conf->courmat[i].has_cons) { INC_FITBYSECT(i, fit_mSoft); }

#define INC_FITBYSECT(i, fit_type) fit.fitnessBySect[i][fit_type]++; \
				fit.fitnessBySect[i][fit_mGeneTotal]++;

#define IS_SAME_SEMESTER(i, j)	conf->courmat[i].semid == conf->courmat[j].semid
#define IS_SAME_LECTURER(i, j)	conf->courmat[i].lecturerID == conf->courmat[j].lecturerID
#define IS_SAME_SLOT(i, j)	chromosome->get_slot(i) == chromosome->get_slot(j)

namespace std {

FitnessExp::FitnessExp(const Chromosome* chrom) {
	this->chromosome = chrom;
	conf = Common::getConf();
	fitness = new fitness_t(conf->ChromSize);
	fill(fitness->fitness, fitness->fitness + TOT_FIT_N, 0);
}

FitnessExp::FitnessExp(const IFitness* source) {
	conf = Common::getConf();
	fitness = new fitness_t(conf->ChromSize);
	chromosome = NULL;
	*fitness = source->getFit();
}

FitnessExp::~FitnessExp() {
	delete fitness;
}

void FitnessExp::calcFit(fitness_t& fit, int type, int print) {
	fit.hard_fit = 0;
	fit.soft_fit = 0;
	//init chrom IDs in fitnessBySect struct. it is to be used while sorting
	for (int i = 0; i < conf->ChromSize; ++i) {
		fit.fitnessBySect[i].fill(0);
		fit.fitnessBySect[i][fit_mGeneID] = i;
	}
	fill(fit.fitness, fit.fitness + TOT_FIT_N, 0);

	//hard fitness calculation
	if (type == hc_hard || type == hc_both) {
		course_conflict(fit);
	}
	//soft fitness calculation
	if (type == hc_soft || type == hc_both) {

	}
	fit.total_fit = fit.hard_fit + fit.soft_fit;
}

/*
 * && ((
 && conf->courmat[i].hours == conf->courmat[j].hours)
 || (conf->courmat[i].hours != conf->courmat[j].hours
 && chromosome->get_day(i) == chromosome->get_day(j)
 && ((chromosome->get_period(i) == 3 && chromosome->get_period(j) == 3)
 || (conf->courmat[i].hours == 1 && conf->courmat[j].hours == 2
 && (chromosome->get_period(i) == 0 || chromosome->get_period(i) == 1)
 && chromosome->get_period(j) == 1)
 || (conf->courmat[i].hours == 2 && conf->courmat[j].hours == 1
 && (chromosome->get_period(j) == 0 || chromosome->get_period(j) == 1)
 && chromosome->get_period(i) == 1)))))
 *  */

/**
 * it checks for courses in the same semester not to be overlapped and courses by the same lecturer
 * not to be overlapped
 * @param fit fitness to be changed
 */
void std::FitnessExp::course_conflict(fitness_t& fit) {
	int i, j;
	for (i = 0; i < conf->ChromSize; ++i) {
		for (j = i + 1; j < conf->ChromSize; ++j) {
			if ((IS_SAME_SEMESTER(i, j) || IS_SAME_LECTURER(i, j)) && do_overlap(i, j)) {
				INC_HARD_2_ARGS(fit_hConfmat, i, j)
			}
		}
	}
}


void FitnessExp::lecturer_checks(fitness_t& fit) {

	for (size_t l = 0; l < conf->lecturers.size(); l++) {
		vector<int> *mycourses = &(conf->lecturers[l].myCourses);

	}
}

/**
 * It checks for consecutive semester conflict (CSC). CSCs are when two cse courses in
 * consecutive semesters overlapped. The reason we don't want this is that students can choose
 * a course from next or previous semester of their current curriculum.
 * Condition check is the following:
 * 		there should be no prerequisites between two courses. if there is, then conflict will be ignored.
 *
 * @param fit
 */
void FitnessExp::consecutive_semester_conflict(fitness_t& fit) {
	size_t size = conf->cse.size();
	auto it_end = conf->cse.end();
	for (auto it_i = conf->cse.begin(); it_i != it_end; it_i++) {
		for (auto it_j = conf->cse.begin(); it_j != it_end; it_j++) {
			//don't check itself. no need to increase fitness for the same course right?
			if (*it_i == *it_j) {
				continue;
			}
			if (conf->prereq[*it_i][*it_j] == 0 && do_overlap(*it_i, *it_j)) {
				INC_SOFT_2_ARGS(fit_sConsecSemester, *it_i, *it_j)
			}
		}
	}
}

/**
 * It checks if given two courses in courmat overlaps.
 * @param i
 * @param j
 * @return
 */
bool FitnessExp::do_overlap(int i, int j) {
	//todo write this function
	return true;
}

} /* namespace std */
