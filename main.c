/* main.c neural net of xor logical door */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "algebra.h"

#define input_len 2
#define output_len 1
#define wanted_len 1


//neuronal struct
struct nr {
    double     *w;    //the arrays of weights
    double      b,  r, y, dy; // b: threshold, r: result
};


//layer struct
struct layer {
    size_t  number, size, wlen;//size: nbr of neurons, wlen: len of the [] weights
    struct  nr **tab; //the arrays of neurons
};


//struct network
struct network {
    size_t size;
    double input[input_len], output[output_len], wanted[wanted_len];
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
    printf("Layer %lu weights :\n", lay->number);
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
    printf("Layer %lu results :\n", lay->number);
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
        free_layer(rs->lay[i]); //free each layer
    free(rs->lay);            //free the layer array
    free(rs);                 //free the network
}


//init the first layer with the correct values
void init_first_layer(struct layer *lay, double input[])
{
    for(size_t i = 0; i < lay->size; i++)
        lay->tab[i]->r = input[i];
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
        lay->tab[i]->r  = 0;
        lay->tab[i]->y  = 0;
        lay->tab[i]->dy = 0;
        lay->tab[i]->b  = rnd();

        lay->tab[i]->w = malloc(size_prev * sizeof(double));
        for(size_t j = 0; j < lay->wlen; j++)
            lay->tab[i]->w[j] = rnd();
    }
}


//create and init the wire
struct network* create_network(size_t size, size_t layer_size[])
{
    struct network *rs = malloc(sizeof(struct network));
    rs->size = size;
    rs->lay = malloc(size * sizeof(struct layer));

    for(size_t i = 0; i < size; i++)
        rs->lay[i] = create_layer(layer_size[i], i);

    init_layer(rs->lay[0], 0);
    for(size_t i = 1; i < size; i++)
        init_layer(rs->lay[i], rs->lay[i-1]->size);

    rs->input[0] = 1;
    rs->input[1] = 1;

    //special init for the inputs layer
    init_first_layer(rs->lay[0], rs->input);
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


//back propagation for the last layer
void back_last(struct layer *prev, struct layer *lay,
               double output[],    double wanted[])
{
    for(size_t i = 0; i < lay->size; i++)
    {
        //compute the delta y aka dy = sig'(y) * (output - wanted)
        lay->tab[i]->dy = sigmoid_prime(lay->tab[i]->y)*(wanted[0]-output[0]);

        //by += dy * 1  ;  1 is the learning coeff
        lay->tab[i]->b += lay->tab[i]->dy * 1;
 
        //compute the new weights w +=  r * dy
        for(size_t j = 0; j < lay->wlen; j++)
            lay->tab[i]->w[j] +=  prev->tab[j]->r * lay->tab[i]->dy;
     }
}


//backpropagation for normal layer
void back(struct layer *previous, struct layer *first, struct layer *second)
{
    for(size_t i = 0; i < first->size; i++)
    {
        //dyi = sig'(yi) * second->dy * second->w
        first->tab[i]->dy = sigmoid_prime(first->tab[i]->y) *
        second->tab[0]->dy * second->tab[0]->w[i];

        // bi += dy * 1
        first->tab[i]->b += first->tab[i]->dy * 1;

        //compute the new weights wj +=  prev rj * dyi
        for(size_t j = 0; j < first->wlen; j++)
            first->tab[i]->w[j] +=  first->tab[i]->dy * previous->tab[j]->r;
    }
}


//the backpropagation responsbile for the learning
void back_prop(struct network *rs)
{
    size_t s = rs->size;
    back_last(rs->lay[s - 2], rs->lay[s - 1], rs->output, rs->wanted);

    for(int i = rs->size - 2; i > 0; i--)
        back(rs->lay[i - 1], rs->lay[i], rs->lay[i + 1]);
}


//train the network
void train(void)
{
    ///random init
    srand((unsigned) time(NULL));

    //size of the differents layers;
    size_t len_layers[] = { input_len, 2, output_len };
    size_t len_reseau = 3;

    struct network *rs = create_network(len_reseau, len_layers);

    //print the weights
    print_wn(rs);

    //add weights
    forward(rs);

    int wanted_res[] = { 0, 1, 1, 0 };
    int inputs[] = { 0, 0, 1, 0, 0, 1, 1, 1};

    for(size_t i = 0; i < 2000; i++)
    {
        for (size_t j = 0; j < 4; j++)
        {
            rs->input[0] = inputs[2 * j];
            rs->input[1] = inputs[2 * j + 1];
            rs->output[0] = rs->lay[rs->size - 1]->tab[0]->r;
            rs->wanted[0] = wanted_res[j];

            forward(rs);
            back_prop(rs);
            printf("%d XOR %d = %f\n", inputs[2*j], inputs[2*j+1],
            rs->lay[rs->size - 1]->tab[0]->r);
        }
        printf("\n\n");
    }

    //free the layers
    free_network(rs);
}


//main function of the program
int main()
{
    train();
    return 0;
}
