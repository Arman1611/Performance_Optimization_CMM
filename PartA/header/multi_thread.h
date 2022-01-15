#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <assert.h>
#include <math.h>
using namespace std;

// maximum number of threads
#define MAX_THREAD 4

int *matA;
int *matC;
int *matB;
int *output;
int N;
void* multi(void* arg)
{ 
  int i, j, k, tid, portion_size, row_start, row_end;
  tid = *(int *)arg; // get the thread ID assigned sequentially
  portion_size = N / MAX_THREAD;
  row_start = tid * portion_size;
  row_end = (tid+1) * portion_size;
  
    int rowA, colSetB, sum,indexA , indexC;	
    for(rowA = row_start; rowA < row_end; rowA++){
        for(colSetB = 0; colSetB < N; colSetB += 2){
            sum = 0;
            if(rowA & 1){    
                // handle odd rows in matA
                
                // iterate over even positions in rowA
                // and elements in matC corresponding to odd positions in column colSetB in matB
                indexA = rowA*N;
                indexC = colSetB * N + 1;
                for(; indexA < (rowA+1)*N; indexA += 2, indexC +=2)
                {
                    sum += matA[indexA] * matC[indexC];
                }
                
                // iterate over odd positions in rowA
                // and elements in matC corresponding even positions in column colSetB+1 in matB
                indexA = rowA*N+1;
		 indexC =(colSetB +1)*N;
                for(; indexA < (rowA+1)*N; indexA += 2, indexC += 2)				
                    sum += matA[indexA] * matC[indexC];
                
            
            } else {
                // handle even rows in matA

                // iterate over even positions in rowA
                // and elements in matC corresponding odd positions in column colSetB+1 in matB
                indexA = rowA*N;
                indexC = N*(colSetB+1) + 1;
                for(; indexA < (rowA+1)*N; indexA += 2, indexC += 2)
                  {
                    sum += matA[indexA] * matC[indexC];
                  }
                

                // iterate over odd positions in rowA
                // and elements in matC corresponding even positions in column colSetB in matB
                indexA = rowA*N+1;
                indexC = colSetB * N; 
                for(; indexA < (rowA+1)*N; indexA += 2, indexC += 2)
                    {
                    sum += matA[indexA] * matC[indexC];
                    }
                
            }
            
            // compute output indices
            int rowO = rowA>>1;
            int colO = (colSetB>>1) + (N>>1) * (rowA&1);
            int indexO = rowO * N + colO;
            output[indexO] = sum;
        }
    }
    return NULL;
}

void multiThread(int n, int *mat1, int *matB, int *out_put)
{

    assert(n>=4 and n == ( n&~ (n-1)));
    pthread_t  threads[MAX_THREAD];
    matC = new int[n*n];
    matA = mat1;
    output = out_put;
    N = n;
  
    for(int i = 0; i < N; ++i)
       for(int j = 0; j < N; ++j)
	   matC[i*N+j] = matB[j*N+i]; // Transposing the matB and storing it in matC.
	   
    for (int i = 0; i < MAX_THREAD; i++) {
        int* p = (int*) malloc(sizeof(int));
        *p = i;
        pthread_create(&threads[i], NULL, multi, (void*)(p)); //creating the threads 
    }

    for (int i = 0; i < MAX_THREAD; ++i ) {
        pthread_join( threads[i], NULL ); // joining the threads
    }
}
