#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node{
    int last;   // index of the last queen that was set (len(pos)-1)
    int pos[];  // list of positions of queens
};


/**
 * Method that creates a new Node from the given one and sets
 * a new queen at the given row.
 * 
 * @param q_pos Node that should be copied
 * @param col position of new queen that should be appended
 * @return struct Node* 
 */
void append(struct Node * q_pos, int col, struct Node *new_q_pos);

void Node_delete(struct Node *node) {
    free(node);
}


// TODO: create struct for the queue 

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
bool is_valid(struct Node *q_pos);

// TODO: Remove
void queens_rec(int n, struct Node *q_pos, int *solutions);


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

    struct Node *q_pos = (struct Node *) calloc(2, sizeof(int));

    for (int col = 0; col < n; col++) {
        // set queen in first row to column col
        q_pos->pos[0] = col;
        q_pos->last = 0;
        queens_rec(n, q_pos, &solutions);
    }

    Node_delete(q_pos);

    return solutions;
}

// TODO: remove
void queens_rec(int n, struct Node *q_pos, int *solutions) {
    
    if (q_pos->last >= n-1){
        // base case: return 1
        // noo need to validate since only valid solutions are passed on
        (*solutions)++;
    } else {
        struct Node *new_pos = (struct Node *) calloc((q_pos->last) + 3, sizeof(int));

        for (int col = 0; col < n; col++) {
            append(q_pos, col, new_pos);

            // printf("%d    ", col);
            // for (int row = 0; row < (q_pos->last) + 2; row++) {
            //     printf("%d ", new_pos->pos[row]);
            // }
            // printf("\n");

            if (is_valid(new_pos))
                queens_rec(n, new_pos, solutions);
        }

        Node_delete(new_pos);
    }
}

bool is_valid(struct Node *q_pos) {
    int last = q_pos->last;
    for (int r = 0; r < last; r++) {       // iterate over all previous rows
        if (q_pos->pos[r] == q_pos->pos[last]                // same column
            || r - q_pos->pos[r] == last - q_pos->pos[last]  // same diagonal
            || r + q_pos->pos[r] == last + q_pos->pos[last]) // same diagonal
            return false;
    }

    return true;
}


void append(struct Node *q_pos, int col, struct Node *new_q_pos) {
    int last = q_pos->last;

    for (int row = 0; row <= last; row++)
        new_q_pos->pos[row] = q_pos->pos[row];    

    new_q_pos->pos[last + 1] = col;    
    new_q_pos->last = last + 1;
}

