#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <string>

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

double findMean(std::ifstream &infile) {
    // population mean is 4.5 and population std dev is sqrt(8.25)/sqrt(n)
    double i = 0, total = 0; 
    int cur = 0, j = -1;
    std::string inp = "";
    while (i < 50) {
        if (0 == inp.length()) {
            std::getline(infile, inp);
        } else {
            cur = inp[0] - '0';
            total += cur;
            inp.erase(0, 1);
            i++;
        }
    }
    return total / 50;

}

int findNum (int length, int digits, int max, std::ifstream &infile) {
    int fin = INT32_MAX;
    
    std::string search = "", temp = "";
    bool valid = false;
    if (length < digits) {
        while (!valid) {
            while (search.size() <= length) {
                std::getline(infile, temp);
                search += temp;
            }
            search.erase(length);
            fin = std::stoi(search);
            search = "";
            if (pow(10, length-1) < fin+1)
                valid = true;
        }
    }
    else {
        while (fin > max && valid == false) {
            valid = false;
            while (search.size() <= length) {
                std::getline(infile, temp);
                search += temp;
            }
            search.erase(length);
            if (search[0] == '0') {
                search = "";
                continue;
            }
            fin = std::stoi(search);
            search = "";
            if (pow(10, length) < fin+1)
                valid = true;
        }
    }
    
    return fin;
}

int main() {
    std::ifstream infile("nums.txt");
    int Rmin = 0, Rmax = 0, range = 0, digits = 0;
    double sum = 0, p = 0, x = 0, x0 = -10;

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

    double freq[3] = {}, ztot = 0, freq2[100] = {};
    for (int n = 0; n < 1000; n++) {
        double stdev = 0;
        double mean = findMean(infile);
        double z = (mean - 4.5) / (sqrt(8.25) / sqrt(50));
        ztot += z;
        size_t i = 0;
        for (i = 0; i < scores.size(); i++) {
            if (z < scores[i])
                break;
        }
        std::cout << i + 1 << std:: endl;
        freq[i]++;
        int number = findNum(i + 1, digits, range, infile) + (Rmin - 1);
        freq2[number - 1]++;
        std::cout << "Mean: " << mean << " Z Score: " << z << " Number: " << number << std::endl;
    }
    
    std::cout << "Ones: " << freq[0] << " Twos: " << freq[1] << " Threes: " << freq[2] << std::endl;
    std::cout << "Total z: " << ztot << std::endl;
    for (int i = 0; i < 100; i++) {
        std::cout << freq2[i] << " ";
    }
    std::cout << std::endl;

    infile.close();
    return 0;
}