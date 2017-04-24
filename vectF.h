#pragma once
#include "tbb/task_scheduler_init.h"
#include "tbb/parallel_for.h"
#include "tbb/blocked_range.h"
class vectF
{
	float h, x;
	int N;
	float* sResult;
public:
	vectF(float th, float tx, int tN, float* tsResult) : sResult(tsResult), h(th), x(tx), N(tN) {}
	void operator()(const tbb::blocked_range<int>& r) const
	{
		int begin = r.begin(), end = r.end();
		for (int i = begin; i != end; i++)
		{
			sResult[i] = h*x;
		}
	}
};


