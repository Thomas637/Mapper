#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string>

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

    int* data = new int[length];

    for (long i = 0; i < length; i++) {
        data[i] = rand() % maximum + 1;
    }

    return data;
}

// Checks whether data1 and data2 have an empty intersection or not
// in O(n log n + m log m) with n = |data1|, m = |data2|.
bool intersection(int data1[], int data2[], long n, long m) {
    mergeSort(data1, 0, n - 1);
    mergeSort(data2, 0, m - 1);

    long counter1 = 0;
    long counter2 = 0;

    while (counter1 < n && counter2 < m) {
        if (data1[counter1] == data2[counter2]) {
            return true;
        } else if (data1[counter1] < data2[counter2]) {
            counter1++;
        } else {
            counter2++;
        }
    }

    return false;
}

int main() {

    // Random seed
    srand(time(NULL));

    long length1 = 10;
    long length2 = 20;
    int maximum = 1000;

    int* data1;
    int* data2;
    data1 = randomArray(length1, maximum);
    data2 = randomArray(length2, maximum);

    std::cout << "Data 1:\n";

    printArray(data1, length1);

    std::cout << "\nData 2:\n";

    printArray(data2, length2);

    if (intersection(data1, data2, length1, length2)) {
        std::cout << "\nThey intersect!";
    } else {
        std::cout << "\nThey do not intersect!";
    }

    return 0;
}
