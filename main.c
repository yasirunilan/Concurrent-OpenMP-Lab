#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

int N;

int main() {
    printf("Enter the value N:");   //get the value for N as a keyboard input
    scanf("%d", &N);

    srand( (unsigned)time( NULL ) );

    double **matA;
    double **matB;
    double **matResult;
    double **matResultparallel;

    matA = (double **)malloc(N*sizeof(double*));
    for(int j=0;j<N;j++)
        matA[j]=(double*)malloc(N*sizeof(double));

    matB = (double **)malloc(N*sizeof(double*));
    for(int j=0;j<N;j++)
        matB[j]=(double*)malloc(N*sizeof(double));

    matResult = (double **)malloc(N*sizeof(double*));
    for(int j=0;j<N;j++)
        matResult[j]=(double*)malloc(N*sizeof(double));

    matResultparallel = (double **)malloc(N*sizeof(double*));
    for(int j=0;j<N;j++)
        matResultparallel[j]=(double*)malloc(N*sizeof(double));

    for (int x = 0; x < N; x++){    //populate the two matrices of size N*N with random numbers
        for (int y = 0; y < N; y++){
            matA[x][y] = rand();
            matB[x][y] = rand();
        }
    }


    clock_t start_parallel = clock();    //start measuring time before multiplication
    multiplyParallel(matA, matB, matResultparallel);    //multiply the two matrices
    clock_t end_parallel = clock();  //end measuring the time
    float seconds_parallel = (float)(end_parallel - start_parallel) / CLOCKS_PER_SEC;
    printf("\n%f", seconds_parallel);

//    display(matResultparallel);

    clock_t start_serial = clock();    //start measuring time before multiplication
    multiply(matA, matB, matResult);    //multiply the two matrices
    clock_t end_serial = clock();  //end measuring the time
    float seconds_serial = (float)(end_serial - start_serial) / CLOCKS_PER_SEC;
    printf("\n%f", seconds_serial);

//    display(matResult);

    return 0;
}


//parallelized multiplication
void multiplyParallel(double **A, double **B, double **Result){
    int i,j,k;
#pragma omp parallel shared(A,B,Result) private(i,j,k)
    {
#pragma omp for  schedule(static)
        for (i=0; i<N; i=i+1){
            for (j=0; j<N; j=j+1){
                Result[i][j]=0.;
                for (k=0; k<N; k=k+1){
                    Result[i][j]=(Result[i][j])+((A[i][k])*(B[k][j]));
                }
            }
        }
    }
}

//serial multiplication
void multiply(double **A, double **B, double **Result)
{
    int i,j,k;

    for (i=0; i<N; i++)
    {
        for (j=0; j<N; j++)
        {
            Result[i][j] = 0;
            for (k = 0; k < N; k++) {
                Result[i][j] += A[i][k]*B[k][j];
            }
        }
    }
}

//function to display the result matrix
void display(double **mat){
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            printf("%lf\t", mat[i][j]);
        }
        printf("\n");
    }
}