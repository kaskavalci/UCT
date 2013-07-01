/*
 * Population.cpp
 *
 *  Created on: 21 Eki 2012
 *      Author: HalilCan
 */

#include "Population.h"
#include "FileReader.h"
#include <stdlib.h>
#include <iostream>
#include <ctime>
#include <stdio.h>
#include <set>
#include <iomanip>
#include <sstream>

extern int HC_whichFit;

namespace std {

Population::Population(int seed) {
	unsigned int i;
	time(&start);
	//read inputs
	FileReader *reader = new FileReader();
	reader->read();

	//focus on hard fitness first
	HC_whichFit = hc_hard;

	this->conf = Common::getConf();
	crossel1 = crossel2 = 0;

	this->seed = seed;
	srand(seed);

	char buffer[80];
	struct tm * now = localtime(&start);
	strftime(buffer, 80, "%j%H%M%S.html", now);
	printer_fname = string(buffer);
	printer = new Printer(printer_fname);
	printer->printHeader();

	for (i = 0; i < STAT_LEN; ++i) {
		stats[i] = 0;
	}
	pareto_bestID = 0;
	pareto_minHFit = 2000;
	pareto_minTFit = 2000;

	//init pareto front with all false
	inpf3.assign(conf->pop_size, false);

	for (i = 0; i < conf->pop_size; ++i) {
		population.push_back(i);
		pop.push_back(new Individual(conf));
		add_to_pareto(i);
	}
}

Population::~Population() {
	pop.clear();
	if (printer != NULL) {
		delete printer;
	}
	delete conf;
}

void Population::hillclimber() {
	int i;
	//todo: perhaps randomization for hillsize?
	for (i = 0; i < conf->pop_size * conf->hillsize; i++) {
		if (10 * conf->hcrate > RND(1000)) {
			if (pop[i]->hc_worstsection()) {
				if (add_to_pareto(i))
					stats[ParS]++;
				else
					stats[ParF]++;
			}
		}
	}
}
/*
 * Count the individuals in crowding distance in given list. It could be perato front or whole population.
 */
int Population::calc_crowd(int idx, const vector<int> &list) {
	int count = 0;

	vector<int>::const_iterator ite = list.end();
	for (vector<int>::const_iterator it = list.begin(); it != ite; ++it) {
		if (*it == idx)
			continue;
		if (crowd_condition(conf->groups, pop[idx], pop[*it])) {
			count++;
		}
	}
	return count;
}

void Population::tournament(Individual** parent) {
	set<int> selection_pool;
	//get unique IDs
	while (selection_pool.size() < conf->sel_poolsize) {
		selection_pool.insert(RND(conf->pop_size));
	}
	//get TOURNAMENT_CANDIDATE_N unique candidates for comparation
	set<int> candidate;
	vector<int> domination;
	domination.assign(conf->sel_candidatesize, 0);
	while (candidate.size() < conf->sel_candidatesize) {
		candidate.insert(RND(conf->pop_size));
	}
	int i;
	set<int>::iterator itc;
	set<int>::iterator itec = candidate.end();
	for (set<int>::iterator it = selection_pool.begin(); it != selection_pool.end(); ++it) {
		i = 0;
		for (itc = candidate.begin(); itc != itec; ++itc, ++i) {
			if (pop[*itc]->dominates(pop[*it]).type == D_TRUE) {
				domination[i]++;
			}
		}
	}
	int max = 0, maxid = -1;
	bool equal_domination = false;
	//find the candidate that dominates most and check if there is equal domination
	for (i = 0; i < conf->sel_candidatesize; ++i) {
		if (domination[i] > max) {
			max = domination[i];
			maxid = i;
			equal_domination = false;
		}
		//we have individuals that have same domination values. Initiate crowding distance calculation
		else if (domination[i] == max) {
			equal_domination = true;
		}
	}
	if (maxid != -1) {
		itc = candidate.begin();
		advance(itc, maxid);
		*parent = pop[*itc];
	} else if (maxid == -1 || equal_domination) {
		int min_nitch = conf->pop_size * 2; //give min a high number
		set<int>::iterator it_min = itec = candidate.end();
		int dummy;
		//find the candiate that have least individuals surrounding it.
		for (itc = candidate.begin(); itc != itec; ++itc) {
			if ((dummy = calc_crowd(*itc, population)) < min_nitch) {
				min_nitch = dummy;
				it_min = itc;
			}
		}
		//YAY! we found the minimum.
		if (it_min != candidate.end()) {
			*parent = pop[*it_min];
		}
		//This is odd. We should have found something. For the sake of stability, select a random candidate
		else {
			cerr << "No suitable candidate from tournament." << endl;
			itc = candidate.begin();
			advance(itc, RND(conf->sel_candidatesize));
			*parent = pop[*itc];
		}
	}
}

void Population::crossover() {
	int crossrate = (int) (conf->pop_size * conf->crrate / 100);
	Individual *parent1 = NULL, *parent2 = NULL, *child = NULL;
	int old, now;
	for (int i = 0; i < crossrate; ++i) {
		child = new Individual();
		tournament(&parent1);
		tournament(&parent2);
		child->cross(*parent1, *parent2);
		child->updatefitness(0);
		old = child->getFit().total_fit;
		if (old < parent1->getFit().total_fit && old < parent2->getFit().total_fit) {
			stats[CrossS]++;
		} else
			stats[CrossF]++;

		child->hc_worstsection();
		now = child->getFit().total_fit;
		if (now < old)
			stats[HCS]++;
		else
			stats[HCF]++;

		if (add_to_population(child)) {
			stats[PopS]++;
		} else {
			stats[PopF]++;
		}
		delete (child);
	}
}
/**
 * Adds given Individual idx in population to pareto front if it satisfies certain criteria.
 * Crowding Distance is used to maintain diversity.
 * @param idx
 * @return true if Individual is added to pareto front, false otherwise.
 */
bool Population::add_to_pareto(int idx) {
	uint8_t domination;
	vector<int> non_dominated_ind;
	//todo zamaný gelince sil
	if (pareto_minHFit < pop[pareto_bestID]->getFit().hard_fit) {
		cerr << "hata!" << pareto_minHFit << " " << pop[pareto_bestID]->getFit().hard_fit << endl;
	}
	//if individual is already in pareto front, return true
	if (inpf3[idx] == true) {
		update_bestInd(idx);
		return true;
	}
	//if pareto front is empty add the individual directly
	if (paretof.size() == 0) {
		update_pareto(idx);
		return true;
	}
	for (size_t i = 0; i < paretof.size(); i++) {
		domination = pop[idx]->dominates(pop[paretof[i]]).type;
		//candidate dominated perato front. update it.
		if (domination == D_TRUE) {
#ifdef VERBOSE
			if (pareto_bestID == paretof[i])
			cerr << "pareto overwritten eski" << pop[pareto_bestID]->getFit().total_fit << " yeni:" << pop[idx]->getHardFit().total_fit
			<< endl;
#endif
			update_pareto(i, idx);
			return true;
		}
		//no domination. add the individuals that does not dominate each other
		else if ((domination == D_NO_DOMINATION)) {
			non_dominated_ind.push_back(i);
		}
		//candidate is dominated by some individual in pareto front. candidate is not entitled to enter pareto front
		else if (domination == D_FALSE) {
			return false;
		}
	}
	if (non_dominated_ind.size() == 0) {
		//this is odd.
		cerr << "domination returns funny output" << endl;
		return false;
	}
	//we reach this line if there is no domination. add candidate to pareto front if we have enough space
	if (paretof.size() < conf->pareto_size) {
		update_pareto(idx);
		return true;
	}
	//pareto front is full and candidate cannot be dominated by pareto front. Find a suitable individual from pareto front
	//that have crowded area.
	if (paretof.size() >= conf->pareto_size) {
		int max = 0, maxPos = -1, dummy;
		for (size_t i = 0; i < non_dominated_ind.size(); ++i) {
			if (paretof[i] == pareto_bestID)
				continue; // don't touch the best guy
			if ((dummy = calc_crowd(paretof[non_dominated_ind[i]], paretof)) > max) {
				max = dummy;
				maxPos = non_dominated_ind[i];
			}
		}
		//we found something!
		if (max > 3) {
#ifdef VERBOSE
			if (pop[paretof[maxPos]]->getHardFit().total_fit < pop[idx]->getHardFit().total_fit
					|| (pop[paretof[maxPos]]->getHardFit().total_fit <= pop[idx]->getHardFit().total_fit
							&& pop[paretof[maxPos]]->getHardFit().total_fit
							+ pop[paretof[maxPos]]->getSoftFit().total_fit
							< pop[idx]->getSoftFit().total_fit + pop[idx]->getHardFit().total_fit)) {
				cerr << "ERROR PARETO FRONT DEFORMATION" << endl;
			}
#endif
			update_pareto(maxPos, idx);
			return true;
		}
		//we got nothing. pareto front is excellent and full. sorry new guy, you won't be VIP for this party
		else {
			return false;
		}
	} else {
		cerr << "Unhandled condition. Population::add_to_pareto";
		return false;
	}
}

//adds the given individual to population and update pareto.
bool Population::add_to_population(Individual* candidate) {
	for (int i = 0; i < conf->pop_size; ++i) {
		if (!inpf3[i]) {
			if (candidate->dominates(pop[i]).type == D_TRUE) {
				(*pop[i]) = (*candidate);
				if (add_to_pareto(i))
					stats[ParS]++;
				return true;
			}
		}
	}
	/* if we have reached this line, then there is no domination. give a slight chance for
	 * weak individual "candidate" to be inserted into population. No need to try for adding
	 * to pareto front since this individual cannot dominate any individual in the population.
	 */
	stats[ParF]++;
	if (RND(1000) < 10 * conf->rnd_insert_rate) {
		int rnd_pos;
		//don't touch to pareto front.
		do {
			rnd_pos = RND(conf->pop_size);
		} while (inpf3[rnd_pos]);
		(*pop[rnd_pos]) = (*candidate);
		return true;
	}
	return false;
}
/*
 * mutate every individual with a rate of given mutation rates, except for pareto front.
 * mutator gets the probability from Common.h, so calling mutator does not guarantee mutation.
 * If there is a mutation mutate_all() will return true. Then update fitness immediately.
 */
void Population::mutation() {
	for (int i = 0; i < conf->pop_size; ++i) {
		if (inpf3[i])
			continue;
		//if any mutation occurred, re-build timetable.
		if (pop[i]->mutate_all()) {
			pop[i]->updatefitness(0);
			if (add_to_pareto(i))
				stats[MutS]++;
			else
				stats[MutF]++;
		}
	}
}

double Population::getduration() {
	time_t end;
	time(&end);
	return difftime(end, start);
}

void Population::startGA() {
	int it = 0;
	double duration;

	do {
		it++;

		crossover();
		hillclimber();
		mutation();
	} while ((duration = getduration()) <= conf->duration);

	print_footer(duration, it);

	//cout << endl << "The operation completed in " << getduration() << " seconds." << endl;
}

bool Population::crowd_condition(vector<vector<int> > in_group, Individual *subject, Individual *target) {
	bool statement = true;

	for (auto group = in_group.begin(); group != in_group.end(); ++group) {
		int tot_subj = 0, tot_targ = 0;
		for (vector<int>::const_iterator el = group->begin(); el != group->end(); el++) {
			tot_subj += subject->getFit().fitness[*el];
			tot_targ += target->getFit().fitness[*el];
		}
		statement &= (tot_targ < (tot_subj + conf->crowding_dist)) & (tot_targ > (tot_subj - conf->crowding_dist));
	}

	return statement;
}
/*
 * adds the given id to pareto front
 */
inline void Population::update_pareto(int idx) {
	update_bestInd(idx);
	inpf3[idx] = true;
	paretof.push_back(idx);
}
/*
 * overwrites the old individual with new one. it is part of paretof vector.
 */
inline void Population::update_pareto(int goner, int idx) {
	update_bestInd(idx);
	inpf3[idx] = true;
	inpf3[paretof[goner]] = false;
	paretof[goner] = idx;
}

const vector<Individual*>& Population::getpareto() const {
	vector<Individual*> *pareto = new vector<Individual*>();
	for (size_t i = 0; i < paretof.size(); i++) {
		pareto->push_back(pop[paretof[i]]);
	}
	return *pareto;
}

void Population::print_footer(double duration, int it) {
	ostringstream os;
	os << "Finished in " << duration << "sec. Number of it:" << it << ". Smallest:" << pareto_minHFit
			<< "Pareto Front:";
	printer->print(os.str());

	for (size_t i = 0; i < paretof.size(); i++) {
		printer->print(pop[paretof[i]], stats);
	}

#ifdef	LASTTOUCH
	int tot[2] = {};
	for (size_t i = 0; i < paretof.size(); i++) {
		for (int j = 0; j < TOT_FIT_N; ++j) {
			if (j < 3) {
				tot[0] += pop[paretof[i]]->getFit().fitness[j];
			} else {
				tot[1] += pop[paretof[i]]->getFit().fitness[j];
			}
		}
	}
	cout << tot[0] / paretof.size() << "\t" << tot[1] / paretof.size() << "\t";
	tot[0] = tot[1] = 0;
	for (size_t i = 0; i < conf->pop_size; i++) {
		for (int j = 0; j < TOT_FIT_N; ++j) {
			if (j < 3) {
				tot[0] += pop[i]->getFit().fitness[j];
			} else {
				tot[1] += pop[i]->getFit().fitness[j];
			}
		}
	}
	cout << tot[0] / conf->pop_size << "\t" << tot[1] / conf->pop_size << "\t";
	cout << conf->pop_size << "\t" << conf->pareto_size << endl;
#endif

	printer->printFooter();

	//create intermediate structure for printer
	double confVar[c_LENGTH];
	confVar[c_duration] = duration;
	confVar[c_HCRate] = conf->hcrate;
	confVar[c_MutationRate] = conf->mutrate;
	confVar[c_CRRate] = conf->crrate;
	confVar[c_InsertRate] = conf->rnd_insert_rate;
	confVar[c_CrowdingSize] = conf->crowding_dist;
	confVar[c_seed] = seed;
	confVar[c_iteration] = it;
	printer->printStats(confVar, true);
	delete printer;

	printer = new Printer("result.html");
	printer->printLast(pop[pareto_bestID], stats);
	printer->printStats(confVar, false);
	delete printer;
	printer = NULL;
}

/*
 * updates best Individual if given idx is better than old one.
 */
inline void Population::update_bestInd(int idx) {
	bool res = false;
	switch (HC_whichFit) {
	case hc_soft:
		res = (pop[idx]->getFit().soft_fit <= pop[pareto_bestID]->getFit().soft_fit);
		break;
	case hc_hard:
		res = (pop[idx]->getFit().hard_fit < pop[pareto_bestID]->getFit().hard_fit)
				|| ((pop[idx]->getFit().hard_fit == pop[pareto_bestID]->getFit().hard_fit)
						&& pop[idx]->getFit().total_fit < pop[pareto_bestID]->getFit().total_fit);
		break;
	default:
		res = (pop[idx]->getFit().hard_fit < pop[pareto_bestID]->getFit().hard_fit)
				|| ((pop[idx]->getFit().hard_fit <= pop[pareto_bestID]->getFit().hard_fit)
						&& pop[idx]->getFit().total_fit < pop[pareto_bestID]->getFit().total_fit);
		break;
	}
	//res is true or we applied HC to bestID and it needs to be updated.
	if (res
			|| ((idx == pareto_bestID)
					&& (pareto_minHFit > pop[pareto_bestID]->getFit().hard_fit
							|| pareto_minTFit > pop[pareto_bestID]->getFit().total_fit))) {
		if (pareto_minHFit < pop[idx]->getFit().hard_fit) {
			cerr << "hata! pareto front deformation" << endl;
		}
		pareto_bestID = idx;
		pareto_minHFit = pop[pareto_bestID]->getFit().hard_fit;
		pareto_minTFit = pop[pareto_bestID]->getFit().total_fit;
		//if we reach 0 hard fitness, change HC to improve hard and soft.
		if (pareto_minHFit == 0) {
			HC_whichFit = hc_both;
		}
		printer->print(pop[pareto_bestID], stats);
	}
}

} /* namespace std */
