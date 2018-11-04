#ifndef GAMESETTINGSDIALOG_H
#define GAMESETTINGSDIALOG_H

#include <QtCore/QSharedPointer>
#include <QtGui/QDialog>
#include <QtGui/QWidget>

#include "GameSettings.h"

namespace Ui {
    class GameSettingsDialog;
}

class GameSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GameSettingsDialog(
        QSharedPointer<GameSettings> settings, QWidget *parent = 0
    );
    ~GameSettingsDialog();

private:
    Ui::GameSettingsDialog *m_ui;
    QSharedPointer<GameSettings> m_settings;

    void updateSettings();
    bool applySettings();

private slots:
    void onDifficultyLevelChanged(int index);
    void on_buttonBox_accepted();
};

#endif // GAMESETTINGSDIALOG_H
