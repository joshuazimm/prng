#include <iostream>
#include <chrono>
#include <vector>

const int n = 624; // The length of the state array
const int m = 397;
const int w = 31; // word size in bits 
const int r = 31;
const int a = 0x9908B0DF;
const int u = 11;
const int d = 0xFFFFFFFF;
const int s = 7;
const int b = 0x9D2C5680;
const int t = 15;
const int c = 0xEFC60000;
const int l = 18;
const int f = 1812433253;

std::vector<uint32_t> MT(n); // The state array
int index = n + 1;

// Function to seed the generator
void seed_mt(int seed) {
    index = n;
    MT[0] = seed;
    for (int i = 1; i < n; ++i) {
        MT[i] = (f * (MT[i-1] ^ (MT[i-1] >> (w-2))) + i) & ((1 << w) - 1);
    }
}

// Generate the next n values from the series x_i 
void twist() {
    for (int i = 0; i < n; ++i) {
        int x = (MT[i] & (1 << r)) | (MT[(i + 1) % n] & ((1 << r) - 1));
        int xA = x >> 1;
        if (x % 2 != 0) { // lowest bit of x is 1
            xA ^= a;
        }
        MT[i] = MT[(i + m) % n] ^ xA;
    }
    index = 0;
}

// Extract a tempered value based on MT[index]
// calling twist() every n numbers
int extract_number() {
    if (index >= n) {
        if (index > n) {
            throw std::runtime_error("Generator was never seeded");
        }
        twist();
    }

    int y = MT[index];
    y ^= ((y >> u) & d);
    y ^= ((y << s) & b);
    y ^= ((y << t) & c);
    y ^= (y >> l);

    ++index;
    return y & ((1 << w) - 1);
}

int create_number(int range) {
    return (extract_number() % range);
}

int main() {
    seed_mt(std::chrono::system_clock::now().time_since_epoch().count());
    int Rmin = 0, Rmax = 0, range = 0, test = 9, start = 0, length = 1, number = 0, j = 0, attempts = 0, numbers = 0;

    do {
        std::cout << "Lower Bound: ";
        std::cin >> Rmin;
        std::cout << "Upper Bound: ";
        std::cin >> Rmax;
        std::cout << "Number of Numbers: ";
        std::cin >> numbers;

        if (std::cin.fail()) {
            std::cout << "Invalid input. Please enter numeric values." << std::endl;
            std::cin.clear(); // Clear the error state
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore the remaining input in the buffer
            continue;
        }

        if (Rmin > Rmax) {
            std::cout << "Error: Lower bound must be less than or equal to upper bound." << std::endl;
        }
    } while (Rmin > Rmax);

    range = Rmax - Rmin + 1;
    int* freq = new int[range]();

    auto startTime = std::chrono::high_resolution_clock::now();


    while (j < numbers) {
        number = create_number(range) + 1;
        freq[number - 1]++;
        j++;
    }


    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    

    std::cout << "Frequencies of each number: " << std::endl;
    for (int i = 0; i < range; i++) {
        std::cout << freq[i] << " ";
    }
    std::cout << std::endl;
    std::cout << "Generated " << numbers << " numbers between " << Rmin << " and " << Rmax << " in " << duration.count() << " milliseconds." << std::endl;

    delete[] freq;
    return 0;
}