#include "../headers/headers.h"

unsigned char* key = (unsigned char*) "Hello World Key";
unsigned char* iv = (unsigned char*) "Hello World Init Vector";

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
    size_t buffer_len = 256;
    char incoming_buffer[buffer_len];
    int flags = 0; 

    // Receive Message
    ssize_t recvd = 0;
    recvd += recv(new_socket_fd, incoming_buffer, buffer_len, flags);
    printf("[Server] Received Encrypted Message: %s\n", incoming_buffer);


    // Decrypt Message
    char* decrypted_message[256];
    block_decrypt((unsigned char*)incoming_buffer, (int) recvd, key, iv, (unsigned char*)decrypted_message);

    printf("[Server] Decrypted Message: %s \n", decrypted_message);

    // Create message
    char* message = "Hello Miles Dripps from Server!!!";

    // Encrypt Message
    unsigned char* message_encrypted[256];
    block_encrypt((unsigned char*)message, (int) strlen(message), key, iv, message_encrypted);

    // Send Message
    ssize_t sent = send(new_socket_fd, message_encrypted, strlen(message_encrypted), flags);
    printf("[Server] Sent: %s -> Encrypted -> %s \n", message, message_encrypted);


    // close connected socket
    int closed = close(new_socket_fd);  
    printf("[Server] Closed.\n");


}