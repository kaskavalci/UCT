/********************************************************************************
** Form generated from reading UI file 'xmlparser.ui'
**
** Created: Sun 23. Dec 17:20:11 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_XMLPARSER_H
#define UI_XMLPARSER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_XMLParser
{
public:
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;

    void setupUi(QDialog *XMLParser)
    {
        if (XMLParser->objectName().isEmpty())
            XMLParser->setObjectName(QString::fromUtf8("XMLParser"));
        XMLParser->resize(400, 300);
        horizontalLayoutWidget = new QWidget(XMLParser);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(120, 30, 251, 171));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);

        retranslateUi(XMLParser);

        QMetaObject::connectSlotsByName(XMLParser);
    } // setupUi

    void retranslateUi(QDialog *XMLParser)
    {
        XMLParser->setWindowTitle(QApplication::translate("XMLParser", "Dialog", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class XMLParser: public Ui_XMLParser {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_XMLPARSER_H
