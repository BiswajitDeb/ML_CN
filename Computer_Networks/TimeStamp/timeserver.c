#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define MAX_MSG 100

int main(int argc,char *argv[])
{
    int sd, newSd, cliLen, n;
    struct sockaddr_in servAddr, cliAddr;
    char sendBuff[MAX_MSG];

    if(argc < 3){
        printf("input error\n");
        exit(0);
    }

    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(argv[1]);
    servAddr.sin_port = htons(atoi(argv[2]));
    memset(&(servAddr.sin_zero),'\0',8);   //zero the rest of the struct
    memset(sendBuff, '0', sizeof(sendBuff));
    /*create stream socket*/
    sd = socket(AF_INET,SOCK_STREAM, 0);
    printf("sucessfully created stream socket\n");

    /*bind local port number*/
    bind(sd, (struct sockaddr *)&servAddr, sizeof(servAddr));
    printf("bound local port sucessfully\n");
    
    /*specify number of concurrent clients to wait for*/
    listen(sd, 5);
    while(1){
        printf("Waiting for client connection on TCP port %u\n",atoi(argv[2]));
        /*wait for client connection*/
	cliLen = sizeof(cliAddr);
		newSd = accept(sd, (struct sockaddr *)&cliAddr, &cliLen);
		printf("recived connection from host [IP %s, TCP port %d]\n",inet_ntoa(cliAddr.sin_addr), ntohs(cliAddr.sin_port));

		time_t ticks = time(NULL);
		snprintf(sendBuff, sizeof(sendBuff), "%.24s\r\n", ctime(&ticks));
		send(newSd, sendBuff, strlen(sendBuff)+1, 0);

		/*close client connection*/
		printf("closing connection with host [IP %s, TCP port %d]\n",inet_ntoa(cliAddr.sin_addr), ntohs(cliAddr.sin_port));

		close(newSd);
    	}
    	return 0;
}
