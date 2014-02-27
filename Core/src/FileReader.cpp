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
	assign_meeting_slot();
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
	//for each semester
	for (pugi::xml_node semester = doc.child("Semesters").child("Semester"); semester;
			semester = semester.next_sibling("Semester")) {
		//for each course in that semester
		for (pugi::xml_node course = semester.child("Course"); course; course = course.next_sibling("Course")) {
			//for each lab or section in that semester
			for (pugi::xml_node sect_n_lab : course.children()) {
				Course newCourse;
				newCourse.semid = semester.attribute("id").as_int();
				newCourse.cname = course.attribute("id").as_string();
				newCourse.lname = sect_n_lab.attribute("Lecturer").as_string();
				newCourse.hours = sect_n_lab.attribute("Hours").as_int();
				newCourse.isLab = (strcmp(sect_n_lab.name(), "Lab") == 0) ? true : false;
				newCourse.section = sect_n_lab.attribute("id").as_int();
				newCourse.split = sect_n_lab.attribute("Split").as_bool();

				labStatus = newCourse.isLab ? 1 : 0;
				newCourse.uniqueID = hash(
						newCourse.cname + boost::lexical_cast<string>(newCourse.section)
								+ boost::lexical_cast<string>(labStatus));

				//if course have constraint, then update corresponding available slots
				if (sect_n_lab.attribute("ConsSlot") != pugi::xml_attribute()) {
					newCourse.has_cons = true;
					newCourse.cons_slot = sect_n_lab.attribute("ConsSlot").as_int();
					//update available slots for that course
					conf->update_available_slots(&newCourse);
				}

				i = init_course(newCourse, i);
			}
		}

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
		else if (!inpname.compare(PARAM_ITER))
			conf->iteration = (int) inpval;
		else if (!inpname.compare(PARAM_MEETING_SLOT))
			conf->meeting_slot = (int) inpval;
		else if (!inpname.compare(PARAM_MEETING_DURATION))
			conf->meeting_duration = (int) inpval;
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
		} else if (!inpname.compare(PARAM_SLOTS)) {
			vector<int> onehourList, twohourList, threehourList;
			string name;
			for (pugi::xml_node slot_list = course.child("SlotList"); slot_list;
					slot_list = slot_list.next_sibling("SlotList")) {
				name = slot_list.attribute("Name").as_string();
				vector<int> *slots;
				if (!name.compare("OneHour")) {
					slots = &onehourList;
				} else if (!name.compare("TwoHour")) {
					slots = &twohourList;
				} else if (!name.compare("ThreeHour")) {
					slots = &threehourList;
				}
				//iterate all constraints in group
				for (pugi::xml_node constraint = slot_list.child("Slot"); constraint;
						constraint = constraint.next_sibling("Slot")) {
					slots->push_back(constraint.attribute("Name").as_int());
				}
			}

			conf->construct_available_slots(&onehourList, &twohourList, &threehourList);

		}
	}
}
/**
 * This function depends on Course's being object not a pointer. Thus, depends on courmat to be object
 * vector, not a pointer vector. IF YOU CHANGE COURMAT TO POINTER update this function.
 * @param newCourse OBJECT to be inserted. Object will be copied to courmat.
 * @param id number that represents section in the chromosome and courmat.
 * @return id of the last insertion.
 */
int FileReader::init_course(Course newCourse, int id) {
	//split the course
	bool toSplit = false;
	if (newCourse.hours == 3 && newCourse.split) {
		toSplit = true;
		newCourse.hours = 2;
		newCourse.split = false;
	}
	//add lab info
	if (newCourse.isLab) {
		conf->lab.push_back(1);
		conf->labid.push_back(conf->add_labsession(labSession_t(newCourse.cname, id)));
	} else {
		conf->lab.push_back(-1);
		conf->labid.push_back(-1);
	}
	//add lecturer to list and update courses lecturer ID. only add cse courses
	if (newCourse.cname.substr(0, 3) == "cse") {
		newCourse.lecturerID = conf->add_lecturer(newCourse.lname, id);
		newCourse.isCSE = true;
		conf->cse.push_back(id);
	} else {
		//here we assume faculty courses does not have valid lecturer name and they don't need to be in lecturer list.
		//for solid code we should assign them a lecturerID. uniquenes can be achieved by starting lecturersID by 1000.
		//we don't expect lecturers more than 1000.
		newCourse.lecturerID = id + 1000;
		newCourse.isCSE = false;
	}
	//push course to vector
	conf->courmat.push_back(newCourse);

	//add CSE lectures
	if (conf->findlecture(id) == -1 && newCourse.cname.substr(0, 3) == "cse") {
		conf->add_lecture(newCourse.cname, std::hash<std::string>()(newCourse.cname), id);
	}

	if (toSplit) {
		newCourse.hours = 1;
		//overwritten but make sure that split will be false anytime in the execution.
		newCourse.split = false;
		return init_course(newCourse, id + 1);
	}
	return id + 1;
}

/**
 * deletes available slots which conflicts with department meeting hour.
 * logic in this function is that two hour slots which start one hour before meeting hour can
 * conflict with meeting. Let's say meeting will be held on slot 4. Slot 3 in 2-hour slot list conflicts
 * with the meeting. Thus its iteration starts with -1. It checks one previous slot. Same thing is for three hours.
 * It checks 2 slots before. Slot 4 conflicts with slot 2 in 3-hour slot list.
 */
void FileReader::assign_meeting_slot() {
	int meeting = conf->meeting_slot;

	//for each semester
	for (int i = 0; i < 8; ++i) {
		for (int j = -2; j < conf->meeting_duration; ++j) {
			//for one hour slots
			if (j >= 0) {
				conf->erase_slot(&conf->available_slots[i][0], meeting + j);
			}

			//for two hour slots
			if (j >= -1) {
				conf->erase_slot(&conf->available_slots[i][1], meeting + j);
			}

			//for three hour slots
			conf->erase_slot(&conf->available_slots[i][2], meeting + j);
		}
	}

}

} /* namespace std */
