#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        printf("Usage: ./server <Ip Address>\n");
        return 1;
    }

    // create an ipv4 socket object
    struct sockaddr_in sock;

    sock.sin_family = AF_INET;
    sock.sin_port = htons(1111);
    // use the program input for the ip address.
    int success = inet_aton(argv[1], &sock.sin_addr);

    if(!success)
    {
        printf("Invalid address input\n");
        return 1;
    }

    // open a socket file descriptor
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    if(socket_fd == -1)
    {
        printf("Error calling socket(): %s \n", strerror(errno));
        return 1;
    }

    // bind address to socket
    socklen_t len = sizeof(sock);
    int bound = bind(socket_fd, (const struct sockaddr* ) &sock, len);

    if(bound == -1)
    {
        printf("Error calling bind(): %s \n", strerror(errno));
        return 1;
    }

    // listen for connections
    int l = listen(socket_fd, 16);

    if(l == -1)
    {
        printf("Error calling listen(): %s \n", strerror(errno));
        return 1;
    }
    printf("Listening for connections\n");

    // accept new connections
    struct sockaddr_in peer_socket;
    socklen_t peer_len = sizeof(peer_socket);

    // blocks
    int new_socket_fd = accept(socket_fd, (struct sockaddr*) &peer_socket, &peer_len);

    if(new_socket_fd == -1)
    {
        printf("Error calling listen(): %s \n", strerror(errno));
        return 1;
    }
    printf("Client connected. \n");


    // incoming message buffer
    size_t buffer_len = 256;
    char incoming_buffer[buffer_len];
    int flags = 0; 




    // RECEIVE
    ssize_t recvd = 0;
    recvd += recv(new_socket_fd, incoming_buffer, buffer_len, flags);
    printf("Received %zu bytes: %s\n", recvd, incoming_buffer);


    // SEND
    char* message = "Hello Miles Dripps from Server!!!";

    ssize_t sent = send(new_socket_fd, message, strlen(message), flags);
    printf("Sent %zu bytes. \n", sent);

    // close connected socket
    int closed = close(new_socket_fd);  
    printf("Closed.\n");


}