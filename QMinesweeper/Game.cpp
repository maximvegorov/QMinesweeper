#include "Game.h"

#include <QtCore/QDataStream>
#include <QtCore/QFile>
#include <QtCore/QIODevice>
#include <QtCore/QScopedPointer>
#include <QtCore/QTimer>

Game::Game(QSharedPointer<GameSettings> settings)
    : QObject(0),
    m_settings(settings),
    m_playBoard(new PlayBoard(settings.data())),
    m_currentTime(0, 0),
    m_openCount(0),
    m_markedCount(0),
    m_result(Game::Unknown)
{ m_playBoard->placeMines(m_settings->mineCount()); }

Game::Game(
    QSharedPointer<GameSettings> settings,
    PlayBoard *playBoard,
    QTime currentTime,
    int openCount,
    int markedCount
) : m_settings(settings),
    m_playBoard(playBoard),
    m_currentTime(currentTime),
    m_openCount(openCount),
    m_markedCount(markedCount),
    m_result(Game::Unknown)
{ }

Game::~Game()
{ delete (m_playBoard); }

QSharedPointer<Game> Game::create(QSharedPointer<GameSettings> settings)
{
    if (settings.isNull())
        return (QSharedPointer<Game>());
    return (QSharedPointer<Game>(new Game(settings)));
}

void Game::start()
{
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), SLOT(notifyGameTimeChanged()));
    connect(this, SIGNAL(finished(bool)), timer, SLOT(stop()));
    timer->start(1000);
    notifyGameTimeChanged();
    notifyGameStateChanged();
}

QSharedPointer<Game> Game::read(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
        return (QSharedPointer<Game>());

    QDataStream stream(&file);
    stream.setVersion(QDataStream::Qt_4_7);

    QSharedPointer<GameSettings> settings = GameSettings::read(stream);
    if (settings.isNull())
        return (QSharedPointer<Game>());

    QScopedPointer<PlayBoard> playBoard(PlayBoard::read(stream));
    if (playBoard.isNull())
        return (QSharedPointer<Game>());

    QTime currentTime;
    qint32 openCount;
    qint32 markedCount;

    stream >> currentTime >> openCount >> markedCount;

    if (stream.status() != QDataStream::Ok)
        return (QSharedPointer<Game>());

    return (
        QSharedPointer<Game>(
            new Game(
                settings, playBoard.take(), currentTime, openCount, markedCount
            )
        )
    );
}

bool Game::write(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly))
        return (false);

    QDataStream stream(&file);
    stream.setVersion(QDataStream::Qt_4_7);

    if (!m_settings->write(stream))
        return (false);

    if (!m_playBoard->write(stream))
        return (false);

    stream << m_currentTime << (qint32)m_openCount << (qint32)m_markedCount;

    return (stream.status() == QDataStream::Ok);
}

int Game::openCell(int row, int column)
{
    int unmarkedCount;
    int result = m_playBoard->open(row, column, unmarkedCount);
    if (result < 0) {
        notifyFinished(false);
        return (-1);
    }
    m_openCount += result;
    m_markedCount -= unmarkedCount;
    notifyGameStateChanged();
    int closedCount = m_settings->rowCount() * m_settings->columnCount() - m_openCount;
    if (closedCount == m_settings->mineCount()) {
        notifyFinished(true);
        return (-1);
    }
    return (result);
}

bool Game::markCell(int row, int column)
{
    int result = m_playBoard->mark(row, column);
    if (result == 0)
        return (false);
    m_markedCount += result;
    notifyGameStateChanged();
    if (m_markedCount == m_settings->mineCount()) {
        int rowCount = m_settings->rowCount();
        int columnCount = m_settings->columnCount();
        for (int i = 0; i < rowCount; i++) {
            for (int j = 0; j < columnCount; j++) {
                const PlayBoard::Cell &cell = m_playBoard->cell(i, j);
                if (cell.isMarked() && !cell.isMined())
                    return (true);
            }
        }
        notifyFinished(true);
        return (false);
    }
    return (true);
}

void Game::notifyFinished(bool isSuccess)
{
    if (isSuccess)
        m_result = Game::Success;
    else
        m_result = Game::Failed;
    emit finished(isSuccess);
}

void Game::notifyGameTimeChanged()
{
    m_currentTime = m_currentTime.addSecs(1);
    emit gameTimeChanged(
        QString("Time: %1").arg(m_currentTime.toString())
    );
}

void Game::notifyGameStateChanged()
{
    QString newState = QString(tr("Remains: %1"))
        .arg(qMax(0, m_settings->mineCount() - m_markedCount));
    emit gameStateChanged(newState);
}
