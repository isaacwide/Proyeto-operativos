#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char *products[] = {
    "1. juan- $1200",
    "2. hermeneguildo - $800",
    "3. pablito - $350",
    "4. bato loco - $150"
};

void enviar_menu(int sock) {
    char menu[1024] = "\n=== MENÚ DE PRODUCTOS ===\n";
    for (int i = 0; i < 4; i++) {
        strcat(menu, products[i]);
        strcat(menu, "\n");
    }
    strcat(menu, "5. Salir\nSeleccione una opción: ");
    send(sock, menu, strlen(menu), 0);
}

void manejar_cliente(int cliente) {
    char buffer[1024];
    while (1) {
        enviar_menu(cliente);

        memset(buffer, 0, sizeof(buffer));
        int recibido = recv(cliente, buffer, sizeof(buffer) - 1, 0);
        if (recibido <= 0) break;

        int opcion = atoi(buffer);
        char respuesta[256];

        if (opcion >= 1 && opcion <= 4) {
            snprintf(respuesta, sizeof(respuesta), "Has comprado: %s\n", products[opcion - 1]);
        } else if (opcion == 5) {
            strcpy(respuesta, "Gracias por visitar la tienda. ¡Hasta luego!\n");
            send(cliente, respuesta, strlen(respuesta), 0);
            break;
        } else {
            strcpy(respuesta, "Opción inválida. Intente nuevamente.\n");
        }

        send(cliente, respuesta, strlen(respuesta), 0);
    }

    close(cliente);
    exit(0);
}

int main(int argc, char *argv[]) {
    int mi_socket, nuevo, tam;
    struct sockaddr_in mi_estructura;

    mi_estructura.sin_family = AF_INET;
    mi_estructura.sin_port = htons(8080);
    mi_estructura.sin_addr.s_addr = INADDR_ANY;
    memset(&(mi_estructura.sin_zero), '\0', 8);

    mi_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (mi_socket == -1) {
        perror("Error al crear socket");
        exit(1);
    }

    tam = sizeof(struct sockaddr);
    if (bind(mi_socket, (struct sockaddr *)&mi_estructura, tam) == -1) {
        perror("Error en bind");
        exit(1);
    }

    if (listen(mi_socket, 5) == -1) {
        perror("Error en listen");
        exit(1);
    }

    printf("Servidor esperando conexiones en puerto 8080...\n");

    while (1) {
        nuevo = accept(mi_socket, (struct sockaddr *)&mi_estructura, &tam);
        if (nuevo == -1) {
            perror("Error en accept");
            continue;
        }

        if (fork() == 0) {
            close(mi_socket);
            manejar_cliente(nuevo);
        } else {
            close(nuevo);
        }
    }

    return 0;
}