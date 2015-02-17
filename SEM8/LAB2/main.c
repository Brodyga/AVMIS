#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define M 2048
#define N 16

int main()
{
	double execTime;
	double startTime, endTime;

	float *A = (float*)malloc(M * M * sizeof(float));
	float *B = (float*)malloc(M * M * sizeof(float));
	float *C = (float*)malloc(M * M * sizeof(float));

	for(int i = 0; i < M * M; i++)
	{
		A[i] = i + 1;
		B[i] = i + 1;
		C[i] = 0;
	}

	struct timeval start;

	gettimeofday(&start, NULL);
	startTime = (double)(start.tv_sec + start.tv_usec/1000000.0);

#ifdef BLOCK
	for (int i1 = 0; i1 < M; i1 += N)
	{
		for (int k1 = 0; k1 < M; k1 += N)
		{
			for (int j1 = 0; j1 < M; j1 += N)
			{
				for(int i = 0; i < N ; i++)
				{
					for (int k = 0 ; k < N ; k++)
					{	
						for(int j = 0; j < N ; j++)
						{
							C[i1 * M + j1 + i * M + j] += A[i1 * M + k1 + i * M + k] * B[k1 * M + j1 + k * M + j];
						}
					}
				}
			}
		}
	}
#else
	for(int i = 0; i < M ; i++)
	{
		for (int k = 0 ; k < M ; k++)
		{	
			for(int j = 0; j < M ; j++)
			{
				C[i * M + j] += A[i * M + k] * B[k * M + j];
			}
		}
	}
#endif

	gettimeofday(&start, NULL);
	endTime = (double)(start.tv_sec + start.tv_usec/1000000.0);
	execTime = endTime - startTime;

	printf("Execution time is %2.3f seconds\n", execTime);
	return 0;
}