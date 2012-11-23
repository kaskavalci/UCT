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

Population::Population(Common *conf) {
	int i;
	//TODO: timer start - stop iþlemini mainde yap.
	time(&start);
	this->conf = conf;
	crossel1 = crossel2 = 0;

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

	for (i = 0; i < POPUL; ++i) {
		pop.push_back(Individual(conf));
		pop[i].buildtimetable();
		pop[i].updatefitness();
		add_to_pareto(i);
	}
}

Population::~Population() {
	pop.clear();
}

void Population::hillclimbmix2() {
	int m;
	for (m = 0; m < POPUL * conf->hillsize; m++) {
		if (RND(1000) < 1000 * conf->hillboth) {
			if (pop[m].hc_worstsection()) {
				if (add_to_pareto(m)) par_suc++;
				else par_fail++;
			}
		}
	}
}
/*
 * Calculates nitch value from crowding distance variable. Iterate all individuals in population and count how many of them
 * are in crowding distance.
 */
int Population::calc_nitch(int idx) {
	Individual* subject = &pop[idx];
	Individual *target;
	int count = 0;

	int hard1l = subject->fitnessh - conf->crowding_dist, hard1h = subject->fitnessh + conf->crowding_dist;
	int hard2l = subject->fitnessh1 - conf->crowding_dist, hard2h = subject->fitnessh1 + conf->crowding_dist;
	int soft1l = subject->fitnessf - conf->crowding_dist, soft1h = subject->fitnessf + conf->crowding_dist;
	int soft2l = subject->fitnessf2 - conf->crowding_dist, soft2h = subject->fitnessf2 + conf->crowding_dist;

	for (int i = 0; i < POPUL; ++i) {
		if (i == idx) continue;
		target = &pop[i];

		if (target->fitnessh < hard1h && target->fitnessh > hard1l &&
				target->fitnessh1 < hard2h && target->fitnessh1 > hard2l &&
				target->fitnessf < soft1h && target->fitnessf > soft1l &&
				target->fitnessf2 < soft2h && target->fitnessf2 > soft2l) {
			count++;
		}
	}
	return count;
}
/*
 * Count the individuals in crowding distance in PARETO FRONT
 */
int Population::calc_crowd(int idx) {
	Individual* subject = &pop[idx];
	Individual *target;
	int count = 0;

	int hard1l = subject->fitnessh - conf->crowding_dist, hard1h = subject->fitnessh + conf->crowding_dist;
	int hard2l = subject->fitnessh1 - conf->crowding_dist, hard2h = subject->fitnessh1 + conf->crowding_dist;
	int soft1l = subject->fitnessf - conf->crowding_dist, soft1h = subject->fitnessf + conf->crowding_dist;
	int soft2l = subject->fitnessf2 - conf->crowding_dist, soft2h = subject->fitnessf2 + conf->crowding_dist;

	for (size_t i = 0; i < paretof.size(); ++i) {
		if (paretof[i] == idx) continue;
		target = &pop[paretof[i]];

		if (target->fitnessh < hard1h && target->fitnessh > hard1l &&
				target->fitnessh1 < hard2h && target->fitnessh1 > hard2l &&
				target->fitnessf < soft1h && target->fitnessf > soft1l &&
				target->fitnessf2 < soft2h && target->fitnessf2 > soft2l) {
			count++;
		}
	}
	return count;
}

