#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<string.h>

int main()
{
	int welcomeSocket,newSocket;
	char buffer[1024];
	char buf[1024];
	char g;
	struct sockaddr_in serverAddr;
	struct sockaddr_storage serverStorage;
	socklen_t addr_size;
	
	welcomeSocket = socket(AF_INET,SOCK_STREAM,0);
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(2000);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	memset(serverAddr.sin_zero,'\0',sizeof(serverAddr.sin_zero));
	bind(welcomeSocket,(struct sockaddr *)&serverAddr,sizeof(serverAddr));
	
	if(listen(welcomeSocket,5)==0)
		printf("Listening \n");
	else
		printf("Error \n");
	addr_size = sizeof(serverStorage);
	newSocket = accept(welcomeSocket,(struct sockaddr *)&serverStorage,&addr_size);
	while(1)
	{
		recv(newSocket,buffer,1024,0);
		printf("\n\n");
		printf("Message from Client > %s",buffer);
		printf("Message echoed to Client \n");
		send(newSocket,buffer,sizeof(buffer),0);
	}
	return 0;
}
	
