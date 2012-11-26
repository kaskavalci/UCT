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
	for (i = 0; i < POPUL; ++i) {
		inpf3[i] = false;
		pop.push_back(new Individual(conf));
		pop[i]->buildtimetable();
		pop[i]->updatefitness();
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
			if (pop[i]->hc1()) {
				if (add_to_pareto(i))
					par_suc++;
				else
					par_fail++;
			}
		}
	}
}
/*
 * Calculates nitch value from crowding distance variable. Iterate all individuals in population and count how many of them
 * are in crowding distance.
 */
int Population::calc_nitch(int idx) {
	Individual* subject = pop[idx];
	Individual *target;
	int count = 0;

	int hard1l = subject->fitnessh - conf->crowding_dist, hard1h = subject->fitnessh + conf->crowding_dist;
	int hard2l = subject->fitnessh1 - conf->crowding_dist, hard2h = subject->fitnessh1 + conf->crowding_dist;
	int soft1l = subject->fitnessf - conf->crowding_dist, soft1h = subject->fitnessf + conf->crowding_dist;
	int soft2l = subject->fitnessf2 - conf->crowding_dist, soft2h = subject->fitnessf2 + conf->crowding_dist;

	for (int i = 0; i < POPUL; ++i) {
		if (i == idx)
			continue;
		target = pop[i];

		if (target->fitnessh < hard1h && target->fitnessh > hard1l && target->fitnessh1 < hard2h
				&& target->fitnessh1 > hard2l && target->fitnessf < soft1h && target->fitnessf > soft1l
				&& target->fitnessf2 < soft2h && target->fitnessf2 > soft2l) {
			count++;
		}
	}
	return count;
}
/*
 * Count the individuals in crowding distance in PARETO FRONT
 */
