#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    int mi_socket, nuevo, tam;
    struct sockaddr_in mi_estructura;
    
    // Configuración del socket
    mi_estructura.sin_family = AF_INET;  // Faltaba esta línea importante!
    mi_estructura.sin_port = htons(8080);  // Usamos un puerto específico
    mi_estructura.sin_addr.s_addr = INADDR_ANY;
    memset(&(mi_estructura.sin_zero), '\0', 8);  // Corregido a 8 bytes
    
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
            // Proceso hijo
            close(mi_socket);
            
            // Menú de productos
            char menu[] = "\n\n=== MENÚ DE PRODUCTOS ===\n"
                          "1. Laptop Gamer - $1200\n"
                          "2. Smartphone Pro - $800\n"
                          "3. Tablet HD - $350\n"
                          "4. Auriculares Bluetooth - $150\n"
                          "5. Salir\n"
                          "Seleccione una opción: ";
            
            send(nuevo, menu, strlen(menu), 0);
            
            // Aquí podrías agregar lógica para recibir la selección del cliente
            // char buffer[256];
            // recv(nuevo, buffer, 255, 0);
            // procesar selección...
            
            close(nuevo);
            exit(0);
        } else {
            close(nuevo);
        }
    }
    
    return 0;
}
