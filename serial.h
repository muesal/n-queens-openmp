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
void queens_rec(int n, int q_pos[], int row, int *solutions);


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