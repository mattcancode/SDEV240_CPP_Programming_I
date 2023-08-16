// base MagicSquare class - it serves as a base class for the other classes
// and handles management of the call data, plus it includes an interface
// common any child classes

class MagicSquare
{
public:
    // forward declaration for the underlying Cells class with square data
    // (this allows us to hide the original vector stuff that was here before)
    class Cells;

    // the constructor is protected to prevent instantiation of the base class
    // child classes will call this to initialize the square's data
    MagicSquare(unsigned size);

    virtual ~MagicSquare();

    // returns the size of the magic square
    unsigned getSize() const;

    // gets the value of the cell at the specified row and column
    unsigned cell(unsigned row, unsigned column) const;

    // call this method to solve the magic square (ordinarily, it would make
    // sense to simply do this at creation, but separate for discussion)
    void solve();

    // call this method to dump the magic square to the provided output stream
    void dump(std::ostream& out);

protected:
    // child classes need to implement this method to actually solve the square
    virtual void fill(Cells& cells) = 0;

private:
    const unsigned size;

    Cells* pCells;
};

// specialized class for squares with odd (non-even) sides

class OddMagicSquare : public MagicSquare
{
public:
    OddMagicSquare(unsigned size);

protected:
    void fill(Cells& cells);
};

// specialized class for squares whose size is divisible by 2 but not 4

class SinglyEvenMagicSquare : public MagicSquare
{
public:
    SinglyEvenMagicSquare(unsigned size);

protected:
    void fill(Cells& cells);
};

// specialized class for squares whose size is divisible by 4

class DoublyEvenMagicSquare : public MagicSquare
{
public:
    DoublyEvenMagicSquare(unsigned size);

protected:
    void fill(Cells& cells);
};
