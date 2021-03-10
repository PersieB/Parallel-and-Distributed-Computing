/**
 * @file Transpose.c
 * @author Percy Brown
 * @brief The main purpose of this laboratory exercise is to refresh and learn the use of C and then explore,
    *learn and apply shared memory programming libraries, i.e., Pthread and OpenMP, for simple
s   *cientific applications of manipulating very large matrices that are maintained in memory.
 * @version 0.1
 * @date 2021-03-10
 * 
 * @copyright Copyright (c) 2021
 * 
 */
/* Necessary Headers */
#include <stdlib.h>
#include <time.h>
#include <stdio.h> 
#include <omp.h>
#include <pthread.h> 

/* Number of threads */
#define THREADS 4

/*N=128 or N = 1024 or N= 2048 or N = 4096*/
#define N 128
#define N1 1024  
#define N2 2048
#define N3 4096

/* Function Prototypes */

/* Helper methods */
void setMatrix(int **matrix, int size);
void printMatrix(int **matrix, int size);
int **matrixMemory(int size);
void swap(int row, int column);

/* Basic Transpose */
void basicTranspose(int **matrix, int size);

/* Diagonal-Thread using PThreads */
void createPthreads(int numThreads, void *(*next)(void *));
void *diagonalPThreads(void *id);

/* Naive - OpenMP */
void naiveOMPMT(int **matrix, int size);

/* Diagonal-Thread OpenMP */
void diagonalOMPMT(int **matrix, int size);

/**
 * @brief global matrix variable
 * 
 */
int **matrix;

/**
 * @brief Initialise the Matrix with a given size
 * 
 * @param matrix 
 * @param size 
 */
void setMatrix(int **matrix, int size){
    int row, column, element = 0;
    for(row=0; row < size; row++){
        for(column =0; column < size; column++){
            element ++;
            matrix[row][column] = element;
        }
    }
}

/**
 * @brief print Matrix elements
 * 
 * @param matrix 
 * @param size 
 */
void printMatrix(int **matrix, int size){
    int row, column;
    for(row =0; row < size; row++){
        for(column =0; column< size; column++){
            printf("%d\t", matrix[row][column]);
        }
        printf("\n");
    }
}

/**
 * @brief allocates memory to matrix given its size
 * 
 * @param size 
 * @return int** 
 */
int **matrixMemory(int size){
    int position;

    //allocate space. 
    int **matrix; 
    matrix =  malloc(size * sizeof(int*)); 
    for(position=0; position < size; position++){
        matrix[position]= malloc(size * sizeof(int));
    }

    /*Unsuccessful allocation*/
    if(matrix == NULL){
        fprintf(stderr, "Memory allocation unsuccessful, try again\n");
        exit(1);  //terminate
    }

    /*Sucess*/
    return matrix; 
}

/**
 * @brief Swaps the element in a matrix i.e matrix[i,j] and matrix[j,i]
 * 
 * @param row 
 * @param column 
 */
void swap(int row, int column){
    int temp = matrix[row][column]; 
    matrix[row][column] = matrix[column][row];
    matrix[column][row] = temp;   
}

/**
 * @brief Implements the basic transpose of a matrix
 * 
 * @param matrix 
 * @param size 
 */
void basicTranspose(int **matrix, int size){
    int row, column;
    for(row=0; row < size; row++){
        for(column = row + 1; column < size; column++){
            /* swap elements */
            swap(row, column); 
        }
    }
    
}

/**
 * @brief Create a Pthreads object and joins all threads to aid in Diagonal-Threading algorithm
 * 
 * @param numThreads 
 * @param next 
 */
void createPthreads(int numThreads, void *(*next)(void *)){
    pthread_t threads[numThreads];
    int tracker, thread;

    // create threads from 0 to numThreads - 1
    for(thread = 0; thread < numThreads; thread++){        
        tracker = pthread_create(&threads[thread], NULL, next, (void *) thread);

        //non-zero value--true   
        if(tracker){ 
            fprintf(stderr, "Sorry okay: connot create threads\n");
            exit(-1);
        }

    }
    //Join all threads
    for( thread =0; thread < numThreads; thread++){
        pthread_join(threads[thread], NULL);
    } 
}

