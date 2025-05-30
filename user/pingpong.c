#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
        if (argc != 1)
        {
                printf("error: incorrect amount of arguments\n");
                exit(1);
        }

	int p1[2], p2[2];
	pipe(p1);
	pipe(p2);

	int pid = fork();
        if (pid > 0)
	{
		// parent

		// write a byte
		close(p1[0]);
		char buf[1] = {'!'};
		if (write(p1[1], buf, 1) != 1)
		{
			printf("error: write failed\n");
			exit(1);
		}

		// read a byte
		close(p2[1]);
		if (read(p2[0], buf, 1) != 1)
		{
			printf("error: read failed\n");
			exit(1);
		}

		int pid2 = getpid();
		printf("%d: received pong\n", pid2);
	}
	else if (pid == 0)
	{
		// child

		// read a byte
		close(p1[1]);
		char buf[1] = {0};
		if (read(p1[0], buf, 1) != 1)
		{
			printf("error: read failed\n");
			exit(1);
		}

		int pid2 = getpid();
		printf("%d: received ping\n", pid2);

		// write a byte
		close(p2[0]);
		if (write(p2[1], buf, 1) != 1)
		{
			printf("error: write failed\n");
			exit(1);
		}
	}
	else {
		printf("error: fork failed\n");
	}

        exit(0);
}
