#-------------------------------------------------
#
# Project created by QtCreator 2011-08-26T13:01:25
#
#-------------------------------------------------

QT += core gui sql

TARGET = QMinesweeper
TEMPLATE = app

CONFIG += precompile_header

PRECOMPILED_HEADER = stable.h

win32:RC_FILE += QMinesweeper.rc
macx:ICON = res/icons/MainIcon/MainIcon.icns

SOURCES += main.cpp\
        MainWindow.cpp \
    ApplicationInfo.cpp \
    GameSettings.cpp \
    GameSettingsDialog.cpp \
    PlayBoard.cpp \
    Game.cpp \
    GameView.cpp \
    ScoresDialog.cpp \
    ScoresTable.cpp \
    DifficultLevelListModel.cpp

HEADERS += MainWindow.h \
    ApplicationInfo.h \
    GameSettings.h \
    GameSettingsDialog.h \
    PlayBoard.h \
    Game.h \
    GameView.h \
    ScoresDialog.h \
    ScoresTable.h \
    DifficultLevelListModel.h

FORMS += MainWindow.ui \
    GameSettingsDialog.ui \
    ScoresDialog.ui

RESOURCES += QMinesweeper.qrc











