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
	string cname;
	string lname;
	int semid;
	int hours;
	int c2, c2day, c2slot;
	Course*nextc;
};

} /* namespace std */
#endif /* COURSE_H_ */
