#include <unistd.h>          // Para read(), write(), close()
#include <netinet/in.h>      // Para struct sockaddr_in
#include <arpa/inet.h>       // Para inet_aton()
#include <sys/types.h>       // Para tipos de datos
#include <sys/socket.h>      // Para funciones de socket
#include <stdlib.h>          // Para exit(), atoi()
#include <stdio.h>           // Para printf(), perror()
#include <string.h>          // Para memset(), strlen()
#include <time.h>            // Para manejo de tiempos
#include <ctype.h>           // Para isdigit()
#include <errno.h>           // Para errno

#define BUFFER_SIZE 16384    // Aumentamos mucho más el tamaño del buffer

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
    
    // Primero recibimos la longitud esperada del mensaje
    int longitud_esperada = 0;
    int recibido = recv(socket_cliente, &longitud_esperada, sizeof(int), 0);
    
    if (recibido <= 0) {
        return -1;
    }
    
    // Si la longitud esperada excede nuestro buffer, ajustamos para evitar desbordamiento
    if (longitud_esperada >= buffer_size) {
        longitud_esperada = buffer_size - 1;
    }
    
    int total_recibidos = 0;
    int intentos = 0;
    
    // Bucle para recibir datos hasta completar o timeout
    while (total_recibidos < longitud_esperada && intentos < 15) {
        recibido = recv(socket_cliente, buffer + total_recibidos,
                       buffer_size - total_recibidos - 1, 0);
        
        if (recibido <= 0) {
            // Si es timeout, esperar un poco e intentar de nuevo
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                usleep(100000); // 100ms
                intentos++;
                continue;
            } else {
                // Error real o conexión cerrada
                break;
            }
        }
        
        total_recibidos += recibido;
        intentos = 0; // Resetear intentos si recibimos datos
    }
    
    if (total_recibidos > 0) {
        buffer[total_recibidos] = '\0';
    }
    
    return total_recibidos;
}

/**
 * Función para verificar que la entrada sea un número válido
 */
int es_numero_valido(const char *str) {
    if(str == NULL || *str == '\0') {
        return 0;  // Cadena vacía
    }
    
    while(*str) {
        if(!isdigit(*str)) {
            return 0;  // Contiene caracteres no numéricos
        }
        str++;
    }
    
    return 1;  // Solo contiene dígitos
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
    tv.tv_sec = 30;  // Aumentamos a 30 segundos de timeout
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

    // Bucle principal
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
        printf("> "); // Añadir un prompt claro
        fflush(stdout); // Forzar la salida del buffer

        // Leer entrada del usuario
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
        
        // Validar entrada
        if (strlen(input) == 0 || !es_numero_valido(input)) {
            printf("⚠ Por favor ingrese un número válido.\n");
            strcpy(input, "0");  // Opción por defecto segura
        }
        
        // Añadir mensaje de confirmación para depuración
        printf("Enviando opción: '%s'\n", input);
        
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
        if (strcmp(input, "5") == 0 && strstr(buffer, "Seleccione una categoria (1-5)") != NULL) {
            printf("Saliendo de la aplicación...\n");
            break;
        }
        
        // Pequeña pausa para dar tiempo al servidor a procesar
        usleep(100000);  // 100ms
    }

    // Cierre
    close(socket_cliente);
    printf("\nConexión cerrada. Hasta pronto!\n");
    return 0;
}
