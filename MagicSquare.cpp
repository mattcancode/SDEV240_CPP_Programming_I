#include <algorithm>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include "MagicSquare.h"

using namespace std;

// implementation for MagicSquare

// using a vector of vectors to store the matrix of square cells - a multi-
// dimensional array is less cumbersome but using C++ vector but opted for
// the latter since it's a C++ construct and this is a C++ course

class MagicSquare::Cells : public std::vector<std::vector<unsigned>>
{
public:
    Cells(unsigned size)
        : std::vector<std::vector<unsigned>>(size, vector<unsigned>(size, 0))
    {
    }

    void fillFrom(const MagicSquare& src,
        const unsigned rowOffset = 0, const unsigned columnOffset = 0, const unsigned valueOffset = 0)
    {
        // TODO: make sure there is sufficient capacity

        for (unsigned row = 0; row < src.getSize(); ++row)
        {
            for (unsigned column = 0; column < src.getSize(); ++column)
            {
                this->at(row + rowOffset)[column + columnOffset] = src.cell(row, column) + valueOffset;
            }
        }
    }

    void swap(const unsigned row1, const unsigned column1, const unsigned row2, const unsigned column2)
    {
        unsigned temp = this->at(row1)[column1];
        this->at(row1)[column1] = this->at(row2)[column2];
        this->at(row2)[column2] = temp;
    }
};

MagicSquare::MagicSquare(unsigned size)
    : size(size)
{
    // allocate the cells and initialize to 0
    pCells = new Cells(size);
}

MagicSquare::~MagicSquare()
{
    // make sure we don't leak the memory allocated for cells
    delete pCells;
}

unsigned MagicSquare::getSize() const
{
    return size;
}

unsigned MagicSquare::cell(unsigned row, unsigned column) const
{
    return (*pCells)[row][column];
}

void MagicSquare::solve()
{
    // let the child class actually solve the square
    fill(*pCells);
}

void MagicSquare::dump(ostream& out)
{
    cout << "\nDumping " << size << "x" << size << " Magic Square:\n\n";

    // determine maximum digit width, which is the sum of all digits divided
    // by the size of the square, calculated as (size ^ 3 + size) / 2
    // (we could also use base 10 log but then have to include another library)
    int width = 1;

    for (unsigned n = (size * size * size + size) / 2; n > 0; n /= 10)
    {
        ++width;
    }

    // first we'll iterate through the rows

    for (vector<unsigned> row : *pCells)
    {
        unsigned sum = 0;

        for (unsigned value : row)
        {
            out << setw(width) << value;
            sum += value;
        }

        // output the sum of the row at the end
        out << "   " << sum << "\n";
    }

    out << "\n";

    // now sum each of the columns
    for (int col = 0; col < size; col++)
    {
        unsigned sum = 0;

        for (int row = 0; row < size; row++)
        {
            sum += (*pCells)[row][col];
        }

        out << setw(width) << sum;
    }

    out << "\n\n";
}

// implementation for OddMagicSquare

OddMagicSquare::OddMagicSquare(unsigned size)
    : MagicSquare(size)
{
    if (size % 2 == 0)
    {
        throw new invalid_argument("size is not odd");
    }
}

void OddMagicSquare::fill(Cells& cells)
{
    // the square will be filled with digits from 1 to size^2
    const unsigned size = getSize();
    const unsigned n = size * size;

    // start in the top middle square
    unsigned row = 0;
    unsigned col = size / 2;

    // keep going to there all squares are filled

    for (unsigned i = 1; i <= n; i++)
    {
        cells[row][col] = i;

        // if a multiple of size, drop to next row
        if (i % size == 0)
        {
            if (++row == size)
            {
                row = 0;
            }
        }
        else
        {
            // otherwise move up and to the right - wrap as needed
            row = (row == 0 ? size - 1 : row - 1);

            if (++col == size)
            {
                col = 0;
            }
        }
    }
}

