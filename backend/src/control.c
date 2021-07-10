/* @name Control
 * @author Toma Gosselin-St-Pierre
 * @brief the application that control the temperature and light
 */

#include "control.h"

json_object *control_json;

void set_control_from_json(json_object *json)
{
	//todo analyse the json object todo
	//and put ti in //control_tab[];
	struct json_object *json_array_obj, *json_id, *json_type, *json_value;
	uint32_t length, i;
	control_t control;

	length = json_object_array_length(json);

	for (i = 0; i < length; i++)
	{
		json_array_obj = json_object_array_get_idx(json, i);
		control = extract_a_control_from_json(json_array_obj);

		//compare if te control is present
		for (uint32_t j = 0; j < MAX_CONTROLS; j++)
		{
			if (control_tab.available[j] == USED)
			{
				if (compare_control(control, *(control_tab.tab[j])))
				{
					*(control_tab.tab[j]) = control;
				}
			}
		}
	}
}

control_t extract_a_control_from_json(json_object *json)
{
	struct json_object *json_id, *json_type, *json_value;
	control_t control;
	module_type_t type;

	json_id = json_object_object_get(json, "id");
	control.id = atoi(json_object_get_string(json_id));
	json_type = json_object_object_get(json, "type");
	for (uint32_t i = 0; i < NUMBER_OF_MODULE_TYPES; i++)
	{
		if (strcmp(MODULE_TYPE_STRING[i], json_object_get_string(json_type)) == 0)
		{
			type = i;
		}
	}
	control.type = type;
	json_value = json_object_object_get(json, "value");
	control.value = atof(json_object_get_string(json_value));
	return control;
}

uint32_t compare_control(control_t a, control_t b)
{
	if ((a.id == b.id) && (a.type == b.type))
	{
		return 1;
	}
	return 0;
}

void *Control_task(void *id)
{
	logging("STARTING : control task\n");
	uint32_t task_id = *((uint32_t *)id);
	free(id);

	// test end
	if (sem_init(&(control_tab.control_sem_tab[task_id]), 0, 1) != 0)
	{
		return NULL;
	}

	while (control_tab.available[task_id] == USED)
	{
		sem_wait(&(control_tab.control_sem_tab[task_id]));

		//read the control

		//todo send the value to the control to the control esp32
	}
	sem_destroy(&(control_tab.control_sem_tab[task_id]));
	pthread_exit(NULL);
}

void *ControlManager_task(void *vargp)
{
	logging("STARTING : control manager task\n");

	pthread_mutex_lock(&mutex_control_interface);
	//get the controls
	pthread_mutex_unlock(&mutex_control_interface);

	return NULL;
}

void *RefreshControl_task(void *id)
{
	logging("STARTING : refresh control task\n");

	while (1)
	{
		for (uint32_t i = 0; i < MAX_CONTROLS; i++)
		{
			if (control_tab.available[i] == USED)
			{
				sem_post(&(control_tab.control_sem_tab[i]));
			}
		}

		if (/*timeout*/ 0)
		{
			//remove_control(sensor_id);
		}
		sleep(REFRESH_PERIOD_CONTROL);
	}
	pthread_exit(NULL);
}

void *SearchControl_task(void *id)
{
	uint32_t i;
	uint32_t match = 0;
	logging("STARTING : search control task\n");
	//todo remove test variables
	add_control(0);
	control_tab.tab[0]->id = 0;
	control_tab.tab[0]->type = TEMP;
	add_control(1);
	control_tab.tab[1]->id = 1;
	control_tab.tab[1]->type = LIGHT;

	while (1)
	{

		//routine to find new controls
		//if(newsensor){
		//current_state_sensor = ADD_SENSOR;
		//}

		//add control
		/*
		while (!match)
		{
			if (control_tab.available[i] == AVAILABLE)
			{
				add_control(i);
				match = 1;
				i++;
			}
		}
		*/
		sleep(REFRESH_PERIOD_SEARCH_CONTROL);
	}
	pthread_exit(NULL);
}

void *ReadControl_task(void *id)
{
	logging("STARTING : read control task\n");
	while (1)
	{
		pthread_mutex_lock(&mutex_control_interface);
		control_json = json_tokener_parse(control_string); //todo change test2
		pthread_mutex_unlock(&mutex_control_interface);	   //potiential memory leak
		if (control_json != NULL)
		{
			set_control_from_json(control_json);
		}
		printf("%f\n", control_tab.tab[0]->value); // todo remove that
		sleep(REFRESH_PERIOD_INTERFACE);
	}
	pthread_exit(NULL);
}

void add_control(uint32_t id)
{
	control_tab.tab[id] = malloc(sizeof(control_t));
	control_tab.available[id] = USED;
	uint32_t *task_id = malloc(sizeof(uint32_t));
	*task_id = id;
	pthread_create(&(control_tab.thread_control_tab[id]), NULL, Control_task, task_id);
}

void remove_control(uint32_t id)
{
	free(control_tab.tab[id]);
	control_tab.available[id] = AVAILABLE;
	sem_post(&(control_tab.control_sem_tab[id]));
	pthread_join(control_tab.thread_control_tab[id], NULL);
}