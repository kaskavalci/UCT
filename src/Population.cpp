/*
 * Population.cpp
 *
 *  Created on: 21 Eki 2012
 *      Author: HalilCan
 */

#include "Population.h"
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <stdio.h>
#include "Mutation.h"
#include <set>
#include <iomanip>

namespace std {

Population::Population() {
	int i;
	//TODO: timer start - stop iþlemini mainde yap.
	time(&start);
	this->conf = Common::getConf();
	crossel1 = crossel2 = 0;

	updatePareto = false;
	cross_suc = 0;
	cross_fail = 0;
	add_suc = 0;
	add_fail = 0;
	hc_suc = 0;
	hc_fail = 0;
	par_suc = 0;
	par_fail = 0;
	mut_suc = 0;
	mut_fail = 0;
	pareto_bestID = -1;
	pareto_minHFit = 2000;
	pareto_minSFit = 2000;
	for (i = 0; i < POPUL; ++i) {
		population.push_back(i);
		inpf3[i] = false;
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
					par_suc++;
				else
					par_fail++;
			}
		}
	}
}
/*
 * Count the individuals in crowding distance in given list. It could be perato front or whole population.
 */
int Population::calc_crowd(int idx, const vector<int> &list) {
	int count = 0;
	bool statement = true;

	vector<int>::const_iterator ite = list.end();
	for (vector<int>::const_iterator it = list.begin(); it != ite; ++it) {
		if (*it == idx)
			continue;
		statement &= crowd_condition(conf->hardgroup, pop[idx], pop[*it]);
		statement &= crowd_condition(conf->softgroup, pop[idx], pop[*it]);
		if (statement)
			count++;
	}
	return count;
}

void Population::tournament(Individual* parent) {
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
	set<int>::iterator ite = selection_pool.end();
	set<int>::iterator itc;
	set<int>::iterator itec = candidate.end();
	for (set<int>::iterator it = selection_pool.begin(); it != ite; ++it) {
		i = 0;
		for (itc = candidate.begin(); itc != itec; ++itc, ++i) {
			if (pop[*itc]->dominates(pop[*it]) == D_TRUE) {
				domination[i]++;
			}
		}
	}
	int max = 0, maxid = -1;
	bool equal_domination = false;
	for (i = 0; i < TOURNAMENT_CANDIDATE_N; ++i) {
		if (domination[i] > max) {
			max = domination[i];
			maxid = i;
		}
		//we have individuals that have same domination values.
		else if (domination[i] == max) {
			equal_domination = true;
		}
	}
	itc = candidate.begin();
	if (maxid != -1) {
		advance(itc, maxid);
		parent = pop[*itc];
		return;
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
			parent = pop[*itc];
		}
		//This is odd. We should have found something. For the sake of stability, select a random candidate
		else {
			cerr << "No suitable candidate from tournament." << endl;
			itc = candidate.begin();
			advance(itc, RND(TOURNAMENT_CANDIDATE_N));
			parent = pop[*itc];
		}
	}
}

void Population::selection(Individual*parent1, Individual*parent2) {
	tournament(parent1);
	tournament(parent2);
}

void Population::crossover() {
	int crossrate = (int) (POPUL * conf->crrate / 2);
	Individual *parent1 = new Individual(conf), *parent2 = new Individual(conf), *child = NULL;
	bool create_child = true;
	int old, now;
	for (int i = 0; i < crossrate; ++i) {
		if (create_child) {
			child = new Individual();
			child->setConf(conf);
			create_child = false;
		}

		selection(parent1, parent2);
		child->cross(parent1, parent2);
		child->buildtimetable();
		child->updatefitness(0);
		old = child->getHardFit().total_fit;
		if (old < parent1->getHardFit().total_fit && old < parent2->getHardFit().total_fit) {
			cross_suc++;
		} else
			cross_fail++;

		child->hc_worstsection(hc_hard);
		now = child->getHardFit().total_fit;
		if (now < old)
			hc_suc++;
		else
			hc_fail++;

		if (add_to_population(child)) {
			create_child = true;
			add_suc++;
		} else {
			add_fail++;
		}
	}
	if (!create_child) {
		delete (child);
	}
	delete (parent1);
	delete (parent2);
}

