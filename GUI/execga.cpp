#include "execga.h"
#include <ctime>

execGA::execGA(QObject *parent, unsigned long seed) :
  QThread(parent)
{
  if (seed == -1) {
	this->seed = time(NULL);
  } else {
	this->seed = seed;
  }
  population = NULL;
  setTerminationEnabled(true);
}

execGA::~execGA()
{
  delete population;
}

void execGA::run()
{
  //init population
  population = new std::Population(seed);
  int it = 0;
  do {
	it++;

	population->crossover();
	population->hillclimber();
	population->mutation();

	//notify progressbar to change
	emit progressChanged((int) (population->getduration() * 100 / population->conf->duration));
  } while (population->getduration() <= population->conf->duration );
  iter = it;

  population->print_footer(population->getduration(), it);
}
