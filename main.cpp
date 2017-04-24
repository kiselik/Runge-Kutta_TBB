#pragma once
#include <ctime>
#include <iostream>
#include <conio.h>
#include "koeffXvect.h"
#include "sum4Vect.h"
#include "sumVect.h"
#include "vectF.h"
#include "vectXmatr.h"

#include "tbb/tick_count.h"
using namespace tbb;
using namespace std;

void DataInitialization(float *Matrix, float *Vector, int N)
{
	for (int i = 0; i < N; i++)
	{
		Vector[i] = 0.082;
	}
	for (int i = 0; i < N; i++)
	{
		int seed=3+i;
		srand(seed);
		for (int j = 0; j < N ; j++)
		{
			Matrix[i*N + j] = ((float)(rand() % 10 - 5)) / 10;
		}
	}
}

bool isCorrectSolution(float * vect1, float *vect2, int N)
{
	double eps=0.01;
	for (int i = 0; i<N; i++)
	{
		if (abs(vect1[i] - vect2[i] )>eps)
		{
			return false;
		}
	}
	return true;
}

void printResult(float *Vector, int N)
{
	for (int i = 0; i < N; i++)
	{
		printf(" %.3f", Vector[i]);
	}
	printf("\n");
}

float* Runge_kytta(float* A,int size,float start,float finish,float h,float* y,int count_process){
	tbb::task_scheduler_init init(count_process);
	float *a1 = new float[size];
	float *a2 = new float[size];
	float *a3 = new float[size];
	float *a4 = new float[size];
	float *k1 = new float[size];
	float *k2 = new float[size];
	float *k3 = new float[size];
	float *k4 = new float[size];
	float *dy = new float[size];

	float x =start;
	int grainsize = size/count_process;
	tick_count t1 = tick_count::now();
	for (int index = 0; x < finish; index++)
	{
		x = start + index*h;
		//blocked_range итератор от 0 до size, grainsize-порция вычислений
		parallel_for(tbb::blocked_range<int>(0, size, grainsize), vectXmatr(h, A, y, size, a1));
		parallel_for(tbb::blocked_range<int>(0, size, grainsize), vectF(h, x, size, a2));
		parallel_for(tbb::blocked_range<int>(0, size, grainsize), sumVect(a1, a2, size, k1));
		parallel_for(tbb::blocked_range<int>(0, size, grainsize), koeffXvect(0.5, k1, size, a1));
		parallel_for(tbb::blocked_range<int>(0, size, grainsize), sumVect(y, a1, size, a2));
		parallel_for(tbb::blocked_range<int>(0, size, grainsize), vectXmatr(h, A, a2, size, a3));
		parallel_for(tbb::blocked_range<int>(0, size, grainsize), vectF(h, x + h / 2, size, a4));
		parallel_for(tbb::blocked_range<int>(0, size, grainsize), sumVect(a3, a4, size, k2));
		parallel_for(tbb::blocked_range<int>(0, size, grainsize), koeffXvect(0.5, k2, size, a1));
		parallel_for(tbb::blocked_range<int>(0, size, grainsize), sumVect(y, a1, size, a2));
		parallel_for(tbb::blocked_range<int>(0, size, grainsize), vectXmatr(h, A, a2, size, a3));
		parallel_for(tbb::blocked_range<int>(0, size, grainsize), vectF(h, x + h / 2, size, a4));
		parallel_for(tbb::blocked_range<int>(0, size, grainsize), sumVect(a3, a4, size, k3));
		parallel_for(tbb::blocked_range<int>(0, size, grainsize), sumVect(y, k3, size, a1));
		parallel_for(tbb::blocked_range<int>(0, size, grainsize), vectXmatr(h, A, a1, size, a2));
		parallel_for(tbb::blocked_range<int>(0, size, grainsize), vectF(h, x + h, size, a3));
		parallel_for(tbb::blocked_range<int>(0, size, grainsize), sumVect(a2, a3, size, k4));
		parallel_for(tbb::blocked_range<int>(0, size, grainsize), sum4Vect(k1, k2, k3, k4, size, dy));
		parallel_for(tbb::blocked_range<int>(0, size, grainsize), sumVect(y, dy, size, y));
		parallel_for(tbb::blocked_range<int>(0, size, grainsize), sumVect(y, dy, size, y));
	}
	tick_count t2 = tick_count::now();
	cout << "work time for "<<count_process<<" process: " <<  (t2 - t1).seconds()<< endl;;
	if(size<5)
		printResult(y,size);
	delete[] k1;
	delete[] k2;
	delete[] k3;
	delete[] k4;
	delete[] a1;
	delete[] a2;
	delete[] a3;
	delete[] a4;
	delete[] dy;
	//класс для создания потоков конструктор по умолчанию
	init.terminate();
	return y;
}

int main()
{

	int size = 0;
	cout<<"Please, write count of equations:"<<endl;
	cin >> size;

	float start = -2.5, finish = 2.5, h = 0.1, x;
	float *y = new float[size];
	float *A = new float[size*size];
	float *yn = new float[size];
	float *y_result = new float[size];
	float *y_result2 = new float[size];
	DataInitialization(A, yn, size);
	y_result=Runge_kytta(A,size,start,finish,h,y_result,1);
	y_result2=Runge_kytta(A,size,start,finish,h,y_result2,2);

	if (isCorrectSolution( y_result,y_result2, size))
		printf("Solution is correct!!!");
	printf("\n");
	delete[] A;
	delete[] y;
	delete[] y_result;
	delete[] y_result2;

	system("pause");
	return 0;
}