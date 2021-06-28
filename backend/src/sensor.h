#ifndef SENSOR_H
#define SENSOR_H

#include "define.h"

void *Sensor_task(void *vargp);
void *SensorManager_task(void *vargp);
json_object* get_json_from_sensor(sensor_t sensor);

#endif