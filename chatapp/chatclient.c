/* echoclient.c */

#include "include/cnaiapi.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <poll.h>
#include <stropts.h>

int main(int argc, char *argv[])
{
	appnum port;
	computer server;
    char buffer[10000];
	connection connect;

	if ( argc != 3 )
    {
        printf( "usage: %s computer port \n", argv[0] );
	    return 0;
    }

    else
    {
        server = cname_to_comp(argv[1]);

        if(server == -1)
            printf("host not recognized\n");
        else
            printf("host recognized\n");

        port = atoi(argv[2]);

        if(port == -1)
            printf("port failed\n");
        else
            printf("port recognized\n");
        
        connect = make_contact(server, port);

        if(connect == -1)
        {
            printf("created server!\n");
            connect = await_contact(port);
        }
        if (connect != -1)
        {
            printf("connected!\n");
        }
    }

    struct pollfd fds[2];
    fds[0].fd = 0;
    fds[1].fd = connect;
    fds[0].events = POLLIN;
    fds[1].events = POLLIN;
    int timeout = 30;

    while(connect != -1)
    {
        size_t size;
        poll(fds, 2, timeout);

                if (fds[0].revents & POLLIN)
                {
                    if((size = read(0,buffer,sizeof(buffer)-1)) > 0)
                    	write(connect,buffer,size);  	
               	}

                if (fds[1].revents & POLLIN)
                {
                    if((size = read(connect,buffer,sizeof(buffer)-1)) > 0)
                    	write(0,buffer,size);  	
               	}
	}

	return 0;
}
