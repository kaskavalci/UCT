/*
 * Common.cpp
 *
 *  Created on: 21 Eki 2012
 *      Author: HalilCan
 */

#include "Common.h"
#include <iostream>
#include <fstream>

namespace std {

//singleton pattern
bool Common::instanceFlag = false;
Common * Common::conf = NULL;

Common* Common::getConf() {
	if (!instanceFlag) {
		conf = new Common();
		instanceFlag = true;
		return conf;
	}
	else {
		return conf;
	}
}

/*
 * adds a lecturer if not in list, otherwise update it with new course.
 */
int Common::add_lecturer(string name, int idx) {
	for (vector<Lecturer>::iterator it = lecturers.begin(); it != lecturers.end(); ++it) {
		if (it->name == name) {
			it->myCourses.push_back(idx);
			return it - lecturers.begin();
		}
	}
	lecturers.push_back(Lecturer(name, idx));
	return lecturers.size() - 1;
}

int Common::add_labsession(labSession_t lab) {
	for (vector<labSession_t>::iterator it = labcourses.begin(); it != labcourses.end(); ++it) {
		if (it->idx == lab.idx) return (it - labcourses.begin());
	}
	labcourses.push_back(lab);
	return labcourses.size() - 1;
}
/*
 * search for lectureID in lectures vector. if found, insert it to its courses.
 * courses is std::set, so only unique elements will be there.
 * lectid is hash output of lectname.
 */
int Common::add_lecture(string name, int lectid, int idx) {
	for (vector<Lecture>::iterator it = lectures.begin(); it != lectures.end(); ++it) {
		if (it->lectid == lectid) {
			it->courses.insert(idx);
			initlab(&*it, idx);
			return it - lectures.begin();
		}
	}
	Lecture lect(name, lectid);
	initlab(&lect, idx);
	lect.courses.insert(idx);
	lectures.push_back(lect);
	return lectures.size() - 1;
}

void Common::initlab(Lecture* lect, int idx) {
	if (courmat[idx].isLab && courmat[idx].section == 1 && courmat[idx].hours == 2) {
		lect->lab[lect_lab1][lect_idx] = idx;
	}
	if (courmat[idx].isLab && courmat[idx].section == 2 && courmat[idx].hours == 2) {
		lect->lab[lect_lab2][lect_idx] = idx;
	}
	if (courmat[idx].isLab && courmat[idx].hours == 1) {
		lect->lab[lect_lab3][lect_idx] = idx;
	}
}

Common::Common() {
	mutrate = 1;
	duration = 2000;
	hillsize = 1.00;
	hcrate = 0.6;
	crrate = 0.9;
	rnd_insert_rate = 0.001;
	crowding_dist = 3;
	hc_gene_count = 10;
	ChromSize = -1;
	pop_size = 0;
	pareto_size = 0;
}

Common::~Common() {
	instanceFlag = false;
	prereq.clear();
	courmat.clear();
	courses.clear();
	lecturers.clear();
	lectures.clear();
	labcourses.clear();
	labid.clear();
	lab.clear();
	cse.clear();
	for (auto it = groups.begin(); it != groups.end(); ++it) {
	  it->clear();
	}
}

/*
 * returns "the position" in labcourses vector, if idx is found.
 * returns -1 otherwise.
 */
int Common::findlabcourse(int idx) {
	for (vector<labSession_t>::const_iterator it = labcourses.begin(); it != labcourses.end(); ++it) {
		if (it->idx == idx) return (it - labcourses.begin());
	}
	return -1;
}

int Common::findlecture(int idx) {
	for (vector<Lecture>::const_iterator it = lectures.begin(); it != lectures.end(); ++it) {
		if (it->courses.find(idx) != it->courses.end()) {
			//item found!
			return (it - lectures.begin());
		}
	}
	return -1;
}

} /* namespace std */
