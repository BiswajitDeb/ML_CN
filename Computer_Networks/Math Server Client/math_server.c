#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include  <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>


#define MAX_MSG 100


int main(int argc , char *argv[]){
	int sd , newSd , cliLen , n;
	struct sockaddr_in servAddr , cliAddr;
	char line[MAX_MSG];
	
	if(argc < 3){
		printf("input error\n");
		exit(0);	
	}
	// bzero((char *)&servAddr , sizeof(servAddr));

	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	servAddr.sin_port = htons(atoi(argv[2]));
	memset(&(servAddr.sin_zero),'\0',8);   //zero the rest of the struct

	/*creating the stream socket*/
	sd=socket(AF_INET,SOCK_STREAM,0);
	printf("successfully created the stream socket\n");
	
	/*bind local port number*/
	bind(sd,(struct sockaddr *)&servAddr,sizeof(servAddr));
	printf("bound local port successfully\n");
	

	/*specify number of concurrent clients to wait for*/
	listen(sd, 5);
	int op[3];
	char opr[MAX_MSG];
	int res = 0;
	while(1){
		int i = 0;
		printf("waiting for the client connection of TCP port %u\n", atoi(argv[2]));
		
		/* wait for client connection */
		cliLen = sizeof(cliAddr);
		newSd = accept(sd,(struct sockaddr *)&cliAddr,&cliLen);
		printf("received connection from host[IP %s,TCP port %d]\n",inet_ntoa(cliAddr.sin_addr), ntohs(cliAddr.sin_port));
		do{
			memset(line, 0,MAX_MSG);
			n=recv(newSd, line, MAX_MSG,0);
			op[i] = atoi(line);
			printf("recieved from host [IP %s,TCP port %d]: %s\n",
					inet_ntoa(cliAddr.sin_addr), ntohs(cliAddr.sin_port),line);
			i++;
		}while(i<2);
		memset(opr, 0, MAX_MSG);
		recv(newSd, opr, MAX_MSG,0);
		printf("recieved from host [IP %s,TCP port %d]: %s\n",
					inet_ntoa(cliAddr.sin_addr), ntohs(cliAddr.sin_port),opr);
		if(*opr == '+')
		{
			res = op[0] + op[1];
			printf("The Result obtained : %d\n", res);
		}
		else if(*opr=='-')
		{
			res = op[0] - op[1];
			printf("The Result obtained : %d\n", res);
		}
		else if(*opr=='*')
		{
			res = op[0] * op[1];
			printf("The Result obtained : %d\n", res);
		}
		else if(*opr=='/')
		{
			res = op[0] / op[1];
			printf("The Result obtained : %d\n", res);
		}
		else if(*opr=='%')
		{
			res = op[0] % op[1];
			printf("The Result obtained : %d\n", res);
		}
		else
		{
			res = -9990001;
			printf("Unrecognized symbol ( %s )\n", opr);
		}
		
		send(newSd, &res, sizeof(res), 0);
		printf("%s: Result Sent ( %d )\n", argv[0], res);
		
		printf("closing connection with host[IP %s,TCP port %d]\n",inet_ntoa(cliAddr.sin_addr), ntohs(cliAddr.sin_port));
		close(newSd);
			
	}
	
	return 0;
		
}


