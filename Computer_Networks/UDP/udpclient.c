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


	sd = socket(AF_INET , SOCK_DGRAM , 0);
	printf("Successfully created datagram socket\n");


	do {
		printf("Enter data to send : ");
		scanf("%s", line);


		sendto(sd , line , strlen(line)+1 , 0,(struct sockaddr *)&servAddr,sizeof(servAddr));
	}while(strcmp(line , "quit"));

	

	
	return 0;

}

