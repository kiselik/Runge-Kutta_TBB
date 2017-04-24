#pragma once
#include "tbb/task_scheduler_init.h"
#include "tbb/parallel_for.h"
#include "tbb/blocked_range.h"
class sum4Vect
{
	float *V1, *V2, *V3, *V4;
	int N;
	float* sResult;
public:
	sum4Vect(float *tV1, float *tV2, float *tV3, float *tV4, int tN, float* tsResult) : sResult(tsResult), V1(tV1), V2(tV2), V3(tV3), V4(tV4), N(tN) {}
	void operator()(const tbb::blocked_range<int>& r) const
	{
		int begin = r.begin(), end = r.end();
		for (int i = begin; i != end; i++)
		{
			sResult[i] = (V1[i] + 2 * V2[i] + 2 * V3[i] + V4[i]) / 6;
		}
	}
};
