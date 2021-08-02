#ifndef SENSOR_H
#define SENSOR_H

#include "define.h"

void *Sensor_task(void *vargp);
void *SearchSensor_task(void *vargp);
void *SaveSensor_task(void *vargp);
void *ReadControl_task(void *id);
void *SaveControl_task(void *vargp);
void add_sensor(uint32_t id, in_addr_t ip);
void remove_sensor(uint32_t id);
void add_control(uint32_t id);
void remove_control(uint32_t id);
json_object *get_json_from_sensor(sensor_t sensor);
json_object *get_json_from_control(control_t control);
void set_control_from_json(json_object *json);
control_t extract_a_control_from_json(json_object *json);
uint32_t compare_control(control_t a, control_t b);

#endif