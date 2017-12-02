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


//print the weights of all neurons in a layer
void print_w(struct nr *layer[], size_t size)
{
    for(size_t i = 0; i < size; i++)
    {
        for(size_t j = 0; layer[i]->wlen > i; i++)
            printf("%f | ", layer[i]->w[j]);
        printf("\n");
    }
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
    {
        free(layer[i]->w);
        free(layer[i]);
    }
    free(layer);
}


//init a layer with random values
void init_layer(struct nr *layer[], size_t size, size_t size_prev, double rnd)
{
    for(size_t i = 0; i < size; i++)
    {
        struct nr *neural = malloc(sizeof(struct nr));
        layer[i] = neural;
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
    rnd = rand() % 1;


    //size of the differents layers;
    size_t len_layer1 = 2;
    size_t len_layer2 = 2;
    size_t len_layer3 = 1;

    //arrays of neurons aka layers
    struct nr **layer1 = malloc(len_layer1 * sizeof(struct nr));
    struct nr **layer2 = malloc(len_layer2 * sizeof(struct nr));
    struct nr **layer3 = malloc(len_layer3 * sizeof(struct nr));

    //init the layers
    init_layer(layer1, len_layer1, 0         , rnd);
    init_layer(layer2, len_layer2, len_layer1, rnd);
    init_layer(layer3, len_layer3, len_layer2, rnd);

    
    print_w(layer1, len_layer1);





    free_layer(layer1, len_layer1);
    free_layer(layer2, len_layer2);
    free_layer(layer3, len_layer3);

}


//main function of the program
int main()
{
    xor();
    return 0;
}
