/********************************************************************************
** Form generated from reading UI file 'settings.ui'
**
** Created: Tue 25. Dec 01:38:29 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTINGS_H
#define UI_SETTINGS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QFormLayout>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>
#include <QtGui/QTabWidget>
#include <QtGui/QTableView>
#include <QtGui/QTreeView>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Settings
{
public:
    QDialogButtonBox *buttonBox;
    QTabWidget *tabCourses;
    QWidget *tab_4;
    QListWidget *listWidget_2;
    QWidget *tab;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QGroupBox *groupBox;
    QLabel *label_2;
    QLineEdit *t_duration;
    QLabel *label;
    QGroupBox *groupBox_2;
    QLabel *label_6;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QLabel *label_3;
    QLineEdit *t_hsize;
    QLabel *label_10;
    QGroupBox *groupBox_4;
    QLabel *label_8;
    QWidget *horizontalLayoutWidget_2;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_4;
    QLineEdit *t_hcrate;
    QLabel *label_11;
    QGroupBox *groupBox_5;
    QLabel *label_14;
    QWidget *horizontalLayoutWidget_4;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_15;
    QLineEdit *t_crossover;
    QLabel *label_16;
    QGroupBox *groupBox_3;
    QLabel *label_9;
    QWidget *horizontalLayoutWidget_3;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_12;
    QLineEdit *t_mutation;
    QLabel *label_13;
    QGroupBox *groupBox_7;
    QLabel *label_5;
    QWidget *horizontalLayoutWidget_6;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_7;
    QLineEdit *t_hcmax;
    QGroupBox *groupBox_6;
    QLabel *label_17;
    QWidget *horizontalLayoutWidget_5;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_18;
    QLineEdit *t_randomInsertion;
    QLabel *label_19;
    QGroupBox *groupBox_8;
    QLabel *label_20;
    QWidget *horizontalLayoutWidget_7;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_22;
    QLineEdit *t_crowdingDist;
    QGroupBox *groupBox_9;
    QLabel *label_21;
    QWidget *tab_2;
    QLabel *label_23;
    QGroupBox *groupBox_10;
    QWidget *formLayoutWidget;
    QFormLayout *formLayout;
    QLabel *label_24;
    QSpinBox *t_hcourseconf;
    QLabel *label_25;
    QLabel *label_26;
    QLabel *label_27;
    QSpinBox *t_hsameday;
    QLabel *label_28;
    QSpinBox *t_hmidday;
    QGroupBox *groupBox_11;
    QWidget *gridLayoutWidget_2;
    QGridLayout *gridLayout_2;
    QLabel *label_29;
    QLabel *label_30;
    QLabel *label_32;
    QLabel *label_31;
    QLabel *label_33;
    QLabel *label_34;
    QLabel *label_35;
    QLabel *label_37;
    QLabel *label_39;
    QLabel *label_41;
    QSpinBox *t_sdepmeet;
    QSpinBox *t_sHWlab;
    QSpinBox *t_slecturer;
    QSpinBox *t_sconsecCourse;
    QSpinBox *t_snolunch;
    QSpinBox *t_smorninglab;
    QLabel *label_42;
    QLabel *label_43;
    QLabel *label_40;
    QSpinBox *t_seveningLecture;
    QLabel *label_38;
    QSpinBox *t_sconsecLab;
    QLabel *label_36;
    QSpinBox *t_sLTL;
    QLabel *label_44;
    QSpinBox *spinBox;
    QSpinBox *spinBox_2;
    QSpinBox *spinBox_3;
    QWidget *t_couse;
    QTreeView *treeView;
    QWidget *horizontalLayoutWidget_8;
    QHBoxLayout *horizontalLayout_8;
    QPushButton *pushAddCourse;
    QPushButton *pushDeleteCourse;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    QPushButton *pushButton;
    QWidget *tab_3;
    QGroupBox *groupBox_12;
    QTableView *tableView;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QPushButton *pushButton_5;
    QPushButton *pushButton_2;
    QPushButton *pushButton_8;
    QPushButton *pushButton_6;
    QPushButton *pushButton_7;
    QLabel *label_45;

    void setupUi(QDialog *Settings)
    {
        if (Settings->objectName().isEmpty())
            Settings->setObjectName(QString::fromUtf8("Settings"));
        Settings->resize(716, 533);
        buttonBox = new QDialogButtonBox(Settings);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(490, 490, 221, 41));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        tabCourses = new QTabWidget(Settings);
        tabCourses->setObjectName(QString::fromUtf8("tabCourses"));
        tabCourses->setGeometry(QRect(0, 0, 711, 491));
        tabCourses->setMovable(false);
        tab_4 = new QWidget();
        tab_4->setObjectName(QString::fromUtf8("tab_4"));
        listWidget_2 = new QListWidget(tab_4);
        listWidget_2->setObjectName(QString::fromUtf8("listWidget_2"));
        listWidget_2->setGeometry(QRect(90, 120, 301, 241));
        tabCourses->addTab(tab_4, QString());
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        gridLayoutWidget = new QWidget(tab);
        gridLayoutWidget->setObjectName(QString::fromUtf8("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(0, 10, 701, 451));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        groupBox = new QGroupBox(gridLayoutWidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 20, 211, 41));
        label_2->setWordWrap(true);
        t_duration = new QLineEdit(groupBox);
        t_duration->setObjectName(QString::fromUtf8("t_duration"));
        t_duration->setGeometry(QRect(60, 70, 91, 20));
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 70, 51, 16));

        gridLayout->addWidget(groupBox, 0, 0, 1, 1);

        groupBox_2 = new QGroupBox(gridLayoutWidget);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        label_6 = new QLabel(groupBox_2);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(10, 20, 201, 31));
        label_6->setWordWrap(true);
        horizontalLayoutWidget = new QWidget(groupBox_2);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(10, 60, 181, 31));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label_3 = new QLabel(horizontalLayoutWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout->addWidget(label_3);

        t_hsize = new QLineEdit(horizontalLayoutWidget);
        t_hsize->setObjectName(QString::fromUtf8("t_hsize"));
        t_hsize->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout->addWidget(t_hsize);

        label_10 = new QLabel(horizontalLayoutWidget);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        horizontalLayout->addWidget(label_10);


        gridLayout->addWidget(groupBox_2, 0, 1, 1, 1);

        groupBox_4 = new QGroupBox(gridLayoutWidget);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        label_8 = new QLabel(groupBox_4);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(10, 20, 201, 31));
        label_8->setWordWrap(true);
        horizontalLayoutWidget_2 = new QWidget(groupBox_4);
        horizontalLayoutWidget_2->setObjectName(QString::fromUtf8("horizontalLayoutWidget_2"));
        horizontalLayoutWidget_2->setGeometry(QRect(10, 60, 181, 31));
        horizontalLayout_4 = new QHBoxLayout(horizontalLayoutWidget_2);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        label_4 = new QLabel(horizontalLayoutWidget_2);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        horizontalLayout_4->addWidget(label_4);

        t_hcrate = new QLineEdit(horizontalLayoutWidget_2);
        t_hcrate->setObjectName(QString::fromUtf8("t_hcrate"));
        t_hcrate->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_4->addWidget(t_hcrate);

        label_11 = new QLabel(horizontalLayoutWidget_2);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        horizontalLayout_4->addWidget(label_11);


        gridLayout->addWidget(groupBox_4, 1, 1, 1, 1);

        groupBox_5 = new QGroupBox(gridLayoutWidget);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
        label_14 = new QLabel(groupBox_5);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setGeometry(QRect(10, 20, 201, 21));
        label_14->setWordWrap(true);
        horizontalLayoutWidget_4 = new QWidget(groupBox_5);
        horizontalLayoutWidget_4->setObjectName(QString::fromUtf8("horizontalLayoutWidget_4"));
        horizontalLayoutWidget_4->setGeometry(QRect(10, 60, 181, 31));
        horizontalLayout_5 = new QHBoxLayout(horizontalLayoutWidget_4);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalLayout_5->setContentsMargins(0, 0, 0, 0);
        label_15 = new QLabel(horizontalLayoutWidget_4);
        label_15->setObjectName(QString::fromUtf8("label_15"));

        horizontalLayout_5->addWidget(label_15);

        t_crossover = new QLineEdit(horizontalLayoutWidget_4);
        t_crossover->setObjectName(QString::fromUtf8("t_crossover"));
        t_crossover->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_5->addWidget(t_crossover);

        label_16 = new QLabel(horizontalLayoutWidget_4);
        label_16->setObjectName(QString::fromUtf8("label_16"));

        horizontalLayout_5->addWidget(label_16);


        gridLayout->addWidget(groupBox_5, 2, 0, 1, 1);

        groupBox_3 = new QGroupBox(gridLayoutWidget);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        label_9 = new QLabel(groupBox_3);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(10, 10, 211, 41));
        label_9->setWordWrap(true);
        horizontalLayoutWidget_3 = new QWidget(groupBox_3);
        horizontalLayoutWidget_3->setObjectName(QString::fromUtf8("horizontalLayoutWidget_3"));
        horizontalLayoutWidget_3->setGeometry(QRect(10, 60, 191, 31));
        horizontalLayout_3 = new QHBoxLayout(horizontalLayoutWidget_3);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        label_12 = new QLabel(horizontalLayoutWidget_3);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        horizontalLayout_3->addWidget(label_12);

        t_mutation = new QLineEdit(horizontalLayoutWidget_3);
        t_mutation->setObjectName(QString::fromUtf8("t_mutation"));
        t_mutation->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_3->addWidget(t_mutation);

        label_13 = new QLabel(horizontalLayoutWidget_3);
        label_13->setObjectName(QString::fromUtf8("label_13"));

        horizontalLayout_3->addWidget(label_13);


        gridLayout->addWidget(groupBox_3, 1, 0, 1, 1);

        groupBox_7 = new QGroupBox(gridLayoutWidget);
        groupBox_7->setObjectName(QString::fromUtf8("groupBox_7"));
        label_5 = new QLabel(groupBox_7);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(10, 20, 201, 41));
        label_5->setWordWrap(true);
        horizontalLayoutWidget_6 = new QWidget(groupBox_7);
        horizontalLayoutWidget_6->setObjectName(QString::fromUtf8("horizontalLayoutWidget_6"));
        horizontalLayoutWidget_6->setGeometry(QRect(10, 70, 160, 31));
        horizontalLayout_7 = new QHBoxLayout(horizontalLayoutWidget_6);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        horizontalLayout_7->setContentsMargins(0, 0, 0, 0);
        label_7 = new QLabel(horizontalLayoutWidget_6);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        horizontalLayout_7->addWidget(label_7);

        t_hcmax = new QLineEdit(horizontalLayoutWidget_6);
        t_hcmax->setObjectName(QString::fromUtf8("t_hcmax"));
        t_hcmax->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_7->addWidget(t_hcmax);


        gridLayout->addWidget(groupBox_7, 0, 2, 1, 1);

        groupBox_6 = new QGroupBox(gridLayoutWidget);
        groupBox_6->setObjectName(QString::fromUtf8("groupBox_6"));
        label_17 = new QLabel(groupBox_6);
        label_17->setObjectName(QString::fromUtf8("label_17"));
        label_17->setGeometry(QRect(10, 20, 211, 51));
        label_17->setWordWrap(true);
        horizontalLayoutWidget_5 = new QWidget(groupBox_6);
        horizontalLayoutWidget_5->setObjectName(QString::fromUtf8("horizontalLayoutWidget_5"));
        horizontalLayoutWidget_5->setGeometry(QRect(10, 70, 181, 31));
        horizontalLayout_6 = new QHBoxLayout(horizontalLayoutWidget_5);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        horizontalLayout_6->setContentsMargins(0, 0, 0, 0);
        label_18 = new QLabel(horizontalLayoutWidget_5);
        label_18->setObjectName(QString::fromUtf8("label_18"));

        horizontalLayout_6->addWidget(label_18);

        t_randomInsertion = new QLineEdit(horizontalLayoutWidget_5);
        t_randomInsertion->setObjectName(QString::fromUtf8("t_randomInsertion"));
        t_randomInsertion->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_6->addWidget(t_randomInsertion);

        label_19 = new QLabel(horizontalLayoutWidget_5);
        label_19->setObjectName(QString::fromUtf8("label_19"));

        horizontalLayout_6->addWidget(label_19);


        gridLayout->addWidget(groupBox_6, 2, 1, 1, 1);

        groupBox_8 = new QGroupBox(gridLayoutWidget);
        groupBox_8->setObjectName(QString::fromUtf8("groupBox_8"));
        label_20 = new QLabel(groupBox_8);
        label_20->setObjectName(QString::fromUtf8("label_20"));
        label_20->setGeometry(QRect(10, 20, 201, 41));
        label_20->setWordWrap(true);
        horizontalLayoutWidget_7 = new QWidget(groupBox_8);
        horizontalLayoutWidget_7->setObjectName(QString::fromUtf8("horizontalLayoutWidget_7"));
        horizontalLayoutWidget_7->setGeometry(QRect(10, 70, 181, 31));
        horizontalLayout_2 = new QHBoxLayout(horizontalLayoutWidget_7);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        label_22 = new QLabel(horizontalLayoutWidget_7);
        label_22->setObjectName(QString::fromUtf8("label_22"));

        horizontalLayout_2->addWidget(label_22);

        t_crowdingDist = new QLineEdit(horizontalLayoutWidget_7);
        t_crowdingDist->setObjectName(QString::fromUtf8("t_crowdingDist"));
        t_crowdingDist->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout_2->addWidget(t_crowdingDist);


        gridLayout->addWidget(groupBox_8, 1, 2, 1, 1);

        groupBox_9 = new QGroupBox(gridLayoutWidget);
        groupBox_9->setObjectName(QString::fromUtf8("groupBox_9"));
        label_21 = new QLabel(groupBox_9);
        label_21->setObjectName(QString::fromUtf8("label_21"));
        label_21->setGeometry(QRect(50, 20, 131, 121));
        label_21->setPixmap(QPixmap(QString::fromUtf8(":/Resource/happyface.png")));

        gridLayout->addWidget(groupBox_9, 2, 2, 1, 1);

        tabCourses->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        label_23 = new QLabel(tab_2);
        label_23->setObjectName(QString::fromUtf8("label_23"));
        label_23->setGeometry(QRect(10, 10, 631, 31));
        label_23->setWordWrap(true);
        groupBox_10 = new QGroupBox(tab_2);
        groupBox_10->setObjectName(QString::fromUtf8("groupBox_10"));
        groupBox_10->setGeometry(QRect(10, 70, 201, 131));
        formLayoutWidget = new QWidget(groupBox_10);
        formLayoutWidget->setObjectName(QString::fromUtf8("formLayoutWidget"));
        formLayoutWidget->setGeometry(QRect(10, 20, 181, 101));
        formLayout = new QFormLayout(formLayoutWidget);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setContentsMargins(0, 0, 0, 0);
        label_24 = new QLabel(formLayoutWidget);
        label_24->setObjectName(QString::fromUtf8("label_24"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_24);

        t_hcourseconf = new QSpinBox(formLayoutWidget);
        t_hcourseconf->setObjectName(QString::fromUtf8("t_hcourseconf"));
        t_hcourseconf->setMinimum(1);

        formLayout->setWidget(1, QFormLayout::FieldRole, t_hcourseconf);

        label_25 = new QLabel(formLayoutWidget);
        label_25->setObjectName(QString::fromUtf8("label_25"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label_25);

        label_26 = new QLabel(formLayoutWidget);
        label_26->setObjectName(QString::fromUtf8("label_26"));

        formLayout->setWidget(0, QFormLayout::FieldRole, label_26);

        label_27 = new QLabel(formLayoutWidget);
        label_27->setObjectName(QString::fromUtf8("label_27"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label_27);

        t_hsameday = new QSpinBox(formLayoutWidget);
        t_hsameday->setObjectName(QString::fromUtf8("t_hsameday"));
        t_hsameday->setMinimum(1);

        formLayout->setWidget(2, QFormLayout::FieldRole, t_hsameday);

        label_28 = new QLabel(formLayoutWidget);
        label_28->setObjectName(QString::fromUtf8("label_28"));

        formLayout->setWidget(3, QFormLayout::LabelRole, label_28);

        t_hmidday = new QSpinBox(formLayoutWidget);
        t_hmidday->setObjectName(QString::fromUtf8("t_hmidday"));
        t_hmidday->setMinimum(1);
        t_hmidday->setValue(2);

        formLayout->setWidget(3, QFormLayout::FieldRole, t_hmidday);

        groupBox_11 = new QGroupBox(tab_2);
        groupBox_11->setObjectName(QString::fromUtf8("groupBox_11"));
        groupBox_11->setGeometry(QRect(240, 70, 421, 211));
        gridLayoutWidget_2 = new QWidget(groupBox_11);
        gridLayoutWidget_2->setObjectName(QString::fromUtf8("gridLayoutWidget_2"));
        gridLayoutWidget_2->setGeometry(QRect(10, 20, 401, 171));
        gridLayout_2 = new QGridLayout(gridLayoutWidget_2);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        label_29 = new QLabel(gridLayoutWidget_2);
        label_29->setObjectName(QString::fromUtf8("label_29"));

        gridLayout_2->addWidget(label_29, 1, 0, 1, 1);

        label_30 = new QLabel(gridLayoutWidget_2);
        label_30->setObjectName(QString::fromUtf8("label_30"));

        gridLayout_2->addWidget(label_30, 1, 1, 1, 1);

        label_32 = new QLabel(gridLayoutWidget_2);
        label_32->setObjectName(QString::fromUtf8("label_32"));

        gridLayout_2->addWidget(label_32, 1, 3, 1, 1);

        label_31 = new QLabel(gridLayoutWidget_2);
        label_31->setObjectName(QString::fromUtf8("label_31"));

        gridLayout_2->addWidget(label_31, 1, 2, 1, 1);

        label_33 = new QLabel(gridLayoutWidget_2);
        label_33->setObjectName(QString::fromUtf8("label_33"));

        gridLayout_2->addWidget(label_33, 2, 0, 1, 1);

        label_34 = new QLabel(gridLayoutWidget_2);
        label_34->setObjectName(QString::fromUtf8("label_34"));

        gridLayout_2->addWidget(label_34, 2, 2, 1, 1);

        label_35 = new QLabel(gridLayoutWidget_2);
        label_35->setObjectName(QString::fromUtf8("label_35"));

        gridLayout_2->addWidget(label_35, 4, 0, 1, 1);

        label_37 = new QLabel(gridLayoutWidget_2);
        label_37->setObjectName(QString::fromUtf8("label_37"));

        gridLayout_2->addWidget(label_37, 5, 0, 1, 1);

        label_39 = new QLabel(gridLayoutWidget_2);
        label_39->setObjectName(QString::fromUtf8("label_39"));

        gridLayout_2->addWidget(label_39, 6, 0, 1, 1);

        label_41 = new QLabel(gridLayoutWidget_2);
        label_41->setObjectName(QString::fromUtf8("label_41"));

        gridLayout_2->addWidget(label_41, 7, 0, 1, 1);

        t_sdepmeet = new QSpinBox(gridLayoutWidget_2);
        t_sdepmeet->setObjectName(QString::fromUtf8("t_sdepmeet"));
        t_sdepmeet->setMinimum(1);

        gridLayout_2->addWidget(t_sdepmeet, 2, 1, 1, 1);

        t_sHWlab = new QSpinBox(gridLayoutWidget_2);
        t_sHWlab->setObjectName(QString::fromUtf8("t_sHWlab"));
        t_sHWlab->setMinimum(1);

        gridLayout_2->addWidget(t_sHWlab, 2, 3, 1, 1);

        t_slecturer = new QSpinBox(gridLayoutWidget_2);
        t_slecturer->setObjectName(QString::fromUtf8("t_slecturer"));
        t_slecturer->setMinimum(1);

        gridLayout_2->addWidget(t_slecturer, 4, 1, 1, 1);

        t_sconsecCourse = new QSpinBox(gridLayoutWidget_2);
        t_sconsecCourse->setObjectName(QString::fromUtf8("t_sconsecCourse"));
        t_sconsecCourse->setMinimum(1);

        gridLayout_2->addWidget(t_sconsecCourse, 5, 1, 1, 1);

        t_snolunch = new QSpinBox(gridLayoutWidget_2);
        t_snolunch->setObjectName(QString::fromUtf8("t_snolunch"));
        t_snolunch->setMinimum(1);
        t_snolunch->setValue(2);

        gridLayout_2->addWidget(t_snolunch, 6, 1, 1, 1);

        t_smorninglab = new QSpinBox(gridLayoutWidget_2);
        t_smorninglab->setObjectName(QString::fromUtf8("t_smorninglab"));
        t_smorninglab->setMinimum(1);
        t_smorninglab->setValue(2);

        gridLayout_2->addWidget(t_smorninglab, 7, 1, 1, 1);

        label_42 = new QLabel(gridLayoutWidget_2);
        label_42->setObjectName(QString::fromUtf8("label_42"));

        gridLayout_2->addWidget(label_42, 3, 2, 1, 1);

        label_43 = new QLabel(gridLayoutWidget_2);
        label_43->setObjectName(QString::fromUtf8("label_43"));

        gridLayout_2->addWidget(label_43, 3, 0, 1, 1);

        label_40 = new QLabel(gridLayoutWidget_2);
        label_40->setObjectName(QString::fromUtf8("label_40"));

        gridLayout_2->addWidget(label_40, 7, 2, 1, 1);

        t_seveningLecture = new QSpinBox(gridLayoutWidget_2);
        t_seveningLecture->setObjectName(QString::fromUtf8("t_seveningLecture"));
        t_seveningLecture->setMinimum(1);
        t_seveningLecture->setValue(2);

        gridLayout_2->addWidget(t_seveningLecture, 7, 3, 1, 1);

        label_38 = new QLabel(gridLayoutWidget_2);
        label_38->setObjectName(QString::fromUtf8("label_38"));

        gridLayout_2->addWidget(label_38, 6, 2, 1, 1);

        t_sconsecLab = new QSpinBox(gridLayoutWidget_2);
        t_sconsecLab->setObjectName(QString::fromUtf8("t_sconsecLab"));
        t_sconsecLab->setMinimum(1);
        t_sconsecLab->setValue(2);

        gridLayout_2->addWidget(t_sconsecLab, 6, 3, 1, 1);

        label_36 = new QLabel(gridLayoutWidget_2);
        label_36->setObjectName(QString::fromUtf8("label_36"));

        gridLayout_2->addWidget(label_36, 5, 2, 1, 1);

        t_sLTL = new QSpinBox(gridLayoutWidget_2);
        t_sLTL->setObjectName(QString::fromUtf8("t_sLTL"));
        t_sLTL->setMinimum(1);

        gridLayout_2->addWidget(t_sLTL, 5, 3, 1, 1);

        label_44 = new QLabel(gridLayoutWidget_2);
        label_44->setObjectName(QString::fromUtf8("label_44"));

        gridLayout_2->addWidget(label_44, 4, 2, 1, 1);

        spinBox = new QSpinBox(gridLayoutWidget_2);
        spinBox->setObjectName(QString::fromUtf8("spinBox"));
        spinBox->setMinimum(1);

        gridLayout_2->addWidget(spinBox, 3, 1, 1, 1);

        spinBox_2 = new QSpinBox(gridLayoutWidget_2);
        spinBox_2->setObjectName(QString::fromUtf8("spinBox_2"));
        spinBox_2->setMinimum(1);

        gridLayout_2->addWidget(spinBox_2, 3, 3, 1, 1);

        spinBox_3 = new QSpinBox(gridLayoutWidget_2);
        spinBox_3->setObjectName(QString::fromUtf8("spinBox_3"));
        spinBox_3->setMinimum(1);

        gridLayout_2->addWidget(spinBox_3, 4, 3, 1, 1);

        tabCourses->addTab(tab_2, QString());
        t_couse = new QWidget();
        t_couse->setObjectName(QString::fromUtf8("t_couse"));
        treeView = new QTreeView(t_couse);
        treeView->setObjectName(QString::fromUtf8("treeView"));
        treeView->setGeometry(QRect(10, 40, 681, 351));
        treeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
        treeView->setSortingEnabled(true);
        treeView->setExpandsOnDoubleClick(false);
        horizontalLayoutWidget_8 = new QWidget(t_couse);
        horizontalLayoutWidget_8->setObjectName(QString::fromUtf8("horizontalLayoutWidget_8"));
        horizontalLayoutWidget_8->setGeometry(QRect(290, 400, 402, 41));
        horizontalLayout_8 = new QHBoxLayout(horizontalLayoutWidget_8);
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        horizontalLayout_8->setContentsMargins(0, 0, 0, 0);
        pushAddCourse = new QPushButton(horizontalLayoutWidget_8);
        pushAddCourse->setObjectName(QString::fromUtf8("pushAddCourse"));
        pushAddCourse->setEnabled(true);

        horizontalLayout_8->addWidget(pushAddCourse);

        pushDeleteCourse = new QPushButton(horizontalLayoutWidget_8);
        pushDeleteCourse->setObjectName(QString::fromUtf8("pushDeleteCourse"));

        horizontalLayout_8->addWidget(pushDeleteCourse);

        pushButton_3 = new QPushButton(horizontalLayoutWidget_8);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setEnabled(true);

        horizontalLayout_8->addWidget(pushButton_3);

        pushButton_4 = new QPushButton(horizontalLayoutWidget_8);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));

        horizontalLayout_8->addWidget(pushButton_4);

        pushButton = new QPushButton(horizontalLayoutWidget_8);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        horizontalLayout_8->addWidget(pushButton);

        tabCourses->addTab(t_couse, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        groupBox_12 = new QGroupBox(tab_3);
        groupBox_12->setObjectName(QString::fromUtf8("groupBox_12"));
        groupBox_12->setGeometry(QRect(10, 10, 601, 361));
        tableView = new QTableView(groupBox_12);
        tableView->setObjectName(QString::fromUtf8("tableView"));
        tableView->setGeometry(QRect(10, 70, 261, 271));
        verticalLayoutWidget = new QWidget(groupBox_12);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(280, 70, 81, 161));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        pushButton_5 = new QPushButton(verticalLayoutWidget);
        pushButton_5->setObjectName(QString::fromUtf8("pushButton_5"));

        verticalLayout->addWidget(pushButton_5);

        pushButton_2 = new QPushButton(verticalLayoutWidget);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));

        verticalLayout->addWidget(pushButton_2);

        pushButton_8 = new QPushButton(verticalLayoutWidget);
        pushButton_8->setObjectName(QString::fromUtf8("pushButton_8"));

        verticalLayout->addWidget(pushButton_8);

        pushButton_6 = new QPushButton(verticalLayoutWidget);
        pushButton_6->setObjectName(QString::fromUtf8("pushButton_6"));

        verticalLayout->addWidget(pushButton_6);

        pushButton_7 = new QPushButton(verticalLayoutWidget);
        pushButton_7->setObjectName(QString::fromUtf8("pushButton_7"));

        verticalLayout->addWidget(pushButton_7);

        label_45 = new QLabel(groupBox_12);
        label_45->setObjectName(QString::fromUtf8("label_45"));
        label_45->setGeometry(QRect(10, 20, 631, 31));
        tabCourses->addTab(tab_3, QString());
#ifndef QT_NO_SHORTCUT
        label_2->setBuddy(t_duration);
        label->setBuddy(t_duration);
        label_6->setBuddy(t_hsize);
        label_3->setBuddy(t_hsize);
        label_8->setBuddy(t_hcrate);
        label_4->setBuddy(t_hcrate);
        label_14->setBuddy(t_crossover);
        label_15->setBuddy(t_crossover);
        label_9->setBuddy(t_mutation);
        label_12->setBuddy(t_mutation);
        label_5->setBuddy(t_hcmax);
        label_7->setBuddy(t_hcmax);
        label_17->setBuddy(t_randomInsertion);
        label_18->setBuddy(t_randomInsertion);
        label_20->setBuddy(t_crowdingDist);
        label_22->setBuddy(t_crowdingDist);
        label_24->setBuddy(t_hcourseconf);
        label_27->setBuddy(t_hsameday);
        label_28->setBuddy(t_hmidday);
        label_33->setBuddy(t_sdepmeet);
        label_34->setBuddy(t_sHWlab);
        label_35->setBuddy(t_slecturer);
        label_37->setBuddy(t_sconsecCourse);
        label_39->setBuddy(t_snolunch);
        label_41->setBuddy(t_smorninglab);
        label_42->setBuddy(spinBox_2);
        label_43->setBuddy(spinBox);
        label_40->setBuddy(t_seveningLecture);
        label_38->setBuddy(t_sconsecLab);
        label_36->setBuddy(t_sLTL);
        label_44->setBuddy(spinBox_3);
#endif // QT_NO_SHORTCUT

        retranslateUi(Settings);
        QObject::connect(buttonBox, SIGNAL(accepted()), Settings, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), Settings, SLOT(reject()));

        tabCourses->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(Settings);
    } // setupUi

    void retranslateUi(QDialog *Settings)
    {
        Settings->setWindowTitle(QApplication::translate("Settings", "Settings", 0, QApplication::UnicodeUTF8));
        tabCourses->setTabText(tabCourses->indexOf(tab_4), QApplication::translate("Settings", "Page", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("Settings", "Duration", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("Settings", "Number of seconds program will execute. Longer runs tend to have better results. For convenient results, give at least 400.", 0, QApplication::UnicodeUTF8));
        t_duration->setText(QApplication::translate("Settings", "400", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        label->setToolTip(QApplication::translate("Settings", "How long the program will execute, in seconds. Bigger values tend to have better results. For convenient results, give at least 400.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        label->setText(QApplication::translate("Settings", "Duration", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("Settings", "Hill Climber Size", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("Settings", "It's the percentage of population HC will be applied. 100% is recommended.", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("Settings", "Hill Size", 0, QApplication::UnicodeUTF8));
        t_hsize->setText(QApplication::translate("Settings", "100", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("Settings", "%", 0, QApplication::UnicodeUTF8));
        groupBox_4->setTitle(QApplication::translate("Settings", "Hill Climber Rate", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("Settings", "It's the percentage HC will be applied to random Individual. 50% is recommended.", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("Settings", "HC Rate:", 0, QApplication::UnicodeUTF8));
        t_hcrate->setText(QApplication::translate("Settings", "50", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("Settings", "%", 0, QApplication::UnicodeUTF8));
        groupBox_5->setTitle(QApplication::translate("Settings", "Crossover", 0, QApplication::UnicodeUTF8));
        label_14->setText(QApplication::translate("Settings", "Percentage that crossover will be applied to produce new individuals.", 0, QApplication::UnicodeUTF8));
        label_15->setText(QApplication::translate("Settings", "Crossover", 0, QApplication::UnicodeUTF8));
        t_crossover->setText(QApplication::translate("Settings", "90", 0, QApplication::UnicodeUTF8));
        label_16->setText(QApplication::translate("Settings", "%", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("Settings", "Mutation", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("Settings", "Percentage that mutation will be applied to random individual. 1% is recommended.", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("Settings", "Mutation", 0, QApplication::UnicodeUTF8));
        t_mutation->setText(QApplication::translate("Settings", "1", 0, QApplication::UnicodeUTF8));
        label_13->setText(QApplication::translate("Settings", "%", 0, QApplication::UnicodeUTF8));
        groupBox_7->setTitle(QApplication::translate("Settings", "Hill Climb - Gene Number", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("Settings", "Apply HC to number of worst genes in individual. Higher values yield better individuals. ", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("Settings", "HC Gene #", 0, QApplication::UnicodeUTF8));
        t_hcmax->setText(QApplication::translate("Settings", "5", 0, QApplication::UnicodeUTF8));
        groupBox_6->setTitle(QApplication::translate("Settings", "Random Insertion", 0, QApplication::UnicodeUTF8));
        label_17->setText(QApplication::translate("Settings", "This helps to increase diversity. However higher values will corrupt the population. Use with care. 0.1% is recommended.", 0, QApplication::UnicodeUTF8));
        label_18->setText(QApplication::translate("Settings", "Random Insertion", 0, QApplication::UnicodeUTF8));
        t_randomInsertion->setText(QApplication::translate("Settings", "0.1", 0, QApplication::UnicodeUTF8));
        label_19->setText(QApplication::translate("Settings", "%", 0, QApplication::UnicodeUTF8));
        groupBox_8->setTitle(QApplication::translate("Settings", "Crowding Distance", 0, QApplication::UnicodeUTF8));
        label_20->setText(QApplication::translate("Settings", "Distance that pareto front members considered as \"near\" eachother. More info in help.", 0, QApplication::UnicodeUTF8));
        label_22->setText(QApplication::translate("Settings", "Crowding Distance", 0, QApplication::UnicodeUTF8));
        t_crowdingDist->setText(QApplication::translate("Settings", "8", 0, QApplication::UnicodeUTF8));
        groupBox_9->setTitle(QApplication::translate("Settings", "Happy Face", 0, QApplication::UnicodeUTF8));
        label_21->setText(QString());
        tabCourses->setTabText(tabCourses->indexOf(tab), QApplication::translate("Settings", "Parameters", 0, QApplication::UnicodeUTF8));
        label_23->setText(QApplication::translate("Settings", "Constraints can be combined via groups. Here you can choose groups for constraints. If every objective has a different group, it may take long time to find a feasible solution. When combining objectives, they are counted as one objective.", 0, QApplication::UnicodeUTF8));
        groupBox_10->setTitle(QApplication::translate("Settings", "Hard Objectives", 0, QApplication::UnicodeUTF8));
        label_24->setText(QApplication::translate("Settings", "Course Conflict", 0, QApplication::UnicodeUTF8));
        label_25->setText(QApplication::translate("Settings", "Conflict Name", 0, QApplication::UnicodeUTF8));
        label_26->setText(QApplication::translate("Settings", "Group ID", 0, QApplication::UnicodeUTF8));
        label_27->setText(QApplication::translate("Settings", "Same Day", 0, QApplication::UnicodeUTF8));
        label_28->setText(QApplication::translate("Settings", "Mid Day", 0, QApplication::UnicodeUTF8));
        groupBox_11->setTitle(QApplication::translate("Settings", "Soft Objectives", 0, QApplication::UnicodeUTF8));
        label_29->setText(QApplication::translate("Settings", "Conflict Name", 0, QApplication::UnicodeUTF8));
        label_30->setText(QApplication::translate("Settings", "Group ID", 0, QApplication::UnicodeUTF8));
        label_32->setText(QApplication::translate("Settings", "Group ID", 0, QApplication::UnicodeUTF8));
        label_31->setText(QApplication::translate("Settings", "Conflict Name", 0, QApplication::UnicodeUTF8));
        label_33->setText(QApplication::translate("Settings", "Department Meeting", 0, QApplication::UnicodeUTF8));
        label_34->setText(QApplication::translate("Settings", "HW Labs", 0, QApplication::UnicodeUTF8));
        label_35->setText(QApplication::translate("Settings", "Lecturer 4h consecutive work", 0, QApplication::UnicodeUTF8));
        label_37->setText(QApplication::translate("Settings", "Consecutive Semester Course", 0, QApplication::UnicodeUTF8));
        label_39->setText(QApplication::translate("Settings", "No Lunch Hour", 0, QApplication::UnicodeUTF8));
        label_41->setText(QApplication::translate("Settings", "Morning LAB", 0, QApplication::UnicodeUTF8));
        label_42->setText(QApplication::translate("Settings", "Lecturer over work", 0, QApplication::UnicodeUTF8));
        label_43->setText(QApplication::translate("Settings", "Lecturer No Free Day", 0, QApplication::UnicodeUTF8));
        label_40->setText(QApplication::translate("Settings", "Evening Lecture", 0, QApplication::UnicodeUTF8));
        label_38->setText(QApplication::translate("Settings", "Consecutive Semester LAB", 0, QApplication::UnicodeUTF8));
        label_36->setText(QApplication::translate("Settings", "LTL Conflict", 0, QApplication::UnicodeUTF8));
        label_44->setText(QApplication::translate("Settings", "Lecturer Morning Course", 0, QApplication::UnicodeUTF8));
        tabCourses->setTabText(tabCourses->indexOf(tab_2), QApplication::translate("Settings", "Groups", 0, QApplication::UnicodeUTF8));
        pushAddCourse->setText(QApplication::translate("Settings", "Add Course", 0, QApplication::UnicodeUTF8));
        pushDeleteCourse->setText(QApplication::translate("Settings", "Delete Course", 0, QApplication::UnicodeUTF8));
        pushButton_3->setText(QApplication::translate("Settings", "Open", 0, QApplication::UnicodeUTF8));
        pushButton_4->setText(QApplication::translate("Settings", "Save As", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("Settings", "Save", 0, QApplication::UnicodeUTF8));
        tabCourses->setTabText(tabCourses->indexOf(t_couse), QApplication::translate("Settings", "Courses", 0, QApplication::UnicodeUTF8));
        groupBox_12->setTitle(QApplication::translate("Settings", "Prerequisites", 0, QApplication::UnicodeUTF8));
        pushButton_5->setText(QApplication::translate("Settings", "Delete", 0, QApplication::UnicodeUTF8));
        pushButton_2->setText(QApplication::translate("Settings", "Add", 0, QApplication::UnicodeUTF8));
        pushButton_8->setText(QApplication::translate("Settings", "Open", 0, QApplication::UnicodeUTF8));
        pushButton_6->setText(QApplication::translate("Settings", "Save", 0, QApplication::UnicodeUTF8));
        pushButton_7->setText(QApplication::translate("Settings", "Save As", 0, QApplication::UnicodeUTF8));
        label_45->setText(QApplication::translate("Settings", "Prerequisite courses. Only give 6 letters. Course 1 and Course 2 will be marked as prerequisite to eachother.", 0, QApplication::UnicodeUTF8));
        tabCourses->setTabText(tabCourses->indexOf(tab_3), QApplication::translate("Settings", "Prerequisites", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Settings: public Ui_Settings {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGS_H