int Population::calc_crowd(int idx) {
	Individual* subject = pop[idx];
	Individual *target;
	int count = 0;

	int hard1l = subject->fitnessh - conf->crowding_dist, hard1h = subject->fitnessh + conf->crowding_dist;
	int hard2l = subject->fitnessh1 - conf->crowding_dist, hard2h = subject->fitnessh1 + conf->crowding_dist;
	int soft1l = subject->fitnessf - conf->crowding_dist, soft1h = subject->fitnessf + conf->crowding_dist;
	int soft2l = subject->fitnessf2 - conf->crowding_dist, soft2h = subject->fitnessf2 + conf->crowding_dist;

	for (size_t i = 0; i < paretof.size(); ++i) {
		if (paretof[i] == idx)
			continue;
		target = pop[paretof[i]];

		if (target->fitnessh < hard1h && target->fitnessh > hard1l && target->fitnessh1 < hard2h
				&& target->fitnessh1 > hard2l && target->fitnessf < soft1h && target->fitnessf > soft1l
				&& target->fitnessf2 < soft2h && target->fitnessf2 > soft2l) {
			count++;
		}
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

void Population::selection(Individual*parent1, Individual*parent2) {
	tournament(parent1);
	tournament(parent2);
}

void Population::crossover() {
	int crossrate = (int) (POPUL * conf->crrate / 2);
	Individual *parent1 = new Individual(conf), *parent2 = new Individual(conf), *child;
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
		child->updatefitness();
		old = child->fitnessh + child->fitnessh1;
		if (old < parent1->fitnessh + parent1->fitnessh1 && old < parent2->fitnessh + parent2->fitnessh1) {
			cross_suc++;
		} else
			cross_fail++;

		child->hc_worstsection();
		now = child->fitnessh + child->fitnessh1;
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
		return true;
	}
	for (size_t i = 0; i < paretof.size(); i++) {
		domination = pop[idx]->dominates(pop[paretof[i]]);
		//candidate dominated perato front. update it.
		if (domination == D_TRUE) {
			if (pop[paretof[i]]->fitnessh + pop[paretof[i]]->fitnessh1 < pop[idx]->fitnessh + pop[idx]->fitnessh1) {
				cerr << "true domination " << pop[paretof[i]]->fitnessh + pop[paretof[i]]->fitnessh1 << "\t"
						<< pop[idx]->fitnessh + pop[idx]->fitnessh1 << endl;
			}
			inpf3[idx] = true;
			inpf3[paretof[i]] = false;
			paretof[i] = idx;
			updatePareto = true;
			return true;
		}
		//no domination. add the individuals that does not dominate each other
		else if (domination == D_NO_HARDDOMINATION) {
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
		updatePareto = true;
		return true;
	}
	//pareto front is full and candidate cannot be dominated by pareto front. Find a suitable individual from pareto front
	//that have crowded area.
	if (paretof.size() >= PARETO_SIZE && non_dominated_ind.size() > 0) {
		int max = 0, maxid = -1, dummy;
		for (size_t i = 0; i < non_dominated_ind.size(); ++i) {
			if ((dummy = calc_crowd(paretof[non_dominated_ind[i]])) > max) {
				max = dummy;
				maxid = non_dominated_ind[i];
			}
		}
		//we found something!
		if (max != 0) {
			if (pop[paretof[maxid]]->fitnessh + pop[paretof[maxid]]->fitnessh1
					< pop[idx]->fitnessh + pop[idx]->fitnessh1) {
				cerr << "ERROR PARETO FRONT DEFORMATION" << endl;
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
			pop[i]->updatefitness();
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
	int smallest = 20000, smallestidx = -1;
	int prevtime = 10;
	double duration = getduration();
	size_t m;
	if (conf->verbose_level == 2 || conf->verbose_level == 4) {
		cout << "id\tfitTotal\tfitH1\tfitH2\tfitS1\tfitS2\tCR+\tCR-\tHC+\tHC-\tADD+\tADD-\tPAR+\tPAR-\tMUT+\tMUT-"
				<< endl;
		smallestidx = get_smallest_hfit();
		print_fit(smallestidx);
		cout << endl;
	}
	while ((int) duration <= conf->dur) {
		it++;
		duration = getduration();
		if ((int) duration % 10 == 0 && prevtime == (int) duration) {
			prevtime = prevtime + 10;
		}
		if (updatePareto && (conf->verbose_level == 3 || conf->verbose_level == 4)) {
			smallestidx = get_smallest_hfit();
			smallest = pop[paretof[smallestidx]]->fitnessh + pop[paretof[smallestidx]]->fitnessh1;

			if (conf->verbose_level == 3) {
				cout << "id" << smallestidx << "\tmin:" << smallest << endl;
			} else if (conf->verbose_level == 4) {
				print_fit(paretof[smallestidx]);
				print_stat();
			}
			updatePareto = false;
		}
		if (it % 20 == 0 && (conf->verbose_level == 1 || conf->verbose_level == 2)) {
			smallestidx = get_smallest_hfit();

			if (conf->verbose_level == 1) {
				cout << "# of iterations " << it << " duration " << duration << endl;
				cout << "# smallest " << smallestidx << endl;
			} else if (conf->verbose_level == 2) {
				print_fit(paretof[smallestidx]);
				print_stat();
			}
		}
		crossover();
		hillclimbmix2();
		mutation();
	}

	smallestidx = get_smallest_hfit();
	smallest = pop[paretof[smallestidx]]->fitnessh + pop[paretof[smallestidx]]->fitnessh1;

	printf("# of iterations %d duration %d\n", it, (int) duration);
	printf("# smallest  %d \n", smallestidx);
	printf("THE SOULUTION IS  \n");
	//pop[paretof[smallestidx]]->printlect();
	//pop[paretof[smallestidx]]->printdekanlik();
	//pop[paretof[smallestidx]]->printtable();
	pop[paretof[smallestidx]]->fitnessHCAL(1);
	pop[paretof[smallestidx]]->fitnessFCAL(1);
	pop[paretof[smallestidx]]->fitnessF1CAL(1);
	pop[paretof[smallestidx]]->fitnessF2CAL(1);
	pop[paretof[smallestidx]]->fitnessF3CAL(1);
	for (m = 0; m < paretof.size(); m++) {
		printf("THE SOULUTION IS %d \n", m);
		//pop[paretof[m]]->printlect();
		//pop[paretof[m]]->printdekanlik();
		//pop[paretof[m]]->printtable();
		pop[paretof[m]]->fitnessHCAL(1);
		pop[paretof[m]]->fitnessFCAL(1);
		pop[paretof[m]]->fitnessF1CAL(1);
		pop[paretof[m]]->fitnessF2CAL(1);
		pop[paretof[m]]->fitnessF3CAL(1);
	}
	fstream fresult;
	fresult.open("result.txt", fstream::app | fstream::out);
	fresult << "hard1:" << pop[paretof[smallestidx]]->fitnessh << "\thard2:" << pop[paretof[smallestidx]]->fitnessh1
			<< "\tsoft1:" << pop[paretof[smallestidx]]->fitnessf << "\tsoft2:" << pop[paretof[smallestidx]]->fitnessf2
			<< "\tduration: " << duration << "\thillrnd: " << conf->hillrnd << "\thillboth:" << conf->hillboth
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

void Population::print_fit(int smallestidx) {
	int fit = pop[smallestidx]->fitnessh;
	int fit2 = pop[smallestidx]->fitnessh1;
	int fitH = fit + fit2;
	int fit3 = pop[smallestidx]->fitnessf;
	int fit4 = pop[smallestidx]->fitnessf2;
	cout << smallestidx << "\t" << fitH << "\t" << fit << "\t" << fit2 << "\t" << fit3 << "\t" << fit4 << "\t";
}

int Population::get_smallest_hfit() {
	int fitTotal = 2000, smallest = 2000;
	int smallestidx = -1;
	for (size_t m = 0; m < paretof.size(); m++) {
		int fit = pop[paretof[m]]->fitnessh + pop[paretof[m]]->fitnessh1;
		if (conf->verbose_level == 3 || conf->verbose_level == 1) {
			cout << "id: " << setw(2) << m << "\tfith1:" << setw(4) << pop[paretof[m]]->fitnessh << "\tfith2:"
					<< setw(4) << pop[paretof[m]]->fitnessh1 << "\tfits1:" << setw(4) << pop[paretof[m]]->fitnessf
					<< "\tfits2:" << setw(4) << pop[paretof[m]]->fitnessf2 << endl;
		}
		if (fit < smallest) {
			smallest = fit;
			smallestidx = m;
			fitTotal = fit + pop[paretof[m]]->fitnessf + pop[paretof[m]]->fitnessf2;
		} else if (fit <= smallest && fit + pop[paretof[m]]->fitnessf + pop[paretof[m]]->fitnessf2 < fitTotal) {
			smallest = fit;
			smallestidx = m;
			fitTotal = fit + pop[paretof[m]]->fitnessf + pop[paretof[m]]->fitnessf2;
			;
		}
	}
	return smallestidx;
}

} /* namespace std */
