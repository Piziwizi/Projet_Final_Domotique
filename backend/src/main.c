/* @name main.c
 * @author Toma Gosselin-St-Pierre
 * @brief main application of the backend
 */
#include "define.h"
#include "control.h"
#include "interface.h"
#include "logging.h"
#include "sensor.h"

int main()
{
	pthread_t thread_sensor;
	pthread_t thread_sensor_manager;
	pthread_t thread_control;
	pthread_t thread_control_manager;
	pthread_t thread_interface;
	pthread_t thread_logging;
	printf("Before Thread\n");
	pthread_create(&thread_sensor, NULL, Sensor_task, NULL);
	pthread_create(&thread_sensor_manager, NULL, SensorManager_task, NULL);
	pthread_create(&thread_control, NULL, Control_task, NULL);
	pthread_create(&thread_control_manager, NULL, ControlManager_task, NULL);
	pthread_create(&thread_interface, NULL, Interface_task, NULL);
	pthread_create(&thread_logging, NULL, Logging_task, NULL);

	pthread_join(thread_sensor, NULL);
	pthread_join(thread_sensor_manager, NULL);
	pthread_join(thread_control, NULL);
	pthread_join(thread_control_manager, NULL);
	pthread_join(thread_interface, NULL);
	pthread_join(thread_logging, NULL);
	printf("After Thread\n");
	exit(0);
}