/* @name Sensor
 * @author Toma Gosselin-St-Pierre
 * @brief the application that received the temperature and light
 */
#include "sensor.h"

json_object* test2;

void *Sensor_task(void *vargp)
{
	printf("sensor task\n");
	// test
	test2 = json_object_new_array();
	sensor_t sensor;
	sensor.id=0;
	sensor.type=TEMP;
	sensor.value=24.5; 
	
	json_object_array_add(test2,get_json_from_sensor(sensor));
    // test end
	return NULL;
}

void *SensorManager_task(void *vargp){
	sensor_t* temp_sensor;
	uint32_t i;
	uint32_t match;
	uint32_t sensor_id;
	printf("sensor manager task\n");

	switch (current_state)
	{
	case ADD_SENSOR:
		while(!match){
			if(sensor_tab.available[i]==AVAILABLE){
				sensor_tab.tab[i++]=malloc(sizeof(sensor_t));
				match=1;
				sensor_tab.available[i]=USED;
			}
		}
		break;
	case REMOVE_SENSOR:
		temp_sensor=sensor_tab.tab[sensor_id];
		free(temp_sensor);
		sensor_tab.available[sensor_id]=AVAILABLE;
		break;
	case REFRESH_SENSOR:
		//call sem of Sensor_task
		break;
	case TO_INTERFACE:
	//set the values from the sensors
		pthread_mutex_lock(&mutex_sensor);
		sensor_string = json_object_to_json_string(test2);//todo change test2
		pthread_mutex_unlock(&mutex_sensor);
		break;
	default:
		break;
	}

	return NULL;
}

json_object* get_json_from_sensor(sensor_t sensor){
	json_object* json = json_object_new_object();
	json_object_object_add(json, "id", json_object_new_int(sensor.id));
	json_object_object_add(json, "type", json_object_new_string(SENSOR_TYPE_STRING[sensor.type]));
	json_object_object_add(json, "value", json_object_new_double(sensor.value));
	return json;
}

