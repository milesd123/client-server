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
        printf("usage: ./client <dest ip>");
        return 1;
    }

    // create socket
    struct sockaddr_in sock;

    sock.sin_family = AF_INET;
    sock.sin_port = htons(1111);
    socklen_t len = sizeof(sock);

    // use the program input for the ip address.
    int success = inet_aton(argv[1], &sock.sin_addr);

    if(!success)
    {
        printf("Invalid address input\n");
        return 1;
    }

    // open socket file descriptor
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0); // maybe use SOCK_RAW? or ipproto = 0

    if(socket_fd == -1)
    {
        printf("Error calling socket(): %s \n", strerror(errno));
        return 1;
    }

    int connected = connect(socket_fd, (const struct sockaddr* ) &sock, len);

    if(connected == -1)
    {
        printf("Error calling connect(): %s \n", strerror(errno));
        return 1;
    }
    printf("Connected!\n");

    size_t buffer_len = 256;
    char* message = "Hello Server from Miles Dripps!";
    char incoming_buffer[buffer_len];
    int flags = 0; 




    // SEND 
    ssize_t sent = send(socket_fd, message, strlen(message), flags);
    printf("Sent %zu bytes. \n", sent);

    // RECEIVE
    ssize_t recvd = 0;
    recvd += recv(socket_fd, incoming_buffer + recvd, buffer_len, flags);
    printf("Received %zu bytes: %s.\n", recvd, incoming_buffer);

    int closed = close(socket_fd);  
    printf("Closed.\n");
}
