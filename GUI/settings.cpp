#include "Constants.h"
#include "settings.h"
#include "ui_settings.h"
#include "comboboxdelegate.h"
#include <QFileDialog>
#include <QtDebug>

Settings::Settings(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::Settings)
{
  ui->setupUi(this);

  //create the model;
  modelCourses = new QStandardItemModel(0, 8, this);
  modelPrereq = new QStandardItemModel(0, 2);
  modelGroups = new QStandardItemModel(0, 2);
  clearTree();

  //set default file names
  pathParameters = "Resources/parameters.xml";
  pathCourse = "Resources/courses.xml";
  pathPrereq = "Resources/prereq.xml";

  //read conf
  ReadParam(pathParameters);
  ReadCourses(pathCourse);
  ReadPrereq(pathPrereq);

  ui->treeView->setModel(modelCourses);
  ui->tableView->setModel(modelPrereq);
  ui->treeGroups->setModel(modelGroups);

  ui->treeView->setItemDelegateForColumn(p_sem, new ComboBoxSemester());
  ui->treeView->setItemDelegateForColumn(p_hour, new ComboBoxNumHour());
  ui->treeView->setItemDelegateForColumn(p_day, new ComboBoxDay());
  ui->treeView->setItemDelegateForColumn(p_slot, new ComboBoxSlot());
  ui->treeView->setItemDelegateForColumn(p_islab, new ComboBoxLab());

  ui->treeGroups->setItemDelegateForColumn(1, new ComboBoxGroup());
}

Settings::~Settings()
{
  delete ui;
}

//save everything!
void Settings::on_buttonBox_accepted()
{
  WriteParam(pathParameters);
  WritePrereq(pathPrereq);
  WriteCourses(pathCourse);
  if (ui->checkBox->checkState() == Qt::Checked) {
	seed = -1;
  } else {
	seed = ui->t_seed->text().toDouble();
  }
}

void Settings::ReadCourses(QString filename)
{
  clearTree();
  QDomDocument doc;
  //load xml
  QFile file(filename);
  if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
	doc.setContent(&file);
	file.close();
  } else {
	QMessageBox::critical(this, "Error!", "Cannot read courses.xml. Executable is corrupted.");
  }

  //get the xml root element
  QDomElement xmlroot = doc.firstChildElement();
  //traverse
  QDomNodeList courses = xmlroot.elementsByTagName("Course");

  QList<QStandardItem*> *list = new QList<QStandardItem*>();

  for(int i = 0; i < courses.count(); i++) {
	QDomElement course = courses.at(i).toElement();

	list->append(new QStandardItem(course.attribute("Name")));
	list->append(new QStandardItem(course.attribute("Section")));
	int lab = course.attribute("isLab").toInt();
	QString labStatus;
	if (lab == 0) {
	  labStatus = NO;
	} else if (lab == 1){
	  labStatus = YES;
	}
	list->append(new QStandardItem(labStatus));
	list->append(new QStandardItem(course.attribute("Lecturer")));

	QString semName;
	int sem = course.attribute("Semester").toInt();
	switch(sem) {
	  case 1:
		semName = course.attribute("Semester") + "st Semester";
		break;
	  case 2:
		semName = course.attribute("Semester") + "nd Semester";
		break;
	  case 3:
		semName = course.attribute("Semester") + "rd Semester";
		break;
	  default:
		semName = course.attribute("Semester") + "th Semester";
		break;
	}
	QStandardItem *courseSem = new QStandardItem(semName);
	courseSem->setData(QVariant(sem));
	list->append(courseSem);

	QStandardItem *courseHour;
	int hour = course.attribute("NumHours").toInt();
	switch(hour) {
	  case 1:
		courseHour = new QStandardItem(hour0);
		break;
	  case 2:
		courseHour = new QStandardItem(hour1);
		break;
	  case 3:
		courseHour = new QStandardItem(hour2);
		break;
	}

	list->append(courseHour);

	QStandardItem *courseConsDay;
	int day = course.attribute("ConstDay").toInt();
	switch (day) {
	  case -1:
		courseConsDay = new QStandardItem(none);
		break;
	  case 0:
		courseConsDay = new QStandardItem(day0);
		break;
	  case 1:
		courseConsDay= new QStandardItem(day1);
		break;
	  case 2:
		courseConsDay= new QStandardItem(day2);
		break;
	  case 3:
		courseConsDay= new QStandardItem(day3);
		break;
	  case 4:
		courseConsDay= new QStandardItem(day4);
		break;
	}
	list->append(courseConsDay);

	QStandardItem *courseConsSlot;
	int slot = course.attribute("ConstSlot").toInt();
	switch(slot) {
	  case 0:
		courseConsSlot = new QStandardItem(slot0);
		break;
	  case 1:
		courseConsSlot = new QStandardItem(slot1);
		break;
	  case 2:
		courseConsSlot = new QStandardItem(slot2);
		break;
	  case 3:
		courseConsSlot = new QStandardItem(slot3);
		break;
	  default:
		courseConsSlot = new QStandardItem(none);
		break;
	}

	list->append(courseConsSlot);

	modelCourses->appendRow(*list);
	list->clear();
  }

  ui->pushAddCourse->setEnabled(true);
}

