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
#include <boost/tr1/functional.hpp>
#include "pugixml.hpp"

namespace std {

FileReader::FileReader() {
	conf = Common::getConf();
}
void FileReader::read() {
	readinputparam();
	readcourses();
	readreq();
}

void FileReader::readreq() {
	fstream input("prereq.txt");
	string cour1, cour2;
	int i, j;
	for (i = 0; i < CHROML; i++) {
		for (j = 0; j < CHROML; j++) {
			conf->prereq[i][j] = 0;
		}
	}
	while (!input.eof()) {
		input >> cour1 >> cour2;
		for (i = 0; i < CHROML; i++) {
			for (j = 0; j < CHROML; j++) {
				if (conf->courmat[i].cname.substr(0, 6) == cour1 && conf->courmat[j].cname.substr(0, 6) == cour2)
					conf->prereq[i][j] = 1;
			}
		}
	}
	input.close();
}
void FileReader::readcourses() {
	int i = 0;
	pugi::xml_document doc;
	if (!doc.load_file("courses.xml")) {
		cerr << "courses.xml cannot be found. Please provide it under executable directory" << endl;
	}

	tr1::hash<string> hash;
	//traverse xml and fill courmat array
	for (pugi::xml_node course = doc.child("CourseTable").child("Course"); course;
			course = course.next_sibling("Course"), i++) {
		conf->courmat[i].cname = course.attribute("Name").as_string();
		conf->courmat[i].uniqueID = hash(conf->courmat[i].cname.substr(0, 9));
		conf->courmat[i].lname = course.attribute("Lecturer").as_string();
		conf->courmat[i].semid = course.attribute("Semester").as_int();
		conf->courmat[i].hours = course.attribute("NumHours").as_int();
		if (course.attribute("ConstDay").as_int() == -1) {
			conf->courmat[i].has_constraint = -1;
			conf->courmat[i].c2day = -1;
		} else {
			conf->courmat[i].has_constraint = 1;
			conf->courmat[i].c2day = course.attribute("ConstDay").as_int();
		}
		if (course.attribute("ConstSlot").as_int() == -1) {
			conf->courmat[i].c2slot = -1;
		} else {
			conf->courmat[i].c2slot = course.attribute("ConstSlot").as_int();
		}
		//TODO: PLEASE PLEASE PLEASE
		if (conf->courmat[i].cname.at(7) == 'L') {
			conf->courmat[i].isLab = true;
		}
		else {
			conf->courmat[i].isLab = false;
		}
		//add lecturer to list and update courses lecturer ID
		conf->courmat[i].lecturerID = conf->add_lecturer(conf->courmat[i].lname, i);
		//add lab sessions
		if (conf->courmat[i].cname.size() > 8 && conf->courmat[i].cname.at(7) == 'L') {
			conf->labid[i] = conf->add_labsession(labSession_t(conf->courmat[i].cname.substr(0, 6), i));
		}
		//add CSE lectures
		if (conf->findlecture(i) == -1 && conf->courmat[i].cname.substr(0, 3) == "cse") {
			conf->add_lecture(conf->courmat[i].cname.substr(0, 6), hash(conf->courmat[i].cname.substr(0, 6)), i);
		}
		//add lab info
		if (conf->courmat[i].cname.at(7) == 'L') {
			conf->lab[i] = 1;
		}
		if (conf->courmat[i].cname.substr(0, 3) == "cse")
			conf->cse[i] = 1;
	}
}

void FileReader::readinputparam() {
	fstream input("inputparam.txt");
	string inpname;
	float inpval;
	string line;
	stringstream ss(stringstream::in | stringstream::out);
	while (getline(input, line)) {
		if (line.empty() || line.at(0) == '#')
			continue;
		ss << line;
		ss >> inpname >> inpval;
		if (!inpname.compare("dur"))
			conf->dur = inpval;
		else if (!inpname.compare("hillsize"))
			conf->hillsize = inpval;
		else if (!inpname.compare("hillrnd"))
			conf->hillrnd = inpval;
		else if (!inpname.compare("hillboth"))
			conf->hillboth = inpval;
		else if (!inpname.compare("crrate"))
			conf->crrate = inpval;
		else if (!inpname.compare("mutg1rate"))
			conf->mutg1rate = inpval;
		else if (!inpname.compare("mutg3rate"))
			conf->mutg3rate = inpval;
		else if (!inpname.compare("mutg5rate"))
			conf->mutg5rate = inpval;
		else if (!inpname.compare("randomly_insert_to_population"))
			conf->insert_popul_rate = inpval;
		else if (!inpname.compare("crowding_dist"))
			conf->crowding_dist = inpval;
		else if (!inpname.compare("hc_max_ind"))
			conf->hc_max_ind = inpval;
		else if (!inpname.compare("verbose_level"))
			conf->verbose_level = inpval;
		else if (!inpname.compare("hard_fitness_group")) {
			vector<int> hardgr;
			while (inpval != -1) {
				hardgr.push_back(inpval);
				ss >> inpval;
			}
			conf->hardgroup.push_back(hardgr);
		} else if (!inpname.compare("soft_fitness_group")) {
			vector<int> softgr;
			while (inpval != -1) {
				softgr.push_back(inpval);
				ss >> inpval;
			}
			conf->softgroup.push_back(softgr);
		}
		ss.clear();
	}
	input.close();
}

} /* namespace std */
