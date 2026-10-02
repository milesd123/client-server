#include "../headers/headers.h"

unsigned char* key = (unsigned char*) "Hello World Key";
unsigned char* iv = (unsigned char*) "Hello World Init Vector";

int same(unsigned char* a, unsigned char* b, size_t size)
{

    for(size_t i = 0; i < size; i++)
    {
        if(a[i] != b[i]) return 0;
    }

    return 1; // true, they are the same
}


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
        printf("[Server] Invalid address input\n");
        return 1;
    }

    // open a socket file descriptor
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    if(socket_fd == -1)
    {
        printf("[Server] Error calling socket(): %s \n", strerror(errno));
        return 1;
    }

    // bind address to socket
    socklen_t len = sizeof(sock);
    int bound = bind(socket_fd, (const struct sockaddr* ) &sock, len);

    if(bound == -1)
    {
        printf("[Server] Error calling bind(): %s \n", strerror(errno));
        return 1;
    }

    // listen for connections
    int l = listen(socket_fd, 16);

    if(l == -1)
    {
        printf("[Server] Error calling listen(): %s \n", strerror(errno));
        return 1;
    }
    printf("[Server] Listening for connections\n");

    // accept new connections
    struct sockaddr_in peer_socket;
    socklen_t peer_len = sizeof(peer_socket);

    // blocks
    int new_socket_fd = accept(socket_fd, (struct sockaddr*) &peer_socket, &peer_len);

    if(new_socket_fd == -1)
    {
        printf("[Server] Error calling listen(): %s \n", strerror(errno));
        return 1;
    }
    printf("[Server] Client connected. \n");

    // incoming message buffer
    size_t buffer_len = 2048;
    unsigned char incoming_buffer[buffer_len];
    int flags = 0; 

    // Receive Message
    ssize_t recvd = 0;
    recvd += recv(new_socket_fd, incoming_buffer, buffer_len, flags);

    unsigned char message_hash[1024];
    unsigned char message[32];
    memcpy(message, incoming_buffer, 31);
    memcpy(message_hash, incoming_buffer + 31, recvd - 31);



    printf("[Server] Received %zu bytes. Message: %s with hash\n", recvd, message);


    unsigned char cal_hash[1024];
    unsigned amt = cal_hmac(cal_hash, message);

    if(amt != recvd - 31) {printf("[Server] Hashes not same size");}
    else{
        if(same(cal_hash, message_hash, amt)) printf("[Server] Hashes %s and %s Match\n", cal_hash, message_hash);
        else printf("[Server] Hashes %s and %s Don't Match\n", cal_hash, message_hash);
    }
    


    // close connected socket
    int closed = close(new_socket_fd);  
    printf("[Server] Closed.\n");


}