void Settings::WriteCourses(QString filename)
{
  //write the xml file
  QDomDocument doc;
  //Make a root node
  QDomElement xmlroot = doc.createElement("Courses");
  doc.appendChild(xmlroot);

  for(int i = 0; i < modelCourses->rowCount(); i++)
  {
	QStandardItem *courseName = modelCourses->item(i, p_cname);
	QStandardItem *section = modelCourses->item(i, p_section);
	QStandardItem *islab = modelCourses->item(i, p_islab);
	QStandardItem *courseLect = modelCourses->item(i, p_lect);
	QStandardItem *courseSem = modelCourses->item(i, p_sem);
	QStandardItem *courseHour = modelCourses->item(i, p_hour);
	QStandardItem *courseConsDay = modelCourses->item(i, p_day);
	QStandardItem *courseConsSlot = modelCourses->item(i, p_slot);

	QDomElement xmlbook = doc.createElement("Course");
	xmlbook.setAttribute("Name",courseName->text());
	xmlbook.setAttribute("Section",section->text());
	int lab;
	if (islab->text() == YES) {
	  lab = 1;
	} else {
	  lab = 0;
	}
	xmlbook.setAttribute("isLab",lab);

	xmlbook.setAttribute("Lecturer",courseLect->text());
	//first item is semester number
	xmlbook.setAttribute("Semester",courseSem->text().at(0));
	//first item is number of hours
	xmlbook.setAttribute("NumHours",courseHour->text().at(0));
	int day = -1;
	if (courseConsDay->text() == day0) {
	  day = 0;
	} else if (courseConsDay->text() == day1) {
	  day = 1;
	} else if (courseConsDay->text() == day2) {
	  day = 2;
	} else if (courseConsDay->text() == day3) {
	  day = 3;
	} else if (courseConsDay->text() == day4) {
	  day = 4;
	} else {
	  day = -1;
	}
	xmlbook.setAttribute("ConstDay",day);

	int slot = -1;
	if (courseConsSlot->text() == slot0) {
	  slot = 0;
	} else if (courseConsSlot->text() == slot1) {
	  slot = 1;
	} else if (courseConsSlot->text() == slot2) {
	  slot = 2;
	} else if (courseConsSlot->text() == slot3) {
	  slot = 3;
	} else {
	  slot = -1;
	}
	xmlbook.setAttribute("ConstSlot", slot);

	xmlroot.appendChild(xmlbook);
  }

  //save to disk
  QFile file(filename);
  if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
  {
	QMessageBox::critical(this, "Error!", "Failed to write file.");
  }

  QTextStream stream(&file);
  stream << doc.toString();
  file.close();
}

void Settings::clearTree()
{
  modelCourses->clear();
  QStringList header;
  header << "Course Name" << "Section" << "Lab?" << "Lecturer" << "Semester" << "# Hours" << "Fixed Day" << "Fixed Slot";

  modelCourses->setHorizontalHeaderLabels(header);
}

