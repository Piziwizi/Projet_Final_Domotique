/* @name Sensor
 * @author Toma Gosselin-St-Pierre
 * @brief the application that received the temperature and light
 */
#include "sensor.h"

void *Sensor_task(void *vargp)
{
	printf("sensor task\n");
	return NULL;
}

void *SensorManager_task(void *vargp)
{
	printf("sensor manager task\n");
	return NULL;
}