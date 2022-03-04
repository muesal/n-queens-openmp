#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>


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
int queens_rec(int n, int a[], int i);


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
bool is_valid(int a[], int i);


int main(int argc, char **argv) {
    int n = argc > 1 ? atoi(argv[1]) : 2;
    
    int solutions = queens(n);
    printf("Number of solutions with %d queens: %d\n",
        n, solutions);
    return 0;
}

int queens(int n) {

    int solutions = 0;

    for (int i = 0; i < n; i++) {
        int* a = &i; // set first queen to column i
        solutions += queens_rec(n, a, 1);
    }

    return solutions;

}

int queens_rec(int n, int a[], int i) {
    // base case: return 1 (only valid solutions are passed on)
    if (i >= n){
        for (int q = 0; q < n; q++) {
            printf(" %d", a[q]);
        }
        printf("\n");
        return 1;
    }

    int solutions = 0;

    for (int j = 0; j < n; j++) {

        int *a_new = (int *) malloc(i * sizeof(int));
        a_new = a; // deep copy of array, I think
        a_new[i] = j;

        if (is_valid(a_new, i)) {
            solutions += queens_rec(n, a_new, i + 1);
        }
    }

    return solutions;
}

bool is_valid(int a[], int i) {
    for (int j = 0; j < i; j++) {
        if (a[j] == a[i] // same column
            || a[i] - a[j] == i - j // same diagonally
            || j - a[j] == i - a[i] // same diagonally
            || j - a[j] == a[i]- i) // same diagonally
            return false;
    }

    return true;
}
