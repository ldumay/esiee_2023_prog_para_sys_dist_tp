#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

static long MULTIPLIER = 1366;
static long ADDEND = 150889;
static long PMOD = 714025;
long random_last = 0;
double random_low, random_hi;

double drandom()
{
    // Generate a new random number using a linear congruential algorithm
    long random_next = (MULTIPLIER * random_last + ADDEND) % PMOD;
    random_last = random_next;

    // Scale the random number to the desired range
    double ret_val = ((double)random_next / (double)PMOD) * (random_hi - random_low) + random_low;

    return ret_val;
}

void seed(double low_in, double hi_in)
{
    // Set the lower and upper limits for the random number generator
    if (low_in < hi_in) {
        random_low = low_in;
        random_hi = hi_in;
    } else {
        random_low = hi_in;
        random_hi = low_in;
    }

    // Initialize the random number generator
    random_last = PMOD / ADDEND; // Just pick something
}

void compute_average_of_random_numbers(int num_random_numbers, double random_low, double random_hi, double* average)
{
    int i;
    double sum = 0.0;
    double random_num;

    // Seed the random number generator
    seed(random_low, random_hi);

    // Generate and sum the random numbers
    #pragma omp parallel for reduction(+:sum)
    for (i = 0; i < num_random_numbers; i++) {
        random_num = drandom();
        sum += random_num;
    }

    // Compute the average of the random numbers
    *average = sum / num_random_numbers;
}

int main()
{
    int num_random_numbers = 1000000;
    double random_low = 0.0;
    double random_hi = 1.0;
    double average_of_random_numbers;

    compute_average_of_random_numbers(num_random_numbers, random_low, random_hi, &average_of_random_numbers);

    printf("The average of %d random numbers between %f and %f is %f\n",
           num_random_numbers, random_low, random_hi, average_of_random_numbers);

    return 0;
}