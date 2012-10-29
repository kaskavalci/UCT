/*
 * Common.h
 *
 *  Created on: 21 Eki 2012
 *      Author: HalilCan
 */

#ifndef COMMON_H_
#define COMMON_H_

#include <vector>
#include <string>
#include "Course.h"
#include "Lecture.h"

#define CHROML 125
#define POPUL 20
#define NCOL 20
#define NUMX 20
#define NMUT 1
#define RND(NUM)	rand() % NUM


namespace std {

class Common {
public:
	Common();
	int findcourse(int idx);
	int findlabcourse(int idx);
	int findlecture(int idx);
	int findlecturer(int idx);


	int confmat[CHROML][CHROML];
	int prereq[CHROML][CHROML];

	Course courmat[CHROML];
	vector<string> courses;
	vector<string> lecturers;
	vector<string> labcourses;
	vector<Lecture> lectures;
	int cid[CHROML];
	int lectid[CHROML];
	int labid[CHROML];
	int lab[CHROML];
	int cse[CHROML];

	int dur;
	float hillsize;
	float hillboth;
	float hillrnd;
	int hciter;
	float crrate;
	float mutg1rate;
	float mutg3rate;
	float mutg5rate;

};

} /* namespace std */
#endif /* COMMON_H_ */
