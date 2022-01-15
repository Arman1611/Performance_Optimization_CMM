void singleThread(int N, int *matA, int *matB, int *output)
{
    // enforce N to be power of 2 and greater than 2
    assert(N>=4 and N == ( N &~ (N-1)));
	int * matC = new int[N*N];

    for(int i = 0; i < N; ++i)
      for(int j = 0; j < N; ++j)
	   matC[i*N+j] = matB[j*N+i]; // Transposing the matB and storing it in matC.
	 
    int rowA, colSetB, sum,indexA , indexC;	
    for(rowA = 0; rowA < N; rowA++){
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
                // and elements in matC corresponding to even positions in column colSetB+1 in matB
                indexA = rowA*N+1;
		 indexC =(colSetB +1)*N;
                for(; indexA < (rowA+1)*N; indexA += 2, indexC += 2)
                   {	
                    sum += matA[indexA] * matC[indexC];
                   }
                
            
            } else {
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
    }
}


