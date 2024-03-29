#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QtXml>
#include <QtCore>
#include <QtGui>

namespace Ui {
  class Settings;
}

class Settings : public QDialog
{
  Q_OBJECT

public:
  explicit Settings(QWidget *parent = 0);
  ~Settings();
  unsigned long seed;


private slots:
  void on_buttonBox_accepted();

  void on_pushButton_clicked();

  void on_pushButton_3_clicked();

  void on_pushAddCourse_clicked();

  void on_pushButton_4_clicked();

  void on_pushDeleteCourse_clicked();

  void on_pushButton_5_clicked();

  void on_pushButton_2_clicked();

  void on_pushButton_8_clicked();

  void on_pushButton_6_clicked();

  void on_pushButton_7_clicked();

  void on_checkBox_stateChanged(int arg1);

private:
  Ui::Settings *ui;
  QStandardItemModel *modelPrereq;
  void ReadPrereq(QString);
  void WritePrereq(QString);
  void clearTable();

  QStandardItemModel *modelCourses;
  void ReadCourses(QString);
  void WriteCourses(QString);
  void clearTree();

  QStandardItemModel *modelGroups;
  void ReadParam(QString);
  void WriteParam(QString);

  QString pathCourse, pathPrereq, pathParameters;
  enum par {p_cname, p_section, p_islab, p_lect, p_sem, p_hour, p_day, p_slot};

};

#endif // SETTINGS_H
