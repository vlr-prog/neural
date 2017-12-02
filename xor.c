/* main.c neural net of xor logical door */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "algebra.h"


/*
void rotation(double theta, double v[], double r[])
{
    double M[] = {
        cos(theta), -sin(theta),
        sin(theta), cos(theta)
    };
    product(M, v, 2, 2, r);
}
*/


//sigmoid(x) activation function
double sigmoid(double x)
{
      return 1 / (1 + exp(-x));
}


//print a vector
void print_v(double v[], size_t size)
{
    for(size_t i = 0; i < size; i++)
        printf("%4g", v[i]);
}



double W[] = {
    6.36634445, 6.36769295
};

double b[] = {
    -9.00631428
};



//the resulst we want to get
int results [] = {
     0, 1, 1, 0
};

int res [] = {
     0, 0, 0, 0
};




//locical door for neurons with 2 input and 1 output
void layer(double W[], double b[], double a[], size_t m, size_t n, double r[])
{
    product(W, a, m, n, r);
    vector_sum(b, r, m, r);
    vector_apply(sigmoid, r, m, r);
}


//logical door XOR
void xor(void)
{

    ///random init
    double rnd;
    srand((unsigned) time(NULL));

    //random int for OR
    int rn = rand()%(10 + 2) - 1;

    //layer 1 neuron 1 weights
    double l1n1w[] = {
        rn, rn
    };

    //random double for AND
    rnd = (rand()/(double)RAND_MAX)*(15.36634445-8.36634445)+8.36634445;


    //layer 1 neuron 2 weights
    double l1n2w[] = {
        rnd, rnd
    };

    //layer 1 neuron 1 threshold
    double b1n1[] = {
        -8
    };

    //layer 1 neuron 2 threshold
    double b1n2[] = {
         -9.00631428
    };

    //layer 2 weights
    double l2w[] = {
        4, -8
    };

    //layer 2 threshold
    double b2[] = {
       -1.5
    };


    double a[] = {0,0};
    double r[] = {0}; //stock the XOR res
    double l1r1[] = {0}; //stock the OR res
    double l1r2[] = {0}; //stock the AND res
    double l1r[2]; //stock the OR and the AND


    double andh[2] = {-0.125, -0.125}; //correct AND weights
    double orl[2] = {0.5,0.5};

    int i = 0;
    //corection till we have the good results
    while(comp_arr(results, res, 4) == 0)
    {

        double co = 0.27;
        //xor with 0 0
        //OR
        layer(l1n1w, b1n1, a, 1, 2, l1r1);


        //AND
        layer(l1n2w, b1n2, a, 1, 2, l1r2);

        l1r[0] = l1r1[0];
        l1r[1] = l1r2[0];

        layer(l2w, b2, l1r, 1, 2, r);

        printf("0 XOR 0 -> %g\n", r[0]);

        res[0] =  r[0] + co;



        //xor with 1 0
        a [0] = 1; a[1] = 0;

        layer(l1n1w, b1n1, a, 1, 2, l1r1);

        if(l1r1[0] < 0.98) //or too low
            vector_sum(l1n1w, orl, 2, l1n1w);

        layer(l1n2w, b1n2, a, 1, 2, l1r2);

        if(l1r2[0] > 0.02) //and too high
            vector_sum(l1n2w, andh , 2, l1n2w);

        l1r[0] = l1r1[0];
        l1r[1] = l1r2[0];

        layer(l2w, b2, l1r, 1, 2, r);

        printf("1 XOR 0 -> %g\n", r[0]);

        res[1] =  r[0] + co;



        //xor with 0 1
        a[0] = 0; a[1] = 1;

        layer(l1n1w, b1n1, a, 1, 2, l1r1);

        if(l1r1[0] < 0.98) //or too low
            vector_sum(l1n1w, orl, 2, l1n1w);

        layer(l1n2w, b1n2, a, 1, 2, l1r2);

        if(l1r2[0] > 0.02) //and too high
            vector_sum(l1n2w, andh, 2, l1n2w);


        l1r[0] = l1r1[0];
        l1r[1] = l1r2[0];

        layer(l2w, b2, l1r, 1, 2, r);

        printf("0 XOR 1 -> %g\n", r[0]);

        res[2] =  r[0] + co;



        //xor with 1 1
        a[0] = 1; a[1] = 1;

        layer(l1n1w, b1n1, a, 1, 2, l1r1);

        layer(l1n2w, b1n2, a, 1, 2, l1r2);


        l1r[0] = l1r1[0];
        l1r[1] = l1r2[0];

        layer(l2w, b2, l1r, 1, 2, r);

        printf("1 XOR 1 -> %g\n", r[0]);

        res[3] =  r[0] + co;
        printf("\n\n");

        i++;
    }
    printf("Number of trials : %d\n\n",i);
}


//main function of the program
int main()
{
    xor();
    return 0;
}
