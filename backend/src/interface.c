/* @name Interface
 * @author Toma Gosselin-St-Pierre
 * @brief task that write the json file for the interface.
 */

#include "interface.h"
#define BUFFER_LENGHT 1024
char buffer[MAX_CHAR_FILE];

/**
 * @name Interface Task
 * @brief Write the files to communicate with the interface
 * @param vargp not used.
 */
void *Interface_task(void *vargp)
{
	logging("STARTING : interface task\n");
	writing_control = 0;

	while (1)
	{
		if (!writing_control)
		{

			FILE *cptr = fopen(CONTROL_FILE, "r");

			if (cptr == NULL)
			{
				logging("ERROR: openning file\n");
			}
			else
			{
				if (fgets(buffer, MAX_CHAR_FILE, cptr) != NULL)
				{
					pthread_mutex_lock(&mutex_control_interface);
					strncpy(control_string, buffer, MAX_CHAR_FILE);
					pthread_mutex_unlock(&mutex_control_interface);
				}
				else
				{
					logging("ERROR: file empty\n");
				}
				fclose(cptr);
			}
		}
		else
		{
			if (control_string_write != NULL)
			{
				FILE *cptr = fopen(CONTROL_FILE, "w");
				pthread_mutex_lock(&mutex_control_interface);
				fprintf(cptr, "%s", control_string_write);
				pthread_mutex_unlock(&mutex_control_interface);
				fclose(cptr);
				writing_control = 0;
			}
		}

		pthread_mutex_lock(&mutex_sensor_interface);
		if (sensor_string != NULL)
		{
			FILE *sptr = fopen(SENSOR_FILE, "w");
			fprintf(sptr, "%s", sensor_string);
			fclose(sptr);
		}
		pthread_mutex_unlock(&mutex_sensor_interface);

		sleep(REFRESH_PERIOD_INTERFACE);
	}

	return NULL;
}