/**
 * @brief Diagonal-Thread Algorithm Using PThreads. For each diagonal position,
the corresponding row elements to the right and the column elements below are interchanged by
the threads assigned to the position 
 * 
 * @param id 
 * @return void* 
 */
void *diagonalPThreads(void *id){
    int start, end, row, column, threadID;
    threadID = (int)id; 

    start = threadID * (N/THREADS);
    end = (threadID + 1) * (N/THREADS);

    for(row = start; row< end; row++){
        for(column= row + 1; column < N; column++){
            /* swap elements */
            swap(row, column);
        }
    }

    pthread_exit(NULL);
}

/**
 * @brief Implements transpose using OpenMP
 * 
 * @param matrix 
 * @param size 
 */
void naiveOMPMT(int **matrix, int size){
    int numThreads;
    #pragma omp parallel
    {
        int threadID, row, column;
        threadID =  omp_get_thread_num();

        /* The first or master thread performs this operation in case threadID is 0 */
        if(threadID == 0){
            numThreads = omp_get_num_threads();
        }

        #pragma omp for nowait
        for(row=0; row < size; row++){
            for(column = row + 1; column < size; column++){
                /* swap elements */
                swap(row, column);
            }
        }
    }
}

/**
 * @brief Diagonal-Threading using OpenMP
 * 
 * @param matrix 
 * @param size 
 */
void diagonalOMPMT(int **matrix, int size){

    #pragma omp parallel
    {
        int start, end, row, column, threadID, nThreads;
        nThreads = omp_get_num_threads();

        threadID =  omp_get_thread_num();

        start = threadID * (size/nThreads);
        end = (threadID + 1) * (size/nThreads);
        
        for(row=start; row< end; row++){
            for(column = row + 1; column< size; column++){
                /* swap elements */
                swap(row, column);  
            }
        }

    }/*End of parallel section.*/
}

/* Variable for matrix size N. Therefore, the size can be changed to N1 or N2 etc to run the program with the given size */
int size = N;  

/**
 * @brief Main methods tests all 3 algorithms to computer matrix transpose
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int main(int argc, char *argv[]){

    time_t timeTaken; // for clocking algorithm to evaluate performance

    /*Create matrix, initiliaze and print it.*/
    matrix = matrixMemory(size);
    setMatrix(matrix, size);
    
    /* 1. Basic Transpose */
    timeTaken =  clock();
    basicTranspose(matrix, N);
    timeTaken = clock() - timeTaken;
    printf("Time taken for Basic Transpose %f s\n", ((double)timeTaken)/CLOCKS_PER_SEC);
    basicTranspose(matrix, N); //reverse transpose to original matrix

    /*2. diagonal threading PThreads algorithm */
    timeTaken = clock();
    createPthreads(THREADS, diagonalPThreads);
    timeTaken = clock() - timeTaken;
    printf("Time taken by PThreads Diagonal Threading: %f s\n", ((double)timeTaken)/CLOCKS_PER_SEC);
    createPthreads(THREADS, diagonalPThreads); //reverse transpose to original matrix

    /*3. OpenMP naive threaded algoirthm */
    timeTaken =  clock();
    naiveOMPMT(matrix, N);
    timeTaken = clock() - timeTaken;
    printf("Time taken for Naive OpemMP Parallelism: %f s\n", ((double)timeTaken)/CLOCKS_PER_SEC);
    naiveOMPMT(matrix, N); //reverse transpose to original matrix


    /*4. diagonal threading OpenMp algorithm */
    timeTaken = clock();
    diagonalOMPMT(matrix, size);
    timeTaken = clock() - timeTaken;
    printf("Time taken by OpenMP Diagonal Threading: %f s\n", ((double)timeTaken)/CLOCKS_PER_SEC);
    diagonalOMPMT(matrix, size);  //reverse transpose to original matrix

    return (EXIT_SUCCESS);

    
}