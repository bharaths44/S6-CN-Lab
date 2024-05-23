#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    int server_socket;
    struct sockaddr_in server_address, client_address, client_addresses[10];
    char buf[1024];
    int num_clients = 0, broadcasEnable = 1;

    server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    setsockopt(server_socket, SOL_SOCKET, SO_BROADCAST, &broadcasEnable, sizeof(broadcasEnable));

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(12345);

    bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address));

    printf("Server is running on port %d...\n", 12345);

    while (1)
    {
        int recv_size = recvfrom(server_socket, buf, sizeof(buf), 0, (struct sockaddr *)&client_address, &(socklen_t){sizeof(client_address)});
        buf[recv_size] = '\0';

        int client_index = -1;
        for (int i = 0; i < num_clients; ++i)
        {
            if (client_addresses[i].sin_addr.s_addr == client_address.sin_addr.s_addr &&
                client_addresses[i].sin_port == client_address.sin_port)
            {
                client_index = i;
                break;
            }
        }

        if (client_index == -1 && num_clients < 10)
        {
            client_addresses[num_clients] = client_address;
            client_index = num_clients++;
            printf("Client%d connected\n", client_index + 1);
        }

        printf("Message from client%d: %s\n", client_index + 1, buf);

        for (int i = 0; i < num_clients; ++i)
        {
            sendto(server_socket, buf, strlen(buf), 0, (struct sockaddr *)&client_addresses[i], sizeof(client_addresses[i]));
        }
    }

    close(server_socket);

    return 0;
}