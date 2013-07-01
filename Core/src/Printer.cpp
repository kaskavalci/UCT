/*
 * Printer.cpp
 *
 *  Created on: 20 Ara 2012
 *      Author: HalilCan
 */

#include "Printer.h"

#define TD(x)	"<td>" << x << "</td>"

namespace std {
/**
 * Opens filename.
 * @param filename
 */
Printer::Printer(string filename) {
	if (filename.length() == 0) {
		cerr << "filename is invalid. Output.html will be used." << endl;
		filename = "output.html";
	}
	out.open(filename.c_str(), fstream::out | fstream::app);
}

Printer::~Printer() {
	out.close();
}
/**
 * Prints given Individual subject and stats associated with it. Adds </tr> in the end.
 * @param subject
 * @param stat
 */
void Printer::print(Individual* subject, int stat[]) {
	out << "<tr>" << endl;
	for (int i = 0; i < TOT_FIT_N; ++i) {
		out << TD((int)subject->getFit().fitness[i]);
	}
	for (int i = 0; i < STAT_LEN; i += 2) {
		out << TD("<b>" << stat[i] << "</b>/" << stat[i+1]);
	}
	out << "</tr>";
}
/**
 * Prints given Individual subject and stats associated with it. DOES NOT ADD </tr> in the end.
 * Only difference is that.
 * @param subject
 * @param stat
 */
void Printer::printLast(Individual* subject, int stat[]) {
	out << "<tr>" << endl;
	for (int i = 0; i < TOT_FIT_N; ++i) {
		out << TD((int)subject->getFit().fitness[i]);
	}
	for (int i = 0; i < STAT_LEN; i += 2) {
		out << TD("<b>" << stat[i] << "</b>/" << stat[i+1]);
	}
}
/**
 * Prints whole population.
 * @param pop
 * @param stat
 */
void Printer::print(vector<Individual*>&pop, int stat[]) {
	out << "<tr><td>All Population</td>" << endl;
	for (unsigned int i = 0; i < Common::getConf()->pop_size; ++i) {
		print(pop[i], stat);
	}
}

void Printer::printHeader() {
	out << "<html>" << endl;
	out << "<body>" << endl;
	out << "<title>UCT Debugger Output</title>" << endl;
	out << "</body>" << endl;
	out << "<body>" << endl;
	out << "<table border=1>" << endl;
	out << "<tr>" << endl;
	for (int i = 0; i < HARD_FIT_N; ++i) {
		out << TD("hard[" << i << "]");
	}
	for (int i = HARD_FIT_N; i < TOT_FIT_N; ++i) {
		out << TD("soft[" << i << "]");
	}
	out << TD("Crossover") << TD("Add Popul") << TD("HillClimber") << TD("Add pareto") << TD("Mutation") << endl;
	out << "</tr>" << endl;
}

void Printer::printStats(double conf[], bool flag) {
	if (flag) {
		out << "<table border=1>" << endl;
		out << "<tr>" << TD("duration") << TD("HCRate") << TD("MutationRate") << TD("CRRate") << TD("InserRate")
				<< TD("CrowdingSize") << TD("seed") << TD("iteration") << "</tr>";
		out << "<tr>";
	}
	for (int i = 0; i < c_LENGTH; ++i) {
		out << TD(conf[i]);
	}
	out << "</tr>";
	if (flag) {
		out << "</table>" << endl;
	}
}

void Printer::print(string msg) {
	out << "<tr><th colspan=12>" << msg << "</th></tr>" << endl;
}

void Printer::printFooter() {
	out << "</table>" << endl;
	out << "</body>" << endl << "</html>" << endl;
}

} /* namespace std */
