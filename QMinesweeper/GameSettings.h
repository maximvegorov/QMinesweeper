#ifndef GAMESETTINGS_H
#define GAMESETTINGS_H

#include <QtCore/QSharedPointer>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QDataStream>

class GameSettings
{
public:
    enum DifficultyLevel {
        Beginner, Intermediate, Expert, Custom
    };

    GameSettings();
    GameSettings(const GameSettings &source);

    static QStringList difficultyLevelNames();

    DifficultyLevel level() const;
    void setLevel(DifficultyLevel value);
    int rowCount() const;
    void setRowCount(int value);
    int columnCount() const;
    void setColumnCount(int value);
    int mineCount() const;
    void setMineCount(int value);

    void setValues(int rowCount, int columnCount, int mineCount);

    static QSharedPointer<GameSettings> read(QDataStream &stream);
    bool write(QDataStream &stream);

    QSharedPointer<GameSettings> clone() const;

private:
    DifficultyLevel m_level;
    int m_rowCount;
    int m_columnCount;
    int m_mineCount;

    GameSettings(
        DifficultyLevel level, int rowCount, int columnCount, int mineCount
    );

};

inline GameSettings::DifficultyLevel GameSettings::level() const
{ return (m_level); }

inline int GameSettings::rowCount() const
{ return (m_rowCount); }

inline int GameSettings::columnCount() const
{ return (m_columnCount); }

inline int GameSettings::mineCount() const
{ return (m_mineCount); }

#endif // GAMESETTINGS_H