void Settings::ReadParam(QString filename)
{
  QStringList header;
  header << "Constraint Name" << "Group";
  modelGroups->setHorizontalHeaderLabels(header);
  QDomDocument doc;
  //load xml
  QFile file(filename);
  if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
	doc.setContent(&file);
	file.close();
  } else {
	QMessageBox::critical(this, "Error!", "Cannot read prereq.xml. Executable is corrupted. Provide new one.");
  }
  //get the xml root element
  QDomElement xmlroot = doc.firstChildElement();
  //traverse
  QDomNodeList parameters = xmlroot.elementsByTagName("Parameter");
  QList<QStandardItem*> list;
  for(int i = 0; i < parameters.count(); i++) {
	QDomElement parameter = parameters.at(i).toElement();
	if (parameter.attribute("Name") == PARAM_DUR) {
	  ui->t_duration->setText(parameter.attribute("Val"));
	} else if (parameter.attribute("Name") == PARAM_HCSIZE) {
	  ui->t_hsize->setText(parameter.attribute("Val"));
	} else if (parameter.attribute("Name") == PARAM_HCRATE) {
	  ui->t_hcrate->setText(parameter.attribute("Val"));
	}else if (parameter.attribute("Name") == PARAM_MUTRATE) {
	  ui->t_mutation->setText(parameter.attribute("Val"));
	}else if (parameter.attribute("Name") == PARAM_CRRATE) {
	  ui->t_crossover->setText(parameter.attribute("Val"));
	} else if (parameter.attribute("Name") == PARAM_RNDRATE) {
	  ui->t_randomInsertion->setText(parameter.attribute("Val"));
	} else if (parameter.attribute("Name") == PARAM_CROWDINGDIST) {
	  ui->t_crowdingDist->setText(parameter.attribute("Val"));
	} else if (parameter.attribute("Name") == PARAM_HCGENECOUNT) {
	  ui->t_hcGeneCount->setText(parameter.attribute("Val"));
	} else if (parameter.attribute("Name") == PARAM_POPULATION) {
	  ui->t_populationSize->setText(parameter.attribute("Val"));
	} else if (parameter.attribute("Name") == PARAM_PARETO) {
	  ui->t_paretofront->setText(parameter.attribute("Val"));
	} else if (parameter.attribute("Name") == PARAM_SELECTIONPOOLSIZE) {
	  ui->t_poolsize->setText(parameter.attribute("Val"));
	} else if (parameter.attribute("Name") == PARAM_SELECTIONCANDIDATESIZE) {
	  ui->t_candidateSize->setText(parameter.attribute("Val"));
	} else if (parameter.attribute("Name") == "Groups") {
	  //found group list. get them all.
	  QDomNodeList groups = parameter.elementsByTagName("Group");
	  //iterate every group type. +1 because we want groups to start from 1
	  for (int j = 1; j < groups.count() + 1; j++) {
		//at every group, get Constraints and iterate through them
		QDomNodeList cons = groups.at(j - 1).toElement().elementsByTagName("Constraint");
		for (int k = 0; k < cons.size(); k++) {
		  QStandardItem *item = new QStandardItem(cons.at(k).toElement().attribute("Name"));
		  item->setToolTip(cons.at(k).toElement().attribute("Desc"));
		  item->setData(cons.at(k).toElement().attribute("ID").toInt());
		  list.append(item);
		  list.append(new QStandardItem(QString("Group %1").arg(j)));
		  modelGroups->appendRow(list);
		  list.clear();
		}
	  }
	}
  }
}

