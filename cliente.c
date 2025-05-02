#include <unistd.h>          // Para read(), write(), close()
#include <netinet/in.h>      // Para struct sockaddr_in
#include <arpa/inet.h>       // Para inet_aton()
#include <sys/types.h>       // Para tipos de datos
#include <sys/socket.h>      // Para funciones de socket
#include <stdlib.h>          // Para exit(), atoi()
#include <stdio.h>           // Para printf(), perror()
#include <string.h>          // Para memset(), strlen()
#include <time.h>            // Para manejo de tiempos

#define BUFFER_SIZE 8192     // Aumentamos aún más el tamaño para manejar respuestas grandes

/**
 * Función para limpiar el buffer de entrada
 */
void limpiar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

/**
 * Función para manejar mensajes de sincronización del servidor
 */
void manejar_sync(int socket_cliente) {
    char buffer[10];
    memset(buffer, 0, sizeof(buffer));
    
    // Leer mensaje de sincronización
    recv(socket_cliente, buffer, sizeof(buffer), 0);
    
    // Si es un mensaje de sincronización, responder
    if(strcmp(buffer, "<SYNC>") == 0) {
        send(socket_cliente, "<SYNC_ACK>", 10, 0);
    }
}

/**
 * Función para recibir datos del servidor con mejor manejo de errores
 */
int recibir_datos(int socket_cliente, char *buffer, int buffer_size) {
    memset(buffer, 0, buffer_size);
    
    // Recibir datos con timeout
    fd_set read_fds;
    struct timeval timeout;
    
    FD_ZERO(&read_fds);
    FD_SET(socket_cliente, &read_fds);
    
    timeout.tv_sec = 5;  // 5 segundos de timeout
    timeout.tv_usec = 0;
    
    // Esperar datos con timeout
    int ret = select(socket_cliente + 1, &read_fds, NULL, NULL, &timeout);
    if(ret <= 0) {
        // Timeout o error
        return -1;
    }
    
    // Recibir datos
    int bytes_recibidos = read(socket_cliente, buffer, buffer_size - 1);
    if(bytes_recibidos > 0) {
        buffer[bytes_recibidos] = '\0';
    }
    
    return bytes_recibidos;
}

/**
 * Función principal mejorada
 */
int main(int argc, char *argv[]) {
    int socket_cliente;
    socklen_t tam;
    char buffer[BUFFER_SIZE] = {0};
    char input[256] = {0};  // Aumentamos el tamaño para entradas más largas

    // Verificación de argumentos
    if (argc != 3) {
        printf("Uso correcto: %s <DIRECCION_IP> <PUERTO>\n", argv[0]);
        printf("Ejemplo: %s 127.0.0.1 8080\n", argv[0]);
        exit(-1);
    }

    // Configuración del servidor
    struct sockaddr_in dir_servidor;
    dir_servidor.sin_family = AF_INET;
    dir_servidor.sin_port = htons(atoi(argv[2]));
    inet_aton(argv[1], &(dir_servidor.sin_addr));
    memset(&(dir_servidor.sin_zero), '\0', 8);

    // Crear socket
    socket_cliente = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_cliente == -1) {
        perror("Error al crear el socket");
        exit(1);
    }

    // Configurar timeout para socket (evita bloqueos indefinidos)
    struct timeval tv;
    tv.tv_sec = 10;  // 10 segundos de timeout
    tv.tv_usec = 0;
    setsockopt(socket_cliente, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);

    // Conexión
    tam = sizeof(struct sockaddr);
    printf("Conectando al servidor %s:%s...\n", argv[1], argv[2]);
    if (connect(socket_cliente, (struct sockaddr *)&dir_servidor, tam) == -1) {
        perror("Error al conectar con el servidor");
        exit(1);
    }
    printf("Conexión establecida. Bienvenido a la tienda interactiva!\n\n");

    // Bucle principal mejorado
    while (1) {
        // Recibir menú/respuesta con mejor manejo de errores
        int bytes_recibidos = recibir_datos(socket_cliente, buffer, BUFFER_SIZE);
        
        if (bytes_recibidos <= 0) {
            printf("\nLa conexión con el servidor se ha perdido o ha expirado el tiempo de espera\n");
            break;
        }
        
        // Comprobar si es un mensaje de sincronización
        if(strncmp(buffer, "<SYNC>", 6) == 0) {
            send(socket_cliente, "<SYNC_ACK>", 10, 0);
            continue;
        }
        
        // Mostrar lo recibido (menú, productos, etc.)
        printf("%s", buffer);
        
        // Manejar salida si es el mensaje de despedida
        if (strstr(buffer, "Sesión finalizada") != NULL || 
            strstr(buffer, "Gracias por su compra") != NULL) {
            printf("\nFinalizando sesión...\n");
            break;
        }

        // Obtener entrada del usuario con mejor manejo de errores
        memset(input, 0, sizeof(input));
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Error de entrada. Saliendo...\n");
            break;
        }
        
        // Limpiar buffer de entrada si es necesario
        if (strchr(input, '\n') == NULL) {
            limpiar_buffer();
        } else {
            input[strcspn(input, "\n")] = 0; // Eliminar salto de línea
        }
        
        // Si la entrada está vacía, proporcionar un valor por defecto
        if (strlen(input) == 0) {
            strcpy(input, "0");  // Opción por defecto para volver
        }
        
        // Enviar opción al servidor con retry en caso de error
        int intentos = 0;
        int enviado = 0;
        
        while (intentos < 3 && !enviado) {
            if (send(socket_cliente, input, strlen(input), 0) > 0) {
                enviado = 1;
            } else {
                perror("Error al enviar datos");
                intentos++;
                usleep(500000);  // Esperar 500ms antes de reintentar
            }
        }
        
        if (!enviado) {
            printf("No se pudieron enviar datos al servidor después de varios intentos\n");
            break;
        }

        // Si el usuario eligió salir (opción 5 en menú principal)
        if (atoi(input) == 5) {
            printf("Saliendo de la aplicación...\n");
            break;
        }
    }

    // Cierre
    close(socket_cliente);
    printf("\nConexión cerrada. Hasta pronto!\n");
    return 0;
}