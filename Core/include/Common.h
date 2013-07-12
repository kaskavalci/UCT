/*
 * Common.h
 *
 *  Created on: 21 Eki 2012
 *      Author: HalilCan
 */

#ifndef COMMON_H_
#define COMMON_H_

#include <vector>
#include <string>
#include <list>
#include "Course.h"
#include "Lecture.h"
#include "Constants.h"
#include "Lecturer.h"

namespace std {

class Common {
public:
	static Common *getConf();
	virtual ~Common();
	int findlabcourse(int idx);
	int findlecture(int idx);
	int add_lecturer(string, int);
	int add_labsession(labSession_t);
	int add_lecture(string, int, int);
	int assign_random_slot(int courseID);
	void construct_available_slots(vector<int> *, vector<int> *, vector<int> *);
	void update_available_slots(Course *);

	vector< vector<int> > prereq;

	vector<Course> courmat;
	vector<string> courses;
	vector<Lecturer> lecturers;
	vector<labSession_t> labcourses;
	vector<Lecture> lectures;
	vector<int> labid, lab, cse;


	int ChromSize;
	time_t duration;
	double hillsize;
	double hcrate;
	double crrate;
	double mutrate;
	double rnd_insert_rate;
	int crowding_dist;
	int hc_gene_count;
	size_t pop_size, sel_poolsize, sel_candidatesize;
	int pareto_size;
	vector<vector<int> > groups;
	vector<vector<int> > available_slots[8];
private:
	static bool instanceFlag;
	static Common *conf;
	void initlab(Lecture*, int);
	void erase_slot(vector<int> *, int);
	Common();

};

} /* namespace std */
#endif /* COMMON_H_ */
