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
    unsigned char* message = "Hello Server from Miles Dripps!";
    int flags = 0; 

    unsigned char message_hash[1024];
    unsigned char hash_len = cal_hmac(message_hash, (unsigned char*) message);

    
    unsigned char message_with_hash[2048];
    memcpy(message_with_hash, message, 31);

    memcpy(message_with_hash + 31, message_hash, hash_len);

    // Send 
    ssize_t sent = send(socket_fd, message_with_hash, strlen(message_with_hash), flags);
    printf("[Client] Sent: %s with hash  %s to server\n", message, message_hash);

    // Close Connection
    int closed = close(socket_fd);  
    printf("Closed.\n");
}


