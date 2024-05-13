// CLIENT OF CONCURRENT SERVER

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

int main()
{
    int clientSocket;
    char buffer[1024];
    char g;
    char buf[1024];
    struct sockaddr_in serverAddr;
    socklen_t addr_size;
    struct sockaddr_storage serverStorage;

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(2000);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);
    addr_size = sizeof serverAddr;

    if (connect(clientSocket, (struct sockaddr *)&serverAddr, addr_size))
    {
        perror("Connection failed. Error");
        return 1;
    }
    puts("Connected\n");

    while (1)
    {
        printf("Enter the message: ");
        fgets(buf, 1024, stdin);
        printf("Message sent to Server \n");

        strcpy(buffer, buf);
        send(clientSocket, buffer, sizeof(buffer), 0);

        recv(clientSocket, buffer, 1024, 0);
        printf("Reply from server > %s \n", buffer);
    }

    return 0;
}