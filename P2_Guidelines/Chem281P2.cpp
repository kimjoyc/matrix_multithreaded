
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <cstdint>
#include <omp.h>
#if defined USE_MKL
#define BLAS_ENABLED
#include <mkl.h>
#endif // USE_MKL
#if defined USE_LIBSCI
#define BLAS_ENABLED
#include <cblas.h>
#endif // LIBSCI


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

void gendata(double* a, uint64_t len)
{
  uint64_t looplen = len;
  double factor = (double) RAND_MAX;
  for (unsigned int k=0; k<looplen; k++)
    a[k] = (double)rand() / factor;
  for (unsigned int k=0; k<looplen; k++)
    a[k] = rand()%2 ? -a[k] : a[k];
  return;
}

void fillMatrices(double*& a, double*& b, double*& c,
		  const unsigned int rowsA, const unsigned int colsA,
		  const unsigned int colsB)
{
  a = new double[rowsA*colsA];
  gendata(a, rowsA*colsA);
  b = new double[colsA*colsB];
  gendata(b, colsA*colsB);
  c = new double[rowsA*colsB];
  gendata(c, rowsA*colsB);
  return;
}

void matmulloop(double* __restrict__ amat, double* __restrict__ bmat,
	     double* __restrict__ cmat,
             const unsigned int rowsA, const unsigned int colsA,
	     const unsigned int colsB)
{ 
  typedef double (*matrix_ptrA)[rowsA];
  typedef double (*matrix_ptrB)[colsB];
  typedef double (*matrix_ptrC)[colsB];
  matrix_ptrA a = (matrix_ptrA) amat;
  matrix_ptrB b = (matrix_ptrB) bmat;
  matrix_ptrC c = (matrix_ptrC) cmat;

  /*
  Questions:
  A developer wrote this version of matrix matrix multiplication. Their boss
  asked you to improve the performance of this code.
  Is this the optimal arrangement of the loops?
  If not, reorganize the loops for best cache performance
  Paralellize this code using OpenMP
  Which loops can be safely parallelized?
  You can parallelize one or more loops
  Which option gives the best performance
  Experiment with different number of threads. How does the code scale?
  (one thread vs 2 threads vs 4 threads);
   */
  
  for (unsigned j=0; j<colsB; j++)     // loop j
    for (unsigned i=0; i<rowsA; i++)   // loop i
      for (unsigned k=0; k<colsA; k++) // loop k
	c[i][j] += a[i][k]*b[k][j];
}


void matmultile(double* __restrict__ amat, double* __restrict__ bmat,
		double* __restrict__ cmat,
		const unsigned int rowsA, const unsigned int colsA,
		const unsigned int colsB,
		const unsigned int tilei, const unsigned int tilej,
		const unsigned int tilek)
{
  /*
  In this version of the matrix matrix multiplication we have three outer loops
  to logically break up the matrix into tiles to potentially reduce the
  number of cache misses. You are asked to write the inner version of this
  algorithm (the i, j, k loops) that operate tile by tile. Then you can
  experiment with different versions of the tiling parameters.
  Experiment with different number of threads. How does the code scale?
  (one thread vs 2 threads vs 4 threads);
   */
  typedef double (*matrix_ptrA)[rowsA];
  typedef double (*matrix_ptrB)[colsB];
  typedef double (*matrix_ptrC)[colsB];
  matrix_ptrA a = (matrix_ptrA) amat;
  matrix_ptrB b = (matrix_ptrB) bmat;
  matrix_ptrC c = (matrix_ptrC) cmat;
  const unsigned int s=tilei;
  const unsigned int p=tilej;
  const unsigned int t=tilek;
  {
    // Write the inner loop here
    // Then parallelize it using OpenMP
  }
}

enum RunMode { LOOP, TILED, BLAS };

struct P2Config
{
  P2Config() : runMode(LOOP), row_tile(4096), col_tile(4096), inner_tile(4096) {}
  RunMode      runMode;
  unsigned int row_tile;
  unsigned int col_tile;
  unsigned int inner_tile;
};


unsigned int get_token(const char* regex_text, const char* kwd)
{
  return strstr(regex_text, kwd) - regex_text;
}

