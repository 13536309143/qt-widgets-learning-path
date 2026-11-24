QT += widgets

CONFIG += c++17

#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

SOURCES += \
    drawingarea.cpp \
    main.cpp \
    widget.cpp

HEADERS += \
    drawingarea.h \
    verify.h \
    widget.h

FORMS += \
    widget.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
