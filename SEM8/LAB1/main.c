#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pmmintrin.h>
#include <immintrin.h>

#define N 512

int main()
{
	double execTime;
	double startTime, endTime;

	float A[N * N]__attribute__((aligned(16)));
	float B[N * N]__attribute__((aligned(16)));
	float C[N * N]__attribute__((aligned(16)));

	for(int i = 0; i < N * N; i++)
	{
		A[i] = i + 1;
		B[i] = i + 1;
		C[i] = 0;
	}

	double duration = 0.0;
	struct timeval start;

	gettimeofday(&start, NULL);
	startTime = (double)(start.tv_sec + start.tv_usec/1000000.0);

#ifdef SSE2
	__m128 temp_a;
	__m128 temp_b;
	__m128 temp_c;
	__m128 temp_d;
	for(int i = 0; i < N ; i++)
	{
		for(int j = 0; j < N ; j++)
		{
			temp_d = _mm_setzero_ps();
			for (int k = 0 ; k < N ; k += 4)
			{
				temp_a = _mm_load_ps(A + i * N + k);
				temp_b = _mm_set_ps(B[(k + 3) * N + j], B[(k + 2) * N + j],	B[(k + 1) * N + j],	B[(k + 0) * N + j]);
				temp_c = _mm_mul_ps(temp_a, temp_b);
				temp_d = _mm_add_ps(temp_d, temp_c);
			}
			temp_d = _mm_hadd_ps(temp_d, temp_d);
			temp_d = _mm_hadd_ps(temp_d, temp_d);
			_mm_store_ss(C + i * N + j, temp_d);
		}
	}
#else
	for(int i = 0; i < N ; i++)
	{
		for (int k = 0 ; k < N ; k++)
		{	
			for(int j = 0; j < N ; j++)
			{
				C[i * N + j] += A[i * N + k] * B[k * N + j];
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
