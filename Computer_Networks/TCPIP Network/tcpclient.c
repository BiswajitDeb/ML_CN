#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>


#define MAX_MSG 100

int main(int argc,char *argv[])
{
	int sd,cliLen,n;
	struct sockaddr_in servAddr,cliAddr;
	char line[MAX_MSG];
	
	if(argc<3)
	{
		printf("Input error/n");
		exit(0);
	}

	/* build server address structure */
	//bzero((char *)&servAddr,sizeof(servAddr));
	servAddr.sin_family=AF_INET;
	servAddr.sin_addr.s_addr=inet_addr(argv[1]);
	servAddr.sin_port=htons(atoi(argv[2]));
	memset(&(servAddr.sin_zero),'\0',8); //zero the rest of the struct

	/*build client address structure */
	/*bzero((char *)&cliAddr,sizeof(cliAddr));*/
	cliAddr.sin_family=AF_INET;
	cliAddr.sin_addr.s_addr=INADDR_ANY;
	cliAddr.sin_port=htons(0);
	
	/* create stream socket*/
	sd=socket(AF_INET,SOCK_STREAM,0);
	printf("SUCCESSFULLY CREATED STREAM SOCKET\n");
	/*bind local port number*/
	
	connect(sd,(struct sockaddr *)&servAddr,sizeof(servAddr));
	printf("connected to server successfully\n");

	/* send data to server */
	do
	{
		printf("Enter string to send to server: ");
		scanf("%s",line);

		send(sd,line,strlen(line)+1,0);
		printf("data sent (%s)\n",line);
	}
	while(strcmp(line,"quit"));
	
	printf("closing connection with the server\n");
	close(sd);

	return 0;
}

