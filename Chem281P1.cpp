#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <cstdint>
#include <string.h>
#include <omp.h>

// Timer Utility

const uint64_t ONETHOUSAND = 1000UL;
const uint64_t ONEMILLION  = 1000000UL;

class Timer281
{
public:
  Timer281()  { }
  ~Timer281() { }
  void start() { clock_gettime(CLOCK_MONOTONIC, &tstart); }
  uint64_t stop()
  {
    timespec tend;
    clock_gettime(CLOCK_MONOTONIC, &tend);
    return interval = (tend.tv_sec-tstart.tv_sec)*ONEMILLION +
      tend.tv_nsec/ONETHOUSAND - tstart.tv_nsec/ONETHOUSAND;
  }
  uint64_t query_elapsed_time() const { return interval; }
private:
  timespec tstart;
  uint64_t interval;
};

void gendata(int64_t* a, unsigned int len)
{
  unsigned int looplen = len;
  for (unsigned int k=0; k<looplen; k++)
    a[k] = rand() % 5;
  for (unsigned int k=0; k<looplen; k++)
    a[k] = rand()%2 ? -a[k] : a[k];
  return;
}

int main(int argc, char* argv[])
{
  srand(223011);

  unsigned int l = 512*512*4+3; // default size


  unsigned int number_of_threads = 0;

  for (unsigned int k=1; k<argc; )
    {
    if (strcmp(argv[k], "--threads") == 0)
      {
      if (argc<k+2)
	{
        printf("error: --threads has to be followed by a number\n");
	exit(1);
	}
      else
	omp_set_num_threads(atoi(argv[k+1]));
      k+=2;
      }
    else if (strcmp(argv[k], "--size") == 0)
      {
      if (argc<k+2)
	{
        printf("error: --size has to be followed by a number\n");
	exit(1);
	}
      else
	l = atoi(argv[k+1]);
      k+=2;
      }
    else
      {
      printf("error: %s unrecognized keyword\n", argv[k]);
      exit(1);
      }
    }


#pragma omp parallel
  {
  // if --threads is not set the number of threads is equal to the value
  // of the environment variable OMP_NUM_THREADS
  // if OMP_NUM_THREADS is not set, the number of threads is
  // implementation dependent
  number_of_threads = omp_get_num_threads();


  }

  printf ("number of threads=%d\n", number_of_threads);
  int64_t* v = new int64_t[l];
  gendata(v, l);

  // you can use s to check your parallel code
  int64_t* s = new int64_t[l];
  memcpy(s, v, l*sizeof(int64_t));

  // time the serial execution
  Timer281 timer;
  timer.start();
  for (unsigned int k=1; k<l; k++) // serial loop
    s[k] += s[k-1];

  uint64_t elapsed_serial = timer.stop();

  int64_t* locallast = new int64_t[number_of_threads+1];
  locallast[0] = 0;
  timer.start();

  // Create a parallel region. Make all the predefined variables shared
  // by default - hint: use the clause default(shared)

  #pragma omp parallel default(shared)
  {
    // partition the array into as many subarray chunks as number_of_threads
    // compute the chunk size to be assigned to each thread
    // For each threads
    // compute the beginning and end (length) of the corresponding
    // partition of the array. hint: You can use the thread number to
    // compute the corresponding chunk.

    // Caution: the length of the last chunk may be different from the
    // length of the other chunks if the size of the data is not a multiple
    // of the number of threads

    int id = omp_get_thread_num(); 
    int size= l/number_of_threads;

    // for each thread serially accumulate the entries on its subarray/partition
    // as has been done in the serial loop for s above
    // when done, store the accumulated value in the last entry of the thread's
    // into the corresponding entry of *locallast* array.

    int startid=id*size;
    int endid=startid+size;

    // The location of this local maximum in the *locallast* array is
    // the thread number plus 1

    // Next, you need to synchronize to ensure that all threads have completed
    // their task before moving to next step
    #pragma omp critical
    locallast[id+1]+=v[endid];

    // Next, create a serial region (single thread). You can use the master
    // thread to execute this region
    #pragma omp master 
    for (unsigned int k=1; k<l; k++)
    {
      v[k] += v[k-1];

    // in the serial region, accumulate all the values in *locallast* array
    }
    locallast[0]+=locallast[id+1];
    // All other threads must wait until the serial region finishes accumulating
    // the entries in *locallast* array. Synchronize again before proceeding
    // to the next step


    // Now you need each thread has to loop over its array partition
    // to add to each entry in its local subarray the value of the
    // accumulated entry in the global array v corresponding to the entry
    // in v that immediately preceeds the first entry in the subarray of
    // v that the thread is processing.



  
    // This value to add is the value in *locallast* computed previously in the
    // preceeding serial region. The corresponding entry is equal to the
    // loop index assigned to the thread by the for loop




    
  }




  uint64_t elapsed_time = timer.stop();
  printf("serial run tool %lu microsecs\n", elapsed_serial);
  printf("running on %u threads took %lu microsecs\n", number_of_threads, elapsed_time);
  printf("right answer %ld parallel answer: %ld\n", s[l-1], v[l-1]);
  delete[] locallast;
  delete[] s;
  delete[] v;
  return 0;
}

