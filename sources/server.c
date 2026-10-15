#include "../headers/headers.h"

unsigned char* key = (unsigned char*) "Hello World!!! This msg is 32 b";
unsigned char* iv = (unsigned char*) "Hello World Init Vector";
unsigned char aad[] = "";
int aad_len = 0;

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
    unsigned char incoming_buffer[1024];
    unsigned char decrypted_message[1024];

    int flags = 0;

    ssize_t recvd = recv(
        new_socket_fd, incoming_buffer, sizeof(incoming_buffer), flags
    );

    unsigned char iv[IV_LEN];
    unsigned char tag[TAG_LEN];

    memcpy(iv, incoming_buffer, IV_LEN);

    int ciphertext_len = (int)recvd - IV_LEN - TAG_LEN;

    unsigned char *ciphertext = incoming_buffer + IV_LEN;

    memcpy(tag, incoming_buffer + IV_LEN + ciphertext_len, TAG_LEN);

    printf("Encrypted bytes: ");
    for (int i = 0; i < ciphertext_len; i++)
        printf("%02x", ciphertext[i]);
    printf("\n");

    int plaintext_len = gcm_decrypt(
        ciphertext, ciphertext_len,
        aad, aad_len,
        tag, key, iv, IV_LEN,
        decrypted_message
    );

    if (plaintext_len >= 0) {
        decrypted_message[plaintext_len] = '\0';
        printf("Decrypted message: %s\n", decrypted_message);
    } else {
        printf("Decryption failed: auth tag mismatch.\n");
    }

    // close the connection
    int closed = close(new_socket_fd);
    printf("[Server] Closed.\n");
}