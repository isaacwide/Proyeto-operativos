#include <unistd.h>          // Para read(), write(), close()
#include <netinet/in.h>      // Para struct sockaddr_in
#include <arpa/inet.h>       // Para inet_aton()
#include <sys/types.h>       // Para tipos de datos
#include <sys/socket.h>      // Para funciones de socket
#include <stdlib.h>          // Para exit(), atoi()
#include <stdio.h>           // Para printf(), perror()
#include <string.h>          // Para memset(), strlen()

#define BUFFER_SIZE 2048     // Tamaño del buffer de comunicación

int main(int argc, char *argv[]) {
    int socket_cliente;
    socklen_t tam;
    char buffer[BUFFER_SIZE] = {0};
    char input[100] = {0};

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

    // Conexión
    tam = sizeof(struct sockaddr);
    printf("Conectando al servidor %s:%s...\n", argv[1], argv[2]);
    if (connect(socket_cliente, (struct sockaddr *)&dir_servidor, tam) == -1) {
        perror("Error al conectar con el servidor");
        exit(1);
    }
    printf("Conexión establecida. Bienvenido a la tienda interactiva!\n");

    // Bucle principal
    while (1) {
        // Recibir menú
        memset(buffer, 0, BUFFER_SIZE);
        if (read(socket_cliente, buffer, BUFFER_SIZE) <= 0) break;
        printf("%s", buffer);
    
        // Enviar opción
        printf("> ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0; // Eliminar salto de línea
        
        if (send(socket_cliente, input, strlen(input), 0) <= 0) break;
    
        // Recibir respuesta específica (para añadir productos, etc.)
        memset(buffer, 0, BUFFER_SIZE);
        if (read(socket_cliente, buffer, BUFFER_SIZE) <= 0) break;
        if (strlen(buffer) > 0) printf("%s\n", buffer);
    
        if (atoi(input) == 5) break; // Salir si eligió 5
    }

    // Cierre
    close(socket_cliente);
    printf("Conexión cerrada. Hasta pronto!\n");
    return 0;
}
