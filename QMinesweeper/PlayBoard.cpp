#include "PlayBoard.h"

#include <QtCore/QtGlobal>
#include <QtCore/QSet>
#include <QtCore/QTime>
#include <QtCore/QQueue>

PlayBoard::Cell::Cell()
    : m_row(-1), m_column(-1), m_flags(0), m_mineCount(0)
{ }

void PlayBoard::Cell::setOpen(bool value)
{
    if (value)
        m_flags |= PlayBoard::Open;
    else
        m_flags &= ~PlayBoard::Open;
}

void PlayBoard::Cell::setMarked(bool value)
{
    if (value)
        m_flags |= PlayBoard::Marked;
    else
        m_flags &= ~PlayBoard::Marked;
}

void PlayBoard::Cell::setMined(bool value)
{
    if (value)
        m_flags |= PlayBoard::Mined;
    else
        m_flags &= ~PlayBoard::Mined;
}

void PlayBoard::Cell::setMineCount(int value)
{ m_mineCount = value; }

void PlayBoard::Cell::read(QDataStream &stream)
{
    qint32 row;
    qint32 column;
    qint8 flags;
    qint8 mineCount;

    stream >> row >> column >> flags >> mineCount;

    m_row = row;
    m_column = column;
    m_flags = flags;
    m_mineCount = mineCount;
}

void PlayBoard::Cell::write(QDataStream &stream) const
{
    stream << (qint32)m_row
        << (qint32)m_column
        << (qint8)m_flags
        << (qint8)m_mineCount;
}



void PlayBoard::Cell::clear()
{
    m_flags = 0;
    m_mineCount = 0;
}

PlayBoard::PlayBoard(GameSettings *settings)
    : m_rowCount(settings->rowCount()),
    m_columnCount(settings->columnCount()),
    m_cells(settings->rowCount() * settings->columnCount())
{
    for (int i = 0; i < m_rowCount; i++) {
        for (int j = 0; j < m_columnCount; j++)
            m_cells[cellIndex(i, j)].setPos(i, j);
    }
}

PlayBoard::PlayBoard(
    int rowCount, int columnCount, QVector<Cell> cells
) : m_rowCount(rowCount), m_columnCount(columnCount), m_cells(cells)
{ }

PlayBoard::~PlayBoard()
{ }

void PlayBoard::placeMines(int mineCount)
{
    qsrand(QTime::currentTime().msec());
    while (mineCount > 0) {
        int row = qrand() % m_rowCount;
        int column = rand() % m_columnCount;
        Cell &cell = m_cells[cellIndex(row, column)];
        if (!cell.isMined()) {
            cell.setMined(true);
            updateMineCountInAdjacentCells(row, column);
            mineCount--;
        }
        else {
            if (
                tryPlaceMine(row - 1, column - 1) ||
                tryPlaceMine(row - 1, column) ||
                tryPlaceMine(row - 1, column + 1) ||
                tryPlaceMine(row, column - 1) ||
                tryPlaceMine(row, column + 1) ||
                tryPlaceMine(row + 1, column - 1) ||
                tryPlaceMine(row + 1, column) ||
                tryPlaceMine(row + 1, column + 1)
            )
                mineCount--;
        }
    }
}

int PlayBoard::open(int row, int column, int &unmarkedCount)
{
    unmarkedCount = 0;

    if (!isValidCell(row, column))
        return (0);

    Cell &cell = m_cells[cellIndex(row, column)];

    if (cell.isMined())
        return (-1);

    if (cell.isOpen())
        return (0);

    if (cell.mineCount() > 0) {
        if (cell.isMarked())
            unmarkedCount++;
        cell.setOpen(true);
        return (1);
    }

    QSet<int> marked;
    QQueue<int> processing;

    int index = cellIndex(row, column);
    marked.insert(index);
    processing.enqueue(index);
    while (!processing.isEmpty()) {
        const Cell &currentCell = m_cells.at(processing.dequeue());
        if (currentCell.mineCount() == 0) {
            for (int i = -1; i < 3; i++) {
                for (int j = -1; j < 3; j++) {
                    int nextRow = currentCell.row() + i;
                    int nextColumn = currentCell.column() + j;
                    if (isValidCell(nextRow, nextColumn)) {
                        int nextIndex = cellIndex(nextRow, nextColumn);
                        if (!marked.contains(nextIndex)) {
                            const Cell &nextCell = m_cells.at(nextIndex);
                            if (!nextCell.isOpen() && !nextCell.isMined()) {
                                marked.insert(nextIndex);
                                processing.enqueue(nextIndex);
                            }
                        }
                    }
                }
            }
        }
    }

    QSetIterator<int> iter(marked);
    while (iter.hasNext()) {
        Cell &cell = m_cells[iter.next()];
        if (cell.isMarked())
            unmarkedCount++;
        cell.setOpen(true);
    }

    return (marked.size());
}

int PlayBoard::mark(int row, int column)
{
    if (!isValidCell(row, column))
        return (0);
    Cell &cell = m_cells[cellIndex(row, column)];
    if (cell.isOpen())
        return (0);
    cell.setMarked(!cell.isMarked());
    if (!cell.isMarked())
        return (-1);
    return (1);
}

PlayBoard * PlayBoard::read(QDataStream &stream)
{
    qint32 rowCount;
    qint32 columnCount;
    QVector<Cell> cells;

    stream >> rowCount >> columnCount >> cells;

    if (stream.status() != QDataStream::Ok)
        return (0);
    return (new PlayBoard(rowCount, columnCount, cells));
}

bool PlayBoard::write(QDataStream &stream)
{
    stream << (qint32)m_rowCount
        << (qint32)m_columnCount
        << m_cells;
    return (stream.status() == QDataStream::Ok);
}

void PlayBoard::clear()
{
    for (int i = 0; i < m_cells.size(); i++)
        m_cells[i].clear();
}

bool PlayBoard::isValidCell(int row, int column) const
{
    return (
        (0 <= row) && (row < m_rowCount) &&
        (0 <= column) && (column < m_columnCount)
    );
}

bool PlayBoard::tryPlaceMine(int row, int column)
{
    if (!isValidCell(row, column))
        return (false);
    Cell &cell = m_cells[cellIndex(row, column)];
    if (cell.isMined())
        return (false);
    cell.setMined(true);
    updateMineCountInAdjacentCells(row, column);
    return (true);
}

void PlayBoard::updateMineCount(int row, int column)
{
    if (!isValidCell(row, column))
        return;
    Cell &cell = m_cells[cellIndex(row, column)];
    if (!cell.isMined())
        cell.setMineCount(cell.mineCount() + 1);
}


void PlayBoard::updateMineCountInAdjacentCells(int row, int column)
{
    updateMineCount(row - 1, column - 1);
    updateMineCount(row - 1, column);
    updateMineCount(row - 1, column + 1);
    updateMineCount(row, column - 1);
    updateMineCount(row, column + 1);
    updateMineCount(row + 1, column - 1);
    updateMineCount(row + 1, column);
    updateMineCount(row + 1, column + 1);
}

QDataStream & operator<< (QDataStream& stream, const PlayBoard::Cell &cell)
{
    cell.write(stream);
    return (stream);
}

QDataStream & operator>> (QDataStream& stream,PlayBoard::Cell &cell)
{
    cell.read(stream);
    return (stream);
}
