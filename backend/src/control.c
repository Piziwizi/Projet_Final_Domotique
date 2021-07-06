/* @name Control
 * @author Toma Gosselin-St-Pierre
 * @brief the application that control the temperature and light
 */

#include "control.h"

void *Control_task(void *vargp)
{
	logging("STARTING : control task\n");
	return NULL;
}

void *ControlManager_task(void *vargp)
{
	logging("STARTING : control manager task\n");

	pthread_mutex_lock(&mutex_control);
	//get the controls
	pthread_mutex_unlock(&mutex_control);

	return NULL;
}