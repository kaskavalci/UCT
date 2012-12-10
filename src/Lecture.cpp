/*
 * Lecture.cpp
 *
 *  Created on: 21 Eki 2012
 *      Author: HalilCan
 */

#include "Lecture.h"

namespace std {

} /* namespace std */

std::Lecture::Lecture(string name, int id) {
	lectname = name;
	lectid = id;
	cid1 = -1;
	cid2 = -1;
	fill_n(&lab[0][0], lect_LABNUM * lect_LABATTR, -1);
	cid1day = -1;
	cid1slot = -1;
	cid2day = -1;
	cid2slot = -1;
	cid3day = -1;
	cid4day = -1;
	cid3slot = -1;
	cid4slot = -1;
}
