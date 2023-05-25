#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include  <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#define MAX_MSG 100
#define TOT_BYTE 5
#define BITS 8

bool parity_checker(char data[]){
	int k =0, l =0;
	char par[TOT_BYTE][BITS];
	for(int i = 0; i<TOT_BYTE-1; i++){
		int count = 0;
		for(int j = 0; j<BITS; j++){
			if(j == BITS - 1){
				par[i][j]=(count%2==0)?'0':'1';
				if(par[i][j] != data[k])
					return false;
				k++;
				continue;
			}	
			par[i][j] = data[k];
			if(data[k] == '1')
				count++;
			k++;
		}
	}
		
	for(int i = 0; i<BITS; i++){
		int count = 0;
		for(int j = 0; j<TOT_BYTE; j++){
			if(j == TOT_BYTE - 1){
				par[j][i]=(count%2==0)?'0':'1';
				if(par[j][i] != data[k])
					return false;
				k++;
				continue;
			}
				
			if(par[j][i] == '1')
				count++;
		}
	}
	return true;
}

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

	while(1){
		printf("waiting for the client connection of TCP port %u\n", atoi(argv[2]));
		
		/* wait for client connection */
		cliLen = sizeof(cliAddr);
		newSd = accept(sd,(struct sockaddr *)&cliAddr,&cliLen);
		printf("%d" , newSd);
		printf("received connection from host[IP %s,TCP port %d]\n",inet_ntoa(cliAddr.sin_addr), ntohs(cliAddr.sin_port));
		do{
			memset(line, 0,MAX_MSG);
			n=recv(newSd, line, MAX_MSG,0);
			line[n]='\n';
			printf("recieved from host [IP %s,TCP port %d]: %s\n",
					inet_ntoa(cliAddr.sin_addr), ntohs(cliAddr.sin_port),line);
			if(strcmp(line, "quit")){
				if(parity_checker(line))
					printf("No error found\n");
				else
					printf("Parity bit error found\n");
			}
			
		}while(abs(strcmp(line,"quit")));
		printf("closing connection with host[IP %s,TCP port %d]\n",inet_ntoa(cliAddr.sin_addr), ntohs(cliAddr.sin_port));
		close(newSd);
			
	}
	
	return 0;
		
}


