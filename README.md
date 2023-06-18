# matrix_multithreaded

Project 1: Parallel implementation of vector accumulation
Provided le: Chem281P1.cpp:
In this project you will have an opportunity to apply the parallel pro-
gramming skills you developed during our live discussions. You are asked to
use OpenMP to parallelize the often-used accumulation of the entries in a
vector. Mathematically, the problem is dened as
xi =
i∑
k=0
yk
This problem can be implemented in C++ using the well-known recurrence
relation
#define N <num>
int y[N];
for (unsigned int i=1; i<N; i++)
y[i] += y[i-1];
The computation of any entry of y depends on the computation of all the
previous elements of the array. This data dependency prevents parallelizing
this algorithm.
One possible solution is to use a divide-and-conquer approach. We will
logically split this array into as many disconnected chunks as threads we are
using. We can compute the beginning of each chunk (partition) using the
thread number of the executing thread. Now, we can can apply the serial
code independently on each chunk. However, the computation is not nished
yet. Now, we have to add to each chunk the accumulated value of the last
entry in the preceeding chunk.
This requirement adds another serialization dependency. However, we
can circumvent this dependecy by using an auxiliar array of size equal to
number_of _threads + 1. Each thread will store the maximum value of the
accumulated sum in its corresponding array in the entry thread_number +1
of the auxiliar array. The 0-th entry in this auxiliar array has previously been
set to 0. Now, we need to choose one thread to accumulate all the entries in
this array. But this array is very small, its length is number_of _threads+1.
However, before performing this computation we must ensure that all the

threads have nished the previous task. We can use an barrier for this pur-
pose.
When the single thread nished updating the small array, now every
thread can read their corresponding values from this array and add it to all
the entries in their own partition. Again, we need a barrier before this task to
ensure that the single thread updating the small array has completed its task.
Your assignment:
 Implement the parallel version of the accumulation algorithm.
 Run the algorithm using only one thread. Check the execution time.
Compare this time to the time taken by the serial version. Explain the
dierence
 Run your code using dierent number of threads. Explain what you
see
 Make sure your code computes the right result regardless of the number
of threads used.
After you successfully completed your programming assignment, address the
following questions
 What is the overhead incurred by parallelization?
 How does your code scale as you add more threads?
 Is this algorithm compute bound or memory-bandwidth bound?
The le Chem281P1.cpp provides a number of auxiliary functions to
help you implement this project. It handles the timing, argument parsing,
result comparison, data generation, etc. It also provide suggestions to help
you implement your code.



Project 1: Performance. Matrix Matrix multiplication.
Provided le: Chem281P2.cpp:
In this project you will analyze how memory access and other progam-
ming details aect the performance of our algorithms. We will also use perf
to do some rudimentary performance analysis. It is recommended that you
run your program on Perlmutter. Instruction on how to use Perlmutter will
be provided on another writeup. The matrix multiplication problem can be
stated as computing each entry cij of an M-row, P-column matrix C as the
product between an M-row, N-column matrix A and an N-row, P-column
matrix B. Each entry is computed using the formula below.
cij =
N∑
k=0
aik ∗ bkj
We have to implement 3 dierent algorithms to perform this computation.
Problem 1:
In the rst one, matmuloop, you will loop over the three indeces i, j, k
as shown in the matmulloop code in the le Chem281P2.cpp. The sequenc-
ing of these loops may or may not be optimal. You should reorder these
loops for maximum performance. After that you will parallelize this routine.
Which loops can be parallelized without creating potential data-dependency
errors? Also, you can parallelize one or more loops. Which option gives the
best performance. Perform a scalability study using 1, 2, 4, and 8 cores (you
can use Perlmutter) How does this code scale?
To invoke this routine, type
./Chem281P2 --loop --threads <num>
Note: if threads is not specied, the number of threads used is taken
from the environment variable OMP_NUM_THREADS. You can change
the value of this environment variable by typing (bash shell)
setenv OMP_NUM_THREADS=<num>
For reference, my implementation of this problem runs on about 21 sec-
onds on 4 Perlmutter cores.

Problem 2:
One way to improve the performance of this algorithm is to tile the
matrices to improve memory locallity and better use of caches. The code
in matmultile implements the outer loop, that is the partitioning of the
matrix into tiles. You task is to implement the inner loop, that is the com-
putation of the matrix product for each tile.
You can run matmultile by typing
./Chem281P2 --row_tile <num> --col_tile <num> --inner_tile <num> --threads<num>
You can set one, two, or three parameters here. The code uses default values
for those parameters that have not been specied. If you just want to use
the default values, invoke the code as
./Chem281P2 --tiled
To keep the code simple only use power of 2 values for the <num> pa-
rameter, that is 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048.
You task is to write the inner loop and parallelize this routine. Based
on your experience in Problem 1, which loop/loops should you parallelize.
Finally, perform a scalability study on one, two, 4, and 8 processors. How
does the code scale?
For reference: my implementation runs on about 5.12 seconds of elapsed
time on 4 cores.
Problem 3:
Implement the matrix vector product using the Cblas routine cblas_dgemm.
Instructions on how to use this routine are in the source le Chem21P2.cpp.
Compare the execution time for this professionally written routine with the
execution time of your handwritten code. You can invoke this routine as
./Chem281P2 --blas
For reference: with cblas_dgemm the multiplication took 3.71 seconds of
elapsed time on 1 cores.

Note: Two scripts are provided to help you compile/run your code on
Perlmutter. The rst script setmsseenv.sh has to be sourced:
source setmsseenv.sh
The second one, buildP2 can be used to compile and link your code.
./buildP2 Chem281P2.cpp
will produce the executable le Chem281P2.
You can compile your code on the login shell. To run your code you have
to get an interactive shell through Perlmutter queueing system. I would rec-
ommend to use the command:
salloc -A mxxxx -N 1 -q interactive -C cpu -t 30 (max 240)
The parameter -t species the number of minutes (in this example, 30). Re-
place mxxxxx by m<your account id>. You can get your account id by login
to iris.nerc.gov.
It may take a short time to get your interactive shell. After that you can
run your code using srun.
srun -n 1 ./Chem281P2 <options>
Problem 4: Performance Analysis
use perf to prole your dierent code versions:
srun -n 1 perf stat ./Chem281P2 --loop --threads 4
srun -n 1 perf stat ./Chem281P2 <best set of tiles> --threads 4
srun -n 1 perf stat ./Chem281P2 --blas
where best set of tiles if the best set of tiled parameters that you found in
Problem 2. perf will output a number of statistics for your run. Here is a
summary(not all of them) of the measurements of perf stat
task-clock:u #aggregated time in millisecs used/
context-switches:u
page-faults:u
cycles:u
instructions:u # with instruction retired per cycle. Higher is better
branch-misses:u # branch mispredictions.
seconds time elapsed (running time)

You can use perf to examine cache missess. For that, you can use the fol-
lowing perf options
perf -e mem\_load_retired.l1\_miss,mem\_load_retired.l1\_hit <program>
perf -e mem_load_retired_l2_hit,mem_load_retired.l2_miss <program>
Use perf to explain the dierent performance behavior between your dierent
implementations of matrix multiplication. Write a report with your ndings.
