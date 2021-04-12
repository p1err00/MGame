QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

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
    AddDialog.cpp \
    Dialog/ChangeCouvertureDIalog.cpp \
    Dialog/SelectTypeDialog.cpp \
    Game.cpp \
    Screenshot/Screenshot.cpp \
    Settings/FavoriDialog.cpp \
    Settings/MainSettingsDialog.cpp \
    Settings/PreferenceSettingsDialog.cpp \
    Settings/Settings.cpp \
    Settings/SettingsDialog.cpp \
    Torrent/Torrent.cpp \
    Torrent/TorrentFile.cpp \
    TransmissionProcess.cpp \
    main.cpp \
    MainWindow.cpp

HEADERS += \
    AddDialog.h \
    Dialog/ChangeCouvertureDIalog.h \
    Dialog/SelectTypeDialog.h \
    Game.h \
    MainWindow.h \
    Screenshot/Screenshot.h \
    Settings/FavoriDialog.h \
    Settings/MainSettingsDialog.h \
    Settings/PreferenceSettingsDialog.h \
    Settings/Settings.h \
    Settings/SettingsDialog.h \
    Torrent/Torrent.h \
    Torrent/TorrentFile.h \
    TransmissionProcess.h

FORMS += \
    AddDialog.ui \
    Dialog/ChangeCouvertureDIalog.ui \
    Dialog/SelectTypeDialog.ui \
    MainWindow.ui \
    Settings/MainSettingsDialog.ui \
    Settings/PreferenceSettingsDialog.ui \
    Settings/SettingsDialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
