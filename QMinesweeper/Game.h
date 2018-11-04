#ifndef GAME_H
#define GAME_H

#include <QtCore/QSharedPointer>
#include <QtCore/QString>
#include <QtCore/QObject>
#include <QtCore/QTime>

#include "GameSettings.h"
#include "PlayBoard.h"

class Game : public QObject
{
    Q_OBJECT

public:
    enum GameResult {
        Unknown,
        Success,
        Failed
    };

    Game(QSharedPointer<GameSettings> settings);
    ~Game();

    static QSharedPointer<Game> create(QSharedPointer<GameSettings> settings);

    const GameSettings * settings() const;
    const PlayBoard * playBoard() const;
    const QTime currentTime() const;
    GameResult result() const;

    void start();

    static QSharedPointer<Game> read(const QString &fileName);
    bool write(const QString &fileName);

    int openCell(int row, int column);
    bool markCell(int row, int column);

signals:
    void gameTimeChanged(const QString &newTime);
    void gameStateChanged(const QString &newState);
    void finished(bool isSuccess);

private:
    QSharedPointer<GameSettings> m_settings;
    PlayBoard *m_playBoard;
    QTime m_currentTime;
    int m_openCount;
    int m_markedCount;
    GameResult m_result;

    Game(
        QSharedPointer<GameSettings> settings,
        PlayBoard *playBoard,
        QTime currentTime,
        int openCount,
        int markedCount
    );

    void notifyFinished(bool isSuccess);

private slots:
    void notifyGameTimeChanged();
    void notifyGameStateChanged();

};

inline const GameSettings * Game::settings() const
{ return (m_settings.data()); }

inline const PlayBoard * Game::playBoard() const
{ return (m_playBoard); }

inline const QTime Game::currentTime() const
{ return (m_currentTime); }

inline Game::GameResult Game::result() const
{ return (m_result); }

#endif // GAME_H
