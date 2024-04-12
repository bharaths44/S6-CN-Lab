#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<string.h>

int main()
{
	int welcomeSocket;
	char buffer[1024];
	char buf[1024];
	struct sockaddr_in serverAddr, clientAddr;
	socklen_t addr_size;
	
	welcomeSocket = socket(AF_INET, SOCK_DGRAM, 0); // Changed SOCK_STREAM to SOCK_DGRAM
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(2000);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	memset(serverAddr.sin_zero,'\0',sizeof(serverAddr.sin_zero));
	
	bind(welcomeSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
	addr_size = sizeof(clientAddr);
	puts("Listening");
	while(1)
	{
		printf("\n\n");
		recvfrom(welcomeSocket, buffer, 1024, 0, (struct sockaddr *)&clientAddr, &addr_size); // Changed recv to recvfrom
		printf("Message from Client > %s \n", buffer);
		
		printf("Message echoed to Client \n");
		
		sendto(welcomeSocket, buffer, strlen(buffer), 0, (struct sockaddr *)&clientAddr, addr_size); // Changed send to sendto
	}
	
	return 0;
}

