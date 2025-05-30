#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
        int ticks;

        if (argc != 2)
        {
		printf("error: incorrect amount of arguments\n");
                exit(1);
        }

        ticks = atoi(argv[1]);

	if (sleep(ticks) != 0)
	{
		printf("error: call to sys_sleep failed\n");
		exit(1);
	}

        exit(0);
}
