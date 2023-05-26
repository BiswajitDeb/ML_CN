#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>


#define MAX_MSG 24

void carrysum(char* result){
	int carry = 1;
	if(result[7] == '0')
		result[7] = '1';
	else{
		for(int i = 7; i >= 0; i--){
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
	for(int i= 0; i< 7; i++){
		if(result[i]=='1')
			result[i] = '0';
		else
			result[i] = '1';
	}
}

void checksum_check(char* line){
	char sum[8];
	int carry = 0;
	for(int i = 7, j = 15, k = 7; i >= 0; i--,j--, k--){
		sum[k] = (line[i]-'0') + (line[j]-'0')+carry+'0';
		if(sum[k]-'0' >= 2)
			carry = 1;
		else
			carry = 0;
		sum[k] = (sum[k]-'0')%2+'0';
	}
	if(carry == 1)
		carrysum(sum);
	carry = 0;
	for(int j = 23, k = 7; k >= 0; j--, k--){
		sum[k] = (line[j]-'0') + (sum[k]-'0')+carry+'0';
		if(sum[k]-'0' >= 2)
			carry = 1;
		else
			carry = 0;
		sum[k] = (sum[k]-'0')%2+'0';
	}
	if(carry == 1)
		carrysum(sum);
	int flag=1;
	for(int i = 0; i <8; i++){
		if(sum[i]=='0')
		{
			flag = 0;
			break;
		}
	}
	if(flag)
		printf("No error found\n");
	else
		printf("Error found\n");
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
		printf("received connection from host[IP %s,TCP port %d]\n",inet_ntoa(cliAddr.sin_addr), ntohs(cliAddr.sin_port));
		memset(line, 0,MAX_MSG);
		n=recv(newSd, line, MAX_MSG,0);
		printf("recieved from host [IP %s,TCP port %d]: %s\n",
				inet_ntoa(cliAddr.sin_addr), ntohs(cliAddr.sin_port),line);
		checksum_check(line);
		printf("closing connection with host[IP %s,TCP port %d]\n",inet_ntoa(cliAddr.sin_addr), ntohs(cliAddr.sin_port));
		close(newSd);
			
	}
	
	return 0;
		
}


