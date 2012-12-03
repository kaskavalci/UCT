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

Common::Common() {
	mutg1rate = 0.008;
	mutg3rate = 0.008;
	mutg5rate = 0.008;
	dur = 2000;
	hillsize = 1.00;
	hillrnd = 0.5;
	hillboth = 0.6;
	crrate = 0.9;
	insert_popul_rate = 0.001;
	crowding_dist = 3;
	hc_max_ind = 10;
	verbose_level = 4;
}

Common::~Common() {
	instanceFlag = false;
}

int Common::findcourse(int idx) {
	int retv = -1;
	size_t i;
	int found = 0;
	for (i = 0; i < courses.size(); i++) {
		if (courmat[idx].cname.substr(0, 9) == courses[i].substr(0, 9)) {
			found = 1;
			break;
		}
	}
	if (found == 1)
		retv = i;
	return retv;
}

int Common::findlabcourse(int idx) {
	int retv = -1;
	size_t i;
	int found = 0;
	for (i = 0; i < labcourses.size(); i++) {
		if (courmat[idx].cname.substr(0, 6) == labcourses[i]) {
			found = 1;
			break;
		}
	}
	if (found == 1)
		retv = i;
	return retv;
}

int Common::findlecture(int idx) {
	int retv = -1;
	size_t i;
	int found = 0;
	for (i = 0; i < lectures.size(); i++) {
		if (courmat[idx].cname.substr(0, 6) == lectures[i].lectname) {
			found = 1;
			break;
		}
	}
	if (found == 1)
		retv = i;
	return retv;
}

int Common::findlecturer(int idx) {
	int retv = -1;
	size_t i;
	int found = 0;
	for (i = 0; i < lecturers.size(); i++) {
		if (courmat[idx].lname == lecturers[i]) {
			found = 1;
			break;
		}
	}
	if (found == 1)
		retv = i;
	return retv;
}

} /* namespace std */
