
/*
Methods for the Board
*/

struct Board {
    int last;   // index of the last queen that was set (len(pos)-1)
    int pos[];  // list of positions of queens
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


/*
Methods for the Queue
*/

struct Node {
    struct Board *board;
    struct Node *next;
};


struct Queue {
    struct Node *head;
    struct Node *tail;
    int size;
    omp_lock_t lock;
};


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
 * in an array of queues. Only every second queue is accessed, the
 * first one is the one with index (n - 1 - offset)
 *
 * @param queues array of queues to po from
 * @param n number of queues in the array
 * @param node where to put the received node
 * @param q index of the queue from which the node was received
 * 
 * @return bool whether a non-empty queue could be found
 */
bool queues_get_node(struct Node **node, int *queue);


/**
 * Method to check whether all queues are empty
 * 
 * @return true if all queues are empty
 * @return false otherwise
 */
bool queues_are_empty();


/*
Methods for the parallel tree search
*/

/**
 * Method that starts the recursive tree search by setting 
 * the queen in the first row to all possible positions
 * and summing the solutions that are reachable from there.
 * 
 * @return int, the number of solutions
 */
int queens();


/**
 * While not all stacks are empty deque the fullest board available,
 * create all its successors and add tha valid ones to the according
 * stack.
 * 
 * @param n_q number of queues in the array
 * @param solutions pointer to the sum of found solutions
 */
void queens_parallel(int n_q, int *solutions);


/**
 * Create all possible and valid successors with two more queens from the given board,
 * and push them two the given queue.
 * 
 * @param q index of the queue to push to
 * @param board for which the successors should be created
 */
void create_successor_two(int q, struct Board *board);


/**
 * Return the number of valid configurations that can be reached within two aditional
 * queens from the given board.
 * 
 * @param board for which the successors should be created
 * 
 * @return number of found solutions
 */
int create_successor_two_final(struct Board *board);


/**
 * Adds one more queen, if a solution can be returned this way returns 1, else 0.
 * 
 * @param board for which the successors should be created
 * 
 * @return 1 if there is a solution to be found, 0 else
 */
int create_successor_final(struct Board *board);


/**
 * Method that checks if a newly built part-solution is valid,
 * by checking whether queen at position i violates any of
 * the given constraints (only one queen per ro, column and
 * diagonally)
 * 
 * @param board: part-solution
 * @return boolean whether the (part-)solution is valid
 */
bool is_valid(struct Board *board);

