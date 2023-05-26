#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define MAX_LEN 100

void crc_gen(char* line, char* div, char* c){
	int l1, l2;
	l1 = strlen(line);
	l2 = strlen(div);
	for(int i = 0; i < l1; i++){
		c[i] = line[i];
	}
	for(int i = l1; i < l1+l2-1; i++){
		c[i] = '0';
	}
	
	printf("The dividend is: %s\n", c);
	printf("The divisor is: %s\n", div);
	
	for(int i = 0; i < l1; i++){
		if(c[i] != '0'){
			for(int j = 0; j < l2; j++){
				if(c[i+j] == div[j])
					c[i+j] = '0';
				else
					c[i+j] = '1';
			}
		}
	}
	
	printf("The CRC is: ");
	for(int i = l1; i < l1+l2-1; i++){
		printf("%c",c[i]);
	}
	
	for(int i = 0; i < l1; i++){
		c[i] = line[i];
	}
	
	printf("\nThe frame is: %s\n", c);
}

int main(int argc, char* argv[]){
	int sd, newSd, cliLen, n;
	struct sockaddr_in servAddr, cliAddr;
	char line[20], div[20], c[20];
	char recvBuff[MAX_LEN];
	if(argc < 3){
		printf("Input error\n");
		exit(0);
	}
	
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	servAddr.sin_port = htons(atoi(argv[2]));
	memset(&(servAddr.sin_zero), '\0', 8);
	
	cliAddr.sin_family = AF_INET;
	cliAddr.sin_addr.s_addr = INADDR_ANY;
	cliAddr.sin_port = htons(0);

	sd = socket(AF_INET, SOCK_STREAM, 0);
	
	printf("successfully created socket\n");
	
	connect(sd, (struct sockaddr *) &servAddr, sizeof(servAddr));
	
	printf("connected to server successfully\n");
	
	printf("Enter the message: ");
	scanf("%s", line);
	printf("Enter the divisor: ");
	scanf("%s", div);
	crc_gen(line, div, c); 
	send(sd, c, strlen(c), 0);
	printf("message sent to server: %s\n", c);
	send(sd, div, strlen(div), 0);
	printf("divisor sent to server: %s\n", div);

	//memset(recvBuff, 0, sizeof(recvBuff));
	//read(sd, recvBuff, sizeof(recvBuff));
	//fputs(recvBuff, stdout);
	printf("closing connection to the server\n");
	close(sd);
	return 0;
}
