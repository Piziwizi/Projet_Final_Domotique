/* @name main.c
 * @author Toma Gosselin-St-Pierre
 * @brief main application of the backend
 */
#include "define.h"
#include "control.h"
#include "interface.h"
#include "logging.h"
#include "sensor.h"
#include "test.c"

void init_pthread(void);
void stop_pthread(void);

int main()
{
	for(uint32_t i=0;i<MAX_SENSORS;i++){
		sensor_tab.available[i]=AVAILABLE;
	}
	init_pthread();
	stop_pthread();
    printf("end\n");
	exit(0);
}

void init_pthread(void){

	if (pthread_mutex_init(&mutex_sensor, NULL) != 0){
        printf("mutex init failed\n");
        return;
    }
	if (pthread_mutex_init(&mutex_control, NULL) != 0){
        printf("mutex init failed\n");
        return;
    }

	pthread_create(&thread_sensor_manager, NULL, SensorManager_task, NULL);
	pthread_create(&thread_control, NULL, Control_task, NULL);
	pthread_create(&thread_control_manager, NULL, ControlManager_task, NULL);
	pthread_create(&thread_interface, NULL, Interface_task, NULL);
	pthread_create(&thread_logging, NULL, Logging_task, NULL);

	pthread_create(&thread_test, NULL, Test_task, NULL);
}

void stop_pthread(void){
	pthread_join(thread_sensor_manager, NULL);
	pthread_join(thread_control, NULL);
	pthread_join(thread_control_manager, NULL);
	pthread_join(thread_interface, NULL);
	pthread_join(thread_logging, NULL);
}