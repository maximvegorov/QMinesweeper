#include "GameView.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QtGlobal>
#include <QtCore/QRect>
#include <QtGui/QInputDialog>
#include <QtGui/QMessageBox>
#include <QtGui/QPalette>
#include <QtGui/QStyle>
#include <QtGui/QStyleOption>
#include <QtGui/QStyleOptionButton>
#include <QtGui/QStylePainter>
#include <QtGui/QSizePolicy>

#include "PlayBoard.h"
#include "ScoresTable.h"

GameView::DisplaySettings::DisplaySettings()
    : m_leftPadding(0),
    m_topPadding(0),
    m_rightPadding(0),
    m_bottomPadding(0),
    m_cellWidth(0),
    m_cellHeight(0)
{ }

void GameView::DisplaySettings::recalc(Game *game, const QSize &size)
{
    m_cellWidth = size.width() / game->settings()->columnCount();
    m_cellHeight = size.height() / game->settings()->rowCount();
    int horizontalPadding = size.width() - m_cellWidth * game->settings()->columnCount();
    int verticalPadding = size.height() - m_cellHeight * game->settings()->rowCount();
    m_leftPadding = horizontalPadding / 2;
    m_rightPadding = horizontalPadding - m_leftPadding;
    m_topPadding = verticalPadding / 2;
    m_bottomPadding = verticalPadding - m_topPadding;
}

GameView::GameView(QWidget *parent)
    : QWidget(parent)
{
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    m_flagImage.load(":/res/images/flag.png");
    m_mineImage.load(":/res/images/mine.png");
}

GameView::~GameView()
{ }

QSize GameView::minimumSizeHint() const
{
    const int MIN_CELL_SIZE = 32;
    if (m_game.isNull())
        return (QSize());
    int width = m_game->settings()->columnCount() * MIN_CELL_SIZE;
    int height = m_game->settings()->rowCount() * MIN_CELL_SIZE;
    return (QSize(width, height));
}

void GameView::setGame(QSharedPointer<Game> game)
{
    Q_ASSERT(!game.isNull());
    m_game = game;
    connect(
        m_game.data(),
        SIGNAL(gameTimeChanged(QString)),
        SIGNAL(gameTimeChanged(QString))
    );
    connect(
        m_game.data(),
        SIGNAL(gameStateChanged(QString)),
        SIGNAL(gameStateChanged(QString))
    );
    connect(
        m_game.data(),
        SIGNAL(finished(bool)),
        SLOT(onFinished(bool)),
        Qt::QueuedConnection
    );
    m_game->start();
    update();
    updateGeometry();
    m_settings.recalc(m_game.data(), size());
}

bool GameView::loadGame(const QString &fileName)
{
    QSharedPointer<Game> game = Game::read(fileName);
    if (game.isNull())
        return (false);
    setGame(game);
    return (true);
}

bool GameView::saveGame(const QString &fileName)
{
    if (m_game.isNull() || m_game->result() != Game::Unknown)
        return (true);
    return (m_game->write(fileName));
}

void drawCenteredImage(QStylePainter *painter, const QRect &rect, const QImage &image)
{
    QSize rectSize = rect.size();
    QSize imageSize = image.size();
    int horizontalMargin = qMax(rectSize.width() - imageSize.width(), 0);
    int verticalMargin = qMax(rectSize.height() - imageSize.height(), 0);
    painter->drawImage(
        rect.x() + horizontalMargin / 2, rect.y() + verticalMargin / 2, image
    );
}

void GameView::paintEvent(QPaintEvent *)
{
    const int CELL_BORDER_SIZE = 1;

    if (m_game.isNull())
        return;

    QStylePainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    int rowCount = m_game->settings()->rowCount();
    int columnCount = m_game->settings()->columnCount();

    painter.setPen(QPen(palette().color(QPalette::Dark), CELL_BORDER_SIZE));

    int firstColumnOffset = m_settings.columnOffset(0);
    int lastColumnOffset = m_settings.columnOffset(columnCount);
    int firstRowIndex = 1;
    if (m_settings.topPadding() > 0)
        firstRowIndex--;
    for (int i = firstRowIndex; i <= rowCount; i++) {
        int rowOffset = m_settings.rowOffset(i);
        painter.drawLine(
            QPoint(firstColumnOffset, rowOffset),
            QPoint(lastColumnOffset, rowOffset)
        );
    }

    int firstRowOffset = m_settings.rowOffset(0);
    int lastRowOffset = m_settings.rowOffset(rowCount);
    int firstColumnIndex = 1;
    if (m_settings.leftPadding() > 0)
        firstColumnIndex--;
    int lastColumnIndex = columnCount - 1;
    if (m_settings.rightPadding() > 0)
        lastColumnIndex++;
    for (int j = firstColumnIndex; j <= columnCount; j++) {
        int columnOffset = m_settings.columnOffset(j);
        painter.drawLine(
            QPoint(columnOffset, firstRowOffset),
            QPoint(columnOffset, lastRowOffset)
        );
    }

    bool isFailed = m_game->result() == Game::Failed;

    QStyleOptionButton cellStyleOption;
    cellStyleOption.initFrom(this);
    cellStyleOption.state = QStyle::State_Enabled;

    int currentY = m_settings.topPadding();
    for (int i = 0; i < rowCount; i++) {
        int currentX = m_settings.leftPadding();
        for (int j = 0; j < columnCount; j++) {
            const PlayBoard::Cell &cell = m_game->playBoard()->cell(i, j);
            QRect cellRect(
                currentX, currentY, m_settings.cellWidth(), m_settings.cellHeight()
            );
            if (!cell.isOpen()) {
                cellStyleOption.rect = cellRect;
                painter.drawControl(QStyle::CE_PushButton, cellStyleOption);
                if (!isFailed) {
                    if (cell.isMarked())
                        drawCenteredImage(&painter, cellRect, m_flagImage);
                }
                else {
                    if (cell.isMined())
                        drawCenteredImage(&painter, cellRect, m_mineImage);
                    else if (cell.isMarked())
                        drawCenteredImage(&painter, cellRect, m_flagImage);
                }
            }
            else {
                if (cell.mineCount() > 0) {
                    painter.drawItemText(
                        cellRect,
                        Qt::AlignHCenter | Qt::AlignCenter,
                        palette(),
                        true,
                        QString::number(cell.mineCount()),
                        QPalette::ButtonText
                    );
                }
            }
            currentX += m_settings.cellWidth();
        }
        currentY += m_settings.cellHeight();
    }
}

void GameView::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_game.isNull() || m_game->result() != Game::Unknown)
        return;

    int row = rowByY(event->y());
    int column = columnByX(event->x());

    if (event->button() == Qt::LeftButton) {
        int openCellCount = m_game->openCell(row, column);
        if (openCellCount > 0)
            update();
    }
    else if (event->button() == Qt::RightButton) {
        if (m_game->markCell(row, column))
            update();
    }
}

void GameView::resizeEvent(QResizeEvent *event)
{
    if (!m_game.isNull())
        m_settings.recalc(m_game.data(), event->size());
    QWidget::resizeEvent(event);
}

void GameView::onFinished(bool isSuccess)
{
    repaint();
    if (!isSuccess)
        QMessageBox::information(0, QString(), tr("Sorry! You lose."));
    else {
        QString name = QInputDialog::getText(
            0, tr("Congratulation!"), tr("Enter your name:")
        );
        if (name.isNull())
            return;
        ScoresTable::insertRecord(
            name, m_game->settings()->level(), m_game->currentTime()
        );
        ScoresTable::show(m_game->settings()->level());
    }
}
