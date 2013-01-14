/********************************************************************************
** Form generated from reading UI file 'aboutbox.ui'
**
** Created: Tue 25. Dec 01:34:56 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ABOUTBOX_H
#define UI_ABOUTBOX_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_AboutBox
{
public:
    QLabel *label;
    QLabel *label_2;
    QPushButton *pushButton;

    void setupUi(QDialog *AboutBox)
    {
        if (AboutBox->objectName().isEmpty())
            AboutBox->setObjectName(QString::fromUtf8("AboutBox"));
        AboutBox->resize(317, 119);
        label = new QLabel(AboutBox);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(60, 10, 201, 21));
        QFont font;
        font.setPointSize(11);
        label->setFont(font);
        label_2 = new QLabel(AboutBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(110, 40, 111, 21));
        pushButton = new QPushButton(AboutBox);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(120, 80, 75, 23));

        retranslateUi(AboutBox);

        QMetaObject::connectSlotsByName(AboutBox);
    } // setupUi

    void retranslateUi(QDialog *AboutBox)
    {
        AboutBox->setWindowTitle(QApplication::translate("AboutBox", "Dialog", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("AboutBox", "University Course Timetabling", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("AboutBox", "Halil Can Ka\305\237kavalc\304\261", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("AboutBox", "OK", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class AboutBox: public Ui_AboutBox {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ABOUTBOX_H
