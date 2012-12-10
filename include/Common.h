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
#include <list>
#include "Course.h"
#include "Lecture.h"
#include "Constants.h"
#include "Lecturer.h"

namespace std {

class Common {
public:
	static Common *getConf();
	static const list<int>& getChrom();
	virtual ~Common();
	int findlabcourse(int idx);
	int findlecture(int idx);
	int add_lecturer(string, int);
	int add_labsession(labSession_t);
	int add_lecture(string, int, int);

	int confmat[CHROML][CHROML];
	int prereq[CHROML][CHROML];

	Course courmat[CHROML];
	vector<string> courses;
	vector<Lecturer> lecturers;
	vector<labSession_t> labcourses;
	vector<Lecture> lectures;
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
	static list<int> chrom;
	void initlab(Lecture*, int);
	Common();

};

} /* namespace std */
#endif /* COMMON_H_ */