void Settings::WriteParam(QString filename)
{
  //write the xml file
  QDomDocument doc;
  //Make a root node
  QDomElement xmlroot = doc.createElement("Parameters");
  doc.appendChild(xmlroot);
  //create sub elements
  QDomElement xmlnode;
  xmlnode = doc.createElement("Parameter");
  xmlnode.setAttribute("Name", PARAM_DUR);
  xmlnode.setAttribute("Val", ui->t_duration->text());
  xmlroot.appendChild(xmlnode);

  xmlnode = doc.createElement("Parameter");
  xmlnode.setAttribute("Name", PARAM_HCSIZE);
  xmlnode.setAttribute("Val", ui->t_hsize->text());
  xmlroot.appendChild(xmlnode);

  xmlnode = doc.createElement("Parameter");
  xmlnode.setAttribute("Name", PARAM_HCRATE);
  xmlnode.setAttribute("Val", ui->t_hcrate->text());
  xmlroot.appendChild(xmlnode);

  xmlnode = doc.createElement("Parameter");
  xmlnode.setAttribute("Name", PARAM_MUTRATE);
  xmlnode.setAttribute("Val", ui->t_mutation->text());
  xmlroot.appendChild(xmlnode);

  xmlnode = doc.createElement("Parameter");
  xmlnode.setAttribute("Name", PARAM_CRRATE);
  xmlnode.setAttribute("Val", ui->t_crossover->text());
  xmlroot.appendChild(xmlnode);

  xmlnode = doc.createElement("Parameter");
  xmlnode.setAttribute("Name", PARAM_RNDRATE);
  xmlnode.setAttribute("Val", ui->t_randomInsertion->text());
  xmlroot.appendChild(xmlnode);

  xmlnode = doc.createElement("Parameter");
  xmlnode.setAttribute("Name", PARAM_CROWDINGDIST);
  xmlnode.setAttribute("Val", ui->t_crowdingDist->text());
  xmlroot.appendChild(xmlnode);

  xmlnode = doc.createElement("Parameter");
  xmlnode.setAttribute("Name", PARAM_HCGENECOUNT);
  xmlnode.setAttribute("Val", ui->t_hcGeneCount->text());
  xmlroot.appendChild(xmlnode);

  xmlnode = doc.createElement("Parameter");
  xmlnode.setAttribute("Name", PARAM_POPULATION);
  xmlnode.setAttribute("Val", ui->t_populationSize->text());
  xmlroot.appendChild(xmlnode);

  xmlnode = doc.createElement("Parameter");
  xmlnode.setAttribute("Name", PARAM_PARETO);
  xmlnode.setAttribute("Val", ui->t_paretofront->text());
  xmlroot.appendChild(xmlnode);

  xmlnode = doc.createElement("Parameter");
  xmlnode.setAttribute("Name", PARAM_SELECTIONPOOLSIZE);
  xmlnode.setAttribute("Val", ui->t_poolsize->text());
  xmlroot.appendChild(xmlnode);

  xmlnode = doc.createElement("Parameter");
  xmlnode.setAttribute("Name", PARAM_SELECTIONCANDIDATESIZE);
  xmlnode.setAttribute("Val", ui->t_candidateSize->text());
  xmlroot.appendChild(xmlnode);

  xmlnode = doc.createElement("Parameter");
  xmlnode.setAttribute("Name", PARAM_GROUPS);
  xmlroot.appendChild(xmlnode);

  QVector< QVector<QDomElement>* > list;
  for(int i = 0; i < MAX_GROUP; ++i) {
	list.append(new QVector<QDomElement>());
  }

  //populate items in appropriate groups
  for (int i=0; i < modelGroups->rowCount(); ++i) {
	QStandardItem *group = modelGroups->item(i);
	QDomElement gxml = doc.createElement("Constraint");;
	gxml.setAttribute("ID", group->data().toString());
	gxml.setAttribute("Name", group->text());
	gxml.setAttribute("Desc", group->toolTip());
	list[modelGroups->item(i, 1)->text().at(6).digitValue()-1]->append(gxml);
  }

  for (int i=0; i < list.size(); ++i) {
	QVector<QDomElement> *sublist = list.at(i);
	if (sublist->size() == 0) {
	  continue;
	}
	//first element in group determines type of the group.
	int id = sublist->at(0).attribute("ID").toInt();
	QString type;
	if (id < 3) {
	  //hard group
	  type = "Hard";
	} else {
	  type = "Soft";
	}
	//create group node
	QDomElement group = doc.createElement("Group");
	group.setAttribute("Type", type);
	//append constraints to group
	for (int j=0; j < sublist->count(); ++j) {
	  group.appendChild(sublist->at(j));
	}
	xmlnode.appendChild(group);
  }



  //save to disk
  QFile file(filename);
  if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
  {
	QMessageBox::critical(this, "Error!", "Failed to write file.");
  }

  QTextStream stream(&file);
  stream << doc.toString();
  file.close();
}

//save course xml
void Settings::on_pushButton_clicked()
{
  WriteCourses(pathCourse);
}
//open xml
void Settings::on_pushButton_3_clicked()
{
  QString file = QFileDialog::getOpenFileName(this, tr("Open XML"), QDir::currentPath(), tr("XML (*.xml)"));
  if (!file.isEmpty()) {
	pathCourse = file;
	ReadCourses(file);
  }
}

void Settings::on_pushAddCourse_clicked()
{
  int row = ui->treeView->selectionModel()->selectedRows().at(0).row();
  QList<QStandardItem*> *list = new QList<QStandardItem*>();
  for (int i = 0; i < modelCourses->columnCount(); i++) {
	list->append(modelCourses->item(row, i)->clone());
  }
  modelCourses->appendRow(*list);

  ui->treeView->setCurrentIndex(modelCourses->index(modelCourses->rowCount() - 1, 0));
}
//save as xml
void Settings::on_pushButton_4_clicked()
{
  QString file = QFileDialog::getSaveFileName(this, tr("Save XML"), QDir::currentPath(), tr("XML (*.xml)"));
  if (!file.isEmpty()) {
	pathCourse = file;
	WriteCourses(file);
  }
}

