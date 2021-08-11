/* @name Sensor
 * @author Toma Gosselin-St-Pierre
 * @brief the application that received the temperature and light
 */
#include "sensor.h"

#define BUFFER_LENGHT 1024
json_object *control_json;

/**
 * @name Sensor Task
 * @brief Communicate with sensors and update everything
 * @param id the task id in the structure
 */
void *Sensor_task(void *id)
{
	logging("STARTING : sensor task\n");
	uint32_t task_id = *((uint32_t *)id);
	uint32_t control_id = task_id * 2;
	free(id);
	sensor_t sensor;
	control_t control_temp;
	control_t control_light;

	int32_t offset_temp = 0;
	uint32_t offset_light = 0;

	sensor.type = TEMP;

	control_temp.type = TEMP;
	control_light.type = LIGHT;

	char *msg6 = "SYSTEM:ID\r";
	char *msg = "SENSOR:TEMP:GET\r";
	char *msg2 = "CONTROL:TEMP:SET:0\r";
	char *msg3 = "CONTROL:TEMP:SET:1\r";
	char *msg4 = "CONTROL:LIGHT:SET:0\r";
	char *msg5 = "CONTROL:LIGHT:SET:1\r";

	char *msg7 = "BUTTON:LIGHT:GET\r";
	char *msg8 = "BUTTON:TEMP:GET\r";
	char buf[BUFFER_LENGHT];

	//wifi config
	int len, mysocket, filelenght;
	struct sockaddr_in dest;

	mysocket = socket(AF_INET, SOCK_STREAM, 0); /* Opret socket */

	memset(&dest, 0, sizeof(dest));				   /* Zero the struct */
	dest.sin_family = AF_INET;					   /* TCP protocol */
	dest.sin_addr.s_addr = sensor_tab.ip[task_id]; /* Set destination IP number */
	dest.sin_port = htons(3334);				   /* Set destination port number */

	/* Connect to server */
	bind(mysocket, (struct sockaddr *)&dest, sizeof(struct sockaddr));
	//---

	int consocket = -1;
	while (sensor_tab.available[task_id] == USED)
	{
		//sem_wait(&(sensor_tab.sensor_sem_tab[task_id]));
		usleep(1);
		if (consocket == 0)
		{
			memset(buf, 0, BUFFER_LENGHT);

			send(mysocket, msg7, strlen(msg7), 0);
			read(mysocket, buf, BUFFER_LENGHT);
			offset_light = atoi(buf);

			memset(buf, 0, BUFFER_LENGHT);

			send(mysocket, msg8, strlen(msg8), 0);
			read(mysocket, buf, BUFFER_LENGHT);
			offset_temp = atoi(buf);

			if (offset_light != 0 || offset_temp != 0)
			{

				control_tab.tab[control_id]->value += offset_temp;
				control_tab.tab[control_id + 1]->value = ((uint32_t)(control_tab.tab[control_id + 1]->value) + offset_light) % 2;

				offset_temp = 0;
				offset_light = 0;

				writing_control = 1;

				sem_post(&control_sem_save);
				sleep(0);
			}

			memset(buf, 0, BUFFER_LENGHT);

			send(mysocket, msg6, strlen(msg6), 0);
			read(mysocket, buf, BUFFER_LENGHT);
			sensor.id = atoi(buf);

			control_temp.id = sensor.id;
			control_light.id = sensor.id;

			control_tab.tab[control_id]->id = control_temp.id;
			control_tab.tab[control_id]->type = control_temp.type;
			control_tab.tab[control_id + 1]->id = control_light.id;
			control_tab.tab[control_id + 1]->type = control_light.type;

			memset(buf, 0, BUFFER_LENGHT);

			send(mysocket, msg, strlen(msg), 0);
			read(mysocket, buf, BUFFER_LENGHT);
			sensor.value = atof(buf);

			printf("refresh sensor %d:%f\n", sensor.id, sensor.value);
			*(sensor_tab.tab[task_id]) = sensor;
			sem_post(&(sensor_tab.sensor_sem_tab[task_id]));

			if ((sensor.value > (control_tab.tab[control_id]->value)) || isnan(sensor.value))
			{ //todo change the 0
				send(mysocket, msg2, strlen(msg2), 0);
			}
			else if (sensor.value < (control_tab.tab[control_id]->value - HYSTERESIS_VALUE))
			{
				send(mysocket, msg3, strlen(msg3), 0);
			}

			if ((control_tab.tab[control_id + 1]->value) == 1)
			{ //todo change the 0
				send(mysocket, msg5, strlen(msg5), 0);
			}
			else
			{
				send(mysocket, msg4, strlen(msg4), 0);
			}
		}
		else
		{
			consocket = connect(mysocket, (struct sockaddr *)&dest, sizeof(struct sockaddr));
		}
		/*
		if (task_id == 0)
		{ // todo remove test sensors
			sensor.id = 0;
			sensor.type = TEMP;
			sensor.value = 18.0;

			pthread_mutex_lock(&mutex_sensor_tab);
			*(sensor_tab.tab[task_id]) = sensor;
			pthread_mutex_unlock(&mutex_sensor_tab);
		}
		if (task_id == 1)
		{ // todo remove test sensors
			sensor.id = 1;
			sensor.type = LIGHT;
			sensor.value = 24.5;

			pthread_mutex_lock(&mutex_sensor_tab);
			*(sensor_tab.tab[task_id]) = sensor;
			pthread_mutex_unlock(&mutex_sensor_tab);
		}*/
	}
	sem_destroy(&(sensor_tab.sensor_sem_tab[task_id]));
	pthread_exit(NULL);
}

