/*
 * Population.cpp
 *
 *  Created on: 21 Eki 2012
 *      Author: HalilCan
 */

#include "Population.h"
#include <stdlib.h>
#include <iostream>
#include <ctime>
#include <stdio.h>
#include "Mutation.h"
#include <set>
#include <iomanip>
#include <sstream>

extern int HC_whichFit;

namespace std {

Population::Population() {
	int i;
	//TODO: timer start - stop iþlemini mainde yap.
	time(&start);
	this->conf = Common::getConf();
	crossel1 = crossel2 = 0;

	char buffer[80];
	struct tm * now = localtime(&start);
	strftime(buffer,80,"%j%H%M%S.html", now);
	printer = new Printer(string(buffer));
	printer->printHeader();

	updatePareto = false;
	for (i = 0; i < STAT_LEN; ++i) {
		stats[i] = 0;
	}
	pareto_bestID = 0;
	pareto_minHFit = 2000;
	fill_n(inpf3, POPUL, false);
	for (i = 0; i < POPUL; ++i) {
		population.push_back(i);
		pop.push_back(new Individual(conf));
		add_to_pareto(i);
	}
}

Population::~Population() {
	pop.clear();
}

void Population::hillclimbmix2() {
	int i;
	for (i = 0; i < POPUL * conf->hillsize; i++) {
		if (RND(1000) < 1000 * conf->hillboth) {
			if (pop[i]->hc_worstsection(hc_hard)) {
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
		if(crowd_condition(conf->hardgroup, conf->softgroup, pop[idx], pop[*it])) {
			count++;
		}
	}
	return count;
}

void Population::tournament(Individual** parent) {
	set<int> selection_pool;
	//get unique IDs
	while (selection_pool.size() < TOURNAMENT_POOLSIZE) {
		selection_pool.insert(RND(POPUL));
	}
	//get TOURNAMENT_CANDIDATE_N unique candidates for comparation
	set<int> candidate;
	int domination[TOURNAMENT_CANDIDATE_N] = { };
	while (candidate.size() < TOURNAMENT_CANDIDATE_N) {
		candidate.insert(RND(POPUL));
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
	for (i = 0; i < TOURNAMENT_CANDIDATE_N; ++i) {
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
		int min_nitch = POPUL * 2; //give min a high number
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
			advance(itc, RND(TOURNAMENT_CANDIDATE_N));
			*parent = pop[*itc];
		}
	}
}

void Population::crossover() {
	int crossrate = (int) (POPUL * conf->crrate / 2);
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

		child->hc_worstsection(hc_hard);
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
/*
 * adds or updates pareto front if given Individual meets criteria.
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
	if (paretof.size() < PARETO_SIZE) {
		update_pareto(idx);
		return true;
	}
	//pareto front is full and candidate cannot be dominated by pareto front. Find a suitable individual from pareto front
	//that have crowded area.
	if (paretof.size() >= PARETO_SIZE) {
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
		if (max != 0) {
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
	for (int i = 0; i < POPUL; ++i) {
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
	if (RND(1000) < 1000 * conf->insert_popul_rate) {
		int rnd_pos;
		//don't touch to pareto front.
		do {
			rnd_pos = RND(POPUL);
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
	Mutation mutator(conf);
	for (int i = 0; i < POPUL; ++i) {
		if (inpf3[i])
			continue;
		mutator.setChromosome(pop[i]->getChromosome());
		//if any mutation occurred, re-build timetable.
		if (mutator.mutate_all()) {
			pop[i]->updatefitness(0);
			if (add_to_pareto(i))
				stats[MutS]++;
			else
				stats[MutF]++;
		}
	}
}

bool Population::foundinpar(int idx) {
	size_t i;
	for (i = 0; i < paretof.size(); i++) {
		if (paretof[i] == idx)
			return true;
	}
	return false;
}

double Population::getduration() {
	time_t end;
	time(&end);
	return difftime(end, start);
}

void Population::run(int seed) {
	int it = 0;
	double duration = getduration();
	//focus on hard fitness first
	HC_whichFit = hc_hard;
	while ((int) duration <= conf->dur) {
		it++;
		duration = getduration();
		if (updatePareto) {
			if (conf->verbose_level == 3) {
				cout << "id" << pareto_bestID << "\tMin Tot:" << pop[pareto_bestID]->getFit().total_fit	<< endl;
				for (size_t i = 0; i < paretof.size(); i++) {
					cout << "id\t";
					print_fitness(cout, pop[paretof[i]]);
				}
			} else if (conf->verbose_level == 4) {
				printer->print(pop[pareto_bestID], stats);
			}
			updatePareto = false;
		}
		if (it % 20 == 0) {
			if (conf->verbose_level == 1) {
				for (size_t i = 0; i < paretof.size(); i++) {
					cout << "id\t";
					print_fitness(cout, pop[paretof[i]]);
				}
				cout << "# of iterations " << it << " duration " << duration << endl;
				cout << "# smallest " << pareto_bestID << endl;
			} else if (conf->verbose_level == 2) {
				printer->print(pop[pareto_bestID], stats);
			}
		}
		crossover();
		hillclimbmix2();
		mutation();
	}
	ostringstream os;
	os << "Finished in " << duration << "sec. Number of it:" << it << ". Smallest:" << pareto_minHFit
			<< "Pareto Front:";
	printer->print(os.str());
	if (conf->verbose_level == 1 || conf->verbose_level == 3 || conf->verbose_level == 4) {
		for (size_t i = 0; i < paretof.size(); i++) {
			printer->print(pop[paretof[i]], stats);
		}
	}
	printer->printFooter();
	double confVar[c_LENGTH];
	confVar[c_duration] = duration;
	confVar[c_HCRate] = conf->hillboth;
	confVar[c_MutationRate] = conf->mutg1rate;
	confVar[c_CRRate] = conf->crrate;
	confVar[c_InsertRate] = conf->insert_popul_rate;
	confVar[c_CrowdingSize] = conf->crowding_dist;
	confVar[c_seed] = seed;
	confVar[c_iteration] = it;
	printer->printStats(confVar, true);
	delete printer;
	/*printf("# smallest  %d \n", pareto_bestID);
	printf("THE SOULUTION IS  \n");
	//pop[paretof[smallestidx]]->printlect();
	//pop[paretof[smallestidx]]->printdekanlik();
	//pop[paretof[smallestidx]]->printtable();
	pop[pareto_bestID]->updatefitness(1);
	for (size_t m = 0; m < paretof.size(); m++) {
		printf("THE SOULUTION IS %d \n", m);
		//pop[paretof[m]]->printlect();
		//pop[paretof[m]]->printdekanlik();
		//pop[paretof[m]]->printtable();
		pop[paretof[m]]->updatefitness(1);
	}*/
	printer = new Printer("result.html");
	printer->printLast(pop[pareto_bestID], stats);
	printer->printStats(confVar, false);
	delete printer;

	cout << endl << "The operation completed in " << getduration() << " seconds." << endl;
}

inline void Population::print_fitness(ostream &fresult, Individual *subject) {
	fresult << subject->getFit().total_fit << "\t";
	for (int i = 0; i < TOT_FIT_N; ++i) {
		fresult << (int) subject->getFit().fitness[i] << "\t";
	}
}

bool Population::crowd_condition(vector<vector<int> > hardgroup, vector<vector<int> > softgroup, Individual *subject, Individual *target) {
	bool statement = true;
	int i = 0;
	vector<vector<int> >::const_iterator group = hardgroup.begin(), ite = hardgroup.end();
	do {
		for (; group != ite; ++group) {
			int tot_subj = 0, tot_targ = 0;
			for (vector<int>::const_iterator el = group->begin(); el != group->end(); el++) {
				tot_subj += subject->getFit().fitness[*el];
				tot_targ += target->getFit().fitness[*el];
			}
			statement &= (tot_targ < (tot_subj + conf->crowding_dist)) & (tot_targ > (tot_subj - conf->crowding_dist));
		}
		group = softgroup.begin();
		ite = softgroup.end();
		i++;
	} while (i < 2);
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
/*
 * updates best Individual if given idx is better than old one.
 */
inline void Population::update_bestInd(int idx) {
	bool res = false;
	switch (HC_whichFit) {
		case hc_soft:
			res = (pop[idx]->getFit().soft_fit <= pop[pareto_bestID]->getFit().soft_fit) &&
			(pop[idx]->getFit().total_fit < pop[pareto_bestID]->getFit().total_fit);
			break;
		case hc_hard:
			res = (pop[idx]->getFit().hard_fit <= pop[pareto_bestID]->getFit().hard_fit) &&
			(pop[idx]->getFit().total_fit < pop[pareto_bestID]->getFit().total_fit);
			break;
		default:
			res = (pop[idx]->getFit().hard_fit <= pop[pareto_bestID]->getFit().hard_fit) &&
				(pop[idx]->getFit().total_fit < pop[pareto_bestID]->getFit().total_fit);
			break;
	}
	if (res) {
		pareto_bestID = idx;
		pareto_minHFit = pop[pareto_bestID]->getFit().hard_fit;
		//if we reach 0 hard fitness, change HC to improve hard and soft.
		if (pareto_minHFit == 0) {
			HC_whichFit = hc_both;
		}
		printer->print(pop[pareto_bestID], stats);
	}
}

} /* namespace std */
