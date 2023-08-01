#include <iostream>
#include <chrono>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <string>

// Linear Congruential Generator parameters
const unsigned int LCG_MULTIPLIER = 1103515245;
const unsigned int LCG_INCREMENT = 12345;
const unsigned int LCG_MODULUS = 2147483648; //2^31

// Function to generate a pseudo-random number using LCG
unsigned int lcg_random(unsigned int& seed) {
    seed = (LCG_MULTIPLIER * seed + LCG_INCREMENT) % LCG_MODULUS;
    return seed;
}

std::string generate_random_string(int length) {
    // Get the current system time as a seed for the LCG
    unsigned int seed = static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count());

    std::string random_string;
    random_string.reserve(length);

    for (int i = 0; i < length; ++i) {
        unsigned int random_number = lcg_random(seed);
        random_string += std::to_string(random_number % 10); // Extract the last digit
    }

    return random_string;
}

std::vector<int> count_numbers(int length, int num, double &sum) {
    std::vector<int> results;

    for (int l = 1; l < length; l++) {                  // for all digits except greatest
        int possibilities = pow(10, l) - pow(10, l-1);
        sum += possibilities;
        results.push_back(possibilities);
    }

    int possibilities = num - pow(10, length-1) + 1;          // greatest digit
    sum += possibilities;
    results.push_back(possibilities);

    return results;
}

double f(double x, double min, double p) {
    // integration of the normal distribution function solving for 0
    return (erf(x / sqrt(2.0)) / 2.0) - (erf(min / sqrt(2.0)) / 2.0) - p;
}

double findMax(double min, double p) {
    double a = min;
    double b = 10;
    double c = (a + b) / 2.0;
    for (int i = 0; i < 20; i++) { // manual reimann sum runs 10 times as a tolerance measure
        if (f(a, min, p) * f(c, min, p) < 0) 
            b = c;
        else 
            a = c;
        c = (a + b) / 2.0;
    }
    return c;
}

double findMean(std::string numbers) {
    // population mean is 4.5 and population std dev is sqrt(8.25)/sqrt(n)
    double i = 0, total = 0; 
    int cur = 0;
    std::string inp = "";
    while (i < 50) {
        cur = numbers[0] - '0';
        total += cur;
        numbers.erase(0, 1);
        i++;
    }
    return total / 50;
}

int findNum(int length, int digits, std::string max) {
    bool valid = false, first_digit = true;
    std::string num = "", total = "";
    if (length < digits) {
        while (!valid) {
            total = generate_random_string(length);
            if (total[0] != '0') {
                valid = true;
            }
        }
    }
    else {
        for (int i = 0; i < length; i++) {
            valid = false;
            while (!valid) {
                num = generate_random_string(1);
                if (!(first_digit == true && num[0] == '0') && (num[0] - '0' <= max[i] - '0')) {
                    total += num[0];
                    first_digit = false;
                    valid = true;
                }
            }
        }
    }
    return std::stoi(total);
}

int main() {

    int Rmin = 0, Rmax = 0, range = 0, digits = 0;
    double sum = 0, p = 0, x = 0, x0 = -10, mean = 0, z = 0;
    std::string random_number = "";

    std::cout << "Lower Bound: ";
    std::cin >> Rmin;
    std::cout << "Upper Bound: ";
    std::cin >> Rmax;

    range = Rmax - Rmin + 1; // range for now is arbitrarily between (1, n)
    int rtemp = range;
    while (rtemp != 0) {
        digits++;
        rtemp /= 10;
    }

    std::vector<int> props = count_numbers(digits, range, sum);

    std::vector<double> scores;
    for (size_t i = 0; i < props.size() - 1; i++) {
        p = props[i] / sum;
        x = findMax(x0, p);
        scores.push_back(x);
        x0 = x;
    }

    random_number = generate_random_string(50);
    mean = findMean(random_number);
    z = (mean - 4.5) / (sqrt(8.25) / sqrt(50));
    for (size_t i = 0; i < scores.size(); i++) {
        if (z < scores[i])
            break;
        }
    int number = findNum(i + 1, digits, std::to_string(range)) + (Rmin - 1);
    std::cout << "Mean: " << mean << " Z Score: " << z << " Number: " << number << std::endl;

    return 0;
}