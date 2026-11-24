QT += widgets

CONFIG += c++17

#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

SOURCES += \
    dialog.cpp \
    main.cpp \
    widget.cpp

HEADERS += \
    dialog.h \
    verify.h \
    widget.h

FORMS += \
    dialog.ui \
    widget.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
