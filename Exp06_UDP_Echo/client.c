#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<string.h>

int main()
{
	int clientSocket;
	char buffer[1024];
	char buff[1024];
	struct sockaddr_in serverAddr;
	socklen_t addr_size;
	
	clientSocket = socket(AF_INET , SOCK_DGRAM, 0); // Changed SOCK_STREAM to SOCK_DGRAM
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(2000);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	memset(serverAddr.sin_zero,'\0',sizeof(serverAddr.sin_zero));
	addr_size = sizeof(serverAddr);
	puts("Connected\n");
	while(1)
	{
		printf("Enter the message :");
		fgets(buff,1024,stdin);
		printf("Message sent to Server \n");
		
		strcpy(buffer,buff);
		sendto(clientSocket, buffer, strlen(buffer), 0, (struct sockaddr *)&serverAddr, addr_size); // Changed send to sendto
		
		recvfrom(clientSocket, buffer, 1024, 0, NULL, NULL); // Changed recv to recvfrom
		printf("Reply from server > %s \n",buffer);
	}
}

