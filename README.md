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

## Final Overall Results 

To evaluate the performance of the accumulation algorithm, we ran the code using different numbers of threads and compared the execution times. The results are as follows:

Running on 1 thread took 3130 microseconds.
Running on 2 threads took 2981 microseconds.
Running on 3 threads took 2899 microseconds.
Running on 4 threads took 3218 microseconds.
Running on 5 threads took 3272 microseconds.
Running on 6 threads took 3442 microseconds.
Running on 7 threads took 4025 microseconds.
Running on 8 threads took 3520 microseconds.
From the results, it can be observed that running the algorithm on 3 threads resulted in the shortest execution time compared to other thread counts.

### Scalability
As the number of threads increased, the execution time generally increased as well. This indicates that the code's scalability is not optimal, as adding more threads did not result in a proportional reduction in execution time. The overhead incurred by parallelization and potential contention for shared resources may have contributed to this behavior.

### Algorithm Characteristics
Based on the observations, the algorithm appears to be compute-bound rather than memory-bandwidth bound. This is evident from the increasing execution time as more threads are added. The parallelization aims to distribute the computational workload across multiple threads, and the limiting factor becomes the compute resources available.

### Matrix Multiplication Implementations
To analyze the performance behavior of different matrix multiplication implementations, we utilized the perf tool to measure cache misses and other performance statistics. We compared three implementations: matmuloop, tiled, and cblas_dgemm.

### Performance Findings
After examining the performance statistics, including L1 and L2 cache miss ratios, we observed the following:

The tiled implementation showed the best performance among the three implementations. It demonstrated lower L2 miss ratios, indicating better utilization of cache memory.
Both the matmuloop and cblas_dgemm implementations had comparable L2 miss ratios, suggesting similar cache efficiency.
The instruction cycles were also comparable between cblas_dgemm and the tiled implementation, while the matmuloop implementation performed the worst in this regard.
Based on these findings, we conclude that the tiled matrix multiplication implementation exhibits the best performance characteristics. It effectively leverages cache memory, resulting in fewer cache misses and improved execution time. However, further analysis is required to understand the impact of different implementations on overall system performance and resource utilization.

## Conclusion
Upon completing the programming assignments, provide a report addressing the following topics:
Overhead incurred by parallelization
Scalability of the code with increasing thread count
Comparison of different matrix multiplication implementations
Findings from perf analysis

In conclusion, the accumulation algorithm demonstrated increased execution time with the addition of more threads, indicating suboptimal scalability. The algorithm appeared to be compute-bound due to parallelization.

Among the matrix multiplication implementations, the tiled implementation outperformed the others in terms of cache efficiency and execution time. It showed lower cache miss ratios and comparable instruction cycles, suggesting better overall performance. However, further analysis and benchmarking may be necessary to assess the impact on the overall system and optimize performance further.


