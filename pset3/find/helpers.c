/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */
 
#include <cs50.h>

#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    if(value<0)
        return false;
        
    // binary search to achieve O(log n) complexity, non-recursive
    int max = n; 
    int min = 0;  
    
    while(max>=min){
        int mid = (min+max)/2;
        if(values[mid]<value) // search right
            min = mid + 1;
        else if(values[mid]>value) // search left
            max = mid -1; // search left side and set max to the previous element of mid point
        else // mid point
            return true;
    }
    return false;
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
    
    for(int i=0;i<n;i++){ // outside loop for array
        for(int j=0;j<i-1;j++){ // inside loop for search and swap
            if(values[j]>values[j+1]){ // compare element
                //swap
                int temp = values[j+1]; // use a temp variable to achieve swap
                values[j+1] = values[j];
                values[j] = temp;
            }
        }
    }
    
}
