#include <stdio.h>
#include <unistd.h>

int main() {
	int var = 1;
	long pid;
	printf("Before fork, var = %d.\n", var);
	pid = fork();
	printf("After fork, var = %d.\n", var);
	if (pid == 0) {
		var = 2;
		sleep(3);
		printf("child got %ld, var = %d", pid, var);
	} else {
		sleep(2);
		printf("parent got %ld, var = %d", pid, var);
	}
	printf(", pid: %ld\n", (long) getpid());
	return 0;
}
