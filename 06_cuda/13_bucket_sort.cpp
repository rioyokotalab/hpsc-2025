#include <cstdio>
#include <cstdlib>
#include <vector>

//homework 
__global__ void count_kernel(const int *key, int *bucket, int n){
    int i = threadIdx.x + blockIdx.x * blockDim.x;
    if (i < n) atomicAdd(&bucket[key[i]], 1);
}

int main() {
  int n = 50;
  int range = 5;
  std::vector<int> key(n);
  for (int i=0; i<n; i++) {
    key[i] = rand() % range;
    printf("%d ",key[i]);
  }
  printf("\n");

  //homework
  int *d_key, *d_bucket;
  cudaMalloc(&d_key,     n * sizeof(int));
  cudaMalloc(&d_bucket,  range * sizeof(int));
  cudaMemcpy(d_key, h_key.data(), n * sizeof(int), cudaMemcpyHostToDevice);
  cudaMemset(d_bucket, 0, range * sizeof(int));
    
  dim3 block(128); 
  dim3 grid((n + block.x - 1) / block.x);
  count_kernel<<<grid, block>>>(d_key, d_bucket, n);
  cudaDeviceSynchronize();

  int h_bucket[range];
  cudaMemcpy(h_bucket, d_bucket, range * sizeof(int), cudaMemcpyDeviceToHost);

  int j = 0;
  for (int i = 0; i < range; ++i)
      while (h_bucket[i]--) h_key[j++] = i;

  
  for (int i=0; i<n; i++) {
    printf("%d ",key[i]);
  }
  printf("\n");
}
