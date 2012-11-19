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

/*
 * todo: hiyerarþik bi yapý içinde common ana classýný oluþtur. uml diyagramý gerekiyor.
 * birbirine referans veren class lardan kurtul.
 */

using namespace std;

void print2(int*p) {
	int i;
	for (i = 0; i < CHROML; i++)
		printf(" \n");
}

int main(int argc, char*argv[]) {
	FILE*tstf = fopen("tst.txt", "w");
	Common conf;
	FileReader reader(&conf);
	reader.readinputparam();
	reader.readcourses();
	reader.readreq();
	reader.readConst("constcse.txt");

	fprintf(tstf, "CHROML %d POPUL %d NUMX %d NMUT %d \n", CHROML, POPUL, NUMX,
			NMUT);
	unsigned long seed;
	if (argc > 2) {
		seed = atoi(argv[1]);
	}
	else {
		seed = time(0);
	}
	srand(seed);
	Population popn(&conf);
	popn.run(seed);

	/*printf("hello\n");
	 for (i = 0; i < lectures.size(); i++) {
	 printf("Lecture %s %d %d %d %d %d\n", lectures[i].lectname.data(),
	 lectures[i].cid1, lectures[i].cid2, lectures[i].lab1,
	 lectures[i].lab2, lectures[i].lab3);
	 }*/


	fclose(tstf);
	return 0;
}
