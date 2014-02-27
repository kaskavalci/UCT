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
	int cons_slot;
	bool has_cons;
	bool isLab;
	bool split;
	bool isCSE;
};

} /* namespace std */
#endif /* COURSE_H_ */
