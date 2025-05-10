#include "../include/core/app_manager.h"
#include "../include/ui/main_window.h"
#include "../include/net/server_connector.h"
#include "../include/config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int app_manager_ejecutar(int socket_cliente) {
    char buffer[BUFFER_SIZE] = {0};
    char input[256] = {0};  // Buffer para entrada del usuario
    
    // Bucle principal mejorado
    while (1) {
        // Recibir menú/respuesta con mejor manejo de errores
        int bytes_recibidos = server_connector_recibir_datos(socket_cliente, buffer, BUFFER_SIZE);
        
        if (bytes_recibidos <= 0) {
            ui_mostrar_mensaje("\nLa conexión con el servidor se ha perdido o ha expirado el tiempo de espera\n");
            break;
        }
        
        // Comprobar si es un mensaje de sincronización
        if(strncmp(buffer, "<SYNC>", 6) == 0) {
            server_connector_manejar_sync(socket_cliente);
            continue;
        }
        
        // Mostrar lo recibido (menú, productos, etc.)
        ui_mostrar_mensaje(buffer);
        
        // Manejar salida si es el mensaje de despedida
        if (strstr(buffer, "Sesión finalizada") != NULL || 
            strstr(buffer, "Gracias por su compra") != NULL) {
            ui_mostrar_mensaje("\nFinalizando sesión...\n");
            break;
        }

        // Obtener entrada del usuario
        memset(input, 0, sizeof(input));
        if (!ui_obtener_entrada("> ", input, sizeof(input))) {
            ui_mostrar_error("Error de entrada. Saliendo...");
            break;
        }
        
        // Si la entrada está vacía, proporcionar un valor por defecto
        if (strlen(input) == 0) {
            strcpy(input, "0");  // Opción por defecto para volver
        }
        
        // Añadir mensaje de confirmación para depuración
        printf("Enviando opción: '%s'\n", input);
        
        // Enviar opción al servidor
        if (!server_connector_enviar_datos(socket_cliente, input)) {
            ui_mostrar_error("No se pudieron enviar datos al servidor después de varios intentos");
            break;
        }

        // Si el usuario eligió salir (opción 5 en menú principal)
        if (strcmp(input, "5") == 0) {
            ui_mostrar_mensaje("Saliendo de la aplicación...\n");
            break;
        }
    }
    
    return 0;
}