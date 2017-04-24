#pragma once
#include "tbb/task_scheduler_init.h"
#include "tbb/parallel_for.h"
#include "tbb/blocked_range.h"
class vectXmatr
{
	float* sResult;
	float k;
	float *Matrix;
	float *Vector;
	int N;
public:
	vectXmatr(float tk, float *tMatrix, float *tVector, int tN, float* tsResult) : sResult(tsResult), k(tk), Matrix(tMatrix), Vector(tVector), N(tN) {}
	void operator()(const tbb::blocked_range<int>& r) const
	{
		float tempSum = 0;
		int begin = r.begin(), end = r.end();
		for (int i = begin; i != end; i++)
		{
			tempSum = 0;
			for (int j = 0; j < N; j++)
			{
				tempSum += k * Matrix[i*N + j] * Vector[j];
			}
			sResult[i] = tempSum;
		}
	}
};

