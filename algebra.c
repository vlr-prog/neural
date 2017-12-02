#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "algebra.h"


//apply a function to every elem in the vector
void vector_apply(func_t f, double vect[], size_t size, double res[])
{
    for (size_t i = 0; i < size; i++)
        res[i] = f(vect[i]);
}


//mult of 2 matrix
void product(double M[], double v[], size_t m, size_t n, double res[])
{
    for (size_t i = 0; i < m; i++)
    {
        for (size_t j = 0; j < n; j++)
        {
            res[i] += M[j + i * n] * v[j];
        }
    }
}


//sum of 2 vectors
void vector_sum(double v1[], double v2[], size_t size, double res[])
{
    for (size_t i = 0; i < size; i++)
        res[i] = v1[i] + v2[i];
}


//compare 2 arrays 1 if equal 0 else
int comp_arr(int a[], int b[], size_t len)
{
    for(size_t i = 0; i < len; i++)
    {
        if(a[i] != b [i])
            return 0;
    }
    return 1;
}
