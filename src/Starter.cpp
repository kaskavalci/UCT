#include <ctime>
#include "Population.h"
#include <cstdlib>

using namespace std;

int main(int argc, char*argv[]) {
	//take seed or generate it
	unsigned long seed;
	if (argc > 2) {
		seed = int(argv[1]);
	}
	else {
		seed = time(0);
	}

	//init population
	Population population(seed);
	//enjoy the magic
	population.startGA();

	return EXIT_SUCCESS;
}
