QT += \
    core gui \
    network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    game/entities/character.cpp \
    game/entities/ghost.cpp \
    game/entities/maze.cpp \
    game/entities/pacman.cpp \
    game/online/client.cpp \
    game/online/host.cpp \
    windows/boardwindow.cpp \
    windows/hostwindow.cpp \
    windows/joinwindow.cpp \
    windows/mainwindow.cpp \
    main.cpp

HEADERS += \
    game/entities/character.h \
    game/entities/ghost.h \
    game/entities/maze.h \
    game/entities/pacman.h \
    game/online/client.h \
    game/online/host.h \
    game/resources.h \
    windows/boardwindow.h \
    windows/hostwindow.h \
    windows/joinwindow.h \
    windows/mainwindow.h

FORMS += \
    windows/boardwindow.ui \
    windows/hostwindow.ui \
    windows/joinwindow.ui \
    windows/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
