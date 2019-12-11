#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 3000
#define MAX_MESSAGE 100

char *strrev(char *str)
{
      char *p1, *p2;

      if (! str || ! *str)
            return str;
      for (p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2)
      {
            *p1 ^= *p2;
            *p2 ^= *p1;
            *p1 ^= *p2;
      }
      return str;
}

void revInput (int newSock) {
   int i;
   
   char buffer[100];
   while(1){
     bzero(buffer, sizeof(buffer));
     i = recv(newSock,buffer, MAX_MESSAGE, 0);
  
	strrev(buffer);
	
     sprintf(buffer,"%s",buffer);
     i = write(newSock,buffer,255);
    }
}
 

int main(){

	int sockDescriptor;
	struct sockaddr_in serverAddr;
	int newSock;
	struct sockaddr_in newAddr;
	socklen_t addr_size;
	char buffer[100];
	pid_t pid;
  //socket creation
	sockDescriptor = socket(AF_INET, SOCK_STREAM, 0);
	
	if(sockDescriptor == -1){
		printf("Couldn't create socket \n");		
	}
	printf("Created Server Socket \n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	
	//connect socket to server
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);

	//binding to port
	if(bind(sockDescriptor, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0){
		printf("binding failed\n");
		exit(1);
	}
	printf("binding to port successful");

	//listening to port
	if(listen(sockDescriptor, 3) == 0){
		printf("Listen to incoming connections\n");
	}else{
		printf("Not listening-binding error\n");
	}
	
	while(1){
		//socket accept
		newSock = accept(sockDescriptor, (struct sockaddr*)&newAddr, &addr_size);
		if(newSock < 0){
			printf("Connection not accepted");
			exit(1);
		}
		printf("Connection accepted!  Address:  %s Port:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
		
    pid = fork();
    if (pid < 0) {
         perror("ERROR on fork");
         exit(1);
      }
    if(pid == 0){
        close(sockDescriptor);
        revInput(newSock);
      }
	}
  close(newSock);
	return 0;
}
