#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void filter_process(int, int);
int create_filter(int, int);


int create_filter(int input_fd, int output_fd)
{
    int pid = fork();
    if (pid == 0)
    {
        filter_process(input_fd, output_fd);
        close(input_fd);
        exit(0);
    }
    return pid;
}
void filter_process(int input_fd, int output_fd)
{
    int prime, number;
    if (read(input_fd, &number, sizeof(int)) > 0) {
        printf("prime %d\n", number);
        prime = number;

        int new_pipe[2];
        pipe(new_pipe);
        create_filter(new_pipe[0], output_fd);

        close(new_pipe[0]);

        while (read(input_fd, &number, sizeof(int)) > 0) {
            if (number % prime != 0) {
                write(new_pipe[1], &number, sizeof(int));
            }
        }

        close(input_fd);
        close(new_pipe[1]);

    }
}

int main(int argc, char *argv[])
{
    int initial[2];
    pipe(initial);
    create_filter(initial[0], initial[1]);
    if (fork() == 0)
    {
        close(initial[0]);
        for (int i = 2; i < 281; i++)
        {
            // printf("sending %d", i);
            write(initial[1], &i, sizeof(int));
        }
        close(initial[1]);
        exit(0);
    }
    close(initial[0]);
    close(initial[1]);
    exit(0);
}
