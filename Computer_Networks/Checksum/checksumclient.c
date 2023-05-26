#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>


#define MAX_MSG 16

void carrysum(char* result){
	int carry = 1;
	if(result[23] == '0')
		result[23] = '1';
	else{
		for(int i = 23; i >= 16; i--){
			result[i] = result[i]+carry;
			if(result[i] == '2')
				carry = 1;
			else
				carry = 0;
			result[i] = (result[i]-'0')%2+'0';
		}
		if(carry)
			carrysum(result);
	}
}
void compliment(char* result){
	for(int i= 16; i< 24; i++){
		if(result[i]=='1')
			result[i] = '0';
		else
			result[i] = '1';
	}
}

void checksum_gen(char* line, char* result){
	for(int i = 0; i < 16; i++){
		result[i] = line[i];
	}
	int carry = 0;
	for(int i = 7, j = 15, k = 23; i >= 0; i--,j--, k--){
		result[k] = (line[i]-'0') + (line[j]-'0')+carry+'0';
		if(result[k]-'0' >= 2)
			carry = 1;
		else
			carry = 0;
		result[k] = (result[k]-'0')%2+'0';
	}
	if(carry == 1)
		carrysum(result);
	compliment(result);
	printf("Checksum: ");
	for(int i = 16; i<24; i++){
		printf("%c", result[i]);
	}
	printf("\nThe message: %s\n", result);

}


int main(int argc , char *argv[]){
	
	int sd , newSd , cliLen , n;
	struct sockaddr_in servAddr, cliAddr;
	char line[MAX_MSG], result[24];


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
	printf("created stream socket successfully\n");

	connect(sd,(struct sockaddr *) &servAddr , sizeof(servAddr));
	printf("connected to server successfully\n");
	printf("Enter string to send to server : ");
	scanf("%s", line);
	checksum_gen(line, result);

	send(sd , result , strlen(result)+1 , 0);
	printf("data sent (%s) \n", result);


	printf("closing connection with the server \n");
	close(sd);
	
	

	
	return 0;

}

