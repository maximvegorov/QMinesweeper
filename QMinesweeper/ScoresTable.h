#ifndef SCORESTABLE_H
#define SCORESTABLE_H

#include <QtCore/QString>
#include <QtCore/QTime>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQueryModel>

#include "GameSettings.h"

class ScoresTable
{
public:
    static QSqlQueryModel * createModel(GameSettings::DifficultyLevel level);
    static bool insertRecord(
        const QString &name, GameSettings::DifficultyLevel level, const QTime &time
    );
    static bool clearTable(GameSettings::DifficultyLevel level);

    static void show(GameSettings::DifficultyLevel difficultLevel);

private:
    static QSqlDatabase openDatabase();

};

#endif // SCORESTABLE_H
