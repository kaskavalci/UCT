#ifndef COMBOBOXDELEGATE_H
#define COMBOBOXDELEGATE_H

#include <QItemDelegate>
#include <QComboBox>

#define YES	"Yes"
#define	NO	"No"
#define sem0    "1st Semester"
#define sem1    "2nd Semester"
#define sem2    "3rd Semester"
#define sem3    "4th Semester"
#define sem4    "5th Semester"
#define sem5    "6th Semester"
#define sem6    "7th Semester"
#define sem7    "8th Semester"
#define hour0   "1 Hour"
#define hour1   "2 Hours"
#define hour2   "3 Hours"
#define none    "None"
#define day0    "Monday"
#define day1    "Tuesday"
#define day2    "Wednesday"
#define day3    "Thursday"
#define day4    "Friday"
#define slot0   "9-11"
#define slot1   "11-13"
#define slot2   "14-16"
#define slot3   "16-18"
#define MAX_GROUP 10

class ComboBoxDelegate : public QItemDelegate
 {
    Q_OBJECT

 public:
    ComboBoxDelegate(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                       const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                   const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor,
     const QStyleOptionViewItem &option, const QModelIndex &index) const;
};


class ComboBoxSemester : public ComboBoxDelegate
{
public:
    QWidget *createEditor(QWidget *parent,
                          const QStyleOptionViewItem &/* option */,
                          const QModelIndex &/* index */) const
    {
        QComboBox *editor = new QComboBox(parent);
        editor->addItem(sem0);
        editor->addItem(sem1);
        editor->addItem(sem2);
        editor->addItem(sem3);
        editor->addItem(sem4);
        editor->addItem(sem5);
        editor->addItem(sem6);
        editor->addItem(sem7);
        return editor;
    }
};

class ComboBoxNumHour : public ComboBoxDelegate
{
public:
    QWidget *createEditor(QWidget *parent,
                          const QStyleOptionViewItem &/* option */,
                          const QModelIndex &/* index */) const
    {
        QComboBox *editor = new QComboBox(parent);
        editor->addItem(hour0);
        editor->addItem(hour1);
		editor->addItem(hour2);
        return editor;
    }
};

class ComboBoxDay : public ComboBoxDelegate
{
public:
    QWidget *createEditor(QWidget *parent,
                          const QStyleOptionViewItem &/* option */,
                          const QModelIndex &/* index */) const
    {
        QComboBox *editor = new QComboBox(parent);
        editor->addItem(none);
        editor->addItem(day0);
        editor->addItem(day1);
        editor->addItem(day2);
        editor->addItem(day3);
        editor->addItem(day4);
        return editor;
    }
};

class ComboBoxSlot : public ComboBoxDelegate
{
public:
    QWidget *createEditor(QWidget *parent,
                          const QStyleOptionViewItem &/* option */,
                          const QModelIndex &/* index */) const
    {
        QComboBox *editor = new QComboBox(parent);
        editor->addItem(none);
        editor->addItem(slot0);
        editor->addItem(slot1);
        editor->addItem(slot2);
        editor->addItem(slot3);
        return editor;
    }
};

class ComboBoxLab : public ComboBoxDelegate
{
public:
	QWidget *createEditor(QWidget *parent,
						  const QStyleOptionViewItem &/* option */,
						  const QModelIndex &/* index */) const
	{
		QComboBox *editor = new QComboBox(parent);
		editor->addItem(NO);
		editor->addItem(YES);
		return editor;
	}
};

class ComboBoxGroup : public ComboBoxDelegate
{
public:
	QWidget *createEditor(QWidget *parent,
						  const QStyleOptionViewItem &/* option */,
						  const QModelIndex &/* index */) const
	{
		QComboBox *editor = new QComboBox(parent);
		editor->addItem("Disable");
		for(int i=0;i<MAX_GROUP;i++) {
		  //BEWARE! If you change Group name, please update WriteParam function too. at(6) returns group name.
		  //also numbers > 9 will not work either.
		  editor->addItem(QString("Group %1").arg(i+1));
		}
		return editor;
	}
};

#endif // COMBOBOXDELEGATE_H
