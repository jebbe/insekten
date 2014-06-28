TEMPLATE = app
TARGET = 
DEPENDPATH += . gui backend/src gui/src
INCLUDEPATH += . backend/src

QMAKE_CXXFLAGS += -std=c++11
OBJECTS_DIR  = obj
MOC_DIR = obj
RCC_DIR = obj

# Input
HEADERS += backend/src/ai.h \
           backend/src/board.h \
           backend/src/game.h \
           backend/src/piece.h \
           backend/src/turn.h \
           gui/src/common.h \
           gui/src/gamescene.h \
           gui/src/inventory.h \
           gui/src/mainwindow.h \
           gui/src/newgamedialog.h
SOURCES += backend/src/ai.cc \
           backend/src/board.cc \
           backend/src/game.cc \
           backend/src/piece.cc \
           backend/src/turn.cc \
           gui/src/gamescene.cc \
           gui/src/inventory.cc \
           gui/src/main.cc \
           gui/src/mainwindow.cc \
           gui/src/newgamedialog.cc
RESOURCES += gui/images.qrc
