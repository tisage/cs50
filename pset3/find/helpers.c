/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 * Tianyu
 * 2017 Spring
 */

#include <cs50.h>

#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    if (value < 0)
        return false;

    // binary search to achieve O(log n) complexity, non-recursive
    int max = n;
    int min = 0;

    while (max >= min) {
        int mid = (min + max) / 2; // set mid point
        if (values[mid] < value) // which means the value we are looking for is on the right half
            min = mid + 1; // add boundary point to shrink search range
        else if (values[mid] > value) // otherwise search left
            max = mid - 1; // less boundary point to shrink search range
        else // otherwise found it
            return true;
    }
    return false;
}

bool binarySearch(int value, int values[], int start, int end)
{
    // check input
    if (start > end)
        return false;

    int mid = (start + end) / 2;

    if (values[mid] == value)
        return true;
    else if (values[mid] < value) // search the right half
        return binarySearch(value, values, mid + 1, end);
    else // search the left half
        return binarySearch(value, values, start, mid - 1);
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    // TODO: implement an O(n^2) sorting algorithm

    /* Bubble sort
    do
        swapped = false
        for i = 1 to indexOfLastUnsortedElement
            if leftElement > rightElement
              swap(leftElement, rightElement)
        swapped = true
    while swapped
    */

    for (int i = 0; i < n - 1; i++) { // outside loop for the whole array
        for (int j = 0; j < n - i - 1; j++) { // inside loop for search and swap
            if (values[j] > values[j + 1]) { // compare element
                //swap
                int temp = values[j + 1]; // use a temp variable to hold swap values
                values[j + 1] = values[j];
                values[j] = temp;
            }
        }
    }

}

/**
 * Sorts array of n values.
 */
void select_sort(int values[], int n)
{
    // TODO: implement an O(n^2) sorting algorithm
    /*
    Select Sorting

    repeat (numOfElements - 1) times
      set the first unsorted element as the minimum
      for each of the unsorted elements
        if element < currentMinimum
          set element as new minimum
      swap minimum with first unsorted position
    */
    for (int i = 0; i < n; i++) { // outside loop for the whole array
        for (int j = i + 1; j < n; j++) { // inside loop for search and swap
            if (values[i] > values[j]) { // compare element
                //swap
                int temp = values[i]; // use a temp variable to hold swap values
                values[i] = values[j];
                values[j] = temp;
            }
        }
    }

}

/**
 * Sorts array of n values.
 */
void insert_sort(int values[], int n)
{
    // TODO: implement an O(n^2) sorting algorithm
    /*
    Select Sorting

    repeat (numOfElements - 1) times
      set the first unsorted element as the minimum
      for each of the unsorted elements
        if element < currentMinimum
          set element as new minimum
      swap minimum with first unsorted position
    */
    int i, j, temp;
    for (i = 0; i < n - 1; i++) {
        temp = values[i];
        for (j = i - 1; (temp < values[j] && j >= 0); j--) {
            values[j + 1] = values[j];
            --j;
        }
        values[j + 1] = temp;
    }

}