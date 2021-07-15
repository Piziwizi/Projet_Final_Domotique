#ifndef SENSOR_H
#define SENSOR_H

#include "define.h"

void *Sensor_task(void *vargp);
void *RefreshSensor_task(void *vargp);
void *SearchSensor_task(void *vargp);
void *SaveSensor_task(void *vargp);
json_object *get_json_from_sensor(sensor_t sensor);
void add_sensor(uint32_t id);
void remove_sensor(uint32_t id);

#endif