/* @name Interface
 * @author Toma Gosselin-St-Pierre
 * @brief task that write the json file for the interface.
 */

#include "interface.h"

void *Interface_task(void *vargp)
{
	logging("STARTING : interface task\n");
	//set the controls
	//will send string

	while (1)
	{
		FILE *cptr = fopen(CONTROL_FILE, "r");

		pthread_mutex_lock(&mutex_control_interface);
		if (cptr != NULL)
		{
			if (fgets(control_string, MAX_CHAR_FILE, cptr) == NULL)
			{
				logging("ERROR: Control file too large\n");
			}
			fclose(cptr);
		}
		pthread_mutex_unlock(&mutex_control_interface);

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