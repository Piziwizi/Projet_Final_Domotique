/* @name Interface
 * @author Toma Gosselin-St-Pierre
 * @brief task that write the json file for the interface.
 */

#include "interface.h"

void *Interface_task(void *vargp)
{
	printf("interface task\n");
	pthread_mutex_lock(&mutex_control);
	//set the controls
	//will send string

	FILE *cptr=fopen(CONTROL_FILE,"r");
	if(cptr == NULL){
		cptr=fopen(CONTROL_FILE,"w");
	}
	fclose(cptr);

	pthread_mutex_unlock(&mutex_control);

	pthread_mutex_lock(&mutex_sensor);
	//get the values from the sensors
	//will receive string
	if(sensor_string != NULL){
		FILE *sptr=fopen(SENSOR_FILE,"w");
		fprintf(sptr,"%s",sensor_string);
		fclose(sptr);
	}
	pthread_mutex_unlock(&mutex_sensor);

	return NULL;
}