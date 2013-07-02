/*
 * FileReader.cpp
 *
 *  Created on: 21 Eki 2012
 *      Author: HalilCan
 */

#include "FileReader.h"
#include "Common.h"
#include "Lecture.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <functional>
#include "pugixml.hpp"
#include "boost/lexical_cast.hpp"

namespace std {

FileReader::FileReader(string path_parameter, string path_course, string path_prereq) {
	conf = Common::getConf();
	p_parameters = path_parameter;
	p_courses = path_course;
	p_prereq = path_prereq;
}

void FileReader::read() {
	readinputparam(p_parameters);
	readcourses(p_courses);
	conf->ChromSize = conf->courmat.size();
	readreq(p_prereq);
}

void FileReader::readreq(string filename) {
	pugi::xml_document doc;
	if (!doc.load_file(filename.c_str())) {
		cerr << filename << " cannot be found. Please provide it under Resources directory" << endl;
		exit(EXIT_FAILURE);
	}
	string cour1, cour2;
	int i, j;
	if (conf->ChromSize == -1) {
		cerr << "ChromSize is not initialized!";
		exit(EXIT_FAILURE);
	}
	for (i = 0; i < conf->ChromSize; i++) {
		conf->prereq.push_back(vector<int>(conf->ChromSize, 0));
	}
	for (pugi::xml_node course = doc.child("Courses").child("Course"); course; course = course.next_sibling("Course")) {
		cour1 = course.attribute("Course1").as_string();
		cour2 = course.attribute("Course2").as_string();
		for (i = 0; i < conf->ChromSize; i++) {
			for (j = 0; j < conf->ChromSize; j++) {
				if (conf->courmat[i].cname == cour1 && conf->courmat[j].cname == cour2)
					conf->prereq[i][j] = 1;
			}
		}
	}
}
/**
 * Reads Resources\courses.xml and initiate courmat, cse, lab, labid arrays.
 */
void FileReader::readcourses(string filename) {
	int i = 0, labStatus;
	pugi::xml_document doc;
	if (!doc.load_file(filename.c_str())) {
		cerr << filename << " cannot be found. Please provide it under Resources directory" << endl;
		exit(EXIT_FAILURE);
	}

	hash<string> hash;
	//traverse xml and fill courmat array
	for (pugi::xml_node course = doc.child("Courses").child("Course"); course; course = course.next_sibling("Course")) {
		Course newCourse;
		newCourse.cname = course.attribute("Name").as_string();
		newCourse.lname = course.attribute("Lecturer").as_string();
		newCourse.semid = course.attribute("Semester").as_int();
		newCourse.hours = course.attribute("NumHours").as_int();
		newCourse.isLab = course.attribute("isLab").as_bool();
		newCourse.section = course.attribute("Section").as_int();

		labStatus = newCourse.isLab ? 1 : 0;
		newCourse.uniqueID = hash(
				newCourse.cname + boost::lexical_cast<string>(newCourse.section)
						+ boost::lexical_cast<string>(labStatus));

		if (course.attribute("ConstDay").as_int() == -1) {
			newCourse.has_constraint = -1;
			newCourse.c2day = -1;
		} else {
			newCourse.has_constraint = 1;
			newCourse.c2day = course.attribute("ConstDay").as_int();
		}
		if (course.attribute("ConstSlot").as_int() == -1) {
			newCourse.c2slot = -1;
		} else {
			newCourse.c2slot = course.attribute("ConstSlot").as_int();
		}

		i = init_course(newCourse, i);

	}
}

void FileReader::readinputparam(string filename) {
	pugi::xml_document doc;
	if (!doc.load_file(filename.c_str())) {
		cerr << filename << " cannot be found. Please provide it under Resources directory" << endl;
		exit(EXIT_FAILURE);
	}

	string inpname;
	double inpval;
	for (pugi::xml_node course = doc.child("Parameters").child("Parameter"); course;
			course = course.next_sibling("Parameter")) {
		inpname = course.attribute("Name").as_string();
		inpval = course.attribute("Val").as_double();
		if (!inpname.compare(PARAM_DUR))
			conf->duration = (time_t) inpval;
		else if (!inpname.compare(PARAM_HCSIZE))
			conf->hillsize = inpval;
		else if (!inpname.compare(PARAM_HCRATE))
			conf->hcrate = inpval;
		else if (!inpname.compare(PARAM_CRRATE))
			conf->crrate = inpval;
		else if (!inpname.compare(PARAM_MUTRATE))
			conf->mutrate = inpval;
		else if (!inpname.compare(PARAM_RNDRATE))
			conf->rnd_insert_rate = inpval;
		else if (!inpname.compare(PARAM_CROWDINGDIST))
			conf->crowding_dist = (int) inpval;
		else if (!inpname.compare(PARAM_HCGENECOUNT))
			conf->hc_gene_count = (int) inpval;
		else if (!inpname.compare(PARAM_POPULATION)) {
			conf->pop_size = (int) inpval;
		} else if (!inpname.compare(PARAM_PARETO)) {
			conf->pareto_size = (int) inpval;
		} else if (!inpname.compare(PARAM_SELECTIONPOOLSIZE)) {
			conf->sel_poolsize = (int) inpval;
		} else if (!inpname.compare(PARAM_SELECTIONCANDIDATESIZE)) {
			conf->sel_candidatesize = (int) inpval;
		} else if (!inpname.compare(PARAM_GROUPS)) {
			//iterate all groups in parameter
			for (pugi::xml_node group = course.child("Group"); group; group = group.next_sibling("Group")) {
				vector<int> groupList;
				//iterate all constraints in group
				for (pugi::xml_node constraint = group.child("Constraint"); constraint;
						constraint = constraint.next_sibling("Constraint")) {
					groupList.push_back(constraint.attribute("ID").as_int());
				}
				conf->groups.push_back(groupList);
			}
		}
	}
}

int FileReader::init_course(Course newCourse, int i) {
	//split the course
	bool toSplit = false;
	if (newCourse.hours == 3) {
		toSplit = true;
		newCourse.hours = 2;
	}
	//add lab info
	if (newCourse.isLab) {
		conf->lab.push_back(1);
		conf->labid.push_back(conf->add_labsession(labSession_t(newCourse.cname, i)));
	} else {
		conf->lab.push_back(-1);
		conf->labid.push_back(-1);
	}
	//add lecturer to list and update courses lecturer ID. only add cse courses
	if (newCourse.cname.substr(0, 3) == "cse") {
		newCourse.lecturerID = conf->add_lecturer(newCourse.lname, i);
	} else {
		//here we assume faculty courses does not have valid lecturer name and they don't need to be in lecturer list.
		//for solid code we should assign them a lecturerID. uniquenes can be achieved by starting lecturersID by 1000.
		//we don't expect lecturers more than 1000.
		newCourse.lecturerID = i + 1000;
	}
	//deneme
	//push course to vector
	conf->courmat.push_back(newCourse);

	//add CSE lectures
	if (conf->findlecture(i) == -1 && newCourse.cname.substr(0, 3) == "cse") {
		conf->add_lecture(newCourse.cname, std::hash<std::string>()(newCourse.cname), i);
	}

	if (newCourse.cname.substr(0, 3) == "cse")
		conf->cse.push_back(1);
	else {
		conf->cse.push_back(-1);
	}

	if (toSplit) {
		newCourse.hours = 1;
		return init_course(newCourse, i + 1);
	}
	return i + 1;
}

} /* namespace std */
