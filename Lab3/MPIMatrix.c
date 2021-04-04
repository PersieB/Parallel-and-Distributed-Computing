/*
*  School: Ashesi University 
*  Course: CS434: Introduction to Parallel & Distribted Computing 
*  Topic: Parallel Programming with Message Passing Interface (MPI)
*  Lab 3
*  Author: Percy Brown 
*/

#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define dim 4     /* dimensions of the matrixes */
#define master 0            
#define master_message 1    // Tag that will be put on the messages from the master process carrying parts of the matrices   
#define slave_message 2        // Tag that will be put on the messages from the worker processes carrying results.


// Useful functions. The comments for each function can be found in the function definitions below.
int **allocarray(int r, int c);
int **matrixMemory(int size);
int ** slice_cols(int **A, int dimension, int start, int end);
int ** create_matrix(int size);
void print_matrix(int **mat, int dimension, char name);
int ** multiply(int **A, int **B, int a_rows, int a_cols, int b_rows, int b_cols);
int ** slice_rows(int **A, int dimension, int start, int end);
int * matrix_to_vec(int **mat, int rows, int cols);
int * matrix_to_vec_col(int **mat, int rows, int cols);
int **mat_to_array(int *vec, int l, int rows, int cols);
void printvector(int *vec, int l);
int * allocvector(int vec_size);
int ** create_initialize(int dimension, int value);

// The struct that will tell the master processor at which index to start placing the code it recieved
struct start_location {
    int x_start;
    int y_start;
};


/*
    Creates a matrix dynammically. The matrices are created in a manner that ensures that the biggest value in a matrix is always 100.
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

int ** create_matrix(int size){
    int **matrix = matrixMemory(size);
    int row, column, element = 0;
    for(row=0; row < size; row++){
        for(column =0; column < size; column++){
            element ++;
            matrix[row][column] = element;
        }
    }
    return matrix;
}



/*
    Function to print a matrix, alongside its letter name.
*/
void print_matrix(int **mat, int dimension, char name){
    printf("Matrix %c:\n", name);
    for (int i=0; i<dimension; i++)
    {
        for (int j=0; j<dimension; j++) 
        printf("%5d   ", mat[i][j]);
    printf("\n");
    }
}

/*
    Allocates memory for an array. Could also work for two-dimensional arrays that are not square matrices. 
    The user has to specify the rows and the columns.
*/
int **allocarray(int r, int c) {
    int **temp = (int **) malloc(r * sizeof(int *));
    for (int i = 0; i < r; i++) {
        temp[i] = (int *) malloc(c * sizeof(int));
    }
    return temp;
}

/*
    This function takes in a matrix, and indices for columns, and returns a slice of the matrix.
    The indices are used in an inclusive manner. In other words, this function returns a continuous set of columns.
*/
int ** slice_cols(int **A, int dimension, int start, int end){
    // Creating the matrix to be returned.
    int size = end - start + 1;
    int **temp = (int **) malloc(dimension * sizeof(int *));
    for (int i = 0; i < dimension; i++) {
        temp[i] = (int *) malloc(size * sizeof(int));
    }
    // Copying the elements from the matrix to temp
    for(int i = start; i <= end; i++){
        for(int j = 0; j < dimension; j++){
            temp[j][i - start] = A[j][i];
        }
    }
    return temp;
}

/* This function multiplies two n-square matrices and returns a matrix containing the result. 
*  The assumption is that the dimensions of the function are already correct
*/
int ** multiply(int **A, int **B, int a_rows, int a_cols, int b_rows, int b_cols){
    // Creating the matrix C
    int **C = (int **) malloc(a_rows * sizeof(int *));
    
    for (int i = 0; i < a_rows; i++) {
        C[i] = (int *) malloc(b_cols * sizeof(int));
    }

    // Carrying out the multiplication
    int i, j, k;
    for(i = 0; i < a_rows; i++){
        for(j = 0; j < b_cols; j++){
            C[i][j] = 0;
            for(k = 0; k < a_cols; k++){
                C[i][j] = C[i][j] + (A[i][k] * B[k][j]);
            }
        }
    }
    return C;
}

