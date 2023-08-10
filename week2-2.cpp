#include <iostream>

int main(int argc, char **argv)
{
    int number;

    std::cout << "Please enter an integer: ";
    std::cin >> number;

    std::cout << "\nThe first 10 multiples of " << number << " are: ";

    int multiple = 1;
    do
    {
        std::cout << number * multiple << ", ";
    } while (++multiple < 10);

    std::cout << "and " << number * multiple << ".\n\n";

    return 0;
}
