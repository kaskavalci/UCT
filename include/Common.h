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
#include "Constants.h"

namespace std {

class Common {
public:
	static Common *getConf();
	virtual ~Common();
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
	float crrate;
	float mutg1rate;
	float mutg3rate;
	float mutg5rate;
	float insert_popul_rate;
	int crowding_dist;
	int hc_max_ind;
	int verbose_level;
	vector<vector<int> > hardgroup, softgroup;
private:
	static bool instanceFlag;
	static Common *conf;
	Common();

};

} /* namespace std */
#endif /* COMMON_H_ */
