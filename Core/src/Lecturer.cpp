/*
 * Lecturer.cpp
 *
 *  Created on: 09 Ara 2012
 *      Author: HalilCan
 */

#include "Lecturer.h"

namespace std {

Lecturer::Lecturer(string name, int idx) {
	this->name = name;
	myCourses.push_back(idx);
}


} /* namespace std */
