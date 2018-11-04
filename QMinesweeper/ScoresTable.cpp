#include "ScoresTable.h"

#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QObject>
#include <QtCore/QScopedPointer>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>

#include "ApplicationInfo.h"
#include "ScoresDialog.h"

QSqlQueryModel * ScoresTable::createModel(GameSettings::DifficultyLevel level)
{
    QScopedPointer<QSqlQueryModel> result(new QSqlQueryModel());
    QSqlDatabase database = openDatabase();
    if (database.isOpen()) {
        QString selectSql =  QString("SELECT [Name], [Time]\n") +
            "FROM [Scores]\n" +
            "WHERE [Level] = %1\n" +
            "ORDER BY [Time]";
        result->setQuery(QSqlQuery(selectSql.arg((int)level), database));
        result->setHeaderData(
            0, Qt::Horizontal, QObject::tr("Name", "ScoresTable")
        );
        result->setHeaderData(
            1, Qt::Horizontal, QObject::tr("Time", "ScoresTable")
        );
    }
    return (result.take());
}

bool ScoresTable::insertRecord(
    const QString &name, GameSettings::DifficultyLevel level, const QTime &time
)
{
    QSqlDatabase database = openDatabase();
    if (!database.isOpen())
        return (false);

    QSqlQuery query(
        QString("INSERT OR REPLACE INTO [Scores]\n") +
            "([Name], [Level], [Time])\n" +
        "VALUES\n" +
            "(?, ?, ?)",
        database
    );

    query.addBindValue(name);
    query.addBindValue((int)level);
    query.addBindValue(time.toString());

    bool result = query.exec();
    if (!result)
        qDebug() << query.lastError().text();
    return (result);
}

bool ScoresTable::clearTable(GameSettings::DifficultyLevel level)
{
    QSqlDatabase database = openDatabase();
    if (!database.isOpen())
        return (false);
    QSqlQuery result = database.exec(
        QString("DELETE FROM [Scores] WHERE [Level] = %1").arg((int)level)
    );
    return (result.lastError().type() == QSqlError::NoError);
}

QSqlDatabase ScoresTable::openDatabase()
{
    QSqlDatabase result = QSqlDatabase::database();
    if (result.isValid())
        return (result);
    result = QSqlDatabase::addDatabase("QSQLITE");
    QDir home = QDir::home();
    QString dirName = QString("%1/.%3").arg(QDir::homePath())
            .arg(ApplicationInfo::internalName());
    home.mkpath(dirName);
    result.setDatabaseName(QString("%1/scores.db").arg(dirName));
    if (result.open()) {
        result.exec(
            QString("CREATE TABLE IF NOT EXISTS [Scores] (\n") +
                "[Name] TEXT NOT NULL PRIMARY KEY,\n" +
                "[Level] INTEGER NOT NULL,\n"
                "[Time] TEXT NOT NULL\n" +
            ")"
        );
    }
    return (result);
}

void ScoresTable::show(GameSettings::DifficultyLevel difficultLevel)
{ ScoresDialog(difficultLevel).exec(); }
