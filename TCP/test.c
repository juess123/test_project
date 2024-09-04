#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <pthread.h>
void* serve_recv(void* sock)
{
   int client_sock = *(int*)sock;
    char buffer[16];
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        int read_size = recv(client_sock, buffer, sizeof(buffer), 0);
        if (read_size > 0) {
            printf("recvive Client: %s\n", buffer);
        } else {
            printf("Client disconnected.\n");
            break;
        }
    }
    return NULL;
}
int main()
{
    struct sockaddr_in serve_addr;
    serve_addr.sin_family=AF_INET;
    serve_addr.sin_port=htons(8080);
    
    inet_pton(AF_INET,"192.168.1.18",&serve_addr.sin_addr);

    int sock=socket(AF_INET,SOCK_STREAM,0);

    socklen_t addrlen=sizeof(serve_addr);
    bind(sock,(struct sockaddr *)&serve_addr,addrlen);

    listen(sock,3);

    int new_socket=accept(sock,(struct sockaddr*)&serve_addr,&addrlen);

    pthread_t thread1;
    pthread_create(&thread1,NULL,serve_recv,&new_socket);
    char message[32];
    while(1)
    {
        printf("Serve: ");
        memset(message,0,32);
        fgets(message, sizeof(message), stdin);
        send(new_socket, message, strlen(message), 0);
    }
    close(new_socket);
    close(sock);
}

