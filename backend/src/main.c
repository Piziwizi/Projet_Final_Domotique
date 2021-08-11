/* @name main.c
 * @author Toma Gosselin-St-Pierre
 * @brief main application of the backend
 */
#include "define.h"
#include "interface.h"
#include "logging.h"
#include "sensor.h"
#include "test.c"

void init_pthread(void);
void stop_pthread(void);

/**
 * @name main
 * @brief Main program, start everything.
 */
int main()
{
	int stop = 0;
	for (uint32_t i = 0; i < MAX_SENSORS; i++)
	{
		sensor_tab.available[i] = AVAILABLE; // test end
		// test end
		if (sem_init(&(sensor_tab.sensor_sem_tab[i]), 0, 0) != 0)
		{
			exit(1);
		}
	}
	for (uint32_t i = 0; i < MAX_CONTROLS; i++)
	{
		control_tab.available[i] = AVAILABLE;
	}
	init_pthread();
	while (!stop)
		;
	stop_pthread();
	printf("end\n");
	exit(0);
}

/**
 * @name init_pthread
 * @brief Init all pthread functions.
 */
void init_pthread(void)
{

	if (pthread_mutex_init(&mutex_sensor_interface, NULL) != 0)
	{
		printf("mutex init failed\n");
		return;
	}
	if (pthread_mutex_init(&mutex_control_interface, NULL) != 0)
	{
		printf("mutex init failed\n");
		return;
	}
	if (pthread_mutex_init(&mutex_sensor_tab, NULL) != 0)
	{
		printf("mutex init failed\n");
		return;
	}
	if (pthread_mutex_init(&mutex_control_tab, NULL) != 0)
	{
		printf("mutex init failed\n");
		return;
	}
	if (pthread_mutex_init(&mutex_log, NULL) != 0)
	{
		printf("mutex init failed\n");
		return;
	}
	if (sem_init(&control_sem_save, 0, 0) != 0)
	{
		printf("sem save failed\n");
		return;
	}

	pthread_create(&thread_search_sensor, NULL, SearchSensor_task, NULL);
	pthread_create(&thread_save_sensor, NULL, SaveSensor_task, NULL);
	pthread_create(&thread_save_control, NULL, SaveControl_task, NULL);
	pthread_create(&thread_read_control, NULL, ReadControl_task, NULL);
	pthread_create(&thread_interface, NULL, Interface_task, NULL);
	pthread_create(&thread_logging, NULL, Logging_task, NULL);

	pthread_create(&thread_test, NULL, Test_task, NULL);
}

/**
 * @name init_pthread
 * @brief Close all pthread functions.
 */
void stop_pthread(void)
{
	pthread_join(thread_search_sensor, NULL);
	pthread_join(thread_save_sensor, NULL);
	pthread_join(thread_save_control, NULL);
	pthread_join(thread_read_control, NULL);
	pthread_join(thread_interface, NULL);
	pthread_join(thread_logging, NULL);

	pthread_join(thread_test, NULL);
}