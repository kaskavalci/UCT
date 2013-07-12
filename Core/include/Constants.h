/*
 * Constants.h
 *
 *  Created on: 06 Kas 2012
 *      Author: HalilCan
 */

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include <string>
#include <cstdint>
#include <array>
#include <vector>
#include <stdlib.h>

#define PERIODS	4
#define NCOL 20
#define RND(NUM)	rand() % NUM

enum {D_TRUE, D_FALSE, D_NO_DOMINATION};

/*
 * last elements in enum indicates the SIZE. if you are to add new fitness, make sure capital letter SOFT_FIT and TOT_FIT_N
 * is at the end.
 */
enum e_HardFitness {fit_hConfmat, fit_hSameDay, fit_hMidHour, HARD_FIT_N};
enum e_SoftFitness {fit_sDepMeeting = HARD_FIT_N, fit_shardlab, fit_slectConsWork, fit_sLectOverWork,
fit_sLectNoFreeDay, fit_sLectMorningCourse, fit_sLTLconflict, fit_sConsecSemester, fit_sConsecSemLab, fit_sNoLunch,
fit_sEveningLecture, fit_sMorningLab, TOT_FIT_N};

/**
 * m_FitnessMeta holds meta-data for fitness structure. mGeneID is ID of gene. At total we have CHROML
 * ids. This is needed because we need ID while sorting and we lose that information on sort.
 * m_GeneTotal is total fitness value caused by that particular gene. It combines both hard and soft fitness.
 */
enum e_FitnessMeta { fit_mGeneID, fit_mHard, fit_mSoft, fit_mGeneTotal, FIT_SECT_L };
#define FitnessRow	std::array<uint8_t, FIT_SECT_L>
#define FitnessTable	std::vector< FitnessRow >

//countable fitness enumerator. because some soft fitness values are not related to one gene.
//multiple genes together produces the fitness thus we cannot separate them. again, last element is SIZE
enum {hc_hard, hc_soft, hc_both};

enum {CrossS, CrossF, PopS, PopF, HCS, HCF, ParS, ParF, MutS, MutF, STAT_LEN};

enum { c_duration, c_HCRate, c_MutationRate, c_CRRate, c_InsertRate, c_CrowdingSize, c_seed, c_iteration, c_LENGTH };

/**
 * fitness_t, has boost::array type and implements copy constructor for the array
 */
struct fitness_t {
	uint8_t fitness[TOT_FIT_N];
	FitnessTable fitnessBySect;
	uint16_t soft_fit, hard_fit, total_fit;
	int size;
	fitness_t (int size) {
		this->size = size;
		fitnessBySect.assign(size, FitnessRow());
		hard_fit = soft_fit = total_fit = 0;
	}
	fitness_t &operator=(const fitness_t &fit) {
		std::copy(fit.fitness, fit.fitness + TOT_FIT_N, this->fitness);
		for (int i = 0; i < size; ++i) {
			std::copy(fit.fitnessBySect[i].begin(), fit.fitnessBySect[i].end(), this->fitnessBySect[i].begin());
		}
		this->total_fit = fit.total_fit;
		this->hard_fit = fit.hard_fit;
		this->soft_fit = fit.soft_fit;
		return *this;
	}
};

struct domination_t {
	int8_t fit[TOT_FIT_N];
	uint8_t type;
};

struct labSession_t {
	std::string course_name;
	int idx;
	labSession_t (std::string c, int id) : course_name(c), idx(id) {}
};

//parameters.xml node names;
#define PARAM_DUR "Duration"
#define PARAM_HCSIZE  "HCSize"
#define PARAM_HCRATE  "HCRate"
#define PARAM_MUTRATE "MutationRate"
#define PARAM_CRRATE  "CrossoverRate"
#define PARAM_RNDRATE "RandomInsertRate"
#define PARAM_CROWDINGDIST	"CrowdingDistance"
#define PARAM_HCGENECOUNT "HCGeneCount"
#define PARAM_POPULATION	"Population"
#define PARAM_PARETO	"ParetoFront"
#define PARAM_SELECTIONPOOLSIZE	"SelectionPoolSize"
#define PARAM_SELECTIONCANDIDATESIZE  "SelectionCandidatePoolSize"
#define PARAM_GROUPS  "Groups"
#define PARAM_SLOTS  "AvailableSlots"

#endif /* CONSTANTS_H_ */
