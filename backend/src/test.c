//temporary task to test proof of concept

//small wifi server at the moment

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define BUFFER_LENGHT 1024

void *Test_task(void *vargp)
{

    char *msg = "SENSOR:TEMP:GET";
    char buf[BUFFER_LENGHT];

    struct sockaddr_in dest; /* socket info about the machine connecting to us */
    struct sockaddr_in serv; /* socket info about our server */
    int mysocket;            /* socket used to listen for incoming connections */
    socklen_t socksize = sizeof(struct sockaddr_in);

    memset(&serv, 0, sizeof(serv));           /* zero the struct before filling the fields */
    serv.sin_family = AF_INET;                /* set the type of connection to TCP/IP */
    serv.sin_addr.s_addr = htonl(INADDR_ANY); /* set our address to any interface */
    serv.sin_port = htons(3333);              /* set the server port number */

    mysocket = socket(AF_INET, SOCK_STREAM, 0);

    /* bind serv information to mysocket */
    bind(mysocket, (struct sockaddr *)&serv, sizeof(struct sockaddr));

    while (1)
    {
        /* start listening, allowing a queue of up to 1 pending connection */
        if (listen(mysocket, 1) == 0)
        {
            int consocket = accept(mysocket, (struct sockaddr *)&dest, &socksize);

            if (consocket)
            {
                printf("Incoming connection from %s - sending welcome\n", inet_ntoa(dest.sin_addr));
                send(consocket, msg, strlen(msg), 0);
                sleep(1);
                read(consocket, buf, BUFFER_LENGHT);
                printf("%s", buf);
                close(consocket);
            }
        }
        sleep(5);
    }
    close(mysocket);
}