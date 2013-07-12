#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "execga.h"
#include <QtWidgets>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_actionAbout_triggered();

    void on_actionSettings_triggered();

	void workDone();

	void updateProgressbar(int);

	void on_push_start_clicked();

	void on_actionPrint_another_triggered();

private:
	Ui::MainWindow *ui;
	unsigned long seed;
	execGA *coreLaunher;
	QStandardItemModel *model[8];
	QTableView *tableView;
	void initTabs();
	void setHeaders();
	void populateTable(int idx);
	int bestID;
	void clearTable();

};

#endif // MAINWINDOW_H
