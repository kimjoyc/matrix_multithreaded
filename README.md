# Part 1: Parallel implementation of vector accumulation
Introduction
This project focuses on applying parallel programming skills using OpenMP to parallelize the accumulation of entries in a vector. The problem involves computing the sum of entries in a vector according to the mathematical formula: xi = ∑yk for i ranging from 0 to N-1. The provided code Chem281P1.cpp implements a serial version of the accumulation algorithm using a recurrence relation. The goal is to parallelize this algorithm and evaluate its performance.

## Implementation Steps
Reorder the loops in the matmulloop function for optimal performance.
Parallelize the matmulloop function using OpenMP directives.
Evaluate the execution time of the parallel version with varying numbers of threads.
Compare the execution time of the parallel version with the serial version.
Analyze the scalability of the parallel version as the number of threads increases.
Verify that the code produces correct results for different thread counts.

## Usage
To run the parallel version of the accumulation algorithm, follow these steps:

Compile the code:

```
g++ -fopenmp Chem281P1.cpp -o chem281p1
```

Run the code with a specified number of threads:

```
bash
./chem281p1 --threads <num>
```

Replace <num> with the desired number of threads. If the --threads option is not specified, the code will use the value set in the OMP_NUM_THREADS environment variable.

## Performance Analysis
After successfully implementing the parallel version, address the following:
What is the overhead incurred by parallelization?
How does the code scale as the number of threads increases?
Is this algorithm compute-bound or memory-bandwidth bound?


# Part 2: Performance - Matrix Matrix multiplication

## Introduction
In this project, the performance of matrix multiplication algorithms will be analyzed. The goal is to study the impact of memory access patterns and programming details on algorithm performance. The provided code Chem281P2.cpp contains three different matrix multiplication algorithms that need to be implemented and evaluated.

## 1: matmuloop
The first algorithm, matmuloop, requires the reordering of loops i, j, and k for optimal performance. The code should be parallelized by identifying and parallelizing loops that do not create potential data dependency errors. A scalability study should be performed using 1, 2, 4, and 8 cores.

To run this algorithm, use the following command:

```
./Chem281P2 --loop --threads <num>
Replace <num> with the desired number of threads.
```
## 2: matmultile
The matmultile algorithm improves performance by using tiling techniques to enhance memory locality. The outer loop (partitioning the matrix into tiles) is already implemented. The task is to implement the inner loop (computing the matrix product for each tile) and parallelize it. Based on the experience from Problem 1, identify which loop(s) should be parallelized. Perform a scalability study on 1, 2, 4, and 8 processors.

To run this algorithm, use the following command:
```
./Chem281P2 --row_tile <num> --col_tile <num> --inner_tile <num> --threads <num>
```

You can set one, two, or three parameters for the tile sizes. Use power of 2 values for the <num> parameter.

## 3: matrix vector product using CBLAS
This problem involves implementing the matrix-vector product using the CBLAS routine cblas_dgemm. Compare the execution time of this routine with the execution time of the handwritten code. To run this routine, use the following command:

```
./Chem281P2 --blas
```

Usage and Compilation on Perlmutter
To compile and run the code on Perlmutter, two scripts are provided:

Source the setmsseenv.sh script:
```
bash
Copy code
source setmsseenv.sh
```
Use the buildP2 script to compile and link your code:
```
bash
./buildP2 Chem281P2.cpp
```

This will produce the executable file Chem281P2.

Compile your code on the login shell, and then get an interactive shell through the Perlmutter queuing system using the salloc command. You can run your code using srun with appropriate options.

## 4: Performance Analysis using perf
Use the perf tool to profile the different code versions. For example:
```
bash
srun -n 1 perf stat ./Chem281P2 --loop --threads 4
srun -n 1 perf stat ./Chem281P2 <best set of tiles> --threads 4
srun -n 1 perf stat ./Chem281P2 --blas
```

perf will output various statistics related to the run, including time measurements, context switches, cache misses, and branch mispredictions. Use perf to examine cache misses and analyze the performance behavior between different implementations of matrix multiplication.

## Conclusion
Upon completing the programming assignments, provide a report addressing the following topics:
Overhead incurred by parallelization
Scalability of the code with increasing thread count
Comparison of different matrix multiplication implementations
Findings from perf analysis
