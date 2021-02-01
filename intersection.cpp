#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string>

bool intersection(int** arrayList, int amountOfSets) {

}

// data is an array, length is the length of that array
void printArray(int data[], int length) {
    std::cout << '{';
    for (long i = 0; i < length - 1; i++) {
        std::cout << data[i] << ", ";      // Arrays and pointers are basically the same, [] is dereferencing
    }
    std::cout << data[length - 1] << '}';
}

// Given an array data and sorted subarrays data[left, ..., mergePoint]
// data[mergePoint + 1, right], returns data with data[left, ..., right]
// sorted and the rest left alone.
void merger(int data[], long left, long right, long mergePoint) {
    long l1 = mergePoint - left + 1; // Length of first subarray
    long l2 = right - mergePoint; // Length of second subarray

    // Saving the subarrays
    int* first = new int[l1];
    int* second = new int[l2];

    for (long i = 0; i < l1; i++) {
        first[i] = data[left + i];
    }

    for (long i = 0; i < l2; i++) {
        second[i] = data[mergePoint + 1 + i];
    }

    // We now have k iterate within data, l iterate over the first subarray
    // and m over the second subarray
    long k = left;
    long l = 0;
    long m = 0;

    // To ensure neither l or m goes out of bounds
    while (l < l1 && m < l2) {
        if (first[l] <= second[m]) { // We put first[l] in the array and then go to the next
            data[k] = first[l];      // entry in the first subarray
            l++;
        } else {
            data[k] = second[m];
            m++;
        }
        k++;
    }

    // In case m went out of bounds, but l is not finished yet
    while (l < l1) {
        data[k] = first[l];
        l++;
        k++;
    }

    while (m < l2) {
        data[k] = second[m];
        m++;
        k++;
    }

}

// Takes an array data and sorts array[left, right]
void mergeSort(int data[], long left, long right) {

    // Nothing to do here
    if (left >= right) {
        return;
    }

    long mergePoint = (left + right - 1)/2;
    mergeSort(data, left, mergePoint);
    mergeSort(data, mergePoint + 1, right);
    merger(data, left, right, mergePoint);
}

// Produces a pointer to a random array of length length with values between 1 and maximum.
int* randomArray(long length, int maximum) {

<<<<<<< Updated upstream
    int* data = new int[length];
=======
/*
Creates random sets and runs intersection(). Verifies yes-certificates.
*/
void positiveTester(int numberOfSets, long maxValue, long length) {
>>>>>>> Stashed changes

    // Random seed
    srand(time(NULL));

    for (long i = 0; i < length; i++) {
        data[i] = rand() % maximum + 1;
    }

    return data;
}

int main() {

    // We can play around with this
    // 17 seconds for length 10 million and maximum 1 billion
    long length = 10;
    int maximum = 100;

    int* data;
    data = randomArray(length, maximum);

    std::cout << "Unsorted array:\n\n";

    printArray(data, length);

    std::cout << "\n\nSorted array:\n\n";

    mergeSort(data, 0, length - 1);

<<<<<<< Updated upstream
    printArray(data, length);

    return 0;
=======
int main() {
    // A 100 sets of 1000 integers.
    negativeTester(100, 100000);
    // 10 sets of 1000 integers.
    positiveTester(10, 1000, 1000);
>>>>>>> Stashed changes
}
