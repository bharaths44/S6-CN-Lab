#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<string.h>

int main()
{
	int welcomeSocket;
	char buffer[2048];
	char buf[2048];
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
		recvfrom(welcomeSocket, buffer, 2048, 0, (struct sockaddr *)&clientAddr, &addr_size); // Changed recv to recvfrom
		printf("Reply from Client > %s \n", buffer);
		
		printf("Enter the message : ");
		fgets(buf, 2048, stdin);
		printf("Message sent to Client \n");
		
		sendto(welcomeSocket, buf, strlen(buf), 0, (struct sockaddr *)&clientAddr, addr_size); // Changed send to sendto
	}
	
	return 0;
}

