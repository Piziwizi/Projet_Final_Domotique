#ifndef CONTROL_H
#define CONTROL_H

#include "define.h"

void set_control_from_json(json_object *json);
control_t extract_a_control_from_json(json_object *json);
uint32_t compare_control(control_t a, control_t b);
void *Control_task(void *id);
void *ControlManager_task(void *vargp);
void *RefreshControl_task(void *id);
void *SearchControl_task(void *id);
void *ReadControl_task(void *id);
void add_control(uint32_t id);
void remove_control(uint32_t id);

#endif