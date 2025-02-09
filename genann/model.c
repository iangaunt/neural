#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include "genann.h"

/* This example is to illustrate how to use GENANN.
 * It is NOT an example of good machine learning techniques.
 */

const char *iris_data = "sets/numbers.data";

double *input, *class;
int samples;
const char *class_names[] = {"4", "8"};

void load_data() {
    /* Load the iris data-set. */
    FILE *in = fopen("../sets/numbers.data", "r");
    if (!in) {
        printf("Could not open file: %s\n", iris_data);
        exit(1);
    }

    /* Loop through the data to get a count. */
    char line[1024];
    while (!feof(in) && fgets(line, 1024, in)) {
        ++samples;
    }
    fseek(in, 0, SEEK_SET);

    printf("Loading %d data points from %s\n", samples, iris_data);

    /* Allocate memory for input and output data. */
    input = malloc(sizeof(double) * samples * 400);
    class = malloc(sizeof(double) * samples * 2);

    /* Read the file into our arrays. */
    int i, j;
    for (i = 0; i < samples; ++i) {
        double *p = input + i * 400;
        double *c = class + i;

        if (fgets(line, 1024, in) == NULL) {
            perror("fgets");
            exit(1);
        }

        char *split = strtok(line, ",");
        for (j = 0; j < 400; ++j) {
            p[j] = atof(split);
            split = strtok(0, ",");
        }

        split[strlen(split) - 1] = 0;

        if (strcmp(split, class_names[0]) == 0) {c[0] = 4.0;}
        else if (strcmp(split, class_names[1]) == 0) {c[0] = 8.0;}

        else {
            printf("Unknown class %s.\n", split);
            exit(1);
        }

        printf("Data point %d is %f %f %f %f  ->  %f\n", i, p[0], p[1], p[2], p[64], c[0]);
    }

    fclose(in);
}


int main(int argc, char *argv[])
{
    printf("GENANN example 4.\n");
    printf("Train an ANN on the IRIS dataset using backpropagation.\n");

    srand(time(0));

    /* Load the data from file. */
    load_data();

    /* 400 inputs.
     * 2 hidden layer(s) of 100 neurons.
     * 2 outputs (1 per class)
     */
    genann *ann = genann_init(400, 2, 50, 2);

    int i, j;
    int loops = 5000;

    /* Train the network with backpropagation. */
    printf("Training for %d loops over data.\n", loops);
    for (i = 0; i < loops; ++i) {
        for (j = 0; j < samples; ++j) {
            printf("%d\n", j);
            
            genann_train(ann, input + j * 400, class + j * 2, .01);
        }
        /* printf("%1.2f ", xor_score(ann)); */
    }

    int correct = 0;
    for (j = 0; j < samples; ++j) {
        const double *guess = genann_run(ann, input + j*4);
        if (class[j*3+0] == 1.0) {if (guess[0] > guess[1] && guess[0] > guess[2]) ++correct;}
        else if (class[j*3+1] == 1.0) {if (guess[1] > guess[0] && guess[1] > guess[2]) ++correct;}
        else if (class[j*3+2] == 1.0) {if (guess[2] > guess[0] && guess[2] > guess[1]) ++correct;}
        else {printf("Logic error.\n"); exit(1);}
    }

    printf("%d/%d correct (%0.1f%%).\n", correct, samples, (double)correct / samples * 100.0);


    genann_free(ann);
    free(input);
    free(class);

    return 0;
}
