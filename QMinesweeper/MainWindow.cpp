#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QScopedPointer>
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>

#include "ApplicationInfo.h"
#include "GameSettingsDialog.h"
#include "GameView.h"
#include "ScoresTable.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    m_ui(new Ui::MainWindow)
{
    m_ui->setupUi(this);
    m_gameTimeLabel = createStatusBarLabel();
    m_gameStateLabel = createStatusBarLabel();

    setWindowTitle(ApplicationInfo::displayName());

    initActionsIcon();
    connectActions();

    GameView *gameView = new GameView(this);
    connect(
        gameView, SIGNAL(gameTimeChanged(QString)), SLOT(onGameTimeChanged(QString))
    );
    connect(
        gameView, SIGNAL(gameStateChanged(QString)), SLOT(onGameStateChanged(QString))
    );
    setCentralWidget(gameView);

    m_lastUsedSettings = QSharedPointer<GameSettings>(new GameSettings());

    startGame(m_lastUsedSettings);
}

MainWindow::~MainWindow()
{ delete (m_ui); }

QLabel * MainWindow::createStatusBarLabel()
{
    QScopedPointer<QLabel> label(new QLabel());
    m_ui->statusBar->addPermanentWidget(label.data());
    return (label.take());
}

QIcon MainWindow::iconFromTheme(const QString &name)
{
    QString fallbackName = QString(":/res/icons/%1.png").arg(name);
    QIcon result = QIcon::fromTheme(name, QIcon(fallbackName));
    return (result);
}

void MainWindow::initActionsIcon()
{
    m_ui->newAction->setIcon(iconFromTheme("document-new"));
    m_ui->openAction->setIcon(iconFromTheme("document-open"));
    m_ui->saveAction->setIcon(iconFromTheme("document-save"));
    m_ui->aboutAction->setIcon(iconFromTheme("help-about"));
}

void MainWindow::connectActions()
{
    connect(m_ui->newAction, SIGNAL(triggered()), SLOT(onNewExecute()));
    connect(m_ui->openAction, SIGNAL(triggered()), SLOT(onOpenExecute()));
    connect(m_ui->saveAction, SIGNAL(triggered()), SLOT(onSaveExecute()));
    connect(m_ui->scoresAction, SIGNAL(triggered()), SLOT(onScoresExecute()));
    connect(m_ui->quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(m_ui->aboutAction, SIGNAL(triggered()), SLOT(onAboutExecute()));
    connect(m_ui->aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

bool MainWindow::startGame(QSharedPointer<GameSettings> settings)
{
    GameView *gameView = qobject_cast<GameView *>(centralWidget());
    if (!gameView)
        return (false);
    QSharedPointer<Game> game = Game::create(settings);
    if (game.isNull())
        return (false);
    gameView->setGame(game);
    return (true);
}

void MainWindow::onNewExecute()
{
    QSharedPointer<GameSettings> settings = m_lastUsedSettings->clone();
    GameSettingsDialog dialog(settings);
    if (!dialog.exec())
        return;
    if (!startGame(settings))
        return;
    m_lastUsedSettings = settings;
}

void MainWindow::onOpenExecute()
{
    GameView *gameView = qobject_cast<GameView *>(centralWidget());
    if (!gameView)
        return;
    QString fileName = QFileDialog::getOpenFileName(
        0, QString(), QDir::homePath(), tr("Saved games (*.save)")
    );
    if (fileName.isNull())
        return;
    if (!gameView->loadGame(fileName))
        QMessageBox::warning(0, QString(), "Can\'t load game.");
}

void MainWindow::onSaveExecute()
{
    GameView *gameView = qobject_cast<GameView *>(centralWidget());
    if (!gameView)
        return;
    QString fileName = QFileDialog::getSaveFileName(
        0, QString(), QDir::homePath(), tr("Saved games (*.save)")
    );
    if (fileName.isNull())
        return;
    if (QFile::exists(fileName)) {
        if (!QFile::remove(fileName))
            return;
    }
    if (!gameView->saveGame(fileName))
        QMessageBox::warning(0, QString(), "Can\'t save game.");
}

void MainWindow::onScoresExecute()
{ ScoresTable::show(m_lastUsedSettings->level()); }

void MainWindow::onAboutExecute()
{
    QMessageBox::about(
        0,
        QString(tr("About")),
        QString("%1\n%2\n\n%3").arg(ApplicationInfo::displayName())
            .arg(ApplicationInfo::version())
            .arg(ApplicationInfo::copyrights())
    );
}

void MainWindow::onGameTimeChanged(const QString &newTime)
{ m_gameTimeLabel->setText(newTime); }

void MainWindow::onGameStateChanged(const QString &newState)
{ m_gameStateLabel->setText(newState); }
