/* @name Sensor
 * @author Toma Gosselin-St-Pierre
 * @brief the application that received the temperature and light
 */
#include "sensor.h"

json_object* sensor_json;

void *SensorManager_task(void *vargp){
	sensor_t* temp_sensor;
	uint32_t i;
	uint32_t match=0;
	uint32_t sensor_id;
	uint32_t exit=0;
	sensor_state_machine_t current_state=ADD_SENSOR;
	sensor_json = json_object_new_array();

	printf("sensor manager task\n");
	while(!exit){
		match=0;
		switch (current_state)
		{
		case REFRESH_SENSORS:
			//call sem of Sensor_task
			for (uint32_t i = 0; i < MAX_SENSORS; i++){
				if(sensor_tab.available[i]==USED){
					sem_post(&(sensor_tab.sensor_sem_tab[i]));
					sleep(0);
				}
			}
			current_state=IDLE;
			break;
		case SEARCH_NEW_SENSORS:
			//routine to find new sensors
			current_state=ADD_SENSOR;
			break;
		case ADD_SENSOR:
			while(!match){
				if(sensor_tab.available[i]==AVAILABLE){
					add_sensor(i);
					match=1;
					i++;
				}
			}
			current_state=IDLE;
			break;
		case REMOVE_SENSOR:
			remove_sensor(sensor_id);
			break;
		case EXIT:
			exit=1;
		case REMOVE_ALL_SENSORS:
			for (uint32_t i = 0; i < MAX_SENSORS; i++){
				if(sensor_tab.available[i]==USED){
					remove_sensor(i);
				}
			}
			current_state=IDLE;
			break;
		case TO_INTERFACE:
		//set the values from the sensors
			pthread_mutex_lock(&mutex_sensor);
			sensor_string = json_object_to_json_string(sensor_json);//todo change test2
			pthread_mutex_unlock(&mutex_sensor);
			current_state=IDLE;
			break;
		case IDLE:
			//todo add task management.
			printf("sleep\n");
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

json_object* get_json_from_sensor(sensor_t sensor){
	json_object* json = json_object_new_object();
	json_object_object_add(json, "id", json_object_new_int(sensor.id));
	json_object_object_add(json, "type", json_object_new_string(SENSOR_TYPE_STRING[sensor.type]));
	json_object_object_add(json, "value", json_object_new_double(sensor.value));
	return json;
}

void *Sensor_task(void *id)
{
	printf("sensor task\n");
	uint32_t task_id=*((uint32_t*)id);
	sensor_t sensor;
	
    // test end
	if ( sem_init(&(sensor_tab.sensor_sem_tab[task_id]), 0, 1) != 0 ){
		return NULL;
  	}

	while(sensor_tab.available[task_id] == USED){

		//get sensor value todo connect communication
		sensor.id=0;
		sensor.type=TEMP;
		sensor.value=24.5; 

		json_object_array_add(sensor_json,get_json_from_sensor(sensor));
		sem_wait(&(sensor_tab.sensor_sem_tab[task_id]));
	}
	sem_destroy(&(sensor_tab.sensor_sem_tab[task_id]));
	pthread_exit(NULL);
}

void add_sensor(uint32_t id){
		sensor_tab.tab[id]=malloc(sizeof(sensor_t));
		sensor_tab.available[id]=USED;
		pthread_create(&(sensor_tab.thread_sensor_tab[id]), NULL, Sensor_task,(uint32_t*) &id);
}

void remove_sensor(uint32_t id){
		free(sensor_tab.tab[id]);
		sensor_tab.available[id]=AVAILABLE;
		sem_post(&(sensor_tab.sensor_sem_tab[id]));
		pthread_join(sensor_tab.thread_sensor_tab[id],NULL);
}