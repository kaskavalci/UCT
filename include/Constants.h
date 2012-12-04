/*
 * Constants.h
 *
 *  Created on: 06 Kas 2012
 *      Author: HalilCan
 */

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#define PERIODS	4
#define CHROML 125
#define POPUL 50
#define PARETO_SIZE	20
#define NCOL 20
#define NUMX 20
#define NMUT 1
#define RND(NUM)	rand() % NUM
#define HARD_FIT_N	3
#define SOFT_FIT_N	13
#define TOURNAMENT_POOLSIZE	5
#define TOURNAMENT_CANDIDATE_N	2

enum {D_TRUE, D_FALSE, D_NO_DOMINATION, D_NO_HARDDOMINATION};
enum {fit_sDepMeeting, fit_shardlab, fit_slectConsWork, fit_sLectEmptyHour, fit_sLectOverWork,
fit_sLectNoFreeDay, fit_sLectMorningCourse, fit_sLTLconflict, fit_sConsecSemester, fit_sConsecSemLab, fit_sNoLunch,
fit_sEveningLecture, fit_sMorningLab};
enum {fit_hConfmat, fit_hSameDay, fit_hMidHour};
enum {hc_hard, hc_soft, hc_both};

typedef struct {
	int fitness[HARD_FIT_N];
	int total_fit;
} s_hard_fitness_t;

typedef struct {
	int fitness[SOFT_FIT_N];
	int total_fit;
} s_soft_fitness_t;


#endif /* CONSTANTS_H_ */
