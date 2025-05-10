#include "../include/net/server_connector.h"
#include "../include/config.h"
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

int server_connector_iniciar(const char *ip_servidor, const char *puerto) {
    int socket_cliente;
    struct sockaddr_in dir_servidor;
    
    // Configuración del servidor
    dir_servidor.sin_family = AF_INET;
    dir_servidor.sin_port = htons(atoi(puerto));
    inet_aton(ip_servidor, &(dir_servidor.sin_addr));
    memset(&(dir_servidor.sin_zero), '\0', 8);

    // Crear socket
    socket_cliente = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_cliente == -1) {
        perror("Error al crear el socket");
        return -1;
    }

    // Configurar timeout para socket
    struct timeval tv;
    tv.tv_sec = DEFAULT_TIMEOUT;
    tv.tv_usec = 0;
    setsockopt(socket_cliente, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);

    // Conexión
    socklen_t tam = sizeof(struct sockaddr);
    if (connect(socket_cliente, (struct sockaddr *)&dir_servidor, tam) == -1) {
        perror("Error al conectar con el servidor");
        close(socket_cliente);
        return -1;
    }
    
    return socket_cliente;
}

void server_connector_cerrar(int socket_cliente) {
    close(socket_cliente);
}

void server_connector_manejar_sync(int socket_cliente) {
    char buffer[10];
    memset(buffer, 0, sizeof(buffer));
    
    // Leer mensaje de sincronización
    recv(socket_cliente, buffer, sizeof(buffer), 0);
    
    // Si es un mensaje de sincronización, responder
    if(strcmp(buffer, "<SYNC>") == 0) {
        send(socket_cliente, "<SYNC_ACK>", 10, 0);
    }
}

int server_connector_recibir_datos(int socket_cliente, char *buffer, int buffer_size) {
    memset(buffer, 0, buffer_size);
    
    // Recibir datos con timeout
    fd_set read_fds;
    struct timeval timeout;
    
    FD_ZERO(&read_fds);
    FD_SET(socket_cliente, &read_fds);
    
    timeout.tv_sec = DEFAULT_TIMEOUT;
    timeout.tv_usec = 0;
    
    // Esperar datos con timeout
    int ret = select(socket_cliente + 1, &read_fds, NULL, NULL, &timeout);
    if(ret <= 0) {
        // Timeout o error
        return -1;
    }
    
    // Implementación mejorada para recibir datos completos
    int bytes_recibidos = 0;
    int total_recibidos = 0;
    
    // Leer hasta que se reciba todo el mensaje o se llene el buffer
    while (total_recibidos < buffer_size - 1) {
        bytes_recibidos = recv(socket_cliente, buffer + total_recibidos, buffer_size - total_recibidos - 1, 0);
        
        if (bytes_recibidos <= 0) {
            break;  // Error o conexión cerrada
        }
        
        total_recibidos += bytes_recibidos;
        
        // Comprobar si hay más datos pendientes
        fd_set check_fds;
        struct timeval quick_check;
        
        FD_ZERO(&check_fds);
        FD_SET(socket_cliente, &check_fds);
        
        quick_check.tv_sec = 0;
        quick_check.tv_usec = DEFAULT_QUICK_CHECK_TIMEOUT;
        
        ret = select(socket_cliente + 1, &check_fds, NULL, NULL, &quick_check);
        if (ret <= 0) {
            break;  // No hay más datos o error
        }
    }
    
    if(total_recibidos > 0) {
        buffer[total_recibidos] = '\0';
    }
    
    return total_recibidos;
}

int server_connector_enviar_datos(int socket_cliente, const char *datos) {
    // Enviar opción al servidor con retry en caso de error
    int intentos = 0;
    int enviado = 0;
    
    while (intentos < 3 && !enviado) {
        if (send(socket_cliente, datos, strlen(datos), 0) > 0) {
            enviado = 1;
        } else {
            perror("Error al enviar datos");
            intentos++;
            usleep(500000);  // Esperar 500ms antes de reintentar
        }
    }
    
    return enviado;
}