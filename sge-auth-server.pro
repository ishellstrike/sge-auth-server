TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    ../sge/logger.cpp

win32:LIBS += \
-L../sge/3rdparty/lib/ \
-L../sge/3rdparty/boost_1_58_0/stage/lib \
-llibboost_date_time-vc120-mt-gd-1_58 \
-lmysqlcppconn -lmysqlclient

win32:INCLUDEPATH += ../sge/3rdparty/include
win32:INCLUDEPATH += ../sge/3rdparty/boost_1_58_0

HEADERS += \
    ../sge/logger.h
