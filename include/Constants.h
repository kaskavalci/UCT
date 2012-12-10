/*
 * Constants.h
 *
 *  Created on: 06 Kas 2012
 *      Author: HalilCan
 */

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include <string>

#define PERIODS	4
#define CHROML 125
#define POPUL 50
#define PARETO_SIZE	20
#define NCOL 20
#define RND(NUM)	rand() % NUM
#define TOURNAMENT_POOLSIZE	5
#define TOURNAMENT_CANDIDATE_N	2

enum {D_TRUE, D_FALSE, D_NO_DOMINATION, D_NO_HARDDOMINATION};

/*
 * last elements in enum indicates the SIZE. if you are to add new fitness, make sure capital letter SOFT_FIT and HARD_FIT
 * is at the end.
 */
enum {fit_sDepMeeting, fit_shardlab, fit_slectConsWork, fit_sLectOverWork,
fit_sLectNoFreeDay, fit_sLectMorningCourse, fit_sLTLconflict, fit_sConsecSemester, fit_sConsecSemLab, fit_sNoLunch,
fit_sEveningLecture, fit_sMorningLab, SOFT_FIT_N};
enum {fit_hConfmat, fit_hSameDay, fit_hMidHour, HARD_FIT_N};
//countable fitness enumerator. because some soft fitness values are not related to one gene.
//multiple genes together produces the fitness thus we cannot separate them. again, last element is SIZE
enum {hc_hard, hc_soft, hc_both};

typedef struct {
	int fitness[HARD_FIT_N];
	unsigned short int fitnessBySect[CHROML][HARD_FIT_N];
	int total_fit;
} s_hard_fitness_t;

typedef struct {
	int fitness[SOFT_FIT_N];
	unsigned short int fitnessBySect[CHROML][SOFT_FIT_N];
	int total_fit;
} s_soft_fitness_t;

struct labSession_t {
	std::string course_name;
	int idx;
	labSession_t (std::string c, int id) : course_name(c), idx(id) {}
};

#endif /* CONSTANTS_H_ */
