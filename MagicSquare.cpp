#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include "MagicSquare.h"

using namespace std;

// implementation for MagicSquare

MagicSquare::MagicSquare(unsigned size)
    : size(size)
{
    // allocate the cells and initialize to 0
    pCells = new Cells(size, vector<unsigned>(size, 0));
}

MagicSquare::~MagicSquare()
{
    // make sure we don't leak the memory allocated for cells
    delete pCells;
}

unsigned MagicSquare::getSize()
{
    return size;
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

    // first we'll iterator through the rows

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
}

void SinglyEvenMagicSquare::fill(Cells& cells)
{
    throw "singly-even magic square solution not implemented";
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
        catch(const char* e)
        {
            cerr << "exception thrown: " << e << endl;
        }
        catch(const invalid_argument* e)
        {
            cerr << "exception thrown: " << e->what() << endl;
        }
        catch(const invalid_argument& e)
        {
            cerr << "exception thrown: " << e.what() << endl;
        }
        catch(const exception& e)
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
