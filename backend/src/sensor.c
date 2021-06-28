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

void *SensorManager_task(void *vargp)
{
	printf("sensor manager task\n");

	pthread_mutex_lock(&mutex_sensor);
	//set the values from the sensors
	sensor_string = json_object_to_json_string(test2);
	pthread_mutex_unlock(&mutex_sensor);
	return NULL;
}

json_object* get_json_from_sensor(sensor_t sensor){
	json_object* json = json_object_new_object();
	json_object_object_add(json, "id", json_object_new_int(sensor.id));
	json_object_object_add(json, "type", json_object_new_string(SENSOR_TYPE_STRING[sensor.type]));
	json_object_object_add(json, "value", json_object_new_double(sensor.value));
	return json;
}

