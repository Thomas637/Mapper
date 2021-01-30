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
    virtual double distance(const T& point1, const T& point2) = 0;

    double calculateEccentricity(size_t pointIndex, double r) {
        double sum{0};
        size_t N = points_.size();

        for (size_t i = 0; i < N; i++) {
            sum += std::pow(distance(points_[pointIndex], points_[i]), r);
        }

        return std::pow(sum/N, 1/r);
    }

protected:
    std::vector<T> points_;
};

template<size_t n>
class Rn : public MetricSpace<std::vector<double>> {
public:

    // Constructor
    Rn(std::vector<std::vector<double>> points) {
        points_ = points;
        validate();
    }

    // Checks whether all points have dimension n
    void validate() {
        bool invalidDimension = false;
        for (size_t i = 0; i < points_.size(); i++) {
            if (points_[i].size() != n) {
                invalidDimension = true;
                break;
            }
        }

        if (invalidDimension) {
            throw std::invalid_argument("At least one point has the wrong dimension.");
        }
    }

    // Distance function
    double distance(const std::vector<double>& point1, const std::vector<double>& point2) {
        double squaredDistance{0};

        for (size_t i = 0; i < n; i++) {
            squaredDistance += (point1[i] - point2[i])*(point1[i] - point2[i]);
        }

        return std::sqrt(squaredDistance);
    }
};


// Elements are sequences finite (x_1, ..., x_N) with x_i in {A, C, G, T}
// and the distance d(x, y) is the amount of i for which x_i != y_i.
class DNA: public MetricSpace<std::vector<char>> {
public:

    // Constructor
    DNA(std::vector<std::vector<char>> points) {
        points_ = points;
        validate();
    }

    // Checks whether the right letters are used
    void validate() {
        bool invalidCharacter = false;

        for (long i = 0; i < points_.size(); i++) {
            for (int j = 0; j < points_[i].size(); j++) {
                if ( points_[i][j] != 'A' && points_[i][j] != 'C' && points_[i][j] != 'G' && points_[i][j] != 'T' ) {
                    invalidCharacter = true;
                }
            }
        }

        if (invalidCharacter) {
            throw std::invalid_argument("At least one DNA strand contains a character not equal to A, C, G or T.");
        }
    }


    // Distance function (is actually always an integer)
    double distance(const std::vector<char>& point1, const std::vector<char>& point2) {
        size_t minSize;
        size_t maxSize;

        if (point1.size() < point2.size()) {
            minSize = point1.size();
            maxSize = point2.size();
        } else {
            minSize = point2.size();
            maxSize = point1.size();
        }

        // If DNA strands have different lengths, we add one to the distance if
        // one DNA strand has a nucleobasis at an index and the other does not.
        double counter{maxSize - minSize};

        for (size_t i = 0; i < minSize; i++) {
            if (point1[i] != point2[i]) {
                counter++;
            }
        }

        return counter;
    }
};

template<size_t m>
Rn<m> RnMaker(long numberOfPoints) {

    std::vector<std::vector<double>> points;

    // Setting up the random machine
    double lowerBound = 0;
    double upperBound = 10;
    std::uniform_real_distribution<double> uniform(lowerBound, upperBound);
    std::default_random_engine randomEngine;

    // Creating the random points
    for (long i = 0; i < numberOfPoints; i++) {
        std::vector<double> point;
        for (int j = 0; j < m; j++) {
            point.push_back(uniform(randomEngine));
        }
        points.push_back(point);
    }

    return Rn<m>(points);
}

DNA DNAMaker(int maxLength, long numberOfPoints) {

    std::vector<std::vector<char>> points;

    // Random seed
    srand(time(NULL));

    char characters[4] = {'A', 'C', 'G', 'T'};

    // Creating the random points
    for (long i = 0; i < numberOfPoints; i++) {
        int length = rand() % maxLength + 1;
        std::vector<char> point;
        for (int j = 0; j < length; j++) {
            point.push_back(characters[rand() % 4]);
        }
        points.push_back(point);
    }

    return DNA(points);
}

int main() {
    Rn<3> space = RnMaker<3>(100);
    DNA space2 = DNAMaker(10, 100);

    std::cout << "Eccentricity of the 10'th point in R^n with r = 2.1: " << space.calculateEccentricity(10, 2.1) << std::endl;
    std::cout << "Eccentricity of the 10'th point in a DNA metric space with r = 2.1: " << space2.calculateEccentricity(10, 2.1) << std::endl;
}
