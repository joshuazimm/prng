#include <iostream>
#include <fstream>
#include <cstring>

int main() {
    int counts[10] = {0}; // initialize all counts to zero

    std::ifstream infile("nums.txt");

    // Count the frequency of each digit in each integer
    std::string line;
    std::getline(infile, line);
    std::string first = line;
    int q = 1;
    bool repeat = false;
    while (std::getline(infile, line)) {
        if (line == first) 
            repeat = true;
        else if (repeat == false)
            q++;
        for (char c : line) {
            if (std::isdigit(c)) {
                counts[c - '0']++;
            }
        }
    }
    if (repeat) 
        std::cout << "It took " << q << " lines to repeat" << std::endl;
    else
        std::cout << "No repeat found" << std::endl;

    // Output the counts
    std::cout << "Totals:" << std::endl;
    for (int i = 0; i < 10; i++) {
        std::cout << "Digit " << i << ": " << counts[i] << std::endl;
    }
    std::cout << std::endl;
    infile.close();

    // singular digits places
    for (int n = 0; n < 7; n++) {
        std::ifstream infile("nums.txt");
        int counts[10] = {0};
        while (std::getline(infile, line)) {
            if (std::isdigit(line[n])) {
                counts[line[n] - '0']++;
            }
        }
        std::cout << n << "s Place:" << std::endl;
        for (int i = 0; i < 10; i++) {
            std::cout << "Digit " << i << ": " << counts[i] << std::endl;
        }
        std::cout << std::endl;
        infile.close();
    }

    return 0;
}