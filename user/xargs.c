#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

#define MAX_SIZE 1024

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        fprintf(2, "error: incorrect amount of arguments\n");
        exit(1);
    }

    char buf[MAX_SIZE+1] = {0};
    int ind = 0;
    while (read(0, buf+ind, 1))
    {
        if (buf[ind] == '\n')
        {
            int pid = fork();
            if (pid == 0)
            {
                // child
                char *argv1[MAXARG] = {0};
                int j = 0;

                for (int i = 1; i < argc; i++)
                {
                    argv1[j] = malloc(strlen(argv[j+1]) + 1);
                    memmove(argv1[j], argv[j+1], strlen(argv[j+1])+1);
                    j++;
                }

                for (int i = 0; i < MAX_SIZE && buf[i] != '\0'; i++)
                {
                    int l = 0;
                    while (buf[i+l] != '\n' && buf[i+l] != ' ' && buf[i+l] != '\0')
                        l++;
                    
                    argv1[j] = malloc(l+1);
                    memmove(argv1[j], buf+i, l);
                    argv1[j][l] = '\0';
                    i += l;
                    j++;
                    

                    if (buf[i] == '\n' || j == MAXARG)
                        break;
                }

                exec(argv1[0], argv1);
            }
            else if (pid > 0)
            {
                // parent
                wait((int *)0);
            }
            else
            {
                fprintf(2, "error: fork failed\n");
                exit(0);
            }

            // reuse buffer
            ind = 0;
        }

        ind++;
    }

    exit(0);
}