/* This function takes in a matrix and some row indices and returns a slice of columns. 
    The indices are used in an inclusive manner
*/
int ** slice_rows(int **A, int dimension, int start, int end){
    // Creating the matrix to be returned.
    int size = end - start + 1;
    int **temp = (int **) malloc(size * sizeof(int *));
    for (int i = 0; i < size; i++) {
        temp[i] = (int *) malloc(dimension * sizeof(int));
    }
    // Copying the elements from the matrix to temp
    for(int i = start; i <= end; i++){
        for(int j = 0; j < dimension; j++){
            temp[i - start][j] = A[i][j];
        }
    }
    return temp;
}

/* This function prints a vector. Used mainly for debugging purposes */
void printvector(int *vec, int l){
    for(int i = 0; i < l; i++){
        printf("%d ", vec[i]);
    }
    printf("\n");
}

/* This function takes a matrix and linearizes it, returning a vector that could later on be transformed into a matrix */
int * matrix_to_vec(int **mat, int rows, int cols){
    int *temp = (int *)malloc(rows * cols * sizeof(int));
    int k = 0;
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            temp[k] = mat[i][j];
            k++;
        }
    }
    return temp;
}

/* Because columns are often going to be different, we need a special function to linearize a matrix in a way suitable for columns */
int * matrix_to_vec_col(int **mat, int rows, int cols){
    int *temp = (int *)malloc(rows * cols * sizeof(int));
    int k = 0;
    for(int i = 0; i < cols; i++){
        for(int j = 0; j < rows; j++){
            temp[k] = mat[j][i];
            k++;
        }
    }
    return temp;
}

/* This function takes a matrix and returns an array. Becuase we linearized columns already in a way that standardizes them, we do not 
*   to build a unique function for columns.
*/
int **mat_to_array(int *vec, int l, int rows, int cols){
    int **temp = (int **) malloc(rows * sizeof(int *));
    for (int i = 0; i < rows; i++) {
        temp[i] = (int *) malloc(cols * sizeof(int));
    }
    for(int i = 0; i < l; i++){
        int r = i / rows;
        int c = i % rows;
        temp[c][r] = vec[i];
    }
    return temp;
}

int ** create_initialize(int dimension, int value){
    // creating the array
    int **matrix = (int **) malloc(dimension * sizeof(int *));
    
    for (int i = 0; i < dimension; i++) {
        matrix[i] = (int *) malloc(dimension * sizeof(int));
    }
    for(int i = 0; i < dimension; i++){
        for(int j = 0; j < dimension; j++){
            matrix[i][j] = value;
        }

    }
    return matrix;
}

/* This function allocates space for a vector dynamically*/
int * allocvector(int vec_size){
    int *temp = (int *)malloc(vec_size * sizeof(int));
    return temp;
}

void matrixMultiplicationNaive(int **matrix_a,int **matrix_b,int **matrix_c, int size){
    int i,j,k;
    for(i = 0; i < size; i++){
        for(j= 0; j < size; j++){
            for(k = 0; k < size; k++){
                matrix_c[i][j] += matrix_a[i][k] * matrix_b[k][j];
            }
        }
    }
}

