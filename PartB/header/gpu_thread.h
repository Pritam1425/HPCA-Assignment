// Create other necessary functions here
#define BLOCK_SIZE 32
__global__ void gpu_square_matrix_mult(int *d_a, int *d_b, int *d_result, int m, int n, int k) 
{
    int row = blockIdx.y * blockDim.y + threadIdx.y; 
    int col = blockIdx.x * blockDim.x + threadIdx.x;
    int sum = 0;
    if( col < k && row < m) 
    {
        for(int i = 0; i < n; i++) 
        {
            sum += d_a[row * n + i] * d_b[i * k + col];
        }
        d_result[row * k + col] = sum;
    }
}
// Fill in this function
void gpuThread(int N, int *matA, int *matB, int *output)
{
    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    float gpu_elapsed_time_ms;
    cudaEventRecord(start, 0);
    for (int i = 0; i < N; ++i)
  	for (int j = i+1; j < N; ++j){
  		int tmp = matB[i*N+j];
    		matB[i*N+j] = matB[j*N+i];
    		matB[j*N+i] = tmp;
    		}
    int *h_a, *h_b, *h_c;
    int *newAev,*newBev,*newAod,*newBod,*evop,*odop;
    newBev = (int*)malloc(((N*N)/2)*sizeof(int));
    newAev = (int*)malloc(sizeof(int)*N*N/2);
    evop = (int*)malloc(sizeof(int)*N*N/4);
    
    
    for(int i=0;i<N;i+=2){
    	for(int j=0;j<N;j++){
    		newAev[(i/2)*N+j]=matA[i*N+j];
    	}
    }
    int idx = 0;
    for(int j=0;j<N/2;j++){
    	int incr=2*N;
    	int odd = N+1;
    	int even = 0;
    	for(int k=0;k<N;k++){
    		int temp;
    		if(k%2){
    			temp = matB[even+incr*j];
    			even+=2;
    		}
    		else{
    			temp = matB[odd+incr*j];
    			odd+=2;
    		}
    		newBev[k*(N/2)+j] = temp;
    		
    	}
    }
    cudaMalloc(&h_a, sizeof(int)*N*N/2);
    cudaMalloc(&h_b, sizeof(int)*N*N/2);
    cudaMalloc(&h_c, sizeof(int)*N*N/4);
    
    cudaMemcpy(h_a, newAev, sizeof(int)*N*N/2, cudaMemcpyHostToDevice);
    cudaMemcpy(h_b, newBev, sizeof(int)*N*N/2, cudaMemcpyHostToDevice);
    
    unsigned int grid_rows = (N/2 + BLOCK_SIZE - 1) / BLOCK_SIZE;
    unsigned int grid_cols = (N/2 + BLOCK_SIZE - 1) / BLOCK_SIZE;
    dim3 dimGrid(grid_cols, grid_rows);
    dim3 dimBlock(BLOCK_SIZE, BLOCK_SIZE);
    gpu_square_matrix_mult<<<dimGrid, dimBlock>>>(h_a, h_b, h_c, N/2,N,N/2);
    
    cudaMemcpy(evop, h_c, sizeof(int)*N*N/4, cudaMemcpyDeviceToHost);
    cudaThreadSynchronize();
    // time counting terminate
    cudaEventRecord(stop, 0);
    cudaEventSynchronize(stop);

    // compute time elapse on GPU computing
    cudaEventElapsedTime(&gpu_elapsed_time_ms, start, stop);
    printf("Time elapsed on matrix multiplication of %dx%d . %dx%d on GPU: %f ms.\n\n", N/2, N, N, N/2, gpu_elapsed_time_ms);
    
    
    for(int i=0;i<N/2;i++){
    	for(int j=0;j<N/2;j++){
    		output[idx]=evop[i*(N/2)+j];
    		idx++;
    	}
    	idx+=(N/2);
    }
    cudaFree(h_a);
    cudaFree(h_b);
    cudaFree(h_c);
    cudaEventRecord(start, 0);
    newBod = (int*)malloc(sizeof(int)*N*N/2);
    newAod = (int*)malloc(sizeof(int)*N*N/2);
    odop = (int*)malloc( sizeof(int)*N*N/4);
    
    cudaMalloc(&h_a, sizeof(int)*N*N/2);
    cudaMalloc(&h_b, sizeof(int)*N*N/2);
    cudaMalloc(&h_c, sizeof(int)*N*N/4);
    for(int i=1;i<N;i+=2){
    	for(int j=0;j<N;j++){
    		newAod[(i/2)*N+j]=matA[i*N+j];
    	}
    }
    
    for(int j=0;j<N/2;j++){
    	int incr=2*N;
    	int odd = 1;
    	int even = N;
    	for(int k=0;k<N;k++){
    		int temp;
    		if(k%2){
    			temp = matB[even+incr*j];
    			even+=2;
    		}
    		else{
    			temp = matB[odd+incr*j];
    			odd+=2;
    		}
    		newBod[k*(N/2)+j] = temp;
    	}
    }
    cudaMemcpy(h_a, newAod, sizeof(int)*N*N/2, cudaMemcpyHostToDevice);
    cudaMemcpy(h_b, newBod, sizeof(int)*N*N/2, cudaMemcpyHostToDevice);
    
    gpu_square_matrix_mult<<<dimGrid, dimBlock>>>(h_a, h_b, h_c, N/2,N,N/2);
    
    cudaMemcpy(odop, h_c, sizeof(int)*N*N/4, cudaMemcpyDeviceToHost);
    cudaThreadSynchronize();
    // time counting terminate
    cudaEventRecord(stop, 0);
    cudaEventSynchronize(stop);

    // compute time elapse on GPU computing
    cudaEventElapsedTime(&gpu_elapsed_time_ms, start, stop);
    printf("Time elapsed on matrix multiplication of %dx%d . %dx%d on GPU: %f ms.\n\n", N/2, N, N, N/2, gpu_elapsed_time_ms);

    idx = N/2;
    for(int i=0;i<N/2;i++){
    	for(int j=0;j<N/2;j++){
    		output[idx]=odop[i*(N/2)+j];
    		idx++;
    	}
    	idx+=(N/2);
    }
    cudaFree(h_a);
    cudaFree(h_b);
    cudaFree(h_c);
}