bool Population::add_to_pareto(int idx) {
	int domination;
	vector<int> non_dominated_ind;
	//if individual is already in pareto front, return true
	if (inpf3[idx] == true) {
		updatePareto = true;
		return true;
	}
	//if pareto front is empty add the individual directly
	if (paretof.size() == 0) {
		inpf3[idx] = true;
		paretof.push_back(idx);
		pareto_minHFit = pop[idx]->getHardFit().total_fit;
		pareto_minSFit = pop[idx]->getSoftFit().total_fit;
		pareto_bestID = idx;
		return true;
	}
	for (size_t i = 0; i < paretof.size(); i++) {
		domination = pop[idx]->dominates(pop[paretof[i]]);
		//candidate dominated perato front. update it.
		if (domination == D_TRUE) {
			if (pop[idx]->getHardFit().total_fit < pareto_minHFit
					|| (pop[idx]->getHardFit().total_fit <= pareto_minHFit
							&& (pop[idx]->getHardFit().total_fit + pop[idx]->getSoftFit().total_fit
									< pareto_minHFit + pareto_minSFit))) {
				pareto_minHFit = pop[idx]->getHardFit().total_fit;
				pareto_minSFit = pop[idx]->getSoftFit().total_fit;
				pareto_bestID = idx;
			}
			inpf3[idx] = true;
			inpf3[paretof[i]] = false;
			paretof[i] = idx;
			updatePareto = true;
			return true;
		}
		//no domination. add the individuals that does not dominate each other
		else if (pareto_bestID != (int) i && (domination == D_NO_HARDDOMINATION || domination == D_NO_DOMINATION)) {
			non_dominated_ind.push_back(i);
		}
		//candidate is dominated by some individual in pareto front. candidate is not entitled to enter pareto front
		else if (domination == D_FALSE) {
			return false;
		}
	}
	//there is no domination but fitness are not dominated by candidate. We don't want hard fitness to increase.
	if (non_dominated_ind.size() == 0) {
		return false;
	}
	//we reach this line if there is no domination. add candidate to pareto front if we have enough space
	if (paretof.size() < PARETO_SIZE && non_dominated_ind.size() > 0) {
		if (pop[idx]->getHardFit().total_fit < pareto_minHFit
				|| (pop[idx]->getHardFit().total_fit <= pareto_minHFit
						&& (pop[idx]->getHardFit().total_fit + pop[idx]->getSoftFit().total_fit
								< pareto_minHFit + pareto_minSFit))) {
			pareto_minHFit = pop[idx]->getHardFit().total_fit;
			pareto_minSFit = pop[idx]->getSoftFit().total_fit;
			pareto_bestID = idx;
		}
		inpf3[idx] = true;
		paretof.push_back(idx);
		updatePareto = true;
		return true;
	}
	//pareto front is full and candidate cannot be dominated by pareto front. Find a suitable individual from pareto front
	//that have crowded area.
	if (paretof.size() >= PARETO_SIZE && non_dominated_ind.size() > 0) {
		int max = 0, maxid = -1, dummy;
		for (size_t i = 0; i < non_dominated_ind.size(); ++i) {
			if ((dummy = calc_crowd(paretof[non_dominated_ind[i]], paretof)) > max) {
				max = dummy;
				maxid = non_dominated_ind[i];
			}
		}
		//we found something!
		if (max != 0) {
			if (pop[paretof[maxid]]->getHardFit().total_fit + pop[paretof[maxid]]->getSoftFit().total_fit
					< pop[idx]->getHardFit().total_fit + pop[idx]->getSoftFit().total_fit) {
				cerr << "ERROR PARETO FRONT DEFORMATION" << endl;
			}
			if (pop[idx]->getHardFit().total_fit < pareto_minHFit
					|| (pop[idx]->getHardFit().total_fit <= pareto_minHFit
							&& (pop[idx]->getHardFit().total_fit + pop[idx]->getSoftFit().total_fit
									< pareto_minHFit + pareto_minSFit))) {
				pareto_minHFit = pop[idx]->getHardFit().total_fit;
				pareto_minSFit = pop[idx]->getSoftFit().total_fit;
				pareto_bestID = idx;
			}
			inpf3[idx] = true;
			inpf3[paretof[maxid]] = false;
			paretof[maxid] = idx;
			updatePareto = true;
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
			if (candidate->dominates(pop[i]) == D_TRUE) {
				delete (pop[i]);
				pop[i] = candidate;
				if (add_to_pareto(i))
					par_suc++;
				return true;
			}
		}
	}
	/* if we have reached this line, then there is no domination. give a slight chance for
	 * weak individual "candidate" to be inserted into population. No need to try for adding
	 * to pareto front since this individual cannot dominate any individual in the population.
	 */
	par_fail++;
	if (RND(1000) < 1000 * conf->insert_popul_rate) {
		int rnd_pos;
		//don't touch to pareto front.
		do {
			rnd_pos = RND(POPUL);
		} while (inpf3[rnd_pos]);
		delete (pop[rnd_pos]);
		pop[rnd_pos] = candidate;
		return true;
	}
	return false;
}
/*
 * mutate every individual with a rate of given mutation rates, except for pareto front.
 * mutator gets the probability from Common.h, so calling mutator does not imply mutation.
 * If there is a mutation mutate_all() will return true.
 */
