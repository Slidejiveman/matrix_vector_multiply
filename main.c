#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define ROWS 20
#define COLS 20
#define THREAD_COUNT 8

// Returns the element that would be located in the
// row,col index of a 2D array out of a 1D array.
int getIndex(int row, int col) {
    return row * COLS + col;
}

// Solves y = Ax with a 1D array and a 2D array using a nice
// indexing trick, which is located in the getIndex method.
//
// Reference https://stackoverflow.com/questions/1242705/performance-of-2-dimensional-array-vs-1-dimensional-array
int main()
{
    // seed pseudo-random number generator
    srand(time(NULL));

    // Declare variables
    double a[ROWS][COLS]; // 2D array for standard implementation
    double y[ROWS];
    double x[COLS];
    double a1[ROWS * COLS]; // 1D array for problem implementation
    double temp = 0.0;

    // initialize a, a1, and x
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            temp = rand() % 100 + 1;
            a[i][j] = temp;
            a1[getIndex(i,j)] = temp;
        }
        x[i] = rand() % 10 + 1;
    }

    // initialize ys and put result into them
#    pragma omp parallel for num_threads(THREAD_COUNT) \
     default(none) shared(a, x, y)
    for (int i = 0; i < ROWS; i++) {
        y[i] = 0.0;
        for (int j = 0; j < COLS; j++) {
            y[i] = a[i][j] * x[j];
        }
    }

    // print y's results
    for (int i = 0; i < ROWS; i++) {
        printf("2D Result y[%d]: %f\n", i, y[i]);
    }

    printf("\n\n");

    // now as a 1D array
#    pragma omp parallel for num_threads(THREAD_COUNT) \
     default(none) shared(a1, x, y)
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            y[i] = a1[getIndex(i,j)] * x[j];
        }
    }

    // print y's new results
    for (int i = 0; i < ROWS; i++) {
        printf("2D Result y[%d]: %f\n", i, y[i]);
    }

    return 0;
}