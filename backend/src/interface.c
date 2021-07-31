/* @name Interface
 * @author Toma Gosselin-St-Pierre
 * @brief task that write the json file for the interface.
 */

#include "interface.h"
#define BUFFER_LENGHT 1024
char buffer[MAX_CHAR_FILE];

void *Interface_task(void *vargp)
{
	logging("STARTING : interface task\n");
	//set the controls
	//will send string

	while (1)
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
				logging("ERROR: Control file too large\n");
			}
			fclose(cptr);
		}

		pthread_mutex_lock(&mutex_sensor_interface);
		//get the values from the sensors
		//will receive string
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