void Population::tournament(Individual& parent) {
	set<int> selection_pool;
	//get unique IDs
	while (selection_pool.size() < TOURNAMENT_POOLSIZE) {
		selection_pool.insert(RND(POPUL));
	}
	//get TOURNAMENT_CANDIDATE_N unique candidates for comparation
	set<int> candidate;
	int domination[TOURNAMENT_CANDIDATE_N] = {};
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
			if (pop[*itc].dominates(&pop[*it]) == D_TRUE) {
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
	}
	else if (maxid == -1 || equal_domination) {
		int min_nitch = POPUL * 2; //give min a high number
		set<int>::iterator it_min = itec = candidate.end();
		int dummy;
		//find the candiate that have least individuals surrounding it.
		for (itc = candidate.begin(); itc != itec; ++itc) {
			if ((dummy = calc_nitch(*itc)) < min_nitch) {
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

void Population::selection(Individual&parent1, Individual&parent2) {
	tournament(parent1);
	tournament(parent2);
}

void Population::crossover() {
	int crossrate = (int) (POPUL * conf->crrate / 2);
	Individual parent1(conf), parent2(conf), child;
	child.setConf(conf);
	int old, now;
	for (int i = 0; i < crossrate; ++i) {
		//todo: selection, tournament.
		selection(parent1, parent2);
		child.cross(parent1, parent2);
		child.buildtimetable();
		child.updatefitness();
		old = child.fitnessh + child.fitnessh1;
		if (old < parent1.fitnessh + parent1.fitnessh1 && old < parent2.fitnessh + parent2.fitnessh1) {
			cross_suc++;
		}
		else cross_fail++;

		child.hc_worstsection();
		now = child.fitnessh + child.fitnessh1;
		if (now < old) hc_suc++;
		else hc_fail++;

		if (add_to_population(child)) add_suc++;
		else add_fail++;
	}
}


bool Population::add_to_pareto(int idx) {
	int domination;
	vector<int> non_dominated_ind;
	//if pareto front is empty add the individual directly
	if (paretof.size() == 0) {
		inpf3[idx] = true;
		paretof.push_back(idx);
		return true;
	}
	for (size_t i = 0; i < paretof.size(); i++) {
		domination = pop[idx].dominates(&pop[paretof[i]]);
		//candidate dominated perato front. update it.
		if (domination == D_TRUE) {
			inpf3[idx] = true;
			inpf3[paretof[i]] = false;
			paretof[i] = idx;
			return true;
		}
		//no domination. add the individuals that does not dominate each other
		else if (domination == D_NO_HARDDOMINATION){
			non_dominated_ind.push_back(i);
		}
		//candidate is dominated by some individual in pareto front. candidate is not entitled to enter pareto front
		else if (domination == D_FALSE) {
			return false;
		}
	}
	//there is no domination but hard fitness are not dominated by candidate. We don't want hard fitness to increase.
	if (non_dominated_ind.size() == 0) {
		return false;
	}
	//we reach this line if there is no domination. add candidate to pareto front if we have enough space
	if (paretof.size() < PARETO_SIZE && non_dominated_ind.size() > 0) {
		inpf3[idx] = true;
		paretof.push_back(idx);
		return true;
	}
	//pareto front is full and candidate cannot be dominated by pareto front. Find a suitable individual from pareto front
	//that have crowded area.
	if (paretof.size() >= PARETO_SIZE && non_dominated_ind.size() > 0 ) {
		int max = 0, maxid = -1, dummy;
		for (size_t i = 0; i < non_dominated_ind.size(); ++i) {
			if ((dummy = calc_crowd(paretof[non_dominated_ind[i]])) > max) {
				max = dummy;
				maxid = non_dominated_ind[i];
			}
		}
		//we found something!
		if (max != 0) {
			if (pop[paretof[maxid]].fitnessh + pop[paretof[maxid]].fitnessh1 < pop[idx].fitnessh + pop[idx].fitnessh1) {
				cerr << "ERROR PARETO FRONT DEFORMATION" << endl;
			}
			inpf3[idx] = true;
			inpf3[paretof[maxid]] = false;
			paretof[maxid] = idx;
			return true;
		}
		//we got nothing. pareto front is excellent and full. sorry new guy, you won't be VIP for this party
		else {
			return false;
		}
	}
	else {
		cerr << "Unhandled condition. Population::add_to_pareto";
		return false;
	}
}


//adds the given individual to population and update pareto.
bool Population::add_to_population(Individual& candidate) {
	for (int i = 0; i < POPUL; ++i) {
		if (!inpf3[i]) {
			if (candidate.dominates(&pop[i]) == D_TRUE) {
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
		pop[rnd_pos] = candidate;
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
		mutator.setChromosome(pop[i].getChromosome());
		//if any mutation occured, re-build timetable.
		if (mutator.mutate_all()) {
			pop[i].buildtimetable();
			pop[i].updatefitness();
			if (add_to_pareto(i))
				mut_suc++;
			else mut_fail++;
		}
	}
}
/*
 * depreciated. use selection instead
 */
void Population::selection_old(Individual&parent1, Individual&parent2) {
	int sel1, sel2;
	sel1 = RND(POPUL);
	sel2 = RND(POPUL);
	while (sel1 == sel2) {
		sel2 = RND(POPUL);
	}
	Individual p1 = pop[sel1], p2 = pop[sel2];

	if (p1.dominates(&p2) == D_TRUE) {
		parent1 = p1;
		crossel1 = sel1;
	} else if (p1.dominates(&p2) == D_FALSE) {
		parent1 = p2;
		crossel1 = sel2;
	} else if (RND(100) < 50) {
		parent1 = p1;
		crossel1 = sel1;
	} else {
		parent1 = p2;
		crossel1 = sel2;
	}
	sel1 = RND(POPUL);
	sel2 = RND(POPUL);
	while (sel1 == sel2) {
		sel2 = RND(POPUL);
	}
	Individual p3 = pop[sel1], p4 = pop[sel2];
	if (p3.dominates(&p4) == D_TRUE) {
		parent2 = p3;
		crossel2 = sel1;
	} else if (p3.dominates(&p4) == D_FALSE) {
		parent2 = p4;
		crossel2 = sel2;
	} else if (RND(100) < 50) {
		parent2 = p3;
		crossel2 = sel1;
	} else {
		parent2 = p4;
		crossel2 = sel2;
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
	int smallest = 20000, smallestidx = -1;
	int prevtime = 10;
	double duration = getduration();
	int fit;
	size_t m;

	int fit2, fit4, fith1, fith0;
	int fit6 = 20000;
	if (conf->verbose_level == 2) {
		cout << "id\tfitH1\tfitH2\tfitS1\tfitS2\tCR+\tCR-\tHC+\tHC-\tADD+\tADD-\tPAR+\tPAR-\tMUT+\tMUT-" << endl;
		for (m = 0; m < POPUL; m++) {
			fit = pop[m].fitnessh + pop[m].fitnessh1;
			fith0 = pop[m].fitnessh;
			fith1 = pop[m].fitnessh1;
			fit2 = pop[m].fitnessf;
			fit4 = pop[m].fitnessf2;
			if (fit < smallest) {
				smallest = fit;
				smallestidx = m;
				fit6 = fit + fit2 + fit4;
			} else if (fit <= smallest && fit + fit2 + fit4 < fit6) {
				smallest = fit;
				smallestidx = m;
				fit6 = fit + fit2 + fit4;
			}
		}
		cout << smallestidx << "\t" << pop[smallestidx].fitnessh << "\t" << pop[smallestidx].fitnessh1 << "\t" << pop[smallestidx].fitnessf << "\t"
				<< pop[smallestidx].fitnessf2 << endl;;
	}
	while ((int) duration <= conf->dur) {
		it++;
		duration = getduration();
		if ((int) duration % 10 == 0 && prevtime == (int) duration) {
			prevtime = prevtime + 10;
		}
		if (it % 20 == 0) {
			smallest = 2000;
			smallestidx = -1;
			for (m = 0; m < paretof.size(); m++) {
				fit = pop[paretof[m]].fitnessh + pop[paretof[m]].fitnessh1;
				fith0 = pop[paretof[m]].fitnessh;
				fith1 = pop[paretof[m]].fitnessh1;
				fit2 = pop[paretof[m]].fitnessf;
				fit4 = pop[paretof[m]].fitnessf2;
				if (conf->verbose_level == 1) {
					cout << "id: " << setw(2) << m << "\tfith1:" << setw(4) << fith0 << "\tfith2:" << setw(4) << fith1 <<
						"\tfits1:" << setw(4) << fit2 << "\tfits2:" << setw(4) << fit4 << endl;
				}
				if (fit < smallest) {
					smallest = fit;
					smallestidx = m;
					fit6 = fit + fit2 + fit4;
				} else if (fit <= smallest && fit + fit2 + fit4 < fit6) {
					smallest = fit;
					smallestidx = m;
					fit6 = fit + fit2 + fit4;
				}
			}
			if (conf->verbose_level == 1) {
				cout << "# of iterations " << it << " duration " << duration << endl;
				cout << "# smallest " << smallestidx << endl;
			}
			else if (conf->verbose_level == 2) {
				cout << smallestidx << "\t" << pop[paretof[smallestidx]].fitnessh << "\t" << pop[paretof[smallestidx]].fitnessh1 << "\t" << pop[paretof[smallestidx]].fitnessf << "\t"
								<< pop[paretof[smallestidx]].fitnessf2 << "\t";
				print_stat();
			}
		}
		crossover();
		hillclimbmix2();
		mutation();
	}
	smallest = 2000;
	smallestidx = -1;
	for (m = 0; m < paretof.size(); m++) {
		fit = pop[paretof[m]].fitnessh + pop[paretof[m]].fitnessh1;
		fit2 = pop[paretof[m]].fitnessf;
		fit4 = pop[paretof[m]].fitnessf2;
		cout << "id: " << setw(2) << m << "\tfith1:" << setw(4) << pop[paretof[m]].fitnessh << "\tfith2:" << setw(4) << pop[paretof[m]].fitnessh1 <<
								"\tfits1:" << setw(4) << pop[paretof[m]].fitnessf << "\tfits2:" << setw(4) << pop[paretof[m]].fitnessf2 << " All:" << setw(4) << fit + fit2 + fit4 << endl;
		if (fit < smallest) {
			smallest = fit;
			smallestidx = m;
			fit6 = fit + fit2 + fit4;
		} else if (fit <= smallest && fit + fit2 + fit4 < fit6) {
			smallest = fit;
			smallestidx = m;
			fit6 = fit + fit2 + fit4;
		}
	}
	printf("# of iterations %d duration %d\n", it, (int) duration);
	printf("# smallest  %d \n", smallestidx);
	printf("THE SOULUTION IS  \n");
	//pop[paretof[smallestidx]].printlect();
	//pop[paretof[smallestidx]].printdekanlik();
	//pop[paretof[smallestidx]].printtable();
	fit = pop[paretof[smallestidx]].fitnessHCAL(1);
	fit = pop[paretof[smallestidx]].fitnessFCAL(1);
	fit = pop[paretof[smallestidx]].fitnessF1CAL(1);
	fit = pop[paretof[smallestidx]].fitnessF2CAL(1);
	fit = pop[paretof[smallestidx]].fitnessF3CAL(1);
	for (m = 0; m < paretof.size(); m++) {
		printf("THE SOULUTION IS %d \n", m);
		//pop[paretof[m]].printlect();
		//pop[paretof[m]].printdekanlik();
		//pop[paretof[m]].printtable();
		fit = pop[paretof[m]].fitnessHCAL(1);
		fit = pop[paretof[m]].fitnessFCAL(1);
		fit = pop[paretof[m]].fitnessF1CAL(1);
		fit = pop[paretof[m]].fitnessF2CAL(1);
		fit = pop[paretof[m]].fitnessF3CAL(1);
	}
	fstream fresult;
	fresult.open("result.txt", fstream::app | fstream::out);
	fresult << "hard1:" << pop[paretof[smallestidx]].fitnessh << "\thard2:" << pop[paretof[smallestidx]].fitnessh1
			<< "\tsoft1:" << pop[paretof[smallestidx]].fitnessf << "\tsoft2:" << pop[paretof[smallestidx]].fitnessf2
			<< "\tduration: " << duration << "\thillrnd: " << conf->hillrnd << "\thillboth:" << conf->hillboth
			<< "\tmutrate:" << conf->mutg1rate << "\tcrrate:" << conf->crrate << "\tinsertRate:" << conf->insert_popul_rate
			<< "\tpspace:" << conf->crowding_dist  << "\tseed:" << seed << endl;
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
	cout << mut_suc << "\t" << mut_fail	 << endl;
}

} /* namespace std */
