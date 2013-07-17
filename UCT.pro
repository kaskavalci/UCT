#-------------------------------------------------
#
# Project created by QtCreator 2012-12-21T21:22:25
#
#-------------------------------------------------

TEMPLATE = subdirs

OTHER_FILES += \
    Resources/prereq.xml \
    Resources/courses.xml

SUBDIRS += \
    GUI

CONFIG += ordered

copy.path    = $${OUT_PWD}/GUI/Resources
copy.files   = Core/Resources/*.xml

INSTALLS += copy
