#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
#include "kernel/param.h"

int main(int argc, char *argv[])
{
    int index = 0;
    char buf[512];
    char character;
    int escape = 0;

    while (index < 511 && read(0, &character, 1) == 1)
    {
        if (escape)
        {
            if (character == 'n')
            {
                buf[index++] = '\n';
            }
            else
            {
                buf[index++] = '\\';
                buf[index++] = character;
            }
            escape = 0;
        }
        else if (character == '\\')
        {
            escape = 1;
        }
        else if (character == '\n' || character == ' ' || character == '\t')
        {
            if (index > 0)
            {
                buf[index] = '\0';
                index = 0;

                char *new_argv[MAXARG + 1];
                int new_argc = 0;

                // Copy all original arguments
                for (int i = 1; i < argc; i++)
                {
                    new_argv[new_argc++] = argv[i];
                }
                new_argv[new_argc] = buf;
                if (fork() == 0)
                {
                    exec(argv[1], new_argv);
                    exit(0);
                }
                else
                {
                    wait(0);
                }
            }
        }
        else
        {
            buf[index++] = character;
        }
    }

    if (index > 0)
    {
        buf[index] = '\0';
        printf("%s\n", buf);
    }

    exit(0);
}