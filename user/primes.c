#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// Forward declaration
void process_numbers(int, int);

// This function reads the first prime and creates a new filtering process
void sieve(int input_fd)
{
    int prime;
    // Read the first number (the prime)
    if (read(input_fd, &prime, sizeof(int)) <= 0)
    {
        close(input_fd);
        exit(0);
    }

    // Print the prime
    printf("prime %d\n", prime);

    // Process the remaining numbers
    process_numbers(input_fd, prime);
}

// This function filters out multiples of prime and passes remaining numbers
void process_numbers(int input_fd, int prime)
{
    // Create a pipe for the next process
    int p[2];
    pipe(p);

    if (fork() == 0)
    {
        // Child process
        close(p[1]); // Close write end
        sieve(p[0]);
        exit(0);
    }
    else
    {
        // Parent process
        close(p[0]); // Close read end
        int n;
        while (read(input_fd, &n, sizeof(int)) > 0)
        {
            // Pass numbers that aren't multiples of the prime
            if (n % prime != 0)
            {
                write(p[1], &n, sizeof(int));
            }
        }
        close(p[1]); // Close pipe when done
        close(input_fd);
        wait(0); // Wait for child to finish
        exit(0);
    }
}

int main(int argc, char *argv[])
{
    int p[2];
    pipe(p);

    if (fork() == 0)
    {
        close(p[1]); // Close write end in child
        sieve(p[0]);
        exit(0);
    }
    else
    {
        close(p[0]); // Close read end in parent
        // Write all numbers from 2 to 35
        for (int i = 2; i <= 50; i++)
        {
            write(p[1], &i, sizeof(int));
        }
        close(p[1]); // Close pipe when done
        wait(0);     // Wait for child to finish
        exit(0);
    }
}