#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Board {
    int last;   // index of the last queen that was set (len(pos)-1)
    int pos[];  // list of positions of queens
};

struct Node {
    struct Board *board;
    struct Node *next;
};

struct Queue {
    struct Node *head;
    struct Node *tail;
    int size;
};


/**
 * Method to create a board of length one with the queen at the given position.
 * @param column where the queen should be positioned
 */
struct Board * board_create(int col);


/**
 * Method to delete a board
 * 
 * @param board 
 */
void board_delete(struct Board *board);


/**
 * Method that creates a new Board from the given one and sets
 * a new queen at the given row.
 * 
 * @param board Board that should be copied
 * @param col position of new queen that should be appended
 * @return struct Board* 
 */
void append(struct Board * board, int col, struct Board *new_board);


/**
 * Method to create a new node with a given board
 * 
 * @param board of this node
 */
struct Node * node_create(struct Board **board);


/**
 * Method to delete a node and the corresponding board
 * 
 * @param node 
 */
void node_delete(struct Node *node);


/**
 * Method to create an empty queue. Returns the created queue.
 */
struct Queue * queue_create();


/**
 * Method to delete a queue
 * 
 * @param queue 
 */
void queue_delete(struct Queue *queue);


/**
 * Method to push an element on a queue. Creates a node and appends
 * it to the tail of the queue, increases size by one.
 * 
 * @param queue queue to push on
 * @param board pointer to the board that should be appended 
 */
void queue_push(struct Queue* queue, struct Board **board);


/**
 * Method to pop an element from a queue. 
 * 
 * @param queue queue to push on (in)
 * @param board pointer into which the board will be returned (out)
 * 
 * @return false, if the queue is empty
 */
bool queue_pop(struct Queue* queue, struct Node **node);

/** 
 * Method to get the next element from the last non-empty queue$
 * in an array of queues.
 * 
 * @param queues array of queues to po from
 * @param int number of queues in the array
 * @param node where to put the received node
 * @param q index of the queue from which the node was received
 * 
 * @return bool whether a non-empty queue could be found
 */
bool queues_get_node(struct Queue **queues, int n, struct Node **node, int *queue);


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

/**
 * While not all stacks are empty deque the fullest board available,
 * create all its successors and add tha valid ones to the according
 * stack.
 * 
 * @param n number of queens
 * @param queues array of queues
 * @param solutions pointer to the sum of found solutions
 */
void queens_parallel(int n, struct Queue *queues[], int *solutions);


int main(int argc, char **argv) {
    int n = argc > 1 ? atoi(argv[1]) : 3;
    
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

    // initialise queues: one for every possible size of the board, except the full one
    struct Queue **queues = (struct Queue **) malloc((n-1) * sizeof( *queues));
    for (int i = 0; i < n-1; i++) {
        queues[i] = queue_create(); 
    }

    // add all positions of the first queen to the first queue
    // TODO: parallel for loop ?
    struct Board *board;
    for (int col = 0; col < n; col++) {
        // set queen in first row to column col
        board = board_create(col);
        
        // add configuration to queue
        queue_push(queues[0], &board);
    }

    int solutions = 0;
    queens_parallel(n, queues, &solutions);

    // Free the queues, that does not work, why?
    for (int i = 0; i < n-1; i++)
        queue_delete(queues[i]);

    return solutions;
}


void queens_parallel(int n, struct Queue *queues[], int *solutions) {

    int num_threads = 1; // TODO: get actual number of threads
    int done = num_threads;  // Count done threads to prevent them from falling idle too early. as shared variable in method call

    // TODO: parallel from here
    struct Board *board;
    struct Node *node;
    int q;
    int my_solutions = 0;

    bool working = true;

    // TODO: is the thread only stopping if al threads are done?
    while (done < num_threads || working) {
        done--;

        while (queues_get_node(queues, n-1, &node, &q)) {

            board = node->board;

            printf("%d    ", q);
            for (int row = 0; row < q+1; row++) {
                printf("%d ", board->pos[row]);
            }
            printf("\n");
        
            for (int col = 0; col < n; col++) {
                struct Board *new_board = (struct Board *) calloc(q + 3, sizeof(int));

                append(board, col, new_board);
                // printf("    ", q);
                // for (int row = 0; row < q + 2; row++) {
                //     printf("%d ", new_board->pos[row]);
                // }
                // printf("\n");

                if (is_valid(new_board)) {
                    if (q >= n-2) 
                        my_solutions++;
                    else
                        queue_push(queues[q+1], &new_board);
                }
            }

            node_delete(node);
        }

        working =false;
        done++;

    } // while(done < num_threads)

    // TODO: reduction
    *solutions += my_solutions;
    // TODO: parallel till here
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
    return queue;
}

void queue_delete(struct Queue *queue) {
    free(queue);
}

void queue_push(struct Queue *queue, struct Board **board) {
    // create new node
    struct Node *node = node_create(board);
    
    // TODO: for now: lock over whole thing. Reread section about that in book, maybe there is a more efficient way
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
}

bool queue_pop(struct Queue *queue, struct Node **node) {

    // TODO: for now: lock over whole thing. Reread section about that in book, maybe there is a more efficient way
    if (queue->size > 0) {
        // get the board of the first node, set pointer of head to second element
        queue->size--;
        *node = queue->head;
        queue->head = (*node)->next;

        return true;
    }
    return false;
}

bool queues_get_node(struct Queue **queues, int n, struct Node **node, int *queue) {
    bool non_empty = false;

    for (int q = n-1; q >= 0; q--) {
        if (queue_pop(queues[q], node)) {
            *queue = q;
            non_empty = true;
            break;
        }
    }

    return non_empty;
}
