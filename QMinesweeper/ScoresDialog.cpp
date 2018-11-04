#include "ScoresDialog.h"
#include "ui_ScoresDialog.h"

#include "ScoresTable.h"

ScoresDialog::ScoresDialog(
    GameSettings::DifficultyLevel difficultyLevel, QWidget *parent
) : QDialog(parent),
    m_ui(new Ui::ScoresDialog)
{
    m_ui->setupUi(this);

    m_ui->difficultLevelEdit->setModel(&m_difficultLevelListModel);
    connect(
        m_ui->difficultLevelEdit,
        SIGNAL(currentIndexChanged(QString)),
        SLOT(refreshTable(QString))
    );
    m_ui->difficultLevelEdit->setCurrentIndex(
        m_difficultLevelListModel.indexOf(difficultyLevel)
    );
    if (!m_ui->scoresTable->model())
        refreshTable();
}

ScoresDialog::~ScoresDialog()
{ delete (m_ui); }

void ScoresDialog::refreshTable()
{ refreshTable(m_ui->difficultLevelEdit->currentText()); }

void ScoresDialog::refreshTable(const QString &text)
{
    m_ui->scoresTable->setModel(
        ScoresTable::createModel(m_difficultLevelListModel.value(text))
    );
    m_ui->scoresTable->horizontalHeader()->setResizeMode(
        0, QHeaderView::Stretch
    );
}

void ScoresDialog::on_clearButton_clicked()
{
    QString currentText = m_ui->difficultLevelEdit->currentText();
    ScoresTable::clearTable(
        m_difficultLevelListModel.value(currentText)
    );
    refreshTable(currentText);
}
