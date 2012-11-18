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

namespace std {

Population::Population(Common *conf) {
	int i;
	//TODO: timer start - stop iþlemini mainde yap.
	time(&start);
	this->conf = conf;
	crossel1 = crossel2 = 0;
	for (i = 0; i < POPUL; ++i) {
		pop.push_back(Individual(conf));
		pop[i].buildtimetable();
	}
	initpareto();
}

Population::~Population() {
	pop.clear();
}

void Population::hillclimbmix2() {
	int m;
	for (m = 0; m < POPUL * conf->hillsize; m++) {
		if (RND(1000) < 1000 * conf->hillboth) {
			pop[m].hc_amap();
		}
	}
}

void Population::selection(Individual&parent1, Individual&parent2) {
	//select
}

void Population::crossover() {
	int crossrate = (int) (POPUL * conf->crrate / 2);
	Individual parent1(conf), parent2(conf), child(conf);

	for (int i = 0; i < crossrate; ++i) {
		//todo: selection, tournament.
		selection_old(parent1, parent2);
		child.cross(parent1, parent2);
		child.hc_amap();
		child.buildtimetable();
		add_new_individual(child);
	}
}

//adds the given individual to population and update pareto.
void Population::add_new_individual(Individual& candidate) {
	for (int i = 0; i < POPUL; ++i) {
		if (candidate.dominates(&pop[i]) == D_TRUE) {
			if (!inpf3[i]) {
				pop[i] = candidate;
				initpareto();
				return;
			}
		}

	}
	//if we have reached this line, then there is no domination. give a slight chance for
	//weak individual "candidate" to be inserted into population.
	if (RND(1000) < 1000 * conf->insert_popul_rate) {
		int rnd_pos;
		//don't touch to pareto front.
		do {
			rnd_pos = RND(POPUL);
		} while (inpf3[rnd_pos]);
		pop[rnd_pos] = candidate;
	}
}
/*
 * mutate every individual with a rate of given mutation rates, except for pareto front.
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
		}
	}
}

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
/*
 * clear and re-construct pareto again. Individuals that have too close fitness values
 * are vulnerable to change. This is done to preserve diversity.
 */
void Population::initpareto() {
	int i;
	bool domination;
	size_t j;

	paretof.clear();
	while (paretof.size() != 0) {
		for (j = 0; j < paretof.size(); j++)
			paretof.pop_back();
	}
	paretof.push_back(0);

	for (i = 0; i < POPUL; i++) {
		inpf3[i] = false;
	}
	for (i = 1; i < POPUL; i++) {
		domination = false;
		for (j = 0; j < paretof.size(); j++) {
			if (pop[i].dominates(&pop[paretof[j]]) == D_TRUE) {
				inpf3[i] = true;
				inpf3[paretof[j]] = false;
				domination = true;
				paretof[j] = i;
				break;
			}
			if (pop[i].dominates(&pop[paretof[j]]) == D_FALSE) {
				domination = true;
				break;
			}
		}
		if (!domination) {
			if (paretof.size() < 2 * POPUL / 3) {
				inpf3[i] = true;
				paretof.push_back(i);
			} else {
				for (size_t k = 0; k < paretof.size(); ++k) {
					for (size_t m = k + 1; m < paretof.size(); ++m) {
						//pareto front contains elements that are too close to each other.
						//pareto front should have diversity, thus we replace it with new Individual
						if (pop[paretof[k]].dominates(&pop[paretof[m]]) == D_IN_RANGE) {
							pop[paretof[k]] = pop[i];
						}
					}
				}
			}
		}
	}
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
	ofstream fresult("result.txt", ios::app);

	while ((int) duration <= conf->dur) {
		it++;
		duration = getduration();
		if ((int) duration % 10 == 0 && prevtime == (int) duration) {
			prevtime = prevtime + 10;
		}
		int fit2, fit4, fith1, fith0;
		smallest = 20000;
		smallestidx = -1;
		int fit6 = 20000;
		if (it % 20 == 0) {
			for (m = 0; m < paretof.size(); m++) {
				fit = pop[paretof[m]].fitnessh + pop[paretof[m]].fitnessh1;
				fith0 = pop[paretof[m]].fitnessh;
				fith1 = pop[paretof[m]].fitnessh1;
				fit2 = pop[paretof[m]].fitnessf;
				fit4 = pop[paretof[m]].fitnessf2;
				printf("fitness: %d \tfith: %d\tfith1: %d \tfitf: %d \tfitf1: %d\n", m, fith0, fith1, fit2,
						fit4);
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
		}
		crossover();
		hillclimbmix2();
		mutation();
		initpareto();
	}

	int fit2, fit4, fit6 = 20000;
	fit6 = 20000;
	for (m = 0; m < paretof.size(); m++) {
		fit = pop[paretof[m]].fitnessh + pop[paretof[m]].fitnessh1;
		fit2 = pop[paretof[m]].fitnessf;
		fit4 = pop[paretof[m]].fitnessf2;
		printf("fitness: %d \tfith Total: %d\tfitf1: %d\tfitf2: %d\tAll:: %d\n", m, fit, fit2, fit4,
				fit + fit2 + fit4);
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
	pop[paretof[smallestidx]].printdekanlik();
	pop[paretof[smallestidx]].printtable();
	fit = pop[paretof[smallestidx]].fitnessHCAL(1);
	fit = pop[paretof[smallestidx]].fitnessFCAL(1);
	fit = pop[paretof[smallestidx]].fitnessF1CAL(1);
	fit = pop[paretof[smallestidx]].fitnessF2CAL(1);
	fit = pop[paretof[smallestidx]].fitnessF3CAL(1);
	for (m = 0; m < paretof.size(); m++) {
		printf("THE SOULUTION IS %d \n", m);
		//pop[paretof[m]].printlect();
		pop[paretof[m]].printdekanlik();
		pop[paretof[m]].printtable();
		fit = pop[paretof[m]].fitnessHCAL(1);
		fit = pop[paretof[m]].fitnessFCAL(1);
		fit = pop[paretof[m]].fitnessF1CAL(1);
		fit = pop[paretof[m]].fitnessF2CAL(1);
		fit = pop[paretof[m]].fitnessF3CAL(1);
	}
	fresult << "hard1:" << pop[paretof[smallestidx]].fitnessh << " hard2:" << pop[paretof[smallestidx]].fitnessh1
			<< " soft1:" << pop[paretof[smallestidx]].fitnessf << " soft2:" << pop[paretof[smallestidx]].fitnessf2
			<< " duration: " << duration << "hillrnd: " << conf->hillrnd << "hillboth:" << conf->hillboth
			<< "mutrate:" << conf->mutg1rate << " crrate:" << conf->crrate << " insertRate:" << conf->insert_popul_rate
			<< "pspace:" << conf->paretof_pspace  << " seed:" << seed << endl;
	duration = getduration();
	printf("\n\nThe operation completed in %.2lf seconds.\n", duration);
	fresult.close();
}

} /* namespace std */
