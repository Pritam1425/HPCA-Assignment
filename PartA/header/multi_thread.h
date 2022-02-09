#include <pthread.h>
int numThread = 4;
int size;
int* mA, *mB, *oput;
void * worker1( void *arg )
{
  int i, j, k, tid, portion_size, row_start, row_end;
  double sum;
  
  tid = *(int *)(arg); // get the thread ID assigned sequentially.
  portion_size = size / numThread;
  row_start = tid * portion_size;
  row_end = (tid+1) * portion_size;
  int N = size;
  for (i = row_start; i < row_end; ++i) { // hold row index of 'matrix1'
    if(i&1){
    		
    		int incr = 2*N;
    		for(int j=0;j<N/2;j++){
    			int sum = 0;
    			int odd = 1;
	    		int even = N;
    			for(int k=0;k<N;k++){			
    				if(k%2){
    					sum+=mA[i*N+k]*mB[even+incr*j];
    					even+=2;
    				}
    				else{
    					sum+=mA[i*N+k]*mB[odd+incr*j];
    					odd+=2;
    				}
    			}
    			oput[i*(N/2)+j] = sum;
    		}
    	
    		}
    		else{
    		int incr = 2*N;
    		for(int j=0;j<N/2;j++){
    			int sum = 0;
    			int odd = N+1;
	    		int even = 0;
    			for(int k=0;k<N;k++){
    				
    				if(k%2){
    					sum+=mA[i*N+k]*mB[even+incr*j];
    					even+=2;
    				}
    				else{
    					sum+=mA[i*N+k]*mB[odd+incr*j];
    					odd+=2;
    				}
    			}
    			oput[i*(N/2)+j] = sum;
    			}
    		}
  }
}
void multiThread(int N, int *matA, int *matB, int *output)
{
	
	int idx = 0;
	size = N;
	mA = matA;
	mB = matB;
	oput = output;
	pthread_t * threads2;
    	threads2 = (pthread_t *) malloc( numThread * sizeof(pthread_t) );
	int info = 0;
    	for (info=0;info<numThread;++info){
    		int *tid;
    		tid = (int *) malloc( sizeof(int) );
    		*tid = info;
    		pthread_create( &threads2[info], NULL, worker1, (void *)tid );
    	}

  	for (info = 0; info < numThread; ++info ) {
    		pthread_join( threads2[info], NULL );
  	}
}

/*
// Create other necessary functions here
int* newBev, *newAev, *newAod, *newBod,*odop,*evop;
int size;
int numThread = 512;
void * worker1( void *arg )
{
  int i, j, k, tid, portion_size, row_start, row_end;
  double sum;
  
  tid = *(int *)(arg); // get the thread ID assigned sequentially.
  portion_size = size / numThread;
  row_start = tid * portion_size;
  row_end = (tid+1) * portion_size;
  //cout<<tid<<" "<<portion_size<<"\n";
  //cout<<tid<<": \n"<<portion_size<<" "<<row_start<<" "<<row_end<<"\n";
  for (i = row_start; i < row_end; ++i) { // hold row index of 'matrix1'
    for (j = 0; j < size; ++j) { // hold column index of 'matrix2'
      sum = 0; // hold value of a cell
      /* one pass to sum the multiplications of corresponding cells
	 in the row vector and column vector. *//*
      for (k = 0; k < 2*size; ++k) { 
	sum += newAev[ i*(size*2)+k ] * newBev[ k*(size)+  j];
	//cout<<newAev[ i*(size*2)+k ]<<" "<<newBev[ k*(size)+  j ]<<" ";
      }
//      cout<<"\n"<<sum<<" "<<i<<" "<<j<<"\n";
      evop[ i*(size) + j ] = sum;
    }
  }
}
void * worker2( void *arg )
{
  int i, j, k, tid, portion_size, row_start, row_end;
  double sum;
  
  tid = *(int *)(arg); // get the thread ID assigned sequentially.
  portion_size = size / numThread;
  row_start = tid * portion_size;
  row_end = (tid+1) * portion_size;
  //cout<<tid<<": \n"<<portion_size<<" "<<row_start<<" "<<row_end<<"\n";
  for (i = row_start; i < row_end; ++i) { // hold row index of 'matrix1'
    for (j = 0; j < size; ++j) { // hold column index of 'matrix2'
      sum = 0; // hold value of a cell
      /* one pass to sum the multiplications of corresponding cells
	 in the row vector and column vector. *//*
      for (k = 0; k < 2*size; ++k) { 
	sum += newAod[ i*(size*2)+k ] * newBod[ k*(size)+  j ];
	
	//cout<<newAod[ i*(size*2)+k ] <<" "<< newBod[ k*(size)+  j ]<<"\n";
      }
      odop[ i*size+ j ] = sum;
    }
  }
}
// Fill in this function
void multiThread(int N, int *matA, int *matB, int *output)
{
    size = N/2;
    
    pthread_t * threads;
    threads = (pthread_t *) malloc( numThread * sizeof(pthread_t) );
    newBev = new int[(N/2)*N];
    
    newAev = new int[(N/2)*N];
    evop = new int[(N/2)*(N/2)];
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
    
    
    int info;
    for (info=0;info<numThread;++info){
    	int *tid;
    	tid = (int *) malloc( sizeof(int) );
    	*tid = info;
    	pthread_create( &threads[info], NULL, worker1, (void *)tid );
    }

  for (info = 0; info < numThread; ++info ) {
    pthread_join( threads[info], NULL );
  }
  delete threads;
    /*
    for(int i=0;i<N/2;i++){
    	for(int j=0;j<N/2;j++){
    		for(int k=0;k<N;k++){
    			output[idx]+=newAev[i*N+k]*newBev[k*(N/2)+j];
    		}
    		idx++;
    	}
    	idx+=(N/2);
    }*//*
    
    newAod = new int[(N/2)*N];
    newBod = new int[(N/2)*N];
    odop = new int[(N/2)*(N/2)];
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
    //for(int i=0;i<N*N/2;i++)cout<<newBod[i]<<" ";
    pthread_t * threads2;
    threads2 = (pthread_t *) malloc( numThread * sizeof(pthread_t) );
    idx = 0;
    for (info=0;info<numThread;++info){
    	int *tid;
    	tid = (int *) malloc( sizeof(int) );
    	*tid = info;
    	pthread_create( &threads2[info], NULL, worker2, (void *)tid );
    }

  for (info = 0; info < numThread; ++info ) {
    pthread_join( threads2[info], NULL );
  }
  idx = 0;
    for(int i=0;i<N;i++){
    	for(int j=0;j<N/2;j++){
    		if(i%2){
    			output[idx++] = odop[(i/2)*(N/2)+j];
    		}
    		else{
    			output[idx++] = evop[(i/2)*(N/2)+j];
    		}
    		
    	}
    }
    /*
    for(int i=0;i<N/2;i++){
    	for(int j=0;j<N/2;j++){
    		for(int k=0;k<N;k++){
    			output[idx]+=newAod[i*N+k]*newBod[k*(N/2)+j];
    		}
    		idx++;
    	}
    	idx+=(N/2);
    }*//*
    // start the threads
    
}*/
