#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include "parallel.h"

int n;
int thread_count;
int n_q;
struct Queue **queues;

int main(int argc, char **argv) {
    // get the number of queens and threads from the passed arguments
    n = argc > 1 ? atoi(argv[1]) : 3;
    thread_count = argc > 2 ? atoi(argv[2]) : omp_get_max_threads();

    // initialise timer
    double start; 
    double end; 
    start = omp_get_wtime();

    // compute the number of solutions for n queens
    int solutions = queens();
    printf("Number of solutions with %d queens and %d threads: %d\n",
        n,  thread_count, solutions);

    // compute passed time
    end = omp_get_wtime();
    printf("        elapsed time: %lf\n", end - start);
    return 0;
}


/*
Methods for the parallel tree search
*/

int queens() {
    if (n == 1) {
        // the followoing structure does not allow n < 2.
        return 1;
    }

    // initialise queues: one for every second row (-> floor(n/2) queues)
    queues = (struct Queue **) malloc((n / 2) * sizeof( *queues));
    n_q = n/2;
    for (int i = 0; i < n_q; i++) {
        queues[i] = queue_create(); 
    }

    // add all positions of the first queen to the first queue
    struct Board *board;
    for (int col = 0; col < n; col++) {
        // set queen in first row to column col
        board = board_create(col);
        
        // add configuration to queue
        queue_push(queues[0], &board);
    }

    // start the parallel computation
    int solutions = 0;
    queens_parallel(n_q, &solutions);

    // Free the queues
    for (int i = 0; i < n_q; i++)
        queue_delete(queues[i]);
    free(queues);

    return solutions;
}


void queens_parallel(int n_q, int *solutions) {

    // shared variables
    int done = thread_count;
    int sol = 0; // reduction on pointer does not work

#   pragma omp parallel num_threads(thread_count) reduction(+:sol)
    {
        // private variables
        struct Node *node;
        int q;
        int my_solutions = 0;
        bool working = true;

        while (done < thread_count || working) {
            // work while there are still nodes in any of the queues
            while (queues_get_node(&node, &q)) {
                if (!working) {
                    working = true;
#                   pragma omp atomic
                    done--;
                }

                /*printf("%d    ", q);
                for (int row = 0; row < q+1; row++) {
                    printf("%d ", board->pos[row]);
                }
                printf("\n");*/
                int size = node->board->last;

                if (size + 1 >= n - 1) {
                    // Only one more queen must be added
                    my_solutions += create_successor_final(node->board);
                } else if (size + 2 >= n - 1) {
                    // exactly two more queens must be added
                    my_solutions += create_successor_two_final(node->board);
                } else {
                    // Add two queens and add the valid results to queue q + 1
                    create_successor_two(q+1, node->board);
                }

                node_delete(node);
            }

            if(working) {
                working = false;
#               pragma omp atomic
                done++;
            }

        } // end while(!queues_are_empty)

        sol += my_solutions; // reduction
    } // end parallel block

    *solutions = sol;
}


void create_successor_two(int q, struct Board *board) {
    int size = board->last;

    struct Board *new_board = (struct Board *) calloc(size + 4, sizeof(int));
    for (int col = 0; col < n; col++) {
        // create a new board

        // set the next queen to all columns of the respective row
        append(board, col, new_board);

        // check if the new queen is in a valid spot; if so append another one
        if (is_valid(new_board)) {
            for (int col_2 = 0; col_2 < n; col_2++) {
                // create another board. This must be done to keep them persistent in the queue
                struct Board *new_board_2 = (struct Board *) calloc(size + 4, sizeof(int));

                // set the next queen to all columns of the respective row
                append(new_board, col_2, new_board_2);

                // check if the new queen is in a valid spot, if so push it
                if (is_valid(new_board_2))
                    queue_push(queues[q], &new_board_2);
                else
                    board_delete(new_board_2);
            }
        }

    }
    board_delete(new_board);
}


int create_successor_two_final(struct Board *board) {
    int solutions = 0;
    int size = board->last;

    struct Board *new_board = (struct Board *) calloc(size + 3, sizeof(int));
    for (int col = 0; col < n; col++) {

        // set the next queen to all columns of the respective row
        append(board, col, new_board);

        if (is_valid(new_board)) 
            solutions += create_successor_final(new_board);
    }
    board_delete(new_board);
    return solutions;
}


int create_successor_final(struct Board *board) {
    int size = board->last;
    
    struct Board *new_board = (struct Board *) calloc(size + 4, sizeof(int));

    for (int col_2 = 0; col_2 < n; col_2++) {
        // set the next queen to all columns of the respective row
        append(board, col_2, new_board);

        // check if the new queen is in a valid spot, and increase solution / push node
        if (is_valid(new_board)) {
            board_delete(new_board);
            return 1;
        }
    }
    board_delete(new_board);
    return 0;
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
Methods for the Board
*/

struct Board * board_create(int col) {
    struct Board *board = (struct Board *) calloc(3, sizeof(int));
    board->pos[0] = col;
    board->last = 0;
    return board;
}


void board_delete(struct Board *board) {
    free(board);
}


void append(struct Board *board, int col, struct Board *new_board) {
    int last = board->last;

    for (int row = 0; row <= last; row++)
        new_board->pos[row] = board->pos[row];    

    new_board->pos[last + 1] = col;    
    new_board->last = last + 1;
}


/*
Methods for the Queue
*/

struct Node * node_create(struct Board **board) {
    struct Node *node = (struct Node *) malloc(sizeof(struct Board *) + sizeof(struct Node *));
    node->board = *board;
    return node;
}


void node_delete(struct Node *node) {
    board_delete(node->board);
    free(node);
}


struct Queue * queue_create() {
    struct Queue * queue = (struct Queue *) malloc(sizeof *queue);
    queue->tail = NULL;
    queue->head = NULL;
    queue->size = 0;
    omp_init_lock(&(queue->lock));
    return queue;
}


void queue_delete(struct Queue *queue) {
    omp_destroy_lock(&(queue->lock));
    free(queue);
}


void queue_push(struct Queue *queue, struct Board **board) {
    // create new node
    struct Node *node = node_create(board);
    
    omp_set_lock(&(queue->lock));
    if (queue->size == 0) {
        // empty queue; set as heads and tail
        queue->head = node;
        queue->tail = node;
    } else {
        // set as next of current last node, set as last node
        queue->tail->next = node;
        queue->tail = node;
    }
    queue->size++;
    omp_unset_lock(&(queue->lock));
}


bool queue_pop(struct Queue *queue, struct Node **node) {
    bool non_empty = false;

    omp_set_lock(&(queue->lock));
    if (queue->size > 0) {
        non_empty = true;
        // get the board of the first node, set pointer of head to second element
        queue->size--;
        *node = queue->head;
        queue->head = (*node)->next;
    }

    omp_unset_lock(&(queue->lock));
    return non_empty;
}


bool queues_get_node(struct Node **node, int *queue) {
    bool non_empty = false;

    for (int q = n_q-1; q >= 0; q--) {
        // threads with even number access only queues of even index. this should prevent memory overflow
        if (queue_pop(queues[q], node)) {
            *queue = q;
            non_empty = true;
            //printf("[%d, %d]\n", omp_get_thread_num(), q);
            break;
        }
    }

    return non_empty;
}


bool queues_are_empty() {
    for (int q = 0; q < n_q; q++)
        if (queues[q]->size > 0)
            return false;

    return true;
}

