#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore/QSharedPointer>
#include <QtCore/QString>
#include <QtGui/QMainWindow>
#include <QtGui/QIcon>
#include <QtGui/QLabel>
#include <QtGui/QWidget>

#include "GameSettings.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *m_ui;
    QLabel *m_gameTimeLabel;
    QLabel *m_gameStateLabel;
    QSharedPointer<GameSettings> m_lastUsedSettings;

    QLabel *createStatusBarLabel();

    static QIcon iconFromTheme(const QString &name);

    void initActionsIcon();
    void connectActions();

    bool startGame(QSharedPointer<GameSettings> settings);

private slots:
    void onNewExecute();
    void onOpenExecute();
    void onSaveExecute();
    void onScoresExecute();
    void onAboutExecute();
    void onGameTimeChanged(const QString &newTime);
    void onGameStateChanged(const QString &newState);

};

#endif // MAINWINDOW_H
