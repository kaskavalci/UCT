/*
 * FileReader.h
 *
 *  Created on: 21 Eki 2012
 *      Author: HalilCan
 */

#ifndef FILEREADER_H_
#define FILEREADER_H_

#include "Common.h"

namespace std {

class FileReader {
public:
	FileReader(string path_parameter = "Resources/parameters.xml", string path_course = "Resources/course.xml", string path_prereq = "Resources/prereq.xml");
	void read();
	void readreq(string);
	void readcourses(string);
	void readinputparam(string);
private:
	Common *conf;
	string p_parameters, p_courses, p_prereq;
	int init_course(Course, int);
	void assign_meeting_slot();
};

}

#endif /* FILEREADER_H_ */
