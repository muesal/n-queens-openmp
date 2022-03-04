#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/**
 * Method that starts the recursive tree search by setting 
 * the queen in the first row to all possible positions
 * and summing the solutions that are reachable from there.
 * 
 * @param n: number of queens
 * @return int, the number of solutions
 **/
int queens(int n);


/**
 * Recursive method that returns 1 if the given a is a valid
 * solution (base case) or sets one new queen and makes
 * a recursive call, if the part-solution is still valid. 
 * 
 * @param n: number of queens
 * @param a: part-solution
 * @param i: next row to set queen in
 * @return int, the number of solutions reachable from given a
 **/
void queens_rec(int n, int a[], int row, int *solutions);


/**
 * Method that checks if a newly built part-solution is valid,
 * by checking whether queen at position i violates any of
 * the given constraints (only one queen per ro, column and
 * diagonally)
 * 
 * @param a: part-solution
 * @param i: row of last queen that was set
 * @return boolean whether the (part-)solution is valid
 **/
bool is_valid(int a[], int row);


int main(int argc, char **argv) {
    int n = argc > 1 ? atoi(argv[1]) : 2;
    
    // TODO: add timer
    int solutions = queens(n);
    printf("Number of solutions with %d queens: %d\n",
        n, solutions);
    return 0;
}

int queens(int n) {

    int solutions = 0;
    int *a = (int *) malloc(n * sizeof(int));

    for (int col = 0; col < n; col++) {
        // set queen in first row to column col
        a[0] = col;
        queens_rec(n, a, 1, &solutions);
    }

    return solutions;
}

// TODO: n-row instead of n and row; only one value. base case if 0
void queens_rec(int n, int a[], int row, int *solutions) {
    
    if (row >= n){
        // base case: return 1
        // noo need to validate since only valid solutions are passed on
        (*solutions)++;
    } else {
        for (int col = 0; col < n; col++) {
            a[row] = col;

            if (is_valid(a, row))
                queens_rec(n, a, row + 1, solutions);
        }
    }
}

bool is_valid(int a[], int row) {
    for (int r = 0; r < row; r++) {      // iterate over all previous rows
        if (a[r] == a[row]               // same column
            || r - a[r] == row - a[row]  // same diagonally
            || r + a[r] == row + a[row]) // same diagonally
            return false;
    }

    return true;
}
