//
// Created by pascal on 19.01.2020
//

#define N 4
#define M 4

#include <thrust/host_vector.h>
#include <thrust/device_vector.h>

__global__ void iterate(float *source, float *target, int width, int height)
{
    int x = blockId.x * blockDim.x + threadID.x;
    int y = blockId.y * blockDim.y + threadID.y;

    if (x < width && y < height) {
        int neighbours = 0;

        for (int dx = -1; dx <= 1; ++dx) {
            for (int dy = -1; dy <= 1; ++dy) {
                int xdx = x + dx, ydy = y + dy;

                if (dx != 0 || dy != 0) {
                    if (0 <= xdx && xdx < width && 0 <= ydy && ydy < height) {
                        int idx = getIndex(xdx, ydy, width, height);
                        neighbours += (source[idx] > 0);
                    }
                }
            }
        }

        int idx = getIndex(x, y, width, height);
        target[idx] = (neighbours == 3 || (source[idx] > 0 && neighbours == 2));
    }
}

__device__ int getIndex(int x, int y, int width, int height) {
    return y * width + x;
}

int main(int argc, char const *argv[])
{
    thrust::host_vector<int> bufferA_host(N*M, 0), bufferB_host(N*M, 0);
    thrust::device_vector<int> bufferA_device = bufferA_host, bufferB_device = bufferB_host;


    

    /* code */
    return 0;
}
