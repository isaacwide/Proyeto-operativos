#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SIZE 1024

int main(int argc, char *argv[]) {
    int mi_socket, tam;
    char buffer[SIZE] = {0};
    char input[100];  // <-- Declaramos input
    struct sockaddr_in mi_estructura;

    if (argc != 3) {
        printf("Error: modo de empleo: %s <IP> <PUERTO>\n", argv[0]);
        exit(-1);
    }

    // Configuración de la estructura del servidor
    mi_estructura.sin_family = AF_INET;
    mi_estructura.sin_port = htons(atoi(argv[2]));
    inet_aton(argv[1], &(mi_estructura.sin_addr));
    memset(&(mi_estructura.sin_zero), '\0', 8);

    // Crear socket
    mi_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (mi_socket == -1) {
        perror("Error creando socket");
        exit(1);
    }

    // Conectar al servidor
    tam = sizeof(struct sockaddr);
    if (connect(mi_socket, (struct sockaddr *)&mi_estructura, tam) == -1) {
        perror("Error en connect");
        exit(1);
    }

    // Bucle de comunicación con el servidor
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        //lee lo que envia el servidor 
        read(mi_socket, buffer, sizeof(buffer));
        printf("%s", buffer);

        printf("> ");
        //toma lo que escriba el usuario 
        fgets(input, sizeof(input), stdin);
        //envia los datos al servidor 
        send(mi_socket, input, strlen(input), 0);

        if (atoi(input) == 5) break;

        memset(buffer, 0, sizeof(buffer));
        read(mi_socket, buffer, sizeof(buffer));
        printf("%s\n", buffer);
    }

    close(mi_socket);
    return 0;
}