/**
 * @name Search Sensor Task
 * @brief Search for new sensors and create a sensor task for each
 * @param id do nothing
 */
void *SearchSensor_task(void *id)
{
	uint32_t already_registered = 0;
	uint32_t i = 0;
	char *msg = "SYSTEM:START";
	char buf[BUFFER_LENGHT];
	//wifi
	struct sockaddr_in dest; /* socket info about the machine connecting to us */
	struct sockaddr_in serv; /* socket info about our server */
	int mysocket;			 /* socket used to listen for incoming connections */
	socklen_t socksize = sizeof(struct sockaddr_in);

	memset(&serv, 0, sizeof(serv));			  /* zero the struct before filling the fields */
	serv.sin_family = AF_INET;				  /* set the type of connection to TCP/IP */
	serv.sin_addr.s_addr = htonl(INADDR_ANY); /* set our address to any interface */
	serv.sin_port = htons(3333);			  /* set the server port number */

	mysocket = socket(AF_INET, SOCK_STREAM, 0);
	shutdown(mysocket, 0);

	/* bind serv information to mysocket */
	bind(mysocket, (struct sockaddr *)&serv, sizeof(struct sockaddr));

	//---
	logging("STARTING : search sensor task\n");

	//todo remove test variables
	//add_sensor(0);

	//add_sensor(1);

	while (1)
	{

		/* start listening, allowing a queue of up to 1 pending connection */
		if (listen(mysocket, 64) == 0)
		{
			int consocket = accept(mysocket, (struct sockaddr *)&dest, &socksize);

			if (consocket)
			{
				already_registered = 0;
				for (uint32_t i = 0; i < MAX_SENSORS; i++)
				{
					if (sensor_tab.ip[i] == dest.sin_addr.s_addr)
					{
						already_registered = 1;
					}
				}
				if (!already_registered)
				{
					printf("Sensor available at %s\n", inet_ntoa(dest.sin_addr));

					uint32_t match = 0;
					while (!match)
					{
						if (sensor_tab.available[i] == AVAILABLE)
						{
							match = 1;
							send(consocket, msg, strlen(msg), 0);
							sleep(5);
							read(consocket, buf, BUFFER_LENGHT);
							if (atoi(buf) == 1)
							{
								printf("Sensor Added\n");
								add_sensor(i, dest.sin_addr.s_addr);
								add_control(i * 2);
								add_control((i * 2) + 1);
								i++;
							}
						}
					}
				}
				else
				{
					char *dummy = "test";
					send(consocket, dummy, strlen(dummy), 0);
				}
				close(consocket);
			}
		}
		sleep(REFRESH_PERIOD_SEARCH_SENSOR);
	}
	close(mysocket);

	//routine to find new sensors
	//if(newsensor){
	//current_state_sensor = ADD_SENSOR;
	//}

	//add sensor

	/*
		while (!match)
		{
			if (sensor_tab.available[i] == AVAILABLE)
			{
				add_sensor(i);
				match = 1;
				i++;
			}
		}
		*/
	pthread_exit(NULL);
}

/**
 * @name Save Sensor Task
 * @brief Check all available sensor avec create sensor_string for the interface
 * @param id do nothing
 */
void *SaveSensor_task(void *id)
{
	logging("STARTING : save sensor task\n");
	json_object *sensor_json;

	while (1)
	{
		sensor_json = json_object_new_array();

		pthread_mutex_lock(&mutex_sensor_tab);
		for (uint32_t i = 0; i < MAX_SENSORS; i++)
		{
			if (sensor_tab.available[i] == USED)
			{
				sem_wait(&sensor_tab.sensor_sem_tab[i]);
				json_object_array_add(sensor_json, get_json_from_sensor(*(sensor_tab.tab[i])));
			}
		}
		pthread_mutex_unlock(&mutex_sensor_tab);

		//set the values from the sensors

		if (json_object_to_json_string(sensor_json) != NULL)
		{
			pthread_mutex_lock(&mutex_sensor_interface);
			strcpy(sensor_string, json_object_to_json_string(sensor_json)); //todo change test2
			pthread_mutex_unlock(&mutex_sensor_interface);
			json_object_put(sensor_json);
		}
		sleep(REFRESH_PERIOD_INTERFACE);
	}
	pthread_exit(NULL);
}

/**
 * @name Read Control Task
 * @brief Take control_string and extract all available controls
 * @param id do nothing
 */
