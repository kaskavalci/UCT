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
#include <string.h>
#include <sstream>
#include <algorithm>

namespace std {


void FileReader::readreq() {
	FILE*input;
	char cour1[20], cour2[20];
	int i, j;
	input = fopen("prereq.txt", "r");
	for (i = 0; i < CHROML; i++) {
		for (j = 0; j < CHROML; j++) {
			conf->prereq[i][j] = 0;
		}
	}
	while (!feof(input)) {
		fscanf(input, "%s %s\n", cour1, cour2);
		for (i = 0; i < CHROML; i++) {
			for (j = 0; j < CHROML; j++) {
				if (conf->courmat[i].cname.substr(0, 6) == cour1
						&& conf->courmat[j].cname.substr(0, 6) == cour2)
					conf->prereq[i][j] = 1;
			}
		}
		/*printf("\n");
		for (i = 0; i < CHROML; i++) {
			for (j = 0; j < CHROML; j++) {
				printf(" %d", conf->prereq[i][j]);
			}
			printf("\n");
		}
		printf("\n");*/
	}
	fclose(input);
}
void FileReader::readcourses() {
	//todo: cpp lib'e �evir. veri yap�s�ndaki s���rl��� d�zelt.
	ifstream input;
	input.open("inputcse.txt");
	string cnm, lnm;
	int c2day, c2slot;
	int counter = 0, i, j;
	for (i = 0; i < CHROML; i++) {
		conf->courmat[i].semid = -1;
		conf->courmat[i].hours = -1;
		conf->courmat[i].has_constraint = -1;
		conf->courmat[i].c2day = -1;
		conf->courmat[i].c2day = -1;
	}
	while (!input.eof()) {
		input >> cnm;
		if (cnm.compare("C4") && cnm.compare("C2") && !cnm.empty()) {
			input >> lnm >> conf->courmat[counter].semid >> conf->courmat[counter].hours;
			conf->courmat[counter].cname = cnm;
			conf->courmat[counter].lname = lnm;
			counter++;
		} else if (!cnm.compare("C2") && !cnm.empty()) {
			input >> lnm >> c2day >> c2slot;
			for (j = 0; j < CHROML; j++) {
				if (conf->courmat[j].cname == lnm) {
					conf->courmat[j].has_constraint = 1;
					conf->courmat[j].c2day = c2day;
					conf->courmat[j].c2slot = c2slot;
				}
			}
		}
	}
	input.close();
	Lecture tmplect;
	for (i = 0; i < CHROML; i++) {
		conf->cid[i] = -1;
		conf->lectid[i] = -1;
		conf->labid[i] = -1;
		conf->lab[i] = 0;
		conf->cse[i] = 0;
		if (conf->findcourse(i) == -1)
			conf->courses.push_back(conf->courmat[i].cname);
		if (conf->courmat[i].cname.size() > 8 && conf->courmat[i].cname.at(7) == 'L'
				&& conf->findlabcourse(i) == -1)
			conf->labcourses.push_back(labSession_t(conf->courmat[i].cname.substr(0, 6), i));
		if (conf->findlecturer(i) == -1)
			conf->lecturers.push_back(conf->courmat[i].lname);
		if (conf->findlecture(i) == -1 && conf->courmat[i].cname.substr(0, 3) == "cse") {
			tmplect.lectname = conf->courmat[i].cname.substr(0, 6);
			tmplect.cormat_id = i;
			tmplect.semid = -1;
			tmplect.cid1 = -1;
			tmplect.cid2 = -1;
			tmplect.cid3 = -1;
			tmplect.cid4 = -1;
			tmplect.lab1 = -1;
			tmplect.lab2 = -1;
			tmplect.lab3 = -1;
			tmplect.cid1day = -1;
			tmplect.cid2day = -1;
			tmplect.cid1slot = -1;
			tmplect.cid2slot = -1;
			tmplect.cid3day = -1;
			tmplect.cid4day = -1;
			tmplect.cid3slot = -1;
			tmplect.cid4slot = -1;
			tmplect.lab1day = -1;
			tmplect.lab2day = -1;
			tmplect.lab3day = -1;
			tmplect.lab1slot = -1;
			tmplect.lab2slot = -1;
			tmplect.lab3slot = -1;
			tmplect.labs[0] = -1;
			tmplect.labs[1] = -1;
			tmplect.labs[2] = -1;
			conf->lectures.push_back(tmplect);
		}
	}
	int lidx;
	for (i = 0; i < CHROML; i++) {
		conf->cid[i] = conf->findcourse(i);
		conf->lectid[i] = conf->findlecturer(i);
		conf->labid[i] = conf->findlabcourse(i);
		if (conf->courmat[i].cname.at(7) == 'L')
			conf->lab[i] = 1;
		if (conf->courmat[i].cname.substr(0, 3) == "cse")
			conf->cse[i] = 1;
		if (conf->courmat[i].cname.at(7) == '0' && conf->courmat[i].hours == 1
				&& conf->courmat[i].cname.at(8) == '1' && (lidx = conf->findlecture(i)) >= 0) {
			conf->lectures[lidx].cid1 = i;
			conf->lectures[lidx].semid = conf->courmat[i].semid;
		}
		if (conf->courmat[i].cname.at(7) == '0' && conf->courmat[i].hours == 2
				&& conf->courmat[i].cname.at(8) == '1'  && (lidx = conf->findlecture(i)) >= 0) {
			conf->lectures[lidx].cid2 = i;
			conf->lectures[lidx].semid = conf->courmat[i].semid;
		}
		if (conf->courmat[i].cname.at(7) == '0' && conf->courmat[i].hours == 1
				&& conf->courmat[i].cname.at(8) == '2'  && (lidx = conf->findlecture(i)) >= 0) {
			conf->lectures[lidx].cid3 = i;
		}
		if (conf->courmat[i].cname.at(7) == '0' && conf->courmat[i].hours == 2
				&& conf->courmat[i].cname.at(8) == '2'  && (lidx = conf->findlecture(i)) >= 0) {
			conf->lectures[lidx].cid4 = i;
		}
		if (conf->courmat[i].cname.at(7) == 'L' && conf->courmat[i].cname.at(8) == '1'
				&& conf->courmat[i].hours == 2  && (lidx = conf->findlecture(i)) >= 0) {
			conf->lectures[lidx].lab1 = i;
			conf->lectures[lidx].haslabs = 1;
		}
		if (conf->courmat[i].cname.at(7) == 'L' && conf->courmat[i].cname.at(8) == '2'
				&& conf->courmat[i].hours == 2  && (lidx = conf->findlecture(i)) >= 0) {
			conf->lectures[lidx].lab2 = i;
			conf->lectures[lidx].haslabs = 1;
		}
		if (conf->courmat[i].cname.at(7) == 'L' && conf->courmat[i].hours == 1  && (lidx = conf->findlecture(i)) >= 0) {
			conf->lectures[lidx].lab3 = i;
			conf->lectures[lidx].haslabs = 1;
		}
	}
}
void FileReader::readinputparam() {
	fstream input("inputparam.txt");
	string inpname;
	float inpval;
	string line;
	stringstream ss (stringstream::in | stringstream::out);
	while (getline(input, line)) {
		if (line.empty() || line.at(0) == '#') continue;
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
			while(inpval != -1) {
				hardgr.push_back(inpval);
				ss >> inpval;
			}
			conf->hardgroup.push_back(hardgr);
		}
		else if (!inpname.compare("soft_fitness_group")) {
			vector<int> softgr;
			while(inpval != -1) {
				softgr.push_back(inpval);
				ss >> inpval;
			}
			conf->softgroup.push_back(softgr);
		}
		ss.clear();
	}
	input.close();
}

FileReader::FileReader() {
	conf = Common::getConf();
}

void FileReader::readConst(const char *fname)
{
	int i, j, inode, nnode;
	ifstream input;
	input.open(fname);
	for (i = 0; i < CHROML; i++)
		for (j = 0; j < CHROML; j++)
			conf->confmat[i][j] = 0;
	while (!input.eof()) {
		input >> inode >> nnode;
		conf->confmat[inode - 1][nnode - 1] = 1;
		//cout << "FIND " << inode << nnode << endl;
	}
	input.close();
}
} /* namespace std */
