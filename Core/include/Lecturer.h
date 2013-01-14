/*
 * Lecturer.h
 *
 *  Created on: 09 Ara 2012
 *      Author: HalilCan
 */

#ifndef LECTURER_H_
#define LECTURER_H_
#include <string>
#include <vector>
#include "Course.h"

namespace std {

class Lecturer {
public:
	Lecturer(string name, int);

	string name;
	vector<int> myCourses;
};

} /* namespace std */
#endif /* LECTURER_H_ */