void Population::mutation() {
	Mutation mutator(conf);
	for (int i = 0; i < POPUL; ++i) {
		if (inpf3[i])
			continue;
		mutator.setChromosome(pop[i]->getChromosome());
		//if any mutation occured, re-build timetable.
		if (mutator.mutate_all()) {
			pop[i]->buildtimetable();
			pop[i]->updatefitness(0);
			if (add_to_pareto(i))
				mut_suc++;
			else
				mut_fail++;
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

//TODO: burayý düzenle. bastýrma iþlemini io'da yaptýr.
void Population::run(int seed) {
	int it = 0;
	double duration = getduration();
	size_t m;
	if (conf->verbose_level == 2 || conf->verbose_level == 4) {
		print_header(cout);
		print_fitness(cout, pop[pareto_bestID]);
		cout << endl;
	}
	while ((int) duration <= conf->dur) {
		it++;
		duration = getduration();
		if (updatePareto) {
			if (conf->verbose_level == 3) {
				cout << "id" << pareto_bestID << "\tMin Hard:" << pareto_minHFit << "\tMin Soft:" << pareto_minSFit << endl;
				for (size_t i = 0; i < paretof.size(); i++) {
					cout << "id\t";
					print_fitness(cout, pop[paretof[i]]);
				}
			} else if (conf->verbose_level == 4) {
				print_fitness(cout, pop[pareto_bestID]);
				print_stat();
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
				print_fitness(cout, pop[pareto_bestID]);
				print_stat();
			}
		}
		crossover();
		//hillclimbmix2();
		mutation();
	}

	if (conf->verbose_level == 1 || conf->verbose_level == 3) {
		for (size_t i = 0; i < paretof.size(); i++) {
			cout << "id\t";
			print_fitness(cout, pop[paretof[i]]);
		}
	}

	printf("# of iterations %d duration %d\n", it, (int) duration);
	printf("# smallest  %d \n", pareto_bestID);
	printf("THE SOULUTION IS  \n");
	//pop[paretof[smallestidx]]->printlect();
	//pop[paretof[smallestidx]]->printdekanlik();
	//pop[paretof[smallestidx]]->printtable();
	pop[pareto_bestID]->updatefitness(1);
	for (m = 0; m < paretof.size(); m++) {
		printf("THE SOULUTION IS %d \n", m);
		//pop[paretof[m]]->printlect();
		//pop[paretof[m]]->printdekanlik();
		//pop[paretof[m]]->printtable();
		pop[paretof[m]]->updatefitness(1);
	}
	fstream fresult;
	fresult.open("result.txt", fstream::app | fstream::out);
	print_fitness(fresult, pop[pareto_bestID]);
	fresult << "\tduration: " << duration << "\thillrnd: " << conf->hillrnd << "\thillboth:" << conf->hillboth
			<< "\tmutrate:" << conf->mutg1rate << "\tcrrate:" << conf->crrate << "\tinsertRate:"
			<< conf->insert_popul_rate << "\tpspace:" << conf->crowding_dist << "\tseed:" << seed << endl;
	print_stat();
	fresult.flush();
	fresult.close();
	duration = getduration();
	printf("\n\nThe operation completed in %.2lf seconds.\n", duration);
}

void Population::print_stat() {
	cout << cross_suc << "\t" << cross_fail << "\t";
	cout << hc_suc << "\t" << hc_fail << "\t";
	cout << add_suc << "\t" << add_fail << "\t";
	cout << par_suc << "\t" << par_fail << "\t";
	cout << mut_suc << "\t" << mut_fail << endl;
}

void Population::print_fitness(ostream &fresult, Individual *subject) {
	fresult << subject->getHardFit().total_fit + subject->getSoftFit().total_fit << "\t";
	for (int i = 0; i < HARD_FIT_N; ++i) {
		fresult << subject->getHardFit().fitness[i] << "\t";
	}
	for (int i = 0; i < SOFT_FIT_N; ++i) {
		fresult << subject->getSoftFit().fitness[i] << "\t";
	}
}

void Population::print_header(ostream &out) {
	out << "fitTotal\t";
	for (int i = 0; i < HARD_FIT_N; ++i) {
		out << "hard[" << i << "]\t";
	}
	for (int i = 0; i < SOFT_FIT_N; ++i) {
		out << "soft[" << i << "]\t";
	}
	out << "CR+\tCR-\tHC+\tHC-\tADD+\tADD-\tPAR+\tPAR-\tMUT+\tMUT-" << endl;
}

bool Population::crowd_condition(vector<vector<int> > grup_list, Individual *subject, Individual *target) {
	bool statement = true;
	for (vector<vector<int> >::const_iterator group = grup_list.begin(); group != grup_list.end(); ++group) {
		int tot_subj = 0, tot_targ = 0;
		for (vector<int>::const_iterator el = group->begin(); el != group->end(); el++) {
			tot_subj += subject->getHardFit().fitness[*el];
			tot_targ += target->getHardFit().fitness[*el];
		}
		statement &= (tot_targ < (tot_subj + conf->crowding_dist)) & (tot_targ > (tot_subj - conf->crowding_dist));
	}
	return statement;
}

} /* namespace std */
