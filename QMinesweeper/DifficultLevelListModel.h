#ifndef DIFFICULTLEVELLISTMODEL_H
#define DIFFICULTLEVELLISTMODEL_H

#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtGui/QStringListModel>

#include "GameSettings.h"

class DifficultLevelListModel : public QStringListModel
{
    Q_OBJECT

public:
    explicit DifficultLevelListModel(QObject *parent = 0);
    ~DifficultLevelListModel();

    int indexOf(GameSettings::DifficultyLevel difficultLevel) const;
    GameSettings::DifficultyLevel value(const QString &text) const;

private:
    QMap<GameSettings::DifficultyLevel, int> m_indexes;
    QMap<QString, GameSettings::DifficultyLevel> m_values;

};

inline int DifficultLevelListModel::indexOf(GameSettings::DifficultyLevel difficultLevel) const
{ return (m_indexes[difficultLevel]); }

inline GameSettings::DifficultyLevel DifficultLevelListModel::value(const QString &text) const
{ return (m_values.value(text)); }

#endif // DIFFICULTLEVELLISTMODEL_H
