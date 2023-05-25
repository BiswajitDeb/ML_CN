#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>


#define MAX_MSG 100



int main(int argc , char *argv[]){
	
	int sd , newSd , cliLen , n;
	struct sockaddr_in servAddr, cliAddr;
	char line[MAX_MSG];


	if (argc <3){
		printf("Input Error \n");
		exit(0);	
	}
	
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	servAddr.sin_port = htons(atoi(argv[2]));
	memset(&(servAddr.sin_zero),'\0',8);


	cliAddr.sin_family = AF_INET;
	cliAddr.sin_addr.s_addr = INADDR_ANY;
	cliAddr.sin_port = htons(0);
	

	sd = socket(AF_INET , SOCK_STREAM , 0);
	printf("connected to serve successfully\n");

	connect(sd,(struct sockaddr *) &servAddr , sizeof(servAddr));
	printf("connected to serve successfully\n");


	printf("Enter first operand to send to server : ");
	scanf("%s", line);
	send(sd, line, strlen(line) + 1, 0);
	printf("%s: data sent (%s)\n", argv[0], line);
	
	printf("Enter second operand to send to server : ");
	scanf("%s", line);
	send(sd, line, strlen(line) + 1, 0);
	printf("%s: data sent (%s)\n", argv[0], line);
	
	printf("Enter operator to send to server : ");
	scanf("%s", line);
	send(sd, line, strlen(line) + 1, 0);
	printf("%s: data sent (%s)\n", argv[0], line);
	
	int res;
	recv(sd, &res, sizeof(res), 0);
	if(res != -9990001)
		printf("Received Result from server : %d\n", res);
	else{
		printf("Error Message from Server : Unrecognized Symbol ( %s ),", line);
		printf("try '+', '-', '*', '\\' or '%' \n");
	} 

	printf("closing connection with the server \n");
	close(sd);
	
	

	
	return 0;

}

