#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int sock;
    struct sockaddr_in serv_addr;

    char username[50], password[50];
    char buffer[1024], message[1024];
    int status;

    sock = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    // 🔐 Login
    printf("Enter username: ");
    scanf("%s", username);

    printf("Enter password: ");
    scanf("%s", password);

    send(sock, username, 50, 0);
    send(sock, password, 50, 0);

    read(sock, &status, sizeof(status));

    if (status == 0) {
        printf("Login failed!\n");
        return 0;
    } else if (status == 2) {
        printf("New user registered!\n");
    } else {
        printf("Login successful!\n");
    }

    getchar(); // clear buffer

    // 💬 Chat
    while (1) {
        printf("You: ");
        fgets(message, 1024, stdin);
        send(sock, message, strlen(message), 0);

        memset(buffer, 0, sizeof(buffer));
        read(sock, buffer, 1024);
        printf("Server: %s\n", buffer);
    }

    return 0;
}