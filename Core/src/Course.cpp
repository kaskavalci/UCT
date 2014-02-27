/*
 * Course.cpp
 *
 *  Created on: 21 Eki 2012
 *      Author: HalilCan
 */

#include "Course.h"

namespace std {

} /* namespace std */

std::Course::Course() {
	cname = "Uninitilized course";
	lname = "Uninitilized course";
	uniqueID = lecturerID = semid = hours = -1;
	cons_slot = -1;
	section = -1;
	has_cons = false;
	isLab = false;
	split = false;
	isCSE = false;
}
