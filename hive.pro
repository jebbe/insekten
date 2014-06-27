TEMPLATE = app
TARGET = 
DEPENDPATH += . backend/src
INCLUDEPATH += . backend/src
QMAKE_CXXFLAGS += -std=c++11
OBJECTS_DIR  = obj
MOC_DIR = obj

# Input
HEADERS += backend/src/ai.h \
           backend/src/board.h \
           backend/src/game.h \
           backend/src/piece.h \
           backend/src/turn.h
SOURCES += backend/src/ai.cc \
           backend/src/board.cc \
           backend/src/game.cc \
           backend/src/piece.cc \
           backend/src/turn.cc
