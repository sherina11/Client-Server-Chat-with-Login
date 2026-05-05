#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

// 🔐 Check or Register User
int authenticate(char username[], char password[]) {
    FILE *fp = fopen("users.txt", "a+");
    char u[50], p[50];

    rewind(fp);

    while (fscanf(fp, "%s %s", u, p) != EOF) {
        if (strcmp(u, username) == 0) {
            if (strcmp(p, password) == 0) {
                fclose(fp);
                return 1; // login success
            } else {
                fclose(fp);
                return 0; // wrong password
            }
        }
    }

    // New user → register
    fprintf(fp, "%s %s\n", username, password);
    fclose(fp);
    return 2; // new user created
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    char buffer[1024] = {0};
    char username[50], password[50];
    char message[1024];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 3);

    printf("Server waiting...\n");

    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);

    // 🔐 Receive login data
    read(new_socket, username, 50);
    read(new_socket, password, 50);

    int status = authenticate(username, password);

    send(new_socket, &status, sizeof(status), 0);

    if (status == 0) {
        printf("Login failed\n");
        close(new_socket);
        return 0;
    }

    printf("User %s connected\n", username);

    // 💬 Chat loop
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        read(new_socket, buffer, 1024);
        printf("%s: %s\n", username, buffer);

        printf("You: ");
        fgets(message, 1024, stdin);
        send(new_socket, message, strlen(message), 0);
    }

    return 0;
}