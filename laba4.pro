TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle

SOURCES += main.cpp \
    User.cpp \
    Root.cpp \
    Date.cpp \
    LineParser.cpp \
    Files/Catalog.cpp \
    Files/Descriptor.cpp \
    Files/File.cpp \
    Files/FilesTable.cpp \
    Files/Permission.cpp

HEADERS += \
    User.h \
    Root.h \
    Date.h \
    Files/Catalog.h \
    Files/Descriptor.h \
    Files/File.h \
    Files/FilesTable.h \
    Files/Permission.h \
    LineParser.h
