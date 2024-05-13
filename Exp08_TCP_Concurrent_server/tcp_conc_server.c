#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

#define PORT 2000
#define MAX_PENDING_CONNECTIONS 5
#define BUFFER_SIZE 1024
#define RESPONSE_SIZE 2048 // Increased buffer size for response

void handle_client(int clientSocket, int clientNumber)
{
    char buffer[BUFFER_SIZE];
    char response[RESPONSE_SIZE]; // Increased buffer size for response
    ssize_t bytes_received;

    while ((bytes_received = recv(clientSocket, buffer, BUFFER_SIZE, 0)) > 0)
    {
        buffer[bytes_received] = '\0'; // Null-terminate the received data
        printf("Received from client %d: %s\n", clientNumber, buffer);

        // Send response back to the client
        printf("Enter the response: ");
        fgets(response, RESPONSE_SIZE, stdin);
        send(clientSocket, response, strlen(response), 0);
        printf("Response sent to Client \n");
    }

    if (bytes_received == 0)
    {
        printf("Client %d disconnected.\n", clientNumber);
    }
    else
    {
        perror("Error receiving data from client.");
    }

    close(clientSocket);
}

int main()
{
    int welcomeSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrSize = sizeof(struct sockaddr_in);
    int clientNumber = 1; // Initialize client number to start from 1

    // Create socket
    welcomeSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (welcomeSocket == -1)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set up server address structure
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    // Bind socket to port
    if (bind(welcomeSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
    {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(welcomeSocket, MAX_PENDING_CONNECTIONS) == -1)
    {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    while (1)
    {
        // Accept incoming connection
        clientSocket = accept(welcomeSocket, (struct sockaddr *)&clientAddr, &addrSize);
        if (clientSocket == -1)
        {
            perror("Accept failed");
            continue;
        }

        printf("New client connected. Client number: %d\n", clientNumber);

        // Fork to handle client connection
        pid_t pid = fork();
        if (pid == -1)
        {
            perror("Fork failed");
            close(clientSocket);
            continue;
        }
        else if (pid == 0)
        {                         // Child process
            close(welcomeSocket); // Close listening socket in child process
            handle_client(clientSocket, clientNumber);
            exit(EXIT_SUCCESS);
        }
        else
        {                        // Parent process
            close(clientSocket); // Close client socket in parent process
            clientNumber++;      // Increment client number for next connection
        }
    }

    // Close the welcome socket before exiting
    close(welcomeSocket);

    return 0;
}
