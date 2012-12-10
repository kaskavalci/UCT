#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<vector>
#include<iostream>
#include<iomanip>
#include "Common.h"
#include "FileReader.h"
#include "Population.h"

using namespace std;

int main(int argc, char*argv[]) {
	FileReader reader;
	reader.read();

	unsigned long seed;
	if (argc > 2) {
		seed = atoi(argv[1]);
	}
	else {
		seed = time(0);
	}
	srand(seed);
	Population popn;
	popn.run(seed);

	return 0;
}
