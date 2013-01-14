/*
 * Printer.h
 *
 *  Created on: 20 Ara 2012
 *      Author: HalilCan
 */

#ifndef PRINTER_H_
#define PRINTER_H_

#include <string>
#include <iostream>
#include <fstream>
#include "Individual.h"

namespace std {

class Printer {
public:
	Printer(string);
	virtual ~Printer();

	void print(Individual*, int[]);
	void printLast(Individual*, int[]);
	void print(vector<Individual*>&, int[]);
	void printStats(double [], bool);
	void print(string);
	void printHeader();
	void printFooter();
private:
	fstream out;

};

} /* namespace std */
#endif /* PRINTER_H_ */
