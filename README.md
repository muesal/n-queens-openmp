# n-queens-openmp
This project implements a parallel tree search for the n-queens problem, using OpenMP, to compute the total number of solutions for a given number of queens.
To compile the program run
>gcc -o parallel.o -fopenmp parallel.c

Compute the number of solutions run:
>./parallel.o [n [thread_count]]

E.g to copmute the number of solutions for 16 queens with 4 threads use one of the following:
>./parallel.o 16 4 <br>
>OMP_NUM_THREADS=4 ./parallel 16
