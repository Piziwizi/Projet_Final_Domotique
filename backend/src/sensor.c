/* @name Sensor
 * @author Toma Gosselin-St-Pierre
 * @brief the application that received the temperature and light
 */
#include "sensor.h"

json_object *sensor_json;
json_object *control_json;

void *SensorManager_task(void *vargp)
{
	sensor_t *temp_sensor;
	uint32_t i;
	uint32_t match = 0;
	uint32_t sensor_id;
	uint32_t exit = 0;
	sensor_state_machine_t current_state = FROM_INTERFACE; //todo change that to idle
	sensor_json = json_object_new_array();
	control_json = json_object_new_array();

	printf("STARTING : sensor manager task\n");
	while (!exit)
	{
		match = 0;
		switch (current_state)
		{
		case REFRESH_SENSORS:
			printf("STATE SENSOR : refresh\n");
			//call sem of Sensor_task
			for (uint32_t i = 0; i < MAX_SENSORS; i++)
			{
				if (sensor_tab.available[i] == USED)
				{
					printf("test3\n");
					sem_post(&(sensor_tab.sensor_sem_tab[i]));
				}
			}
			current_state = IDLE;
			break;
		case SEARCH_NEW_SENSORS:
			printf("STATE SENSOR : search\n");
			//routine to find new sensors
			//if(newsensor){
			current_state = ADD_SENSOR;
			//}
			break;
		case ADD_SENSOR:
			printf("STATE SENSOR : add sensor\n");
			while (!match)
			{
				if (sensor_tab.available[i] == AVAILABLE)
				{
					add_sensor(i);
					match = 1;
					i++;
				}
			}
			current_state = IDLE;
			break;
		case REMOVE_SENSOR:
			printf("STATE SENSOR : remove sensor\n");
			remove_sensor(sensor_id);
			break;
		case EXIT:
			exit = 1;
		case REMOVE_ALL_SENSORS:
			printf("STATE SENSOR : remove all sensor\n");
			for (uint32_t i = 0; i < MAX_SENSORS; i++)
			{
				if (sensor_tab.available[i] == USED)
				{
					remove_sensor(i);
				}
			}
			current_state = IDLE;
			break;
		case TO_INTERFACE:
			//set the values from the sensors
			printf("STATE SENSOR : to interface\n");
			pthread_mutex_lock(&mutex_sensor);
			sensor_string = json_object_to_json_string(sensor_json); //todo change test2
			pthread_mutex_unlock(&mutex_sensor);
			current_state = FROM_INTERFACE; //todo change to idle
			break;
		case FROM_INTERFACE: //todo move to control
							 //put the values to sensors
			printf("STATE SENSOR : from interface\n");
			pthread_mutex_lock(&mutex_control);
			control_json = json_tokener_parse(control_string); //todo change test2
			pthread_mutex_unlock(&mutex_control);			   //potiential memory leak
			json_object *temp = control_json;
			current_state = TO_INTERFACE; //todo change to idle
			break;
		case IDLE:
			//todo add task management.
			printf("STATE SENSOR : idle\n");
			sleep(3);
			current_state = REFRESH_SENSORS;
			break;
		default:
			break;
		}
	}
	pthread_exit(NULL);
	return NULL;
}

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
	printf("STARTING : sensor task\n");
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

		//get sensor value todo connect communication
		sensor.id = 0;
		sensor.type = TEMP;
		sensor.value = 24.5;

		json_object_array_add(sensor_json, get_json_from_sensor(sensor));
		sem_wait(&(sensor_tab.sensor_sem_tab[task_id]));
	}
	sem_destroy(&(sensor_tab.sensor_sem_tab[task_id]));
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