void Settings::on_pushDeleteCourse_clicked()
{
  QItemSelection selection( ui->treeView->selectionModel()->selection() );

  QList<int> rows;
  foreach( const QModelIndex & index, selection.indexes() ) {
	rows.append( index.row() );
  }

  qSort( rows );

  int prev = -1;
  for( int i = rows.count() - 1; i >= 0; i -= 1 ) {
	int current = rows[i];
	if( current != prev ) {
	  modelCourses->removeRows( current, 1 );
	  prev = current;
	}
  }
}

void Settings::ReadPrereq(QString filename)
{
  clearTable();
  QDomDocument doc;
  //load xml
  QFile file(filename);
  if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
	doc.setContent(&file);
	file.close();
  } else {
	QMessageBox::critical(this, "Error!", "Cannot read prereq.xml. Executable is corrupted. Provide new one.");
  }
  //get the xml root element
  QDomElement xmlroot = doc.firstChildElement();
  //traverse
  QDomNodeList courses = xmlroot.elementsByTagName("Course");

  for(int i = 0; i < courses.count(); i++) {
	QList<QStandardItem*> *list = new QList<QStandardItem*>();
	QDomElement course = courses.at(i).toElement();

	QStandardItem *course1 = new QStandardItem(course.attribute("Course1"));
	list->append(course1);

	QStandardItem *course2 = new QStandardItem(course.attribute("Course2"));
	list->append(course2);
	modelPrereq->appendRow(*list);
  }
}

void Settings::WritePrereq(QString filename)
{
  //write the xml file
  QDomDocument doc;
  //Make a root node
  QDomElement xmlroot = doc.createElement("Courses");
  doc.appendChild(xmlroot);

  for(int i = 0; i < modelPrereq->rowCount(); i++)
  {
	QDomElement xmlPrereq = doc.createElement("Course");
	xmlPrereq.setAttribute("Course1",modelPrereq->item(i, 0)->text());
	xmlPrereq.setAttribute("Course2",modelPrereq->item(i, 1)->text());
	xmlroot.appendChild(xmlPrereq);
  }

  //save to disk
  QFile file(filename);
  if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
  {
	QMessageBox::critical(this, "Error!", "Failed to write file.");
  }

  QTextStream stream(&file);
  stream << doc.toString();
  file.close();
}

void Settings::clearTable()
{
  modelPrereq->clear();
  QStringList header;
  header << "Course 1" << "Course 2";
  modelPrereq->setHorizontalHeaderLabels(header);
}

//delete prereq
void Settings::on_pushButton_5_clicked()
{
  QItemSelection selection( ui->tableView->selectionModel()->selection() );

  QList<int> rows;
  foreach( const QModelIndex & index, selection.indexes() ) {
	rows.append( index.row() );
  }

  qSort( rows );

  int prev = -1;
  for( int i = rows.count() - 1; i >= 0; i -= 1 ) {
	int current = rows[i];
	if( current != prev ) {
	  modelPrereq->removeRows( current, 1 );
	  prev = current;
	}
  }
}

//add new prereq
void Settings::on_pushButton_2_clicked()
{
  QList<QStandardItem*> *list = new QList<QStandardItem*>();
  list->append(new QStandardItem("Course 1"));
  list->append(new QStandardItem("Course 2"));
  modelPrereq->appendRow(*list);
}

//open prereq
void Settings::on_pushButton_8_clicked()
{
  QString file = QFileDialog::getOpenFileName(this, tr("Open XML"), QDir::currentPath(), tr("XML (*.xml)"));
  if (!file.isEmpty()) {
	pathPrereq = file;
	ReadPrereq(file);
  }
}

//save prereq
void Settings::on_pushButton_6_clicked()
{
  WritePrereq(pathPrereq);
}

//save as prereq
void Settings::on_pushButton_7_clicked()
{
  QString file = QFileDialog::getSaveFileName(this, tr("Save XML"), QDir::currentPath(), tr("XML (*.xml)"));
  if (!file.isEmpty()) {
	pathPrereq = file;
	WritePrereq(file);
  }
}

void Settings::on_checkBox_stateChanged(int arg1)
{
  if (arg1 == Qt::Checked) {
	ui->t_seed->setEnabled(false);
  } else {
	ui->t_seed->setEnabled(true);
  }
}
