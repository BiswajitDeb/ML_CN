#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>


#define MAX_MSG 100

int main(int argc,char *argv[])
{
	int sd,newSd,cliLen,n;
	struct sockaddr_in servAddr,cliAddr;
	char line[MAX_MSG];
	
	if(argc<3)
	{
		printf("input error\n");
		exit(0);
	}

	/* build server address structure */
	//bzero((char *)&servAddr,sizeof(servAddr));
	servAddr.sin_family=AF_INET;
	servAddr.sin_addr.s_addr=inet_addr(argv[1]);
	servAddr.sin_port=htons(atoi(argv[2]));
	memset(&(servAddr.sin_zero),'\0',8); //zero the rest of the struct
	
	/* create stream socket*/
	sd=socket(AF_INET,SOCK_STREAM,0);
	printf("SUCCESSFULLY CREATED STREAM SOCKET\n");
	/*bind local port number*/
	
	bind(sd,(struct sockaddr *)&servAddr,sizeof(servAddr));
	printf("bound local port successfully\n");

	/* specify number of concurrent clients to wait for */
	
	listen(sd,5);
	while(1)
	{
		printf("waiting for client connection on TCP port %u\n",atoi(argv[2]));
		

	/* wait for client connection*/
	cliLen=sizeof(cliAddr);
	newSd=accept(sd,(struct sockaddr *)&cliAddr,&cliLen);
	printf("/n%d",newSd);
	printf("received connection from host [IP %s,TCP port %d]\n",inet_ntoa(cliAddr.sin_addr),ntohs(cliAddr.sin_port));

	/* wait for data from client*/
	do
	{
		memset(line,0,MAX_MSG);
		n=recv(newSd,line,MAX_MSG,0); 
		line[n]='\n';
		printf("received connection from host [IP %s,TCP port %d]: %s\n",inet_ntoa(cliAddr.sin_addr),ntohs(cliAddr.sin_port),line);
	}
	
	while(abs(strcmp(line,"quit")));

	/*close client connection*/
	printf("closing connection with host [IP %s,TCP port %d]\n",inet_ntoa(cliAddr.sin_addr),ntohs(cliAddr.sin_port));

	close(newSd);
	}

	return 0;
}