// implementation for SinglyEvenMagicSquare

SinglyEvenMagicSquare::SinglyEvenMagicSquare(unsigned size)
    : MagicSquare(size)
{
    if (size % 2 != 0 || size % 4 == 0)
    {
        throw new invalid_argument("size is not singly even");
    }

    if (size < 6)
    {
        throw new invalid_argument("singly even squares cannot be smaller than 6");
    }
}

void SinglyEvenMagicSquare::fill(Cells& cells)
{
    const unsigned size = getSize();
    const unsigned size2 = size / 2;

    // the first step in building a singly even square is to build four odd
    // magic squares
    OddMagicSquare odd(size2);
    odd.solve();

    // start with upper left (which is a straight copy)
    cells.fillFrom(odd);

    // then the bottom right
    cells.fillFrom(odd, size2, size2, size2 * size2);

    // then the upper right
    cells.fillFrom(odd, 0, size2, 2 * size2 * size2);

    // and finally the bottom left
    cells.fillFrom(odd, size2, 0, 3 * size2 * size2);

    // next step is to swap the leftmost columns between the upper left and
    // lower left quadrants (except for the leftmost middle cells)
    unsigned ncolumns = (size - 2) / 4;

    for (unsigned row = 0; row < size2; ++row)
    {
        for (unsigned column = 0; column < ncolumns; ++column)
        {
            if (column > 0 || row != size2 / 2)
            {
                cells.swap(row, column, row + size2, column);
            }
        }
    }

    // next we swap the central cells between the upper left and lower left

    cells.swap(size2 / 2, size2 / 2, size2 / 2 + size2, size2 / 2);

    // finally, we swap some rightmost columns between the upper right and
    // lower right quadrant (though one fewer than we did above)
    if (--ncolumns > 0)
    {
        for (unsigned row = 0; row < size2; ++row)
        {
            for (unsigned column = size - ncolumns; column < size; ++column)
            {
                cells.swap(row, column, row + size2, column);
            }
        }
    }
}

// implementation for DoublyEvenMagicSquare

DoublyEvenMagicSquare::DoublyEvenMagicSquare(unsigned size)
    : MagicSquare(size)
{
    // make sure the size passed in is valid
    if (size % 4 != 0)
    {
        throw new invalid_argument("size is not doubly even");
    }
}

void DoublyEvenMagicSquare::fill(Cells& cells)
{
    throw "doubly-even magic square solution not implemented";
}

// helper function to avoid repeated calls to solve() and dump()

void solveAndDump(unsigned size)
{
    if (size == 0)
    {
        throw new invalid_argument("square must be greater than 0");
    }

    // the size will determine which type of square to create

    MagicSquare* pSquare;

    if (size % 4 == 0)
    {
        pSquare = new DoublyEvenMagicSquare(size);
    }
    else if (size % 2 == 0)
    {
        pSquare = new SinglyEvenMagicSquare(size);
    }
    else
    {
        pSquare = new OddMagicSquare(size);
    }

    // make sure this gets cleaned up even if solve or dump throw an exception
    unique_ptr<MagicSquare> ptr(pSquare);

    ptr->solve();
    ptr->dump(cout);
}

// main function

int main(int argc, char* argv[])
{
    for (int i = 1; i < argc; i++)
    {
        try
        {
            unsigned size = stoul(string(argv[i]));
            solveAndDump(size);
        }
        catch (const char* e)
        {
            cerr << "exception thrown: " << e << endl;
        }
        catch (const invalid_argument* e)
        {
            cerr << "exception thrown: " << e->what() << endl;
        }
        catch (const invalid_argument& e)
        {
            cerr << "exception thrown: " << e.what() << endl;
        }
        catch (const exception& e)
        {
            cerr << "exception thrown: " << e.what() << endl;
        }
    }
    // solveAndDump(1);
    // solveAndDump(3);
    // solveAndDump(7);
    // solveAndDump(13);

    return 0;
}
