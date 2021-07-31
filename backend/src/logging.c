/* @name Logging
 * @author Toma Gosselin-St-Pierre
 * @brief task that log the activity of everything.
 */

#include "logging.h"
#define LOG_BUFFER_SIZE 65536
char log_buffer[LOG_BUFFER_SIZE];

uint32_t buffer_in = 0;
uint32_t buffer_out = 0;

void *Logging_task(void *vargp)
{
	logging("STARTING : logging task\n");

	while (1)
	{
		FILE *log = fopen("../log", "a");

		pthread_mutex_lock(&mutex_log);
		while (buffer_in != buffer_out)
		{
			fprintf(log, "%c", log_buffer[buffer_in]);
			buffer_in = (buffer_in + 1) % LOG_BUFFER_SIZE;
		}

		pthread_mutex_unlock(&mutex_log);

		fclose(log);
	}
	return NULL;
}

void logging(char *message)
{
	pthread_mutex_lock(&mutex_log);
	printf(message);
	for (int i = 0; message[i] != '\0'; i++)
	{
		log_buffer[buffer_out] = message[i];
		buffer_out = (buffer_out + 1) % LOG_BUFFER_SIZE;
	}
	pthread_mutex_unlock(&mutex_log);
}