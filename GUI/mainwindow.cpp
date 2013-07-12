#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutbox.h"
#include "settings.h"
#include <QMessageBox>
#include "Population.h"
#include <QtDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	seed = -1;
	ui->centralWidget->setLayout(ui->gridLayout);
	bestID = 0;
	coreLaunher = NULL;
	initTabs();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionAbout_triggered()
{
    AboutBox about;
    about.setModal(true);
    about.exec();
    /*QMessageBox::about(this, tr("About DOM Bookmarks"),
                       tr("The <b>DOM Bookmarks</b> example demonstrates how to "
                          "use Qt's DOM classes to read and write XML "
                          "documents."));*/
}

void MainWindow::on_actionSettings_triggered()
{
  Settings settings;
  settings.setModal(true);
  settings.exec();
  seed = settings.seed;
}

void MainWindow::workDone()
{
  ui->push_start->setEnabled(true);
  const std::Individual *bestInd = coreLaunher->population->getBestInd();
  int best = coreLaunher->population->getParetoBestId();;

  if (bestInd->getFit().hard_fit != 0 && ui->checkStop->isChecked()) {
	//QMessageBox::critical(this, tr("Sorry!"), tr("Hard fitness is %1. Timetable have conflicts. Number of it: %2").arg(bestInd->getFit().hard_fit).arg(coreLaunher->iter));
	on_push_start_clicked();
	return;
  } else {
	QMessageBox::information(this, tr("Voila!"),
		QString("Hard fitness is now %1 and Soft fitness is %2. Number of it: %3")
							 .arg(bestInd->getFit().hard_fit)
							 .arg(bestInd->getFit().soft_fit)
							 .arg(coreLaunher->iter));
  }
  bestID = best;
  populateTable(best);
}

void MainWindow::updateProgressbar(int dur)
{
  ui->progressBar->setValue(dur);
  ui->progressBar->update();
}

void MainWindow::on_push_start_clicked()
{
  if (coreLaunher != NULL) {
	delete coreLaunher;
	clearTable();
  }
  coreLaunher = new execGA(this, seed);
  coreLaunher->start(QThread::HighestPriority);
  ui->push_start->setEnabled(false);
  connect(coreLaunher, SIGNAL(finished()), this, SLOT(workDone()));
  connect(coreLaunher, SIGNAL(progressChanged(int)), this, SLOT(updateProgressbar(int)));
}

void MainWindow::initTabs()
{
  for (int i = 1; i < 9; ++i) {
	QWidget *newTab = new QWidget(ui->tab_sem);
	QVBoxLayout *layout = new QVBoxLayout(newTab);
	tableView = new QTableView(newTab);
	tableView->verticalHeader()->setDefaultSectionSize(40);
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	model[i-1] = new QStandardItemModel(10, 5, this);

	tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	layout->addWidget(tableView);
	newTab->setLayout(layout);

	tableView->setModel(model[i-1]);
	ui->tab_sem->addTab(newTab, QString("Semester %1").arg(i));
  }
  setHeaders();
}

void MainWindow::setHeaders()
{
  QStringList headerH, headerV;
  headerH << "Monday" << "Tuesday" << "Wednesday" << "Thursday" << "Friday";
  for (int j = 9; j < 19; j++) {
	headerV << QString("%1.00\n%1.50").arg(j);
  }
  for (int i = 0; i < 8; i++) {
	model[i]->setHorizontalHeaderLabels(headerH);
	model[i]->setVerticalHeaderLabels(headerV);
  }
}

void MainWindow::populateTable(int idx)
{
  if (coreLaunher == NULL) {
	return;
  }
  if (coreLaunher->population == NULL) {
	return;
  }
  const std::Individual *bestInd = coreLaunher->population->getInd(idx);
  std::Common *conf = coreLaunher->population->conf;
  int sem, period, day, hour, extra;
  for (int i = 0; i < conf->ChromSize; ++i) {
	QString lab(conf->courmat.at(i).isLab ? "L" : "0");
	QString coursename = tr(conf->courmat.at(i).cname.c_str()) + tr(".") + lab + QString::number(conf->courmat.at(i).section);
	QStandardItem *item = new QStandardItem(coursename);
	sem = conf->courmat.at(i).semid - 1;
	hour = conf->courmat.at(i).hours;
	day = bestInd->getConstChromosome()->get_day(i);
	period = bestInd->getConstChromosome()->get_period(i);
	if (hour == 1) {
	  if (period < 3) {
		period += 2;
	  } else {
		period = 8;
	  }
	  //empty cell. no collision
	  if (model[sem]->item(period, day) == 0) {
		model[sem]->setItem(period, day, item);
	  } else {
		QStandardItem *collisionItem = model[sem]->item(period, day);
		collisionItem->setBackground(QBrush(QColor(255, 0, 0)));
		collisionItem->setText(collisionItem->text() + "\n" + item->text());
	  }
	} else {
	  QStandardItem *item2 = new QStandardItem(coursename);
	  if (period < 2) {
		 extra = 0;
	  } else {
		 extra = 1;
	  }
	  if (model[sem]->item(2*period + extra, day) == 0) {
		model[sem]->setItem(2*period + extra, day, item);
	  } else {
		QStandardItem *collisionItem = model[sem]->item(2*period + extra, day);
		collisionItem->setBackground(QBrush(QColor(255, 0, 0)));
		collisionItem->setText(collisionItem->text() + "\n" + item->text());
	  }
	  if (model[sem]->item(2*period + 1 + extra, day) == 0) {
		model[sem]->setItem(2*period + 1 + extra, day, item2);
	  } else {
		QStandardItem *collisionItem = model[sem]->item(2*period + 1 + extra, day);
		collisionItem->setBackground(QBrush(QColor(255, 0, 0)));
		collisionItem->setText(collisionItem->text() + "\n" + item2->text());
	  }
	}
  }
}

void MainWindow::clearTable()
{
  for (int i = 0; i < 8; i++) {
	model[i]->clear();
  }
  setHeaders();
}

void MainWindow::on_actionPrint_another_triggered()
{
  bool ok;
  int pareto_size =  coreLaunher->population->conf->pareto_size;
  int i = QInputDialog::getInt(this, tr("Print another item in pareto front"),
							   tr("Item ID in pareto front:"), bestID, 0,pareto_size, 1, &ok);
  if (ok) {
	clearTable();
	populateTable(i);
  }
}
