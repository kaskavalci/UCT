/*
 * Population.cpp
 *
 *  Created on: 21 Eki 2012
 *      Author: HalilCan
 */

#include "Population.h"
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <stdio.h>

namespace std {

Population::Population(Common *conf) {
	int i;
	//TODO: timer start - stop i�lemini mainde yap.
	time(&start);
	this->conf = conf;
	crossel1 = crossel2 = 0;
	for (i = 0; i < POPUL; ++i) {
		pop.push_back(Individual(conf));
		ch.push_back(Individual(conf));
	}

	for (i = 0; i < POPUL; i++) {
		pop[i].buildtimetable();
		ch[i].buildtimetable();
		inpf3[i] = 0;
	}
	initpareto();
}

Population::~Population() {
	pop.clear();
	ch.clear();
}

void Population::print() {
	int i;
	for (i = 0; i < CHROML; i++)
		cout << endl;
}
void Population::crossmut3() {
	Individual parent1(conf), parent2(conf), child1(conf), child2(conf);
	int i;
	int crossrate = (int) (POPUL * conf->crrate / 2);
	for (i = 0; i < POPUL; i++) {
		pop[i].copyindividual(pop[i], ch[i]);
	}
	for (i = 0; i < crossrate; i++) {
		selection2(parent1, parent2);
		child1.cross(parent1, parent2, child1, child2);
		child1.copyindividual(child1, ch[crossel1]);
		child2.copyindividual(child2, ch[crossel2]);
	}
	for (i = 0; i < POPUL; i++) {
		ch[i].mutate();
		ch[i].mutateg1();
		ch[i].mutateg3();
		ch[i].mutateg5();
		ch[i].buildtimetable();
		ch[i].fitnessHCAL(0);
		ch[i].fitnessFCAL(0);
		ch[i].fitnessF1CAL(0);
		ch[i].fitnessF2CAL(0);
		ch[i].fitnessF3CAL(0);
	}
	hillclimbmix2();
	for (i = 0; i < POPUL; i++) {
		if (inpf3[i] != 1)
			continue;
		if (ch[i].dominates(ch[i], pop[i]) == 1
				&& !(ch[i].equalsh(ch[i], pop[i])
						&& ch[i].equalss(ch[i], pop[i])))
			ch[i].copyindividual(ch[i], pop[i]);
		else if ((ch[i].fitnessh + ch[i].fitnessh1 + ch[i].fitnessh2
				< pop[i].fitnessh + pop[i].fitnessh1 + pop[i].fitnessh2)\

				&& (ch[i].fitnessf + ch[i].fitnessf1 + ch[i].fitnessf2
						+ ch[i].fitnessf3
						< pop[i].fitnessf + pop[i].fitnessf1 + pop[i].fitnessf2
								+ pop[i].fitnessf3))
			ch[i].copyindividual(ch[i], pop[i]);
	}
	for (i = 0; i < POPUL; i++) {
		if (inpf3[i] == 1) {
			continue;
		}
		if (ch[i].dominates(ch[i], pop[i]) != 2)
			ch[i].copyindividual(ch[i], pop[i]);
	}
}
void Population::hillclimbmix2() {
	int m;
	for (m = 0; m < POPUL * conf->hillsize; m++) {
		if (RND(1000) < 1000 * conf->hillboth)
			ch[m].hc1();
	}

}
void Population::selection(Individual&parent1, Individual&parent2) {
	int i, sel, selsize;
	int popsel[POPUL];
	int dom1i1 = 0;
	int dom1i2 = 0;
	int dom2i1 = 0;
	int dom2i2 = 0;
	int sel1, sel2, selx;
	Individual p1(conf), p2(conf), p3(conf), p4(conf);
	selsize = (int) (POPUL * 0.1);
	int popselidx[selsize];
	for (i = 0; i < POPUL; i++)
		popsel[i] = 0;
	for (i = 0; i < selsize; i++)
		popselidx[i] = -1;
	for (i = 0; i < selsize; i++) {
		sel = RND(POPUL);
		while (popsel[sel] == 1) {
			sel = RND(POPUL);
		}
		popsel[sel] = 1;
		popselidx[i] = sel;
	}
	sel1 = RND(POPUL);
	sel2 = RND(POPUL);
	while (sel1 == sel2) {
		sel2 = RND(POPUL);
	}
	p1.copyindividual(pop[sel1], p1);
	p2.copyindividual(pop[sel2], p2);
	for (i = 0; i < selsize; i++) {
		if (p1.dominates(p1, pop[popselidx[i]]) == 1)
			dom1i1++;
		if (p1.dominates(p1, pop[popselidx[i]]) == 2)
			dom2i1++;
		if (p2.dominates(p2, pop[popselidx[i]]) == 1)
			dom1i2++;
		if (p2.dominates(p2, pop[popselidx[i]]) == 2)
			dom2i2++;
	}
	if (dom1i1 == selsize && dom2i2 == selsize)
		parent1.copyindividual(p1, parent1);
	else if (dom1i2 == selsize && dom2i1 == selsize)
		parent1.copyindividual(p2, parent1);
	else {
		selx = RND(2);
		if (selx == 0)
			parent1.copyindividual(p1, parent1);
		else
			parent1.copyindividual(p2, parent1);
	}
	dom1i1 = 0;
	dom1i2 = 0;
	dom2i1 = 0;
	dom2i2 = 0;
	sel1 = RND(POPUL);
	sel2 = RND(POPUL);
	while (sel1 == sel2) {
		sel2 = RND(POPUL);
	}
	p3.copyindividual(pop[sel1], p3);
	p4.copyindividual(pop[sel2], p4);
	for (i = 0; i < selsize; i++) {
		if (p3.dominates(p3, pop[popselidx[i]]) == 1)
			dom1i1++;
		if (p3.dominates(p3, pop[popselidx[i]]) == 2)
			dom2i1++;
		if (p4.dominates(p4, pop[popselidx[i]]) == 1)
			dom1i2++;
		if (p4.dominates(p4, pop[popselidx[i]]) == 2)
			dom2i2++;
	}
	if (dom1i1 == selsize && dom2i2 == selsize)
		parent2.copyindividual(p3, parent2);
	else if (dom1i2 == selsize && dom2i1 == selsize)
		parent2.copyindividual(p4, parent2);
	else {
		selx = RND(2);
		if (selx == 0)
			parent2.copyindividual(p3, parent2);
		else
			parent2.copyindividual(p4, parent2);
	}
}
void Population::selection2(Individual&parent1, Individual&parent2) {
	int sel1, sel2;
	Individual p1(conf), p2(conf), p3(conf), p4(conf);
	sel1 = RND(POPUL);
	sel2 = RND(POPUL);
	while (sel1 == sel2) {
		sel2 = RND(POPUL);
	}
	p1.copyindividual(pop[sel1], p1);
	p2.copyindividual(pop[sel2], p2);
	if (p1.dominates(p1, p2) == 1) {
		parent1.copyindividual(p1, parent1);
		crossel1 = sel1;
	} else if (p1.dominates(p1, p2) == 2) {
		parent1.copyindividual(p2, parent1);
		crossel1 = sel2;
	} else if (RND(100) < 50) {
		parent1.copyindividual(p1, parent1);
		crossel1 = sel1;
	} else {
		parent1.copyindividual(p2, parent1);
		crossel1 = sel2;
	}
	sel1 = RND(POPUL);
	sel2 = RND(POPUL);
	while (sel1 == sel2) {
		sel2 = RND(POPUL);
	}
	p3.copyindividual(pop[sel1], p3);
	p4.copyindividual(pop[sel2], p4);
	if (p3.dominates(p3, p4) == 1) {
		parent2.copyindividual(p3, parent2);
		crossel2 = sel1;
	} else if (p3.dominates(p3, p4) == 2) {
		parent2.copyindividual(p4, parent2);
		crossel2 = sel2;
	} else if (RND(100) < 50) {
		parent2.copyindividual(p3, parent2);
		crossel2 = sel1;
	} else {
		parent2.copyindividual(p4, parent2);
		crossel2 = sel2;
	}
}
int Population::findmin() {
	int max, maxi, i;
	maxi = -1;
	max = -1;
	for (i = 0; i < POPUL; i++)
		if (max < pop[i].fitnessh + pop[i].fitnessh1 + pop[i].fitnessh2) {
			max = pop[i].fitnessh + pop[i].fitnessh1 + pop[i].fitnessh2;
			maxi = i;
		}
	return maxi % POPUL;
}
int Population::findmax() {
	int max, maxi, i;
	maxi = -1;
	max = 300000;
	for (i = 0; i < POPUL; i++)
		if (max > pop[i].fitnessh + pop[i].fitnessh1 + pop[i].fitnessh2) {
			max = pop[i].fitnessh + pop[i].fitnessh1 + pop[i].fitnessh2;
			maxi = i;
		}
	return maxi % POPUL;
}
void Population::nextgenold(Individual&child) {
	Individual tmpind(conf);
	int i, j;
	int domination = -1;
	for (j = 0; j < paretof.size(); j++) {
		if (child.dominates(child, pop[paretof[j]]) == 1) {
			child.copyindividual(pop[paretof[j]], tmpind);
			child.copyindividual(child, pop[paretof[j]]);
			domination = 1;
			break;
		}
		if (child.dominates(child, pop[paretof[j]]) == 2) {
			domination = 2;
			break;
		}
	}
	for (i = 0; i < POPUL; i++) {
		if (domination == 1 && tmpind.dominates(tmpind, pop[i]) == 1) {
			tmpind.copyindividual(tmpind, pop[i]);
			break;
		}
		if (domination == -1 && child.dominates(child, pop[i]) == 1) {
			child.copyindividual(child, pop[i]);
			break;
		}
		if (domination == 2 && child.dominates(child, pop[i]) == 1) {
			child.copyindividual(child, pop[i]);
			break;
		}
	}
	if (domination == 4) {
	}
}
bool Population::foundinpar(int idx) {
	int i;
	for (i = 0; i < paretof.size(); i++) {
		if (paretof[i] == idx)
			return true;
	}
	return false;
}
void Population::nextgen(Individual child[]) {
	Individual tmpind(conf);
	int i, j;

	for (i = 0; i < NUMX; i++) {
		for (j = 0; j < POPUL; j++) {
			if (!foundinpar(j) && child[i].dominates(child[i], pop[j]) == 1) {
				child[i].copyindividual(child[i], pop[j]);
				break;
			}
		}
	}
	initpareto();
}
void Population::initpareto() {
	int i, j, domination = 0;
	while (paretof.size() != 0) {
		for (j = 0; j < paretof.size(); j++)
			paretof.pop_back();
	}
	paretof.push_back(0);
	for (i = 0; i < POPUL; i++) {
		inpf3[i] = 0;
	}
	inpf3[0] = 1;
	for (i = 1; i < POPUL; i++) {
		domination = 0;
		for (j = 0; j < paretof.size(); j++) {
			if (pop[i].dominates(pop[i], pop[paretof[j]]) == 1) {
				inpf3[i] = 1;
				inpf3[paretof[j]] = 0;
				domination = 1;
				paretof[j] = i;
				break;
			}
			if (pop[i].dominates(pop[i], pop[paretof[j]]) == 2) {
				domination = 1;
				break;
			}
		}
		if (domination == 0) {
			inpf3[i] = 1;
			paretof.push_back(i);
		}
	}
}

double Population::getduration() {
	time_t end;
	time(&end);
	return difftime(end, start);
}

//TODO: buray� d�zenle. bast�rma i�lemini io'da yapt�r.
void Population::run(int seed) {
	int it = 0;
	int smallest = 20000, smallestidx = -1;
	int prevtime = 10;
	double duration;
	int fit, m;
	FILE*resf = fopen("result.txt", "a");
	FILE*poprh = fopen("poprh.txt", "a");
	FILE*poprs = fopen("poprs.txt", "a");

		while ((int) duration <= conf->dur) {
			it++;
			duration = getduration();
			if ((int) duration % 10 == 0 && prevtime == (int) duration) {
				prevtime = prevtime + 10;
				/*fprintf(tstf, "%d\t%.0lf\t%d\n", it, duration,
						pop[paretof[0]].fitnessh
								+ pop[paretof[0]].fitnessf
								+ pop[paretof[0]].fitnessf1
								+ pop[paretof[0]].fitnessf2
								+ pop[paretof[0]].fitnessf3);*/
			}
			int fit2, fit3, fit4, fit5, fith1, fith2, fith0;
			smallest = 20000;
			smallestidx = -1;
			int fit6 = 20000;
			if (it % 20 == 0) {
				for (m = 0; m < paretof.size(); m++) {
					fit = pop[paretof[m]].fitnessh + pop[paretof[m]].fitnessh1
							+ pop[paretof[m]].fitnessh2;
					fith0 = pop[paretof[m]].fitnessh;
					fith1 = pop[paretof[m]].fitnessh1;
					fith2 = pop[paretof[m]].fitnessh2;
					fit2 = pop[paretof[m]].fitnessf;
					fit3 = pop[paretof[m]].fitnessf1;
					fit4 = pop[paretof[m]].fitnessf2;
					fit5 = pop[paretof[m]].fitnessf3;
					printf(
							"fitness---- %d --------%d--  %d-- %d-- %d -- %d -- %d -- %d \n", m, fith0, fith1, fith2, fit2, fit3, fit4, fit5);
					if (fit < smallest) {
						smallest = fit;
						smallestidx = m;
						fit6 = fit + fit2 + fit3 + fit4 + fit5;
					} else if (fit <= smallest
							&& fit + fit2 + fit3 + fit4 + fit5 < fit6) {
						smallest = fit;
						smallestidx = m;
						fit6 = fit + fit2 + fit3 + fit4 + fit5;
					}
				}
				printf("# of iterations %d duration %d\n", it, (int) duration);
				printf("# smallest  %d \n", smallestidx);
			}
			crossmut3();
			initpareto();
		}
		int drr = 1200;

			int fit2, fit3, fit4, fit5, fit6 = 20000;
			fit6 = 20000;
			for (m = 0; m < paretof.size(); m++) {
				fit = pop[paretof[m]].fitnessh + pop[paretof[m]].fitnessh1
						+ pop[paretof[m]].fitnessh2;
				fit2 = pop[paretof[m]].fitnessf;
				fit3 = pop[paretof[m]].fitnessf1;
				fit4 = pop[paretof[m]].fitnessf2;
				fit5 = pop[paretof[m]].fitnessf3;
				printf("fitness---- %d --------  %d-- %d-- %d -- %d -- %d -- %d \n", m,
						fit, fit2, fit3, fit4, fit5, fit + fit2 + fit3 + fit4 + fit5);
				if (fit < smallest) {
					smallest = fit;
					smallestidx = m;
					fit6 = fit + fit2 + fit3 + fit4 + fit5;
				} else if (fit <= smallest && fit + fit2 + fit3 + fit4 + fit5 < fit6) {
					smallest = fit;
					smallestidx = m;
					fit6 = fit + fit2 + fit3 + fit4 + fit5;
				}
			}
			printf("# of iterations %d duration %d\n", it, (int) duration);
			printf("# smallest  %d \n", smallestidx);
			printf("THE SOULUTION IS  \n");
			pop[paretof[smallestidx]].printlect();
			pop[paretof[smallestidx]].printdekanlik();
			pop[paretof[smallestidx]].printtable();
			fit = pop[paretof[smallestidx]].fitnessHCAL(1);
			fit = pop[paretof[smallestidx]].fitnessFCAL(1);
			fit = pop[paretof[smallestidx]].fitnessF1CAL(1);
			fit = pop[paretof[smallestidx]].fitnessF2CAL(1);
			fit = pop[paretof[smallestidx]].fitnessF3CAL(1);
			for (m = 0; m < paretof.size(); m++) {
				printf("THE SOULUTION IS %d \n", m);
				pop[paretof[m]].printlect();
				pop[paretof[m]].printdekanlik();
				pop[paretof[m]].printtable();
				fit = pop[paretof[m]].fitnessHCAL(1);
				fit = pop[paretof[m]].fitnessFCAL(1);
				fit = pop[paretof[m]].fitnessF1CAL(1);
				fit = pop[paretof[m]].fitnessF2CAL(1);
				fit = pop[paretof[m]].fitnessF3CAL(1);
			}
			fprintf(resf, "hard soft  %5d  %5d   dur %5d seed %12d \n",
					pop[paretof[smallestidx]].fitnessh,
					pop[paretof[smallestidx]].fitnessf
							+ pop[paretof[smallestidx]].fitnessf1
							+ pop[paretof[smallestidx]].fitnessf2
							+ pop[paretof[smallestidx]].fitnessf3, drr, seed);
			fprintf(poprh, "%5d \n",
					pop[paretof[smallestidx]].fitnessh
							+ pop[paretof[smallestidx]].fitnessh1
							+ pop[paretof[smallestidx]].fitnessh2);
			fprintf(poprs, "%5d \n",
					pop[paretof[smallestidx]].fitnessf
							+ pop[paretof[smallestidx]].fitnessf1
							+ pop[paretof[smallestidx]].fitnessf2);
			duration = getduration();
			printf("\n\nThe operation completed in %.2lf seconds.\n", duration);
			fclose(poprh);
			fclose(poprs);
			fclose(resf);
}

} /* namespace std */
