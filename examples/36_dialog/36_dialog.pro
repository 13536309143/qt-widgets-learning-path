QT += widgets

CONFIG += c++17

#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

SOURCES += \
    dialog.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    dialog.h \
    mainwindow.h \
    verify.h

FORMS += \
    dialog.ui \
    mainwindow.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