int main (int argc, char *argv[]){

    int	numprocs, my_rank, numworkers, source, dest, message_type, rows;   // Declaring some useful variables
    int n = dim;
    int	i, j, k, rc; 
    int	**a, **b, **c;  
    MPI_Status status;

    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);

    // Creating our special data type.
    MPI_Datatype position_type;
    int lengths[2] = { 1, 1};
    const MPI_Aint displacements[2] = { 0, sizeof(int)};
    MPI_Datatype types[2] = { MPI_INT, MPI_INT};
    MPI_Type_create_struct(2, lengths, displacements, types, &position_type);
    MPI_Type_commit(&position_type);

    double root_p = sqrt(numprocs-1);
    if(fmod(dim, root_p) !=0){
        if (my_rank==0) printf("Matrix size not divisible by square root of number of processors (P - 1) \n");
        MPI_Finalize();
        exit(-1);
    }

    numworkers = numprocs - 1;
    int block_size = n / (int)root_p; // The sizes of the blocks that will be sent
    int buffer_size = block_size * n; // The size of the buffer that will be sent


   // The code for the master process.
   if (my_rank == master)
   {
      printf("mpi_mm has started with %d tasks.\n",numprocs);
      // Generating and printing matrix a
      a = create_matrix(n);
      print_matrix(a, n, 'A');
      // Generating and printing matrix b  
      b = create_matrix(n);
      print_matrix(b, n, 'B');
      
      //Generating C
      c = create_initialize(n, 0);
      double begin = MPI_Wtime();
      matrixMultiplicationNaive(a, b, c, n);
      double end = MPI_Wtime();
      printf("Done in %f seconds for sequential code.\n", end - begin);


      /* Measure start time */
      double start = MPI_Wtime();

      /* Send matrix data to the worker tasks */
      message_type = master_message;
      for (dest=1; dest<=numworkers; dest++){

        int row = ((dest - 1) / (int)root_p) * block_size;
        int col = ((dest - 1) % (int)root_p) * block_size;
        
        // Sending the rows of matrix A as needed.
        int **subRows = slice_rows(a, n, row, row + block_size - 1);
        //printf("Sending to process, %d\n", dest);
        //print_nonsquare(subRows, block_size, n);
        int *subRowsFinal = matrix_to_vec(subRows, block_size, n);
        MPI_Send(&(subRowsFinal[0]), buffer_size, MPI_INT, dest, 10, MPI_COMM_WORLD);
        
        //Sending the columns of matrix B as needed.
        int **subCols = slice_cols(b, n, col, col + block_size - 1);
        //printf("Sending to process, %d\n", dest);
        //print_nonsquare(subCols, n, block_size);
        int *subColsFinal = matrix_to_vec_col(subCols, n, block_size);
        MPI_Send(&(subColsFinal[0]), buffer_size, MPI_INT, dest, 4, MPI_COMM_WORLD);
      }


      /* Receive results from worker tasks */
      message_type = slave_message;
      for (i=1; i<=numworkers; i++){
         int incoming = i;
         int *y = allocvector(block_size * block_size); 
         MPI_Recv(&(y[0]), block_size * block_size, MPI_INT, incoming, 5, MPI_COMM_WORLD, &status);
         struct start_location p_start;
         MPI_Recv(&p_start, 1, position_type, incoming, 15, MPI_COMM_WORLD, &status);
         printvector(y, block_size * block_size);
         printf("From process %d: Which starts at row index %d and column index %d.\n", i, p_start.x_start, p_start.y_start);

         //Putting the various answers in their appropriate positions in matrix C.
         int **mini_res = mat_to_array(y, block_size * block_size, block_size, block_size);
         for(int m = 0; m < block_size; m++){
             for(int n = 0; n < block_size; n++){
                 c[p_start.x_start + m][p_start.y_start + n] = mini_res[m][n];
             }
         }
      }


      /* Measure finish time */
      double finish = MPI_Wtime();
      printf("Done in %f seconds.\n", finish - start);
   }


   // The code for the various worker processes. 
   if (my_rank > master)
   {
      message_type = master_message;

      // Calculating the starting indices of the final result.
      int rowp = ((my_rank - 1) / (int)root_p) * block_size;
      int colp = ((my_rank - 1) % (int)root_p) * block_size;

      // Recive the rows in a vector format, and then converting them back to a normal array to ease multiplication
      int *subRowsFinal = allocvector(buffer_size);
      MPI_Recv(&(subRowsFinal[0]), buffer_size, MPI_INT, master, 10, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      int **subRows = mat_to_array(subRowsFinal, buffer_size, block_size, n);

      // Revieve the cols in vector format, and then converting them back to a normal array form to ease multiplication
      int *subColsFinal = allocvector(buffer_size);;
      MPI_Recv(&(subColsFinal[0]), buffer_size, MPI_INT, master, 4, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      int **subCols = mat_to_array(subColsFinal, buffer_size, n, block_size);

      // multiplying the answers gotten from the rows and cols and linearizing for sending to the master
      int **tempAns = multiply(subRows, subCols, block_size, n, n, block_size);
      int *finRes = matrix_to_vec(tempAns, block_size, block_size);

      // Sending the temp answer back to the master
      MPI_Send(&(finRes[0]), block_size * block_size, MPI_INT, master, 5, MPI_COMM_WORLD);

      // The process also has to send information about the index at which it's answers will be placed in the final matrix.
      struct start_location buffer;
      buffer.x_start = rowp;
      buffer.y_start = colp;
      MPI_Send(&buffer, 1, position_type, master, 15, MPI_COMM_WORLD);
   }
   MPI_Finalize();
   return 0;
}
