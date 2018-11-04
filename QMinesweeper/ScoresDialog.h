#ifndef SCORESDIALOG_H
#define SCORESDIALOG_H

#include <QtGui/QDialog>
#include <QtGui/QWidget>

#include "DifficultLevelListModel.h"

namespace Ui {
    class ScoresDialog;
}

class ScoresDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ScoresDialog(
        GameSettings::DifficultyLevel difficultyLevel, QWidget *parent = 0
    );
    ~ScoresDialog();

private:
    Ui::ScoresDialog *m_ui;
    DifficultLevelListModel m_difficultLevelListModel;

private slots:
    void refreshTable();
    void refreshTable(const QString &text);
    void on_clearButton_clicked();
};

#endif // SCORESDIALOG_H
