#pragma once
#include "tbb/task_scheduler_init.h"
#include "tbb/parallel_for.h"
#include "tbb/blocked_range.h"
class sumVect
{
	float *V1, *V2;
	int N;
	float* sResult;
public:
	sumVect(float *tV1, float *tV2, int tN, float* tsResult) : sResult(tsResult), V1(tV1), V2(tV2), N(tN) {}
	void operator()(const tbb::blocked_range<int>& r) const
	{
		int begin = r.begin(), end = r.end();
		for (int i = begin; i != end; i++)
		{
			sResult[i] = V1[i] + V2[i];
		}
	}
};
