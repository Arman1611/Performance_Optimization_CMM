#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <vector>

__global__ void multiplication(int *matA, int *matC, int* output, int N)
{
     int rowA = blockIdx.y*blockDim.y+threadIdx.y;
     int colSetB = (blockIdx.x*blockDim.x+threadIdx.x)*2;
     int sum = 0,indexA,indexC;
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
                // and elements in matC corresponding to even positions in column colSetB+1 in matB
                indexA = rowA*N+1;
		 indexC =(colSetB +1)*N;
                for(; indexA < (rowA+1)*N; indexA += 2, indexC += 2)
                   {	
                    sum += matA[indexA] * matC[indexC];
                   }
                
            
            }
     else {
                // handle even rows in matA

                // iterate over even positions in rowA
                // and elements in matC corresponding to odd positions in column colSetB+1 in matB
                indexA = rowA*N;
                indexC = N*(colSetB+1) + 1;
                for(; indexA < (rowA+1)*N; indexA += 2, indexC += 2)
                  {
                    sum += matA[indexA] * matC[indexC];
                  }
                

                // iterate over odd positions in rowA
                // and elements in matC corresponding to even positions in column colSetB in matB
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
void gpuThread(int N, int *matA, int *matB, int *output)
{
    
    assert(N>=4 and N == ( N &~ (N-1)));
    //Uncomment below code to check the time taken by the cuda program
   /**
    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    cudaEventRecord(start);
   **/
    int * matC = new int[N*N];

    for(int i = 0; i < N; ++i)
      for(int j = 0; j < N; ++j)
	   matC[i*N+j] = matB[j*N+i]; // Transposing the matB and storing it in matC.
    size_t size_input = N * N * sizeof(int);
    size_t size_output = N * (N >> 1) * sizeof(int);
    int *a,*b,*c;
    
    cudaMalloc(&a,size_input);
    cudaMalloc(&b,size_input);
    cudaMalloc(&c,size_output);
    cudaMemcpy(a, matA, size_input, cudaMemcpyHostToDevice);
    cudaMemcpy(b, matC, size_input, cudaMemcpyHostToDevice);
    
    int BLOCK_SIZE , BLOCKS_X, BLOCKS_Y;
    if(N <= 32)
    	BLOCK_SIZE = N/2;
    
    else 
    	BLOCK_SIZE = 32;
    	
    BLOCKS_Y = (int) ceil(N * 1.0 / BLOCK_SIZE);
    BLOCKS_X = (int) ceil((N>>1) * 1.0/ BLOCK_SIZE);
    
    dim3 threadsperblock(BLOCK_SIZE, BLOCK_SIZE);
    dim3 blockspergrid(BLOCKS_X, BLOCKS_Y);
    
    multiplication<<<blockspergrid,threadsperblock>>>(a,b,c,N);

    cudaMemcpy(output, c, size_output, cudaMemcpyDeviceToHost);
    
    cudaFree(a);
    cudaFree(b);
    cudaFree(c);
    //Uncomment below code to calculte the time take by the cuda program
    /**
    cudaEventRecord(stop);
    cudaEventSynchronize(stop);
    float milliseconds = 0;
    cudaEventElapsedTime(&milliseconds, start, stop);
    printf("GPU time : %f",milliseconds);
    **/
}
