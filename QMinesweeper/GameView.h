#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include <QtCore/QSharedPointer>
#include <QtCore/QSize>
#include <QtGui/QImage>
#include <QtGui/QMouseEvent>
#include <QtGui/QPaintEvent>
#include <QtGui/QResizeEvent>
#include <QtGui/QWidget>

#include "Game.h"

class GameView : public QWidget
{
    Q_OBJECT

public:
    class DisplaySettings
    {
    public:
        DisplaySettings();

        int leftPadding() const;
        int topPadding() const;
        int rightPadding() const;
        int bottomPadding() const;

        int cellWidth() const;
        int cellHeight() const;

        int rowOffset(int row) const;
        int columnOffset(int column) const;

        void recalc(Game *game, const QSize &size);

    private:
        int m_leftPadding;
        int m_topPadding;
        int m_rightPadding;
        int m_bottomPadding;

        int m_cellWidth;
        int m_cellHeight;

    };

    explicit GameView(QWidget *parent = 0);
    ~GameView();

    QSize minimumSizeHint() const;

    void setGame(QSharedPointer<Game> game);

    bool loadGame(const QString &fileName);
    bool saveGame(const QString &fileName);

signals:
    void gameTimeChanged(const QString &newState);
    void gameStateChanged(const QString &newState);

protected:
    void paintEvent(QPaintEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void resizeEvent(QResizeEvent *event);

private:
    QSharedPointer<Game> m_game;
    QImage m_flagImage;
    QImage m_mineImage;
    DisplaySettings m_settings;

    int rowByY(int y);
    int columnByX(int x);

private slots:
    void onFinished(bool isSuccess);

};

inline int GameView::DisplaySettings::leftPadding() const
{ return (m_leftPadding); }

inline int GameView::DisplaySettings::topPadding() const
{ return (m_topPadding); }

inline int GameView::DisplaySettings::rightPadding() const
{ return (m_rightPadding); }

inline int GameView::DisplaySettings::bottomPadding() const
{ return (m_bottomPadding); }

inline int GameView::DisplaySettings::cellWidth() const
{ return (m_cellWidth); }

inline int GameView::DisplaySettings::cellHeight() const
{ return (m_cellHeight); }

inline int GameView::DisplaySettings::rowOffset(int row) const
{ return (m_topPadding + row * m_cellHeight); }

inline int GameView::DisplaySettings::columnOffset(int column) const
{ return (m_leftPadding + column * m_cellWidth); }

inline int GameView::rowByY(int y)
{ return ((y - m_settings.topPadding()) / m_settings.cellHeight()); }

inline int GameView::columnByX(int x)
{ return ((x - m_settings.leftPadding()) / m_settings.cellWidth()); }

#endif // GAMEVIEW_H
