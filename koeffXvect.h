#pragma once
#include "tbb/task_scheduler_init.h"
#include "tbb/parallel_for.h"
#include "tbb/blocked_range.h"
class koeffXvect
{
	float h;
	float *Vect;
	int N;
	float* sResult;
public:
	koeffXvect(float th, float *tVect, int tN, float* tsResult) : sResult(tsResult), h(th), Vect(tVect), N(tN) {}
	void operator()(const tbb::blocked_range<int>& r) const
	{
		int begin = r.begin(), end = r.end();
		for (int i = begin; i != end; i++)
		{
			sResult[i] = h*Vect[i];
		}
	}
};
