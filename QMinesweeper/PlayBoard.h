#ifndef PLAYBOARD_H
#define PLAYBOARD_H

#include <QtCore/QDataStream>
#include <QtCore/QVector>

#include "GameSettings.h"

class PlayBoard
{
public:
    enum CellFlag {
        Open = 0x01, Marked = 0x2, Mined = 0x4
    };

    class Cell
    {
    public:
        Cell();

        int row() const;
        int column() const;
        void setPos(int row, int column);
        bool isOpen() const;
        void setOpen(bool value);
        bool isMarked() const;
        void setMarked(bool value);
        bool isMined() const;
        void setMined(bool value);
        int mineCount() const;
        void setMineCount(int value);

        void read(QDataStream &stream);
        void write(QDataStream &stream) const;

        void clear();

    private:
        int m_row;
        int m_column;
        int m_flags;
        int m_mineCount;

    };

    PlayBoard(GameSettings *settings);
    ~PlayBoard();

    void placeMines(int mineCount);
    const Cell & cell(int row, int column) const;
    int open(int row, int column, int &unmarkedCount);
    int mark(int row, int column);

    static PlayBoard * read(QDataStream &stream);
    bool write(QDataStream &stream);

    void clear();

private:
    int m_rowCount;
    int m_columnCount;
    QVector<Cell> m_cells;

    PlayBoard(
        int rowCount, int columnCount, QVector<Cell> cells
    );

    bool isValidCell(int row, int column) const;
    int cellIndex(int row, int column) const;
    bool tryPlaceMine(int row, int column);
    void updateMineCount(int row, int column);
    void updateMineCountInAdjacentCells(int row, int column);

};

inline int PlayBoard::Cell::row() const
{ return (m_row); }

inline int PlayBoard::Cell::column() const
{ return (m_column); }

inline void PlayBoard::Cell::setPos(int row, int column)
{
    m_row = row;
    m_column = column;
}

inline bool PlayBoard::Cell::isOpen() const
{ return ((m_flags & PlayBoard::Open) != 0); }

inline bool PlayBoard::Cell::isMarked() const
{ return ((m_flags & PlayBoard::Marked) != 0); }

inline bool PlayBoard::Cell::isMined() const
{ return ((m_flags & PlayBoard::Mined) != 0); }

inline int PlayBoard::Cell::mineCount() const
{ return (m_mineCount); }

inline const PlayBoard::Cell & PlayBoard::cell(int row, int column) const
{ return (m_cells.at(cellIndex(row, column))); }

inline int PlayBoard::cellIndex(int row, int column) const
{ return (m_columnCount * row + column); }

QDataStream & operator<< (QDataStream& stream, const PlayBoard::Cell &cell);
QDataStream & operator>> (QDataStream& stream, PlayBoard::Cell &cell);

#endif // PLAYBOARD_H
