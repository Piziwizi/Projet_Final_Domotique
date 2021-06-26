/* @name main.c
 * @author Toma Gosselin-St-Pierre
 * @brief main application of the backend
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void *Sensor(void *vargp)
{
	sleep(1);
	printf("test\n");
	return NULL;
}

int main()
{
	pthread_t thread_id;
	printf("Before Thread\n");
	pthread_create(&thread_id, NULL, Sensor, NULL);
	pthread_join(thread_id, NULL);
	printf("After Thread\n");
	exit(0);
}