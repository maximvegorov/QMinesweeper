#include "DifficultLevelListModel.h"

DifficultLevelListModel::DifficultLevelListModel(QObject *parent)
    : QStringListModel(parent)
{
    setStringList(GameSettings::difficultyLevelNames());

    m_indexes[GameSettings::Beginner] = 0;
    m_indexes[GameSettings::Intermediate] = 1;
    m_indexes[GameSettings::Expert] = 2;
    m_indexes[GameSettings::Custom] = 3;

    QMapIterator<GameSettings::DifficultyLevel, int> iter(m_indexes);
    while (iter.hasNext()) {
        iter.next();
        m_values[stringList().at(iter.value())] = iter.key();
    }
}

DifficultLevelListModel::~DifficultLevelListModel()
{ }