void *ReadControl_task(void *id)
{
	logging("STARTING : read control task\n");
	while (1)
	{
		pthread_mutex_lock(&mutex_control_interface);
		control_json = json_tokener_parse(control_string);
		pthread_mutex_unlock(&mutex_control_interface);
		if (control_json != NULL)
		{
			set_control_from_json(control_json);
			json_object_put(control_json);
		}
		sleep(REFRESH_PERIOD_INTERFACE);
	}
	pthread_exit(NULL);
}

/**
 * @name Save Control Task
 * @brief Check all available control avec create control_string_write for the interface
 * @param id do nothing
 */
void *SaveControl_task(void *id)
{
	logging("STARTING : save control task\n");
	json_object *control_json;

	while (1)
	{
		sem_wait(&control_sem_save);
		control_json = json_object_new_array();

		pthread_mutex_lock(&mutex_control_tab);
		for (uint32_t i = 0; i < MAX_SENSORS; i++)
		{
			if (control_tab.available[i] == USED)
			{
				json_object_array_add(control_json, get_json_from_control(*(control_tab.tab[i])));
			}
		}
		pthread_mutex_unlock(&mutex_control_tab);

		//set the values from the control

		if (json_object_to_json_string(control_json) != NULL)
		{
			pthread_mutex_lock(&mutex_control_interface);
			strcpy(control_string_write, json_object_to_json_string(control_json)); //todo change test2
			pthread_mutex_unlock(&mutex_control_interface);
			json_object_put(control_json);
		}
		sleep(REFRESH_PERIOD_INTERFACE);
	}
	pthread_exit(NULL);
}

/**
 * @name add_sensor
 * @brief Configure the add sensor task and start it
 * @param id the id of the task in memory
 * @param ip the ip of the new sensors 
 */
void add_sensor(uint32_t id, in_addr_t ip)
{
	sensor_tab.tab[id] = malloc(sizeof(sensor_t));
	sensor_tab.available[id] = USED;
	uint32_t *task_id = malloc(sizeof(uint32_t));
	sensor_tab.ip[id] = ip;
	*task_id = id;
	pthread_create(&(sensor_tab.thread_sensor_tab[id]), NULL, Sensor_task, task_id);
}

/**
 * @name remove_sensor
 * @brief Remove the sensor by an id
 * @param id the id of the task sensor to delete
 */
void remove_sensor(uint32_t id)
{
	free(sensor_tab.tab[id]);
	sensor_tab.available[id] = AVAILABLE;
	sem_post(&(sensor_tab.sensor_sem_tab[id]));
	pthread_join(sensor_tab.thread_sensor_tab[id], NULL);
}

/**
 * @name add_control
 * @brief Add a control in the control_tab
 * @param id the id of the task in memory
 */
void add_control(uint32_t id)
{
	control_tab.tab[id] = malloc(sizeof(control_t));
	control_tab.available[id] = USED;
	uint32_t *task_id = malloc(sizeof(uint32_t));
	*task_id = id;
}

/**
 * @name remove_control
 * @brief Remove the control by an id
 * @param id the id of the control to delete
 */
void remove_control(uint32_t id)
{
	free(control_tab.tab[id]);
	control_tab.available[id] = AVAILABLE;
}

/**
 * @name get_json_from_sensor
 * @brief Take a sensor and return a json object
 * @param sensor The sensor we want to transform in json
 * @return json_object of the sensor
 */
json_object *get_json_from_sensor(sensor_t sensor)
{
	json_object *json = json_object_new_object();
	json_object_object_add(json, "id", json_object_new_int(sensor.id));
	json_object_object_add(json, "type", json_object_new_string(MODULE_TYPE_STRING[sensor.type]));
	json_object_object_add(json, "value", json_object_new_double(sensor.value));
	return json;
}

/**
 * @name get_json_from_control
 * @brief Take a control and return a json object
 * @param control The control we want to transform in json
 * @return json_object of the control
 */
json_object *get_json_from_control(control_t control)
{
	json_object *json = json_object_new_object();
	json_object_object_add(json, "id", json_object_new_int(control.id));
	json_object_object_add(json, "type", json_object_new_string(MODULE_TYPE_STRING[control.type]));
	json_object_object_add(json, "value", json_object_new_double(control.value));
	return json;
}

/**
 * @name set_control_from_json
 * @brief Set controls from a json
 * @param json The json we want to set the controls from
 */
void set_control_from_json(json_object *json)
{
	struct json_object *json_array_obj;
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

/**
 * @name extract_a_control_from_json
 * @brief extract a control from a json object
 * @param json The json with one control in it
 * @return the resulting control_t
 */
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

/**
 * @name compare_control
 * @brief compare 2 controls if they have the same type and id
 * @param a the first control
 * @param b the second control
 * @return 1 if the same, 0 if different
 */
uint32_t compare_control(control_t a, control_t b)
{
	if ((a.id == b.id) && (a.type == b.type))
	{
		return 1;
	}
	return 0;
}
