/*
 * Lecture.h
 *
 *  Created on: 21 Eki 2012
 *      Author: HalilCan
 */

#ifndef LECTURE_H_
#define LECTURE_H_

#include <string>
#include "Course.h"
#include <set>

enum {
	lect_lab1,lect_lab2, lect_lab3, lect_LABNUM
};

enum {lect_idx, lect_labday, lect_labslot, lect_LABATTR};

namespace std {

class Lecture {
public:
	Lecture(string,int);
	string lectname;
	int lectid;
	set<int> courses;
	int cid1;
	int cid2;
	int lab[lect_LABNUM][lect_LABATTR];

	int cid1day;
	int cid1slot;
	int cid2day;
	int cid2slot;
	int cid3day;
	int cid4day;
	int cid3slot;
	int cid4slot;

};

} /* namespace std */
#endif /* LECTURE_H_ */