P2Config parseargs(unsigned int arglen, const char* arglist[])
{
  const char* regex_text = "--tiled--row_tile--col_tile--inner_tile--loop--blas--threads";
  P2Config config;
  for (unsigned int k=0; k<arglen; k++)
    {
    unsigned int token = get_token(regex_text, arglist[k]);
    switch(token)
      {
      case 0:
	config.runMode = TILED;
	printf("runMode set to tiled\n");
	break;
      case 7:
	config.runMode = TILED;
	config.row_tile = atoi(arglist[++k]);
	printf("runMode TILED, row_tile set to %d\n", config.row_tile);
	break;
      case 17:
	config.runMode = TILED;
	config.col_tile = atoi(arglist[++k]);
	printf("runMode TILED, col_tile set to %d\n", config.col_tile);
	break;
      case 27:
	config.runMode = TILED;
	config.inner_tile = atoi(arglist[++k]);
	printf("runMode TILED, inner_tile set to %d\n", config.inner_tile);
	break;
      case 39:
	config.runMode = LOOP;
	printf("runMode set to loop\n");
	break;
      case 45:
	config.runMode = BLAS;
	printf("runMode set to blas\n");
	break;
      case 51:
	{
	unsigned int num_threads = atoi(arglist[++k]);
	omp_set_num_threads(num_threads);
	printf("number of threads set to %d\n", num_threads);
	}
	break;
      default:
	printf("invalid parameter %s\n", arglist[k]);
	printf("chem281p2 --loop|--tiled|--blas --row_tile <num> --col_tile <num> --inner_tile <num> --threads <num> \n");
	exit(1);
      }
    }
  return config;
}

int main(int argc, const char* argv[])
{
  srand(223011);
  P2Config config = parseargs(argc-1, argv+1);
  unsigned int rowsA=4096;
  unsigned int colsA=4096;
  unsigned int colsB=4096;

  double* a = NULL;
  double* b = NULL;
  double* c = NULL;

  Timer281 t;
  fillMatrices(a, b, c, rowsA, colsA, colsB);
  
#pragma omp parallel
  {
#pragma omp single
  {
  printf ("number of threads=%d\n", omp_get_num_threads());
  }
  }
  
  if (config.runMode==LOOP)
    {
    t.start();
    matmulloop(a, b , c, rowsA, colsA, colsB);
    uint64_t loop_time = t.stop();

    printf("Time loop %lu\n", loop_time);
    }
  if (config.runMode==TILED)
    {
    t.start();
    matmultile(a, b , c, rowsA, colsA, colsB, config.row_tile,
	       config.col_tile, config.inner_tile);
    uint64_t slice_time = t.stop();

    printf("Time slice %lu\n", slice_time);
    }
#if defined BLAS_ENABLED
  if (config.runMode==BLAS)
    {
    t.start();
    /*
    Here you are asked to use a expertly written matrix matrix routine.
    This routine computes a generalized version of matrix matrix multiplication
     C = alpha op(A) op(B) + beta C
     where alpha and beta are scalars and op() stands for transposed or not
     transposed.
     The parameters for this function are:
     First parameter: matrix ordering, posible options:
         CblasRowMajor, CbalsColMajor
     Second parameter: op applied to the first matrix, matrix A. Options
        CblasTrans (transposed), CbasNoTrans (not transposed)
     Third parameter: op applied to the second matrix, matrix B. Options
        CblasTrans (transposed), CbasNoTrans (not transposed)
     Fourth parameter: the number of rows in matrix A
     Fifth parameter:  the length of the inner loop (cols in A if NoTrans)
     Sixth parameter:  the number of Columns in C
     Seventh parameter: the constant alpha (double)
     Eight parameter: the matrix A
     Nineth parameter: the leading dimension of A
      The matrices A, B and C may have a leading dimension greater than the 
      number of columns specified for the multiplication
     Tenth parameter: the leading dimension of matrix B
     Eleventh parameter: the constant beta (double)
     Twelve parameter: the leading dimension of matrix C
     */
    cblas_dgemm( , , , , , ,
	         , , , , , , , );
    uint64_t blas_time = t.stop();

    printf("Time blas %lu\n", blas_time);
    }
#endif // BLAS_ENABLED
  return 0;
}
