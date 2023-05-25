#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define MAX_MSG 100

int main(int argc,char *argv[])
{
	int sd, newSd, cliLen, n;
    	struct sockaddr_in servAddr, cliAddr;
    	char recvBuff[MAX_MSG];

    	if(argc < 3)
    	{
        	printf("input error\n");
        	exit(0);
    	}

    	servAddr.sin_family = AF_INET;
    	servAddr.sin_addr.s_addr = inet_addr(argv[1]);
    	servAddr.sin_port = htons(atoi(argv[2]));
    	memset(&(servAddr.sin_zero),'\0',8);   //zero the rest of the struct
	memset(recvBuff, '0', sizeof(recvBuff));

	/*build client address structure*/
	cliAddr.sin_family = AF_INET;
	cliAddr.sin_addr.s_addr = INADDR_ANY;
	cliAddr.sin_port = htons (0);

    	/*create stream socket*/
    	sd = socket(AF_INET,SOCK_STREAM, 0);
    	printf("sucessfully created stream socket\n");

    	/*connect to server*/
    	connect(sd, (struct sockaddr *) &servAddr, sizeof(servAddr));
    	printf("connected to server sucessfully\n");
    
    	while( (n = read(sd, recvBuff, sizeof(recvBuff)-1)) > 0)
    	{
    		recvBuff[n]=0;
    		if(fputs(recvBuff, stdout) == EOF)
    		{
    			printf("\n Error: Fputs error\n");
    		}
    	}
	
	if(n < 0)
		printf("\n Read error \n");
	printf("closing connection with the server\n");
	close(sd);

	return 0;
}
