#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>


#define MAX_MSG 100
#define TOT_BYTE 5
#define BITS 9

void parity_generator(char data[], char result[]){
	int k =0, l =0;
	char par[TOT_BYTE][BITS];
	for(int i = 0; i<TOT_BYTE-1; i++){
		int count = 0;
		for(int j = 0; j<BITS; j++){
			if(j == BITS - 1){
				par[i][j]=(count%2==0)?'0':'1';
				result[l] = par[i][j];
				l++;
				continue;
			}	
			result[l] = data[k];
			l++;
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
				result[l] = par[j][i];
				l++;
				continue;
			}
				
			if(par[j][i] == '1')
				count++;
		}
	}
}

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
	printf("connected to server successfully\n");

	connect(sd,(struct sockaddr *) &servAddr , sizeof(servAddr));
	printf("connected to server successfully\n");

	printf("Enter string to send to server : ");
	scanf("%s", line);
	char result[TOT_BYTE*BITS];
	
	parity_generator(line, result);
	
	send(sd , result , strlen(result)+1 , 0);
	printf("data sent (%s) \n", result);


	printf("closing connection with the server \n");
	close(sd);
	
	

	
	return 0;

}

