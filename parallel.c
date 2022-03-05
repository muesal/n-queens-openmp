#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Board{
    int last;   // index of the last queen that was set (len(pos)-1)
    int pos[];  // list of positions of queens
};

// TODO: add structs for Nodes and stacks

/**
 * Method that creates a new Board from the given one and sets
 * a new queen at the given row.
 * 
 * @param board Board that should be copied
 * @param col position of new queen that should be appended
 * @return struct Board* 
 */
void append(struct Board * board, int col, struct Board *new_board);


// TODO: add methods for the node and the stack

/**
 * Method that starts the recursive tree search by setting 
 * the queen in the first row to all possible positions
 * and summing the solutions that are reachable from there.
 * 
 * @param n: number of queens
 * @return int, the number of solutions
 */
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
 */
bool is_valid(struct Board *board);

// TODO: Remove
void queens_rec(int n, struct Board *board, int *solutions);


int main(int argc, char **argv) {
    int n = argc > 1 ? atoi(argv[1]) : 2;
    
    // TODO: add timer
    int solutions = queens(n);
    printf("Number of solutions with %d queens: %d\n",
        n, solutions);
    return 0;
}


/*
Methods for the parallel tree search
*/

int queens(int n) {

    int solutions = 0;

    // TODO: initialise stack

    // TODO: parallel for loop
    for (int col = 0; col < n; col++) {
        // set queen in first row to column col
        struct Board *board = (struct Board *) calloc(2, sizeof(int));
        board->pos[0] = col;
        board->last = 0;
        // TODO: add to stack instead. 
        queens_rec(n, board, &solutions);
        free(board);
    }

    // TODO: call function or implement here: get thing from stack, till all stacks are empty

    return solutions;
}


// TODO: remove
void queens_rec(int n, struct Board *board, int *solutions) {
    
    if (board->last >= n-1){
        // base case: return 1
        // noo need to validate since only valid solutions are passed on
        (*solutions)++;
    } else {
        struct Board *new_pos = (struct Board *) calloc((board->last) + 3, sizeof(int));

        for (int col = 0; col < n; col++) {
            append(board, col, new_pos);

            // printf("%d    ", col);
            // for (int row = 0; row < (board->last) + 2; row++) {
            //     printf("%d ", new_pos->pos[row]);
            // }
            // printf("\n");

            if (is_valid(new_pos))
                queens_rec(n, new_pos, solutions);
        }

        free(new_pos);
    }
}

bool is_valid(struct Board *board) {
    int last = board->last;
    for (int r = 0; r < last; r++) {       // iterate over all previous rows
        if (board->pos[r] == board->pos[last]                // same column
            || r - board->pos[r] == last - board->pos[last]  // same diagonal
            || r + board->pos[r] == last + board->pos[last]) // same diagonal
            return false;
    }

    return true;
}


/*
Methods for the Boards
*/

void append(struct Board *board, int col, struct Board *new_board) {
    int last = board->last;

    for (int row = 0; row <= last; row++)
        new_board->pos[row] = board->pos[row];    

    new_board->pos[last + 1] = col;    
    new_board->last = last + 1;
}


/*
Methods for the Stack
*/

