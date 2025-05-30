#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define BITS_IN_BYTE 256
#define BYTES_IN_INT 4

void bytes_to_int(char* buf, int *num)
{
	*num = 0;
	for (int i = BYTES_IN_INT - 1; i >= 0; i--)
	{
		*num *= BITS_IN_BYTE;
		*num += buf[i];
	}
}

void int_to_bytes(int num, char* buf)
{
	for (int i = 0; i < BYTES_IN_INT; i++)
	{
		buf[i] = num % BITS_IN_BYTE;
		num /= BITS_IN_BYTE;
	}
}

void iter(int input_fd)
{
	//printf("%d\n\n\n", getpid());

	//char buf[BYTES_IN_INT] = {0};

	
	char buf[BYTES_IN_INT] = {0};

	int prime;
	if (read(input_fd, buf, BYTES_IN_INT) == 0)
	{
		close(input_fd); // not needed

		exit(0);
	}
	bytes_to_int(buf, &prime);
	
	printf("prime %d\n", prime);
	

	int p[2];
	pipe(p);

	if (fork() == 0)
	{
		close(input_fd);

		close(p[1]);

		/*char buf[BYTES_IN_INT] = {0};

		int prime;
		if (read(input_fd, buf, BYTES_IN_INT) == 0)
		{
			close(input_fd); // not needed

			exit(0);
		}
		bytes_to_int(buf, &prime);
		
		printf("%d\n", prime);*/

		iter(p[0]);

		close(p[0]); // not needed
	}
	else
	{
		close(p[0]);

		while (read(input_fd, buf, BYTES_IN_INT))
		{
			int i;
			bytes_to_int(buf, &i);

			if (i % prime)
			{
				int_to_bytes(i, buf);
				write(p[1], buf, BYTES_IN_INT);
			}
		}

		close(p[1]);

		close(input_fd); // not needed

		wait((int *)0);
	}
}

int main(int argc, char *argv[])
{
	if (argc != 1)
	{
		printf("error: incorrect amount of arguments\n");
		exit(1);
	}

	int p[2];
	pipe(p);

	if (fork() == 0)
	{
		close(p[1]);

		iter(p[0]);

		close(p[0]); // not needed
	}
	else
	{
		close(p[0]);

		char buf[BYTES_IN_INT] = {0};

		for (int i = 2; i <= 35; i++)
		{
			int_to_bytes(i, buf);
			write(p[1], buf, BYTES_IN_INT);
		}

		close(p[1]);

		wait((int *)0);
	}

	exit(0);

	/*int p[2];
	pipe(p);

	if (fork() == 0)
	{
		// second iter and forward

		LABEL:

		close(p[1]);

		int p2[2];
		pipe(p2);

		char buf[BYTES_IN_INT] = {0};

		if (fork() == 0)
		{
			// i+1 iter

			close(p2[1]);

			goto LABEL;
		}
		else
		{
			// i iter

			close(p2[0]);

			int prime;
			read(p[0], buf, BYTES_IN_INT);
			bytes_to_int(buf, &prime);

			while (read(p[0], buf, BYTES_IN_INT))
			{
				int i;
				bytes_to_int(buf, &i);
				//printf("%d\n", i);
				if (i % prime)
				{
					int_to_bytes(i, buf);
					write(p2[1], buf, BYTES_IN_INT);
				}
			}
		}
	}
	else
	{
		// first iter

		close(p[0]);
		char buf[BYTES_IN_INT] = {0};
		for (int i = 2; i <= 35; i++)
		{
			int_to_bytes(i, buf);
			write(p[1], buf, BYTES_IN_INT);
		}
	}

	exit(0);*/

	/*int pid = fork();
	if (pid > 0)
	{
		// parent
	}
    else if (pid == 0)
	{
		// child
		close(0);
		close(1);

		dup(p[1]);
		close(p[0]);
		int buf[4] = {0};
		read(p[1], buf, 4);
		int prime;
		bytes_to_int(buf, &prime);
		printf("%d", prime);

		while (read(p[1], buf, 4) == 4)
		{
			;
		}
	}
	else {
			printf("error: fork failed\n");
	}*/

	exit(0);
}
