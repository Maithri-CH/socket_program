#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 3000

int main(){
	int clientSocket, ret;
	struct sockaddr_in serverAddr;
	char buffer[100];

	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(clientSocket < 0){
		printf("couldn't create client socket\n");
		exit(1);
	}
	printf("Client Socket created.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	
	if(connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0){
		printf("Cant connect\n");
		exit(1);
	}
	printf("Connected to Server.\n");
	
	while(1){
		printf("Enter message to send to server");
		scanf("%s", &buffer[0]);
		send(clientSocket, buffer, strlen(buffer), 0);
       
		if(strcmp(buffer, ":exit") == 0){
			close(clientSocket);
			printf("closing client socket\n");
			exit(1);
		}

		if(recv(clientSocket, buffer, 1024, 0) < 0){
			printf("not receiving data\n");
		}else{
			printf("Message from Server: \t%s\n", buffer);
		}
	}
	return 0;
}
