/* @name Sensor
 * @author Toma Gosselin-St-Pierre
 * @brief the application that received the temperature and light
 */
#include "sensor.h"

json_object *sensor_json;
/*
void *SensorManager_task(void *vargp)
{
	sensor_t *temp_sensor;
	uint32_t i;
	uint32_t match = 0;
	uint32_t sensor_id;
	uint32_t exit = 0;
	sensor_state_machine_t current_state_sensor = TO_INTERFACE; //todo change that to idle
	control_json = json_object_new_array();

	logging("STARTING : sensor manager task\n");
	while (!exit)
	{
		
	}
	pthread_exit(NULL);
	return NULL;
}
*/
json_object *get_json_from_sensor(sensor_t sensor)
{
	json_object *json = json_object_new_object();
	json_object_object_add(json, "id", json_object_new_int(sensor.id));
	json_object_object_add(json, "type", json_object_new_string(SENSOR_TYPE_STRING[sensor.type]));
	json_object_object_add(json, "value", json_object_new_double(sensor.value));
	return json;
}

void *Sensor_task(void *id)
{
	logging("STARTING : sensor task\n");
	uint32_t task_id = *((uint32_t *)id);
	free(id);
	sensor_t sensor;

	// test end
	if (sem_init(&(sensor_tab.sensor_sem_tab[task_id]), 0, 1) != 0)
	{
		return NULL;
	}

	while (sensor_tab.available[task_id] == USED)
	{
		sem_wait(&(sensor_tab.sensor_sem_tab[task_id]));
		//get sensor value todo connect communication
		sensor.id = 0;
		sensor.type = TEMP;
		sensor.value = 24.5;

		//add array
		//json_object_array_add(sensor_json, get_json_from_sensor(sensor));
	}
	sem_destroy(&(sensor_tab.sensor_sem_tab[task_id]));
	pthread_exit(NULL);
}

void *RefreshSensor_task(void *id)
{
	logging("STARTING : refresh sensor task\n");

	while (1)
	{
		for (uint32_t i = 0; i < MAX_SENSORS; i++)
		{
			if (sensor_tab.available[i] == USED)
			{
				sem_post(&(sensor_tab.sensor_sem_tab[i]));
			}
		}

		if (/*timeout*/ 0)
		{
			//remove_sensor(sensor_id);
		}
		sleep(REFRESH_PERIOD_SENSOR);
	}
	pthread_exit(NULL);
}

void *SearchSensor_task(void *id)
{

	uint32_t i;
	uint32_t match = 0;
	logging("STARTING : search sensor task\n");

	while (1)
	{

		//routine to find new sensors
		//if(newsensor){
		//current_state_sensor = ADD_SENSOR;
		//}

		//add sensor
		while (!match)
		{
			if (sensor_tab.available[i] == AVAILABLE)
			{
				add_sensor(i);
				match = 1;
				i++;
			}
		}

		sleep(REFRESH_PERIOD_SEARCH_SENSOR);
	}
	pthread_exit(NULL);
}

void *SaveSensor_task(void *id)
{
	logging("STARTING : save sensor task\n");
	sensor_json = json_object_new_array();

	while (1)
	{
		//set the values from the sensors
		pthread_mutex_lock(&mutex_sensor);
		sensor_string = json_object_to_json_string(sensor_json); //todo change test2
		pthread_mutex_unlock(&mutex_sensor);
		sleep(REFRESH_PERIOD_INTERFACE);
	}
	pthread_exit(NULL);
}

void add_sensor(uint32_t id)
{
	sensor_tab.tab[id] = malloc(sizeof(sensor_t));
	sensor_tab.available[id] = USED;
	uint32_t *task_id = malloc(sizeof(uint32_t));
	*task_id = id;
	pthread_create(&(sensor_tab.thread_sensor_tab[id]), NULL, Sensor_task, task_id);
}

void remove_sensor(uint32_t id)
{
	free(sensor_tab.tab[id]);
	sensor_tab.available[id] = AVAILABLE;
	sem_post(&(sensor_tab.sensor_sem_tab[id]));
	pthread_join(sensor_tab.thread_sensor_tab[id], NULL);
}
