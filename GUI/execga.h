#ifndef EXECGA_H
#define EXECGA_H

#include <QThread>
#include "Population.h"

class execGA : public QThread
{
  Q_OBJECT
public:
  explicit execGA(QObject *parent = 0, unsigned long seed = -1);
  virtual ~execGA();
  void run();
  std::Population *population;
  int iter;
  
signals:
  void progressChanged(int);

  
public slots:

private:
  unsigned long seed;
};

#endif // EXECGA_H
