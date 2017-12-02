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


struct nr {
    double *w;
    size_t wlen;
    double r;
    double b;
};


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


//the resulst we want to get
int results [] = {
     0, 1, 1, 0
};

int res [] = {
     0, 0, 0, 0
};




//locical door for neurons with 2 input and 1 output
void layer_xor(double W[], double b[], double a[], size_t m, size_t n, double r[])
{
    product(W, a, m, n, r);
    vector_sum(b, r, m, r);
    vector_apply(sigmoid, r, m, r);
}


//free a layer
void free_layer(struct nr *layer[], size_t size)
{
    for(size_t i = 0; i < size; i++) 
        free(layer[i]->w);
    free(layer);
}


//init a layer with random values
void init_layer(struct nr *layer[], size_t size, size_t size_prev, double rnd)
{
    for(size_t i = 0; i < size; i++)
    {
        layer[i]->wlen = size_prev;
        layer[i]->r = 0;
        layer[i]->b = rnd;

        layer[i]->w = malloc(size_prev * sizeof(double));
        for(size_t j = 0; j < layer[i]->wlen; j++)
            layer[i]->w[j] = rnd;
    }
}


//logical door XOR
void xor(void)
{
    ///random init
    double rnd;
    srand((unsigned) time(NULL));


    //size of the differents layers;
    size_t len_layer1 = 2;
    size_t len_layer2 = 2;
    size_t len_layer3 = 1;

    struct nr *layer1 = malloc(len_layer1 * sizeof(struct nr));

    //random int for OR
    int rn = rand()%(10 + 2) - 1;

    //layer 1 neuron 1 weights
    double l1n1w[] = {
        rn, rn
    };

    //random double for AND
    rnd = rand() % 1;


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
        layer_xor(l1n1w, b1n1, a, 1, 2, l1r1);


        //AND
        layer_xor(l1n2w, b1n2, a, 1, 2, l1r2);

        l1r[0] = l1r1[0];
        l1r[1] = l1r2[0];

        layer_xor(l2w, b2, l1r, 1, 2, r);

        printf("0 XOR 0 -> %g\n", r[0]);

        res[0] =  r[0] + co;



        //xor with 1 0
        a [0] = 1; a[1] = 0;

        layer_xor(l1n1w, b1n1, a, 1, 2, l1r1);

        if(l1r1[0] < 0.98) //or too low
            vector_sum(l1n1w, orl, 2, l1n1w);

        if(l1r2[0] > 0.02) //and too high
            vector_sum(l1n2w, andh , 2, l1n2w);

        l1r[0] = l1r1[0];
        l1r[1] = l1r2[0];

        layer_xor(l2w, b2, l1r, 1, 2, r);

        printf("1 XOR 0 -> %g\n", r[0]);

        res[1] =  r[0] + co;



        //xor with 0 1
        a[0] = 0; a[1] = 1;

        layer_xor(l1n1w, b1n1, a, 1, 2, l1r1);

        if(l1r1[0] < 0.98) //or too low
            vector_sum(l1n1w, orl, 2, l1n1w);

        layer_xor(l1n2w, b1n2, a, 1, 2, l1r2);

        if(l1r2[0] > 0.02) //and too high
            vector_sum(l1n2w, andh, 2, l1n2w);


        l1r[0] = l1r1[0];
        l1r[1] = l1r2[0];

        layer_xor(l2w, b2, l1r, 1, 2, r);

        printf("0 XOR 1 -> %g\n", r[0]);

        res[2] =  r[0] + co;



        //xor with 1 1
        a[0] = 1; a[1] = 1;

        layer_xor(l1n1w, b1n1, a, 1, 2, l1r1);

        layer_xor(l1n2w, b1n2, a, 1, 2, l1r2);


        l1r[0] = l1r1[0];
        l1r[1] = l1r2[0];

        layer_xor(l2w, b2, l1r, 1, 2, r);

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
