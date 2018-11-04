#include "GameSettings.h"

#include <QtCore/QObject>

GameSettings::GameSettings()
{ setLevel(Beginner); }

GameSettings::GameSettings(const GameSettings &source)
    : m_level(source.m_level),
    m_rowCount(source.m_rowCount),
    m_columnCount(source.m_columnCount),
    m_mineCount(source.m_mineCount)
{ }

GameSettings::GameSettings(
    DifficultyLevel level, int rowCount, int columnCount, int mineCount
)
    : m_level(level),
    m_rowCount(rowCount),
    m_columnCount(columnCount),
    m_mineCount(mineCount)
{ }


QStringList GameSettings::difficultyLevelNames()
{
    QStringList result = QStringList()
        << QString(QObject::tr("Beginner", "GameSettings"))
        << QString(QObject::tr("Intermediate", "GameSettings"))
        << QString(QObject::tr("Expert", "GameSettings"))
        << QString(QObject::tr("Custom", "GameSettings"));
    return (result);
}

void GameSettings::setLevel(DifficultyLevel value)
{
    if (m_level == value)
        return;
    m_level = value;
    switch (m_level) {
    case Beginner:
        setValues(9, 9, 10);
        break;
    case Intermediate:
        setValues(16, 16, 40);
        break;
    case Expert:
        setValues(16, 30, 99);
        break;
    }
}

void GameSettings::setRowCount(int value)
{ m_rowCount = value; }

void GameSettings::setColumnCount(int value)
{ m_columnCount = value; }

void GameSettings::setMineCount(int value)
{ m_mineCount = value; }

void GameSettings::setValues(int rowCount, int columnCount, int mineCount)
{
    setRowCount(rowCount);
    setColumnCount(columnCount);
    setMineCount(mineCount);
}

QSharedPointer<GameSettings> GameSettings::clone() const
{
    return (
        QSharedPointer<GameSettings>(new GameSettings(*this))
    );
}

QSharedPointer<GameSettings> GameSettings::read(QDataStream &stream)
{
    qint8 level;
    qint32 rowCount;
    qint32 columnCount;
    qint32 mineCount;
    stream >> level >> rowCount >> columnCount >> mineCount;
    if (stream.status() != QDataStream::Ok)
        return (QSharedPointer<GameSettings>());
    return (
        QSharedPointer<GameSettings>(
            new GameSettings((DifficultyLevel)level, rowCount, columnCount, mineCount)
        )
    );
}

bool GameSettings::write(QDataStream &stream)
{
    stream << (qint8)m_level;
    stream << (qint32)m_rowCount;
    stream << (qint32)m_columnCount;
    stream << (qint32)m_mineCount;
    return (stream.status() == QDataStream::Ok);
}
