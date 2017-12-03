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

//neuronal struct
struct nr {
    double     *w;    //the arrays of weights
    double      b, r; // b: threshold, r: result
};


//layer struct
struct layer {
    size_t  number, size, wlen;//size: nbr of neurons, wlen: len of the [] weights
    struct nr **tab; //the arrays of neurons
};


//sigmoid(x) activation function
double sigmoid(double x)
{
      return 1 / (1 + exp(-x));
}


//print the weights of all neurons in a layer
void print_w(struct layer *lay)
{
    printf("Layer %lu :\n", lay->number);
    for(size_t i = 0; i < lay->size; i++)
    {
        printf("Neuron %lu : | ", i);
        for(size_t j = 0; lay->wlen > j; j++)
            printf("%f | ", lay->tab[0]->w[j]);
        printf("\n");
    }
    printf("\n");
}


//print the results of all neurons in a layer
void print_r(struct layer *lay)
{
    printf("Layer %lu :\n", lay->number);
    for(size_t i = 0; i < lay->size; i++)
        printf("Neuron %lu : %f ", i, lay->tab[i]->r);
    printf("\n");
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
void free_layer(struct layer *lay)
{
    for(size_t i = 0; i < lay->size; i++)
    {
        free(lay->tab[i]->w); //free the weights arrays
        free(lay->tab[i]);    //free the neuron
    }
    free(lay->tab);           //free the arrays of neuron
    free(lay);                //free the layer
}


//init the first layer with the correct values
void init_first_layer(struct layer *lay, size_t f, size_t s)
{
    lay->tab[0]->r = f;
    lay->tab[1]->r = s;
}


//create and return and layer with only size initialized
struct layer* create_layer(size_t size, size_t number)
{
    struct layer *lay = malloc(sizeof( struct layer));
    lay->size = size;
    lay->number = number;
    return lay;
}


//init a layer with random values
void init_layer(struct layer *lay, size_t size_prev, double rnd)
{
    lay->tab = malloc(lay->size * sizeof(struct nr));

    for(size_t i = 0; i < lay->size; i++)
    {
        struct nr *neuron = malloc(sizeof(struct nr));
        lay->tab[i] = neuron;
        lay->wlen = size_prev;
        lay->tab[i]->r = 0;
        lay->tab[i]->b = rnd;

        lay->tab[i]->w = malloc(size_prev * sizeof(double));
        for(size_t j = 0; j < lay->wlen; j++)
            lay->tab[i]->w[j] = rnd;
    }
}


//forward function of neural network
void forward(struct layer *first, struct layer *second)
{
    for(size_t i = 0; i < second->size; i++)
    {
        for(size_t j = 0; j < first->size; j++)
            second->tab[i]->r += first->tab[j]->r * second->tab[i]->w[j];
        second->tab[i]->r += second->tab[i]->b;
    }
}


//logical door XOR
void xor(void)
{
    ///random init
    double rnd;
    srand((unsigned) time(NULL));
    rnd = (double)rand()/(double)(RAND_MAX);


    //size of the differents layers;
    size_t len_layer1 = 2;
    size_t len_layer2 = 2;
    size_t len_layer3 = 1;

    //create the layers
    struct layer *layer1 = create_layer(len_layer1, 1);
    struct layer *layer2 = create_layer(len_layer2, 2);
    struct layer *layer3 = create_layer(len_layer3, 3);

    //init the layers
    init_layer(layer1, 0         , rnd);
    init_layer(layer2, len_layer1, rnd);
    init_layer(layer3, len_layer2, rnd);

    init_first_layer(layer1, 1, 1);

    print_w(layer1);
    print_w(layer2);
    print_w(layer3);
  
    forward(layer1, layer2);
    forward(layer2, layer3);


    print_r(layer1);
    print_r(layer2);
    print_r(layer3);
  
    //free the layers
    free_layer(layer1);
    free_layer(layer2);
    free_layer(layer3);

}


//main function of the program
int main()
{
    xor();
    return 0;
}
