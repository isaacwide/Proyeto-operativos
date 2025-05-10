#include "../include/net/server_manager.h"
#include "../include/config.h"
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int server_manager_iniciar(int puerto) {
    int socket_servidor;
    struct sockaddr_in dir_servidor;
    
    // Configuración de la dirección del servidor
    dir_servidor.sin_family = AF_INET;
    dir_servidor.sin_port = htons(puerto);
    dir_servidor.sin_addr.s_addr = INADDR_ANY;
    memset(&(dir_servidor.sin_zero), '\0', 8);
    
    // Crear socket
    socket_servidor = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_servidor == -1) {
        perror("Error al crear socket");
        return -1;
    }
    
    // Configurar socket para reutilizar dirección
    int opt = 1;
    if (setsockopt(socket_servidor, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("Error en setsockopt");
        close(socket_servidor);
        return -1;
    }
    
    // Vincular socket a dirección y puerto
    if (bind(socket_servidor, (struct sockaddr *)&dir_servidor, sizeof(struct sockaddr)) == -1) {
        perror("Error en bind");
        close(socket_servidor);
        return -1;
    }
    
    // Poner socket en modo escucha
    if (listen(socket_servidor, MAX_CONEXIONES) == -1) {
        perror("Error en listen");
        close(socket_servidor);
        return -1;
    }
    
    return socket_servidor;
}

void server_manager_cerrar(int socket_servidor) {
    close(socket_servidor);
}

