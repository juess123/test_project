#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
void* receive_messages(void* sock) {
    int client_sock = *(int*)sock;
    char buffer[16];
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        int read_size = recv(client_sock, buffer, sizeof(buffer), 0);
        if (read_size > 0) {
            printf("recvive Server: %s\n", buffer);
        } else {
            printf("Server disconnected.\n");
            break;
        }
    }
    return NULL;
}
int main()
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in client_addr;
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(8080);

    inet_pton(AF_INET, "192.168.1.18", &client_addr.sin_addr.s_addr);

    connect(sock, (struct sockaddr*)&client_addr, sizeof(client_addr));
    pthread_t thread1;
    pthread_create(&thread1,NULL,receive_messages,&sock);
    char message[32];
    while(1)
    {
        printf("Client: ");
        memset(message,0,32);
        fgets(message, sizeof(message), stdin);
        send(sock, message, strlen(message), 0);
    }
    close(sock);

    return 0;
}

