#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "serial.h"


int main(int argc, char **argv) {
    int n = argc > 1 ? atoi(argv[1]) : 2;
    
    // start timer
    time_t start, end;
    start = time(NULL);

    // get number of solutions
    int solutions = queens(n);
    printf("Number of solutions with %d queens: %d\n",
        n, solutions);

    // end timer
    end = time(NULL);
    printf("        elapsed time: %ld\n", end - start);
    return 0;
}

int queens(int n) {

    int solutions = 0;
    int *q_pos = (int *) malloc(n * sizeof(int));

    for (int col = 0; col < n; col++) {
        // set queen in first row to column col
        q_pos[0] = col;
        queens_rec(n, q_pos, 1, &solutions);
    }

    return solutions;
}

void queens_rec(int n, int q_pos[], int row, int *solutions) {
    
    if (row >= n){
        // base case: return 1
        // noo need to validate since only valid solutions are passed on
        (*solutions)++;
    } else {
        for (int col = 0; col < n; col++) {
            // set next queen
            q_pos[row] = col;

            // Check if configuration is valid, if so pass on to next recursive call
            if (is_valid(q_pos, row))
                queens_rec(n, q_pos, row + 1, solutions);
        }
    }
}

bool is_valid(int q_pos[], int row) {
    for (int r = 0; r < row; r++) {      // iterate over all previous rows
        if (q_pos[r] == q_pos[row]               // same column
            || r - q_pos[r] == row - q_pos[row]  // same diagonal
            || r + q_pos[r] == row + q_pos[row]) // same diagonal
            return false;
    }

    return true;
}
