#include <iostream>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <time.h>

/*
Takes a set {A_1, ..., A_n} of ordered subsets of N and a vector (x_1, ..., x_n).
Denote a_i for the x_i'th element of A_i. Let m be the minimum of {a_i}. This function
returns {i | a_i = m}.
*/
std::vector<int> helperMinimum(std::vector<std::vector<long>>& sets, std::vector<long>& counters) {

    // Calculating m.
    std::vector<long> values{};

    for (int i = 0; i < counters.size(); i++) {
        values.push_back(sets[i][counters[i]]);
    }

    // std::min_element returns a pointer/iterator to the value
    long minimum = *std::min_element(values.begin(), values.end());

    std::vector<int> results;

    for (int i = 0; i < counters.size(); i++) {
        if (sets[i][counters[i]] == minimum) {
            results.push_back(i);
        }
    }

    return results;
}

/*
Returns false if one of the counters exceeds the elements in that set.
*/
bool helperCheck(std::vector<std::vector<long>>& sets, std::vector<long>& counters) {
    bool check = true;

    for (int i = 0; i < sets.size(); i++) {
        if (counters[i] == sets[i].size()) {
            check = false;
        }
    }

    return check;
}

/*
Takes a list of subsets of the natural numbers, and returns
a yes-certificate if the intersection is non-empty, or -1
if the intersection is empty.
*/
long intersection(std::vector<std::vector<long>>& sets) {

    // Each set gets a counter that starts at zero,
    // and sorts the sets.
    std::vector<long> counters{};
    for (int i = 0; i < sets.size(); i++) {
        counters.push_back(0);
        std::sort(sets[i].begin(), sets[i].end());
    }

    while (helperCheck(sets, counters)) {

        std::vector<int> results = helperMinimum(sets, counters);

        if (results.size() == counters.size()) {
            return sets[0][counters[0]]; // Apparently the minimum is in all sets
        } else {
            for (int i = 0; i < results.size(); i++) {
                counters[results[i]]++; // The elements at these indices are certainly not in the intersection, so we
                                        // can move on to the next element.
            }
        }
    }

    return -1;
}

/*
Checks whether the intersection function correctly identifies that
there is no intersection between A_n = {i | i < maxInt and i = n mod moduloClass}.
*/
void negativeTester(int moduloClass, long maxInt) {
    // Making the sets
    std::vector<std::vector<long>> sets{};

    for (int n = 0; n < moduloClass; n++) {
        std::vector<long> setn{};
        for (int i = 0; i < maxInt; i++) {
            if (i%moduloClass == n) {
                setn.push_back(i);
            }
        }

        sets.push_back(setn);
    }

    long result = intersection(sets);

    if (result == -1) {
        std::cout << "Correctly identifies the intersection is empty!" << std::endl;
    } else {
        std::cout << "Wrong result, it finds " << result << " in the intersection!" << std::endl;
    }
}

bool elementOf(long element, std::vector<long>& vec) {
    // std::find returns a pointer to the element, or the end of the range
    // if it is not present.
    return std::find(vec.begin(), vec.end(), element) != vec.end();
}

/*
Creates random sets and runs intersection(). Verifies the yes-certificate.
*/
void positiveTester(int numberOfSets, long maxValue, long length) {

    // Creating the sets
    srand(time(NULL));

    std::vector<std::vector<long>> sets{};

    for (int i = 0; i < numberOfSets; i++) {
        std::vector<long> seti{};

        for (int j = 0; j < length; j++) {
            seti.push_back(rand() % maxValue);
        }

        sets.push_back(seti);
    }

    long result = intersection(sets);

    if (result == -1) {
        std::cout << "We could not find an element in the intersection. This may be a false negative." << std::endl;
    } else {
        // Verify yes-certificate
        bool inEverySet = true;
        for (int i = 0; i < numberOfSets; i++) {
            if (!elementOf(result, sets[i])) {
                inEverySet = false;
            }
        }

        if (inEverySet) {
            std::cout << "We correctly found that " << result << " is in every set." << std::endl;
        } else {
            std::cout << "We incorrectly found that " << result << "is in every set." << std::endl;
        }

    }


}

int main() {
    negativeTester(10, 1000);
    positiveTester(10, 1000, 1000);
}
