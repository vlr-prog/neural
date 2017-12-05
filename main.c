/* main.c neural net of xor logical door */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "algebra.h"


//neuronal struct
struct nr {
    double     *w;    //the arrays of weights
    double      b, r, y, dy; // b: threshold, r: result
};


//layer struct
struct layer {
    size_t  number, size, wlen;//size: nbr of neurons, wlen: len of the [] weights
    struct nr **tab; //the arrays of neurons
};


//struct network
struct network {
    size_t size;
    struct layer **lay;
};


//sigmoid(x) activation function
double sigmoid(double x)
{
      return 1 / (1 + exp(-x));
}


//the derivate of the sigmoid
double sigmoid_prime(double x)
{
    return sigmoid(x) * (1 - sigmoid(x));
}


//give a random double between 0 and 1
double rnd()
{
    return (double)rand()/(double)(RAND_MAX);
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


//print all the weights in the network
void print_wn(struct network *rs)
{
    for(size_t i = 0; i < rs->size; i++)
        print_w(rs->lay[i]);
}


//print the results of all neurons in a layer
void print_r(struct layer *lay)
{
    printf("Layer %lu :\n", lay->number);
    for(size_t i = 0; i < lay->size; i++)
        printf("Neuron %lu : %f \n", i, lay->tab[i]->r);
    printf("\n");
}


//print all the results in the network
void print_rn(struct network *rs)
{
    for(size_t i = 0; i < rs->size; i++)
        print_r(rs->lay[i]);
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


//free the network
void free_network(struct network *rs)
{
    for(size_t i = 0; i < rs->size; i++)
        free_layer(rs->lay[i]);
    free(rs->lay);
    free(rs);
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
    struct layer *lay = malloc(sizeof(struct layer));
    lay->size = size;
    lay->number = number;
    return lay;
}


//init a layer with random values
void init_layer(struct layer *lay, size_t size_prev)
{
    lay->tab = malloc(lay->size * sizeof(struct nr));

    for(size_t i = 0; i < lay->size; i++)
    {
        struct nr *neuron = malloc(sizeof(struct nr));
        lay->tab[i] = neuron;
        lay->wlen = size_prev;
        lay->tab[i]->r = 0;
        lay->tab[i]->y = 0;
        lay->tab[i]->b = rnd();

        lay->tab[i]->w = malloc(size_prev * sizeof(double));
        for(size_t j = 0; j < lay->wlen; j++)
            lay->tab[i]->w[j] = rnd();
    }
}


//create and init the wire
struct network* create_network(size_t size, size_t layer[])
{
    struct network *rs = malloc(sizeof(struct network));
    rs->size = size;
    rs->lay = malloc(size * sizeof(struct layer));
    
    for(size_t i = 0; i < size; i++)
        rs->lay[i] = create_layer(layer[i], i);
    
    init_layer(rs->lay[0], 0);
    for(size_t i = 1; i < size; i++)
        init_layer(rs->lay[i], rs->lay[i-1]->size);
    init_first_layer(rs->lay[0], 1, 1);

    return rs;
}


//forward function of neural network
void frd(struct layer *first, struct layer *second)
{
    for(size_t i = 0; i < second->size; i++)
    {
        for(size_t j = 0; j < first->size; j++)
            second->tab[i]->r += first->tab[j]->r * second->tab[i]->w[j];
        second->tab[i]->r += second->tab[i]->b;
        second->tab[i]->y = second->tab[i]->r;
        second->tab[i]->r = sigmoid(second->tab[i]->r);
    }
}


//use frd on the struct network
void forward(struct network *rs)
{
    for(size_t i = 0; i < rs->size - 1; i++)
        frd(rs->lay[i], rs->lay[i+1]);
}


//the backpropagation responsbile for the learning
void back_prop();


//logical door XOR)
void xor(void)
{
    ///random init
    srand((unsigned) time(NULL));

    //size of the differents layers;
    size_t len_layers[] = { 2, 2, 1 };
    size_t len_reseau = 3;

    struct network *rs = create_network(len_reseau, len_layers);

    //print the weights
    print_wn(rs);

    //add weights
    forward(rs);   
    
    //print the results after the foward
    print_rn(rs);

    //free the layers
    free_network(rs);
}


//main function of the program
int main()
{
    xor();
    return 0;
}
