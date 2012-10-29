/*
 * Lecture.h
 *
 *  Created on: 21 Eki 2012
 *      Author: HalilCan
 */

#ifndef LECTURE_H_
#define LECTURE_H_

#include <string>

namespace std {

class Lecture {
public:
	string lectname;
	string lectname2;
	int semid;
	int cid1;
	int cid2;
	int cid3;
	int cid4;
	int lab1;
	int lab2;
	int lab3;
	int cid1day;
	int cid2day;
	int cid1slot;
	int cid2slot;
	int cid3day;
	int cid4day;
	int cid3slot;
	int cid4slot;
	int lab1day;
	int lab2day;
	int lab3day;
	int lab1slot;
	int lab2slot;
	int lab3slot;
	int haslabs;
	int labs[3];
};

} /* namespace std */
#endif /* LECTURE_H_ */
