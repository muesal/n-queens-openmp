#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node{
    int last_row;
    int* q_pos;
};


// TODO: create struct for the queue 

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
 * Method that checks if a newly built part-solution is valid,
 * by checking whether queen at position i violates any of
 * the given constraints (only one queen per ro, column and
 * diagonally)
 * 
 * @param a: part-solution
 * @param i: row of last queen that was set
 * @return boolean whether the (part-)solution is valid
 **/
bool is_valid(int q_pos[], int row);


/**
 * Method that creates a new Node from the given one and sets
 * a new queen at the given row.
 * 
 * @param q_pos 
 * @return struct Node* 
 */
struct Node * append(struct Node * q_pos, int col);


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
    int *q_pos = (int *) malloc(n * sizeof(int));

    \\ TODO: initialise queue
    \\ TODO: start parallel 


    return solutions;
}


bool is_valid(int q_pos[], int row) {
    for (int r = 0; r < row; r++) {      // iterate over all previous rows
        if (q_pos[r] == q_pos[row]               // same column
            || r - q_pos[r] == row - q_pos[row]  // same diagonally
            || r + q_pos[r] == row + q_pos[row]) // same diagonally
            return false;
    }

    return true;
}


struct Node* append(struct Node *q_pos, int col) {
    int last_row = q_pos->last_row;

    struct Node new_q_pos;
    struct Node * n = &new_q_pos;

    n->last_row = last_row + 1;
    n->q_pos = (int*) malloc((last_row + 1) * sizeof(int));
    
    for (int row = 0; row < last_row; row++) {
        n->q_pos[row] = q_pos->q_pos[row];
    }

    n->q_pos[last_row + 1] = col;
}

