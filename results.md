Your assignment:
 Implement the parallel version of the accumulation algorithm.
 Run the algorithm using only one thread. Check the execution time.
Compare this time to the time taken by the serial version. Explain the
dierence
 Run your code using dierent number of threads. Explain what you
see

These are the results and seems like 3 threads took the least amount of time.

running on 1 threads took 3130 microsecs

running on 2 threads took 2981 microsecs

running on 3 threads took 2899 microsecs

running on 4 threads took 3218 microsecs

running on 5 threads took 3272 microsecs

running on 6 threads took 3442 microsecs

running on 7 threads took 4025 microsecs

running on 8 threads took 3520 microsecs

 Make sure your code computes the right result regardless of the number
of threads used.
It computes the right answer regardless of thread.

After you successfully completed your programming assignment, address the
following questions

 What is the overhead incurred by parallelization?



 How does your code scale as you add more threads?
The difference in time with more threads added seemed to take an increased amount of time to complete.


 Is this algorithm compute bound or memory-bandwidth bound?
It is compute bound due to the usage of parallelization.


Use perf to explain the dierent performance behavior between your dierent
implementations of matrix multiplication. 

Write a report with your ndings


The theory behind performance stats entailing a miss in the L1 cache then leading to it being fetched from the L2 cache which has a lower latency, thus, a quite high L1 miss ratio would be acceptable. In contrast, a miss in the L2 cache on the will lead to a long stall while fetching data from main memory, so only a much lower L2 miss ratio is acceptable. 
 
Based on the performance stats of all 3 different types of matrix multiplication implementations, it seems like based on all 3 results, the tiled implementations had the best performance. This is corroborated by the level of L1 misses versus the L2 miss ratios. The L2 miss ratio seemed comparable for both the blas and loop implementations. Thus, I deduced that the tiled matrix multiplication looked like it did the best to me. Furthermore, the instruction cycles were comparable to blas and tiled where the loop implementation did the worst. However,construct the miss ratios were the most important, hence why I believe that the tiled implementation is the best performing implementation.
