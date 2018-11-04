#include "GameSettingsDialog.h"
#include "ui_GameSettingsDialog.h"

#include <QtCore/QtGlobal>
#include <QtCore/QStringList>
#include <QtGui/QStringListModel>

GameSettingsDialog::GameSettingsDialog(
    QSharedPointer<GameSettings> settings, QWidget *parent
) : QDialog(parent),
    m_ui(new Ui::GameSettingsDialog),
    m_settings(settings)
{
    m_ui->setupUi(this);

    int lastSelectedLevel = m_settings->level();
    m_ui->difficultyLevelEdit->setModel(
        new QStringListModel(GameSettings::difficultyLevelNames())
    );
    m_ui->difficultyLevelEdit->setCurrentIndex(lastSelectedLevel);

    connect(
        m_ui->difficultyLevelEdit,
        SIGNAL(currentIndexChanged(int)),
        SLOT(onDifficultyLevelChanged(int))
    );

    updateSettings();
}

GameSettingsDialog::~GameSettingsDialog()
{ delete (m_ui); }

void GameSettingsDialog::updateSettings()
{
    m_ui->settingsPanel->setEnabled(m_settings->level() == GameSettings::Custom);
    m_ui->rowCountEdit->setValue(m_settings->rowCount());
    m_ui->columnCountEdit->setValue(m_settings->columnCount());
    m_ui->mineCountEdit->setValue(m_settings->mineCount());
}

bool GameSettingsDialog::applySettings()
{
    if (m_settings->level() != GameSettings::Custom)
        return (true);
    m_settings->setRowCount(m_ui->rowCountEdit->value());
    m_settings->setColumnCount(m_ui->columnCountEdit->value());
    m_settings->setMineCount(m_ui->mineCountEdit->value());
    return (true);
}

void GameSettingsDialog::onDifficultyLevelChanged(int index)
{
    m_settings->setLevel((GameSettings::DifficultyLevel)index);
    updateSettings();
}

void GameSettingsDialog::on_buttonBox_accepted()
{ applySettings(); }
