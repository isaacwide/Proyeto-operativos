#include "../include/net/server_manager.h"
#include "../include/net/client_handler.h"
#include "../include/data/store.h"
#include "../include/config.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>

int main(int argc, char *argv[]) {
    int socket_servidor, nuevo_socket;
    socklen_t tam;
    struct sockaddr_in dir_cliente;
    
    // Inicializar los datos
    store_inicializar();
    
    // Inicializar semilla para números aleatorios
    srand(time(NULL));
    
    // Iniciar el servidor
    socket_servidor = server_manager_iniciar(PUERTO_SERVIDOR);
    if (socket_servidor == -1) {
        exit(1);
    }
    
    printf("Servidor esperando conexiones en puerto %d...\n", PUERTO_SERVIDOR);
    
    // Bucle principal del servidor
    while (1) {
        tam = sizeof(struct sockaddr_in);
        
        // Aceptar nueva conexión
        nuevo_socket = accept(socket_servidor, (struct sockaddr *)&dir_cliente, &tam);
        if (nuevo_socket == -1) {
            perror("Error en accept");
            continue;  // Continuar esperando conexiones
        }
        
        // Crear proceso hijo para manejar cliente
        pid_t pid = fork();
        if (pid == 0) { // Proceso hijo
            close(socket_servidor);  // El hijo no necesita el socket principal
            client_handler_manejar_cliente(nuevo_socket);
            exit(0);
        } else if (pid > 0) { // Proceso padre
            close(nuevo_socket);  // El padre no necesita el socket del cliente
        } else {
            perror("fork");  // Error al crear proceso hijo
            close(nuevo_socket);
        }
    }
    
    // Cerrar servidor (nunca se alcanza en este ejemplo)
    server_manager_cerrar(socket_servidor);
    return 0;
}