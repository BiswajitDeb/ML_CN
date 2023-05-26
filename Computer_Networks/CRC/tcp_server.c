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

int crc_check(char* line, char* div){
	int l1, l2;
	l1 = strlen(line);
	l2 = strlen(div);
	
	printf("The dividend is: %s\n", line);
	printf("The divisor is: %s\n", div);
	
	for(int i = 0; i <= l1-l2; i++){
		if(line[i] != '0'){
			for(int j = 0; j < l2; j++){
				if(line[i+j] == div[j])
					line[i+j] = '0';
				else
					line[i+j] = '1';
			}
		}
	}
	
	int flag = 1;
	for(int i = l1-l2+1; i < l1; i++){
		if(line[i] != '0'){
			flag = 0;
			break;
		}
	}
	
	return flag;
}

int main(int argc, char* argv[]){
	int sd, newSd, cliLen, n;
	struct sockaddr_in servAddr, cliAddr;
	char line[20], div[20];
	char sendBuff[MAX_LEN];
	if(argc < 3){
		printf("Input error\n");
		exit(0);
	}
	
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	servAddr.sin_port = htons(atoi(argv[2]));
	memset(&(servAddr.sin_zero), '\0', 8);

	sd = socket(AF_INET, SOCK_STREAM, 0);
	
	printf("successfully created socket\n");
	
	bind(sd, (struct sockaddr *)&servAddr, sizeof(servAddr));
	
	printf("successfully bound local port\n");
	
	listen(sd, 5);
	
	while(1){
	
		printf("waiting for client connection on TCP port %u\n", atoi(argv[2]));
		cliLen = sizeof(cliAddr);
		newSd = accept(sd, (struct sockaddr *) &cliAddr, &cliLen);
		printf("recieved connection from host [IP %s TCP port %d]\n", inet_ntoa(cliAddr.sin_addr), ntohs(cliAddr.sin_port));
		memset(line, 0, 20);
		memset(div, 0, 20);
		recv(newSd, line, sizeof(line), 0);
		printf("recieved message from host [IP %s TCP port %d]: %s\n", inet_ntoa(cliAddr.sin_addr), ntohs(cliAddr.sin_port), line);
		recv(newSd, div, sizeof(div), 0);
		printf("recieved divisor from host [IP %s TCP port %d]: %s\n", inet_ntoa(cliAddr.sin_addr), ntohs(cliAddr.sin_port), div);
		if(crc_check(line, div))
			printf("No error found\n");
		else
			printf("Error found\n");
		//memset(sendBuff, 0, sizeof(sendBuff));	
		//time_t ticks = time(NULL);
		//snprintf(sendBuff, sizeof(sendBuff), "%.24s\r\n", ctime(&ticks));
		//send(newSd, sendBuff, strlen(sendBuff), 0);
		
		printf("closing connection with host [IP %s TCP port %d]\n", inet_ntoa(cliAddr.sin_addr), ntohs(cliAddr.sin_port));
		close(newSd);
	}
	return 0;
}
