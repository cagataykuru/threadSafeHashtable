

#include <pthread.h>
#include "hash.h"
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void *testLibrary(void *p)
{
	int i;
    int num = (long) p; 

    
	for (i = 0; i < 1000; ++i) {
		hash_insert(num*1000 + i);
	}
	for (i = 0; i < 100; ++i) {
		hash_delete(num*1000 + i);
	}

	for (i = 0; i < 1000; ++i) {
		hash_get(num*1000 + i);
	}

	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	clock_t begin, end;
	double time_spent;

	begin = clock();
	
	pthread_t tid[MAXTH];
	int i;
	int N;
	
	N = atoi(argv[1]);

	if (N > MAXTH) {
		printf("Can create  at most %d threads\n", MAXTH);
		exit(1);
	}

	hash_init(80);

	for (i = 0; i < N; ++i) {
		pthread_create(&tid[i], NULL, &testLibrary, (void *)(long)i);
	}

	for (i = 0; i < N; ++i)
		pthread_join(tid[i], NULL);


	end = clock();
	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

	printf("%f\n",time_spent );

	return (0);
}
