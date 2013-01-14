/*
 * Course.h
 *
 *  Created on: 21 Eki 2012
 *      Author: HalilCan
 */

#ifndef COURSE_H_
#define COURSE_H_

#include <string>

namespace std {

class Course {
public:
	Course();
	string cname;
	string lname;
	int uniqueID;
	int lecturerID;
	int semid;
	int hours;
	int section;
	int has_constraint, c2day, c2slot;
	bool isLab;
};

} /* namespace std */
#endif /* COURSE_H_ */
