/* @name Sensor
 * @author Toma Gosselin-St-Pierre
 * @brief the application that received the temperature and light
 */
#include "sensor.h"

#define BUFFER_LENGHT 1024
/*
void *SensorManager_task(void *vargp)
{
	sensor_t *temp_sensor;
	uint32_t i;
	uint32_t match = 0;
	uint32_t sensor_id;
	uint32_t exit = 0;
	sensor_state_machine_t current_state_sensor = TO_INTERFACE; //todo change that to idle
	control_json = json_object_new_array();

	logging("STARTING : sensor manager task\n");
	while (!exit)
	{
		
	}
	pthread_exit(NULL);
	return NULL;
}
*/
json_object *get_json_from_sensor(sensor_t sensor)
{
	json_object *json = json_object_new_object();
	json_object_object_add(json, "id", json_object_new_int(sensor.id));
	json_object_object_add(json, "type", json_object_new_string(MODULE_TYPE_STRING[sensor.type]));
	json_object_object_add(json, "value", json_object_new_double(sensor.value));
	return json;
}

void *Sensor_task(void *id)
{
	logging("STARTING : sensor task\n");
	uint32_t task_id = *((uint32_t *)id);
	free(id);
	sensor_t sensor;

	char *msg = "SENSOR:TEMP:GET\r";
	char *msg2 = "CONTROL:TEMP:SET:0\r";
	char *msg3 = "CONTROL:TEMP:SET:1\r";
	char *msg4 = "CONTROL:LIGHT:SET:0\r";
	char *msg5 = "CONTROL:LIGHT:SET:1\r";
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

	// test end
	if (sem_init(&(sensor_tab.sensor_sem_tab[task_id]), 0, 1) != 0)
	{
		return NULL;
	}
	int consocket = -1;
	while (sensor_tab.available[task_id] == USED)
	{
		sem_wait(&(sensor_tab.sensor_sem_tab[task_id]));
		sleep(5);
		if (consocket == 0)
		{
			send(mysocket, msg, strlen(msg), 0);
			read(mysocket, buf, BUFFER_LENGHT);
			sensor.id = 0;
			sensor.type = TEMP;
			sensor.value = atof(buf);
			printf("refresh sensor %f\n", sensor.value);
			*(sensor_tab.tab[task_id]) = sensor;

			if ((sensor.value > (control_tab.tab[0]->value)) || isnan(sensor.value))
			{ //todo change the 0
				send(mysocket, msg2, strlen(msg2), 0);
			}
			else if (sensor.value < (control_tab.tab[0]->value - 0.3))
			{
				send(mysocket, msg3, strlen(msg3), 0);
			}

			if ((control_tab.tab[1]->value) == 1)
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
		sem_post(&(sensor_tab.sensor_sem_tab[task_id])); //todo remove after test
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

void *RefreshSensor_task(void *id)
{
	logging("STARTING : refresh sensor task\n");

	while (1)
	{
		for (uint32_t i = 0; i < MAX_SENSORS; i++)
		{
			if (sensor_tab.available[i] == USED)
			{
				sem_post(&(sensor_tab.sensor_sem_tab[i]));
			}
		}

		if (/*timeout*/ 0)
		{
			//remove_sensor(sensor_id);
		}
		sleep(REFRESH_PERIOD_SENSOR);
	}
	pthread_exit(NULL);
}

void *SearchSensor_task(void *id)
{
	uint32_t already_registered = 0;
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

					uint32_t i = 0;
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
				json_object_array_add(sensor_json, get_json_from_sensor(*(sensor_tab.tab[i])));
			}
		}
		pthread_mutex_unlock(&mutex_sensor_tab);

		//set the values from the sensors

		pthread_mutex_lock(&mutex_sensor_interface);
		sensor_string = json_object_to_json_string(sensor_json); //todo change test2
		pthread_mutex_unlock(&mutex_sensor_interface);
		sleep(REFRESH_PERIOD_INTERFACE);
	}
	pthread_exit(NULL);
}

void add_sensor(uint32_t id, in_addr_t ip)
{
	sensor_tab.tab[id] = malloc(sizeof(sensor_t));
	sensor_tab.available[id] = USED;
	uint32_t *task_id = malloc(sizeof(uint32_t));
	sensor_tab.ip[id] = ip;
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
