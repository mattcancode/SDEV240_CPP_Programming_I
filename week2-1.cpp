#include <iostream>
#include <fstream>

int main(int argc, char **argv)
{
    std::ofstream output("week2.txt");
    if (!output)
    {
        std::cerr << "unable to create week2.txt\n";
        return 1;
    }

    for (int line = 1; line <= 15; ++line) {
        output << std::string(line, 'a') << '\n';
    }

    output.close();

    return 0;
}
