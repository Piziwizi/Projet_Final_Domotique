/* @name Interface
 * @author Toma Gosselin-St-Pierre
 * @brief task that write the json file for the interface.
 */

#include "interface.h"
#include "json-c/json_object.h"
#include "json-c/json_tokener.h"
#define BASE_STRING "{\"get\":[{\"id\":\"0\",\"type\":\"temperature\",\"value\":\"25\"},{\"id\":\"1\",\"type\":\"light\",\"value\":\"1\"}],\"set\":[{\"id\":\"0\",\"type\":\"temperature\",\"value\":\"25\"},{\"id\":\"1\",\"type\":\"light\",\"value\":\"1\"}]}"

void *Interface_task(void *vargp)
{
	json_object* test = json_object_new_object();	
	test = json_tokener_parse(BASE_STRING);
	printf("interface task\n");
	return NULL;
}