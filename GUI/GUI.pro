TEMPLATE = app
QT       += core gui \
            xml \
            widgets

INCLUDEPATH += C:\\boost_1_53_0 \
                ../Core/include/

QMAKE_CXXFLAGS += -std=c++11
CONFIG += c++11
HEADERS += \
    ui_xmlparser.h \
    ui_settings.h \
    ui_mainwindow.h \
    ui_aboutbox.h \
    settings.h \
    mainwindow.h \
    comboboxdelegate.h \
    aboutbox.h \
    UI/ui_xmlparser.h \
    UI/ui_settings.h \
    UI/ui_mainwindow.h \
    UI/ui_aboutbox.h \
    ../Core/include/pugixml.hpp \
    ../Core/include/pugiconfig.hpp \
    ../Core/include/Printer.h \
    ../Core/include/Population.h \
    ../Core/include/Mutation.h \
    ../Core/include/Lecturer.h \
    ../Core/include/Lecture.h \
    ../Core/include/Individual.h \
    ../Core/include/Fitness.h \
    ../Core/include/FileReader.h \
    ../Core/include/Errnode.h \
    ../Core/include/Course.h \
    ../Core/include/Common.h \
    ../Core/include/Chromosome.h \
    execga.h \
    ../Core/include/Constants.h

SOURCES += \
    settings.cpp \
    mainwindow.cpp \
    main.cpp \
    comboboxdelegate.cpp \
    aboutbox.cpp \
    ../Core/src/pugixml.cpp \
    ../Core/src/Printer.cpp \
    ../Core/src/Population.cpp \
    ../Core/src/Mutation.cpp \
    ../Core/src/Lecturer.cpp \
    ../Core/src/Lecture.cpp \
    ../Core/src/Individual.cpp \
    ../Core/src/Fitness.cpp \
    ../Core/src/FileReader.cpp \
    ../Core/src/Errnode.cpp \
    ../Core/src/Course.cpp \
    ../Core/src/Common.cpp \
    ../Core/src/Chromosome.cpp \
    execga.cpp

FORMS += \
    settings.ui \
    mainwindow.ui \
    aboutbox.ui

UI_DIR = UI \

TARGET = ../UCT

RESOURCES += \
    Resource.qrc
win32:RC_FILE = myapp.rc
