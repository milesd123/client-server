#include "../headers/headers.h"

unsigned char* key = (unsigned char*) "Hello World!!! This msg is 32 b";
unsigned char* iv = (unsigned char*) "Hello World Init Vector";
unsigned char aad[] = "";
int aad_len = 0;

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
        unsigned char *message = "Hello Server from Miles Dripps!";

        int flags = 0;
        int message_len = strlen((char *)message);

        unsigned char iv[IV_LEN];
        unsigned char tag[TAG_LEN];
        unsigned char ciphertext[1024];
        unsigned char message_with_hash[1024];

        RAND_bytes(iv, IV_LEN);

        int ciphertext_len = gcm_encrypt(
            message, message_len,
            aad, aad_len,
            key, iv, IV_LEN,
            ciphertext, tag
        );

        // IV || ciphertext || tag 
        memcpy(message_with_hash, iv, IV_LEN);
        memcpy(message_with_hash + IV_LEN, ciphertext, ciphertext_len);
        memcpy(message_with_hash + IV_LEN + ciphertext_len, tag, TAG_LEN);

        int packet_len = IV_LEN + ciphertext_len + TAG_LEN;

        // SEND
        ssize_t sent = send(socket_fd, message_with_hash, packet_len, flags);

        printf("Plaintext sent: %s\n", message);
        printf("Encrypted bytes: ");
        for (int i = 0; i < ciphertext_len; i++){
            printf("%02x", ciphertext[i]);
        }
    printf("\n");

    //Close connection
    int closed = close(socket_fd);
    printf("Closed.\n");
}


