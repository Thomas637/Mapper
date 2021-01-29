#include <iostream>
#include <stdio.h>
#include <stdexcept>
#include <vector>
#include <cmath>
#include <algorithm>
#include <time.h>
#include <random>

template<typename T>
class MetricSpace {
public:
    double d();

private:
    std::vector<T> points_;
};

class Rn : public MetricSpace<std::vector<double>> {
public:
    using MetricSpace::d; // So we can overload the distance function

    // Constructor
    Rn(std::vector<std::vector<double>> points, int n): dimension_{n} {
        points_ = points;

        bool invalidDimension = false;
        for (long i = 0; i < points_.size(); i++) {
            if (points_[i].size() != dimension_) {
                invalidDimension = true;
                break;
            }
        }

        if (invalidDimension) {
            throw std::invalid_argument("At least one point has the wrong dimension.");
        }
    }

    // Distance function
    double d(std::vector<double> point1, std::vector<double> point2) {
        double squaredDistance{0};

        for (int i = 0; i < dimension_; i++) {
            squaredDistance += (point1[i] - point2[i])*(point1[i] - point2[i]);
        }

        return sqrt(squaredDistance);
    }

private:
    std::vector<std::vector<double>> points_;
    int dimension_;
    friend double eccentric(Rn space, long pointIndex, double r);
};

// Elements are sequences finite (x_1, ..., x_N) with x_i in {A, C, G, T}
// and the distance d(x, y) is the amount of i for which x_i != y_i.
class DNA: public MetricSpace<std::vector<char>> {
public:
    using MetricSpace::d;

    // Constructor
    DNA(std::vector<std::vector<char>> points, int length): length_{length} {
        points_ = points;

        bool invalidDimension = false;
        bool invalidCharacter = false;

        for (long i = 0; i < points_.size(); i++) {
            if (points_[i].size() != length_) {
                invalidDimension = true;
            }
            for (int j = 0; j < points_[i].size(); j++) {
                if ( points_[i][j] != 'A' && points_[i][j] != 'C' && points_[i][j] != 'G' && points_[i][j] != 'T' ) {
                    invalidCharacter = true;
                }
            }
        }

        if (invalidDimension) {
            throw std::invalid_argument("At least one DNA strand is too long or too short.");
        }

        if (invalidCharacter) {
            throw std::invalid_argument("At least one DNA strand contains a character not equal to A, C, G or T.");
        }

    }

    // Distance function
    double d(std::vector<char> point1, std::vector<char> point2) {
        int counter{0};
        for (int i = 0; i < point1.size(); i++) {
            if (point1[i] != point2[i]) {
                counter++;
            }
        }

        return counter;
    }

private:
    std::vector<std::vector<char>> points_;
    int length_;
    friend double eccentric2(DNA space, long pointIndex, double r);
};


double eccentric(Rn space, long pointIndex, double r) {
    double sum = 0;
    long N = space.points_.size();
    for (long i = 0; i < N; i++) {
        sum += pow(space.d(space.points_[pointIndex], space.points_[i]), r);
    }

    return pow(sum/N, 1/r);
}

double eccentric2(DNA space, long pointIndex, double r) {
    double sum = 0;
    long N = space.points_.size();
    for (long i = 0; i < N; i++) {
        sum += pow(space.d(space.points_[pointIndex], space.points_[i]), r);
    }

    return pow(sum/N, 1/r);
}

Rn RnMaker(int dimension, long numberOfPoints) {

    std::vector<std::vector<double>> points;

    // Setting up the random machine
    double lowerBound = 0;
    double upperBound = 10;
    std::uniform_real_distribution<double> uniform(lowerBound, upperBound);
    std::default_random_engine randomEngine;

    // Creating the random points
    for (long i = 0; i < numberOfPoints; i++) {
        std::vector<double> point;
        for (int j = 0; j < dimension; j++) {
            point.push_back(uniform(randomEngine));
        }
        points.push_back(point);
    }

    return Rn(points, dimension);
}

DNA DNAMaker(int length, long numberOfPoints) {

    std::vector<std::vector<char>> points;

    // Random seed
    srand(time(NULL));

    char characters[4] = {'A', 'C', 'G', 'T'};

    // Creating the random points
    for (long i = 0; i < numberOfPoints; i++) {
        std::vector<char> point;
        for (int j = 0; j < length; j++) {
            point.push_back(characters[rand() % 4]);
        }
        points.push_back(point);
    }

    return DNA(points, length);
}

int main() {
    Rn space = RnMaker(3, 100);
    DNA space2 = DNAMaker(10, 100);

    std::cout << "Eccentricity of the 10'th point in R^n with r = 2.1: " << eccentric(space, 10, 2.1) << std::endl;
    std::cout << "Eccentricity of the 10'th point in a DNA metric space with r = 2.1: " << eccentric2(space2, 10, 2.1) << std::endl;
}
