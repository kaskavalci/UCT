/*
 * Common.cpp
 *
 *  Created on: 21 Eki 2012
 *      Author: HalilCan
 */

#include "Common.h"
#include <iostream>
#include <fstream>
#include <algorithm>

namespace std {

//singleton pattern
bool Common::instanceFlag = false;
Common * Common::conf = NULL;

Common* Common::getConf() {
	if (!instanceFlag) {
		conf = new Common();
		instanceFlag = true;
		return conf;
	} else {
		return conf;
	}
}

/*
 * adds a lecturer if not in list, otherwise update it with new course.
 */
int Common::add_lecturer(string name, int idx) {
	for (vector<Lecturer>::iterator it = lecturers.begin(); it != lecturers.end(); ++it) {
		if (it->name == name) {
			it->myCourses.push_back(idx);
			return it - lecturers.begin();
		}
	}
	lecturers.push_back(Lecturer(name, idx));
	return lecturers.size() - 1;
}

int Common::add_labsession(labSession_t lab) {
	for (vector<labSession_t>::iterator it = labcourses.begin(); it != labcourses.end(); ++it) {
		if (it->idx == lab.idx)
			return (it - labcourses.begin());
	}
	labcourses.push_back(lab);
	return labcourses.size() - 1;
}
/*
 * search for lectureID in lectures vector. if found, insert it to its courses.
 * courses is std::set, so only unique elements will be there.
 * lectid is hash output of lectname.
 */
int Common::add_lecture(string name, int lectid, int idx) {
	for (vector<Lecture>::iterator it = lectures.begin(); it != lectures.end(); ++it) {
		if (it->lectid == lectid) {
			it->courses.insert(idx);
			initlab(&*it, idx);
			return it - lectures.begin();
		}
	}
	Lecture lect(name, lectid);
	initlab(&lect, idx);
	lect.courses.insert(idx);
	lectures.push_back(lect);
	return lectures.size() - 1;
}

void Common::initlab(Lecture* lect, int idx) {
	if (courmat[idx].isLab && courmat[idx].section == 1 && courmat[idx].hours == 2) {
		lect->lab[lect_lab1][lect_idx] = idx;
	}
	if (courmat[idx].isLab && courmat[idx].section == 2 && courmat[idx].hours == 2) {
		lect->lab[lect_lab2][lect_idx] = idx;
	}
	if (courmat[idx].isLab && courmat[idx].hours == 1) {
		lect->lab[lect_lab3][lect_idx] = idx;
	}
}

int Common::assign_random_slot(int courseID) {
	Course *course = &courmat[courseID];
	//prepare available slots
	if (!course->split && course->hours == 3) {
		//do not split 3 hours course.
		return available_slots[course->semid][2][RND(available_slots[2].size())];
	} else if (course->hours == 2) {
		return available_slots[course->semid][1][RND(available_slots[1].size())];
	} else if (course->hours == 1) {
		return available_slots[course->semid][0][RND(available_slots[0].size())];
	} else {
#ifdef DEBUG
		cerr << "unhandled case!" << endl;
#endif
		return -1;
	}
}
/**
 * Constructs available slots vector for all 8 semesters.
 * @param onehour Available slots for one hour sessions
 * @param twohour Available slots for two hour sessions
 * @param threehour Available slots for three hour sessions
 */
void Common::construct_available_slots(vector<int>* onehour, vector<int>* twohour, vector<int>* threehour) {
	//make it for all semesters
	for (int i = 0; i < 8; ++i) {
		available_slots[i].push_back(*onehour);
		available_slots[i].push_back(*twohour);
		available_slots[i].push_back(*threehour);
	}
}

/**
 * From the given course's cons_slot, updates the available slots per semester.
 * It deletes conflicting slots from one, two and three hour slots in the same semester.
 * It checks for 1-2, 1-3 hour conflicts, 2-1, 2-3 and 3-1, 3-2 conflicts and deletes any conflicting
 * slots.
 * @param course reference course which has cons_slot!
 */
void Common::update_available_slots(Course* course) {
	//semid - 1 because semesters start at 1 not 0.
	int sem = course->semid - 1, modslot = course->cons_slot % 9;
	vector<int> *onehour = &(available_slots[sem][0]);
	vector<int> *twohour = &(available_slots[sem][1]);
	vector<int> *threehour = &(available_slots[sem][2]);
	//delete any available slots correspond to this slot.
	vector<int>::iterator it;
	if (course->hours == 1) {
		//if one hour course, just delete it from one-hour list.
		erase_slot(onehour, course->cons_slot);

		//for two hour list
		//for modslot == 3, both slot and slot - 1 should be deleted because they overlap.
		if (modslot == 2 || modslot == 7 || modslot == 3) {
			erase_slot(twohour, course->cons_slot);
		}
		if (modslot == 3 || modslot == 4 || modslot == 8) {
			erase_slot(twohour, course->cons_slot - 1);
		}

		/*
		 * three hour list, for 2, 4, and 7 slot - 2 should be deleted. the slot they correspond
		 * to in 3-hour list, is the - 2's slot. 3 corresponds to -1 and 2 is both -2 and itself.
		 * consider 1-hour slot 2, it conflicts with both 3-hour slot 2 and 0. Both should be deleted.
		 * consider 1-hour slot 7. it conflicts with 3-hour slot 5. which is 7 - 2 = 5.
		 */
		if (modslot == 2 || modslot == 4 || modslot == 7) {
			erase_slot(twohour, course->cons_slot - 2);
		}
		if (modslot == 2) {
			erase_slot(twohour, course->cons_slot);
		}
		if (modslot == 3) {
			erase_slot(twohour, course->cons_slot - 1);
		}
	} else if (course->hours == 2) {
		//one hour list
		if (modslot == 2 || modslot == 3 || modslot == 7) {
			erase_slot(onehour, course->cons_slot);
			erase_slot(onehour, course->cons_slot + 1);
		}

		//twohour list
		erase_slot(twohour, course->cons_slot);

		//threehour list
		if (modslot == 0 || modslot == 2 || modslot == 5) {
			erase_slot(threehour, course->cons_slot);
		}
		if (modslot == 2 || modslot == 7) {
			erase_slot(threehour, course->cons_slot - 2);
		}
		if (modslot == 3) {
			erase_slot(threehour, course->cons_slot - 1);
		}
	} else if (course->hours == 3) {
		//one hour list
		if (modslot == 0 || modslot == 5 || modslot == 2) {
			erase_slot(onehour, course->cons_slot + 2);
		}
		if (modslot == 2) {
			erase_slot(onehour, course->cons_slot + 1);
			erase_slot(onehour, course->cons_slot);
		}

		//two hour list
		if (modslot == 0 || modslot == 5 || modslot == 2) {
			erase_slot(twohour, course->cons_slot);
		}
		if (modslot == 0 || modslot == 5) {
			erase_slot(twohour, course->cons_slot + 2);
		}
		if (modslot == 2) {
			erase_slot(twohour, course->cons_slot + 1);
		}

		//three hour list
		erase_slot(threehour, course->cons_slot);
	}
}

/**
 * Erases the given slot from given available slot vector
 * @param list onehour, twohour or threeshour vector of available slots
 * @param slot slot to be deleted
 */
void Common::erase_slot(vector<int>* list, int slot) {
	//if one hour course, just delete it from one-hour list.
	vector<int>::iterator it = find(list->begin(), list->end(), slot);
	if (it != list->end()) {
		list->erase(it);
	}
}

Common::Common() {
	mutrate = 1;
	duration = 2000;
	hillsize = 1.00;
	hcrate = 0.6;
	crrate = 0.9;
	rnd_insert_rate = 0.001;
	crowding_dist = 3;
	hc_gene_count = 10;
	ChromSize = -1;
	pop_size = 0;
	pareto_size = 0;
	sel_poolsize = 0;
	sel_candidatesize = 0;
}

Common::~Common() {
	instanceFlag = false;
	prereq.clear();
	courmat.clear();
	courses.clear();
	lecturers.clear();
	lectures.clear();
	labcourses.clear();
	labid.clear();
	lab.clear();
	cse.clear();
	for (auto it = groups.begin(); it != groups.end(); ++it) {
		it->clear();
	}
}

/*
 * returns "the position" in labcourses vector, if idx is found.
 * returns -1 otherwise.
 */
int Common::findlabcourse(int idx) {
	for (vector<labSession_t>::const_iterator it = labcourses.begin(); it != labcourses.end(); ++it) {
		if (it->idx == idx)
			return (it - labcourses.begin());
	}
	return -1;
}

int Common::findlecture(int idx) {
	for (vector<Lecture>::const_iterator it = lectures.begin(); it != lectures.end(); ++it) {
		if (it->courses.find(idx) != it->courses.end()) {
			//item found!
			return (it - lectures.begin());
		}
	}
	return -1;
}

} /* namespace std */
