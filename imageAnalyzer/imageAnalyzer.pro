#QT -= gui

TEMPLATE = lib
DEFINES += IMAGEANALYZER_LIBRARY

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    imageanalyzer.cpp

HEADERS += \
    imageAnalyzer_global.h \
    ../struct_result.h \
    imageanalyzer.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/opencv/build/x64/vc15/lib/ -lopencv_world430
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/opencv/build/x64/vc15/lib/ -lopencv_world430d

INCLUDEPATH += $$PWD/opencv/build/include
DEPENDPATH += $$PWD/opencv/build/include
