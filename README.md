# SV_assignment
Object detection ensemble + matmulti + hangarian algo

## Environment
- Ubuntu 18
- GPU: 2 x RTX 3090
- CPU: 20 x Intel(R) Core(TM) i9-10850K CPU @ 3.60GHz, 10 Cores, L2 cache: 256K

## Libraries

Detectron2 (v0.6), Commit hash code: 6886f85baee349556749680ae8c85cdba1782d8e
Cuda 11.1
Opencv (v4.5.5), Commit hash code: e4abf6e723e80133934a0d979dedb61812e9956e

## Deep Learning
- Models used: Faster RCNN, RetinaNet
- **AP: 0.405**, please check the ppt file for more details

## Hungarian Algorithm
- The algorithm can successfully return the answer when the assignment can be made after the second step.
### Things to improve
- Step 3 and step 4 still need more debugging.
- code quality: make functions

## Matrix Multiplication
Matrix Size: 2048x2048
### Cpu Naive: 
Elapsed time: 60141.561564 ms
### CPU OpenCV
- Elapsed time: 3090.394413 ms
- Gemm Level 3 BLAS
### CUDA
- Elapsed time 0.007574 ms
- To increase the "computation-to-memory ratio", the tiled matrix multiplication can be applied. One thread block computes one tile of matrix C. Each thread in the thread block computes one element of the tile.
