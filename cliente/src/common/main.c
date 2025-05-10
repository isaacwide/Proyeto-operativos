#include "../include/net/server_connector.h"
#include "../include/core/app_manager.h"
#include "../include/ui/main_window.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int socket_cliente;

    // Verificación de argumentos
    if (argc != 3) {
        printf("Uso correcto: %s <DIRECCION_IP> <PUERTO>\n", argv[0]);
        printf("Ejemplo: %s 127.0.0.1 8080\n", argv[0]);
        exit(-1);
    }

    // Conexión al servidor
    printf("Conectando al servidor %s:%s...\n", argv[1], argv[2]);
    socket_cliente = server_connector_iniciar(argv[1], argv[2]);
    if (socket_cliente == -1) {
        exit(1);
    }
    
    printf("Conexión establecida. Bienvenido a la tienda interactiva!\n\n");

    // Ejecutar la aplicación
    app_manager_ejecutar(socket_cliente);
    
    // Cerrar la conexión
    server_connector_cerrar(socket_cliente);
    ui_mostrar_despedida();
    
    return 0;
}