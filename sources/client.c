#include "../headers/headers.h"

unsigned char* key = (unsigned char*) "Hello World Key";
unsigned char* iv = (unsigned char*) "Hello World Init Vector";

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
        printf("[Client] Invalid address input\n");
        return 1;
    }

    // open socket file descriptor
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0); // maybe use SOCK_RAW? or ipproto = 0

    if(socket_fd == -1)
    {
        printf("[Client] Error calling socket(): %s \n", strerror(errno));
        return 1;
    }

    int connected = connect(socket_fd, (const struct sockaddr* ) &sock, len);

    if(connected == -1)
    {
        printf("[Client] Error calling connect(): %s \n", strerror(errno));
        return 1;
    }
    printf("[Client] Connected!\n");

    // Create Message
    char* message = "Hello Server from Miles Dripps!";
    int flags = 0; 


    // Encrypt Message
    unsigned char* message_encrypted[256];
    block_encrypt((unsigned char*)message, (int) strlen(message), key, iv, message_encrypted);


    // Send 
    ssize_t sent = send(socket_fd, message_encrypted, strlen(message_encrypted), flags);
    printf("[Client] Sent: %s -> Encrypted to -> %s \n", message, message_encrypted);
    // printf("[Client] Sent Encrypted Message\n");


    // Receive
    size_t buffer_len = 256;
    char incoming_buffer[buffer_len];

    ssize_t recvd = 0;
    recvd += recv(socket_fd, incoming_buffer + recvd, buffer_len, flags);
    printf("[Client] Received Encrypted Message: %s \n", incoming_buffer);
    // printf("[Client] Received Encrypted Message\n");

    // Decrypt Message
    char* decrypted_message[256];
    block_decrypt((unsigned char*)incoming_buffer, (int) recvd, key, iv, (unsigned char*)decrypted_message);

    printf("[Client] Decrypted Message: %s \n", decrypted_message);

    // Close Connection
    int closed = close(socket_fd);  
    printf("Closed.\n");
}


