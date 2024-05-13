//FTP CLIENT USING TCP

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

int main() {
    int clientSocket;
    char buffer[1024];
    char filename[256];
    FILE *file;
    struct sockaddr_in serverAddr;
    socklen_t addr_size;

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(2000);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    memset(serverAddr.sin_zero, '\0', sizeof(serverAddr.sin_zero));
    addr_size = sizeof(serverAddr);

    if (connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size)) {
        perror("Connection failed. Error");
        return 1;
    }
    puts("Connected\n");

    printf("Enter the filename to transfer: ");
    scanf("%s", filename);

    // Open the file
    file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        close(clientSocket);
        return 1;
    }

    // Send filename length
    size_t filename_len = strlen(filename);
    send(clientSocket, &filename_len, sizeof(filename_len), 0);

    // Send filename
    send(clientSocket, filename, filename_len, 0);

    // Read and send file contents
    printf("Content of file sent: \n");
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        printf("%s", buffer); // Displaying content
        send(clientSocket, buffer, strlen(buffer), 0); // Sending content
    }

    fclose(file);
    close(clientSocket);

    return 0;
}
