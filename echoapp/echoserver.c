/* echoserver.c */

#include <cnaiapi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	appnum port;
	char inbuffer[10000];

	connection connect;

	if ( argc != 2 )
    {
        printf( "usage: %s port \n", argv[0] );
	    return 0;
    }

    else
    {
        port = atoi(argv[1]);
        if(port == -1)
            printf("port failed\n");
        else
            printf("port recognized\n");
    }
	
    connect = await_contact(port);
      
    if(connect == -1)
    {
        printf("failed!\n");
    }
    else
    {
        printf("connected!\n");
    }

	while(connect != -1)
	{   
        int size = read(connect,&inbuffer,sizeof(inbuffer)-1);
        if(size > 0)
        {
            write(0,"input from client: ",20);
            write(0,&inbuffer,size);
    	    write(connect,&inbuffer,size);
            printf("output send!\n");
        }
	}

	return 0;
}
