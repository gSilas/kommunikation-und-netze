/* echoclient.c */

#include <cnaiapi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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
            printf("server not recognized\n");
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
                printf("failed!\n");
                return 0;
            }
            else
            {
                printf("connected!\n");
            }
    }

    while(connect !=-1)
    {
        int size;
        
        if((size = read(0,&buffer,sizeof(buffer)-1)) > 0)
            write(connect,&buffer,size);
        
        if((size = read(connect,&buffer,sizeof(buffer)-1)) > 0)
        {
            write(0,"answer from server: ",21);
            write(0,&buffer,size);
        }
    	
	}

	return 0;
}
