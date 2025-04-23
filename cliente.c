#include <unistd.h>          // Para read(), write(), close()
#include <netinet/in.h>      // Para struct sockaddr_in
#include <arpa/inet.h>       // Para inet_aton()
#include <sys/types.h>       // Para tipos de datos
#include <sys/socket.h>      // Para funciones de socket
#include <stdlib.h>          // Para exit(), atoi()
#include <stdio.h>           // Para printf(), perror()
#include <string.h>          // Para memset(), strlen()

#define BUFFER_SIZE 1024     // Tamaño del buffer de comunicación

int main(int argc, char *argv[]) {
    int socket_cliente;      // Descriptor del socket
    socklen_t tam;           // Tamaño de la estructura de dirección
    char buffer[BUFFER_SIZE] = {0};  // Buffer para datos recibidos
    char input[100] = {0};   // Buffer para entrada del usuario
    
    // Verificación de argumentos
    if (argc != 3) {
        printf("Uso correcto: %s <DIRECCION_IP> <PUERTO>\n", argv[0]);
        printf("Ejemplo: %s 127.0.0.1 8080\n", argv[0]);
        exit(-1);
    }

    // Configuración de la estructura del servidor
    struct sockaddr_in dir_servidor;
    dir_servidor.sin_family = AF_INET;                     // Familia IPv4
    dir_servidor.sin_port = htons(atoi(argv[2]));          // Puerto del servidor
    inet_aton(argv[1], &(dir_servidor.sin_addr));          // Dirección IP
    memset(&(dir_servidor.sin_zero), '\0', 8);             // Relleno con ceros

    // Creación del socket TCP
    socket_cliente = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_cliente == -1) {
        perror("Error al crear el socket");
        exit(1);
    }

    // Conexión al servidor
    tam = sizeof(struct sockaddr);
    printf("Conectando al servidor %s:%s...\n", argv[1], argv[2]);
    if (connect(socket_cliente, (struct sockaddr *)&dir_servidor, tam) == -1) {
        perror("Error al conectar con el servidor");
        exit(1);
    }
    printf("Conexión establecida. Bienvenido a la tienda de videojuegos!\n");

    // Bucle principal de interacción con el servidor
    while (1) {
        // Recibe y muestra el menú del servidor
        memset(buffer, 0, BUFFER_SIZE);
        if (read(socket_cliente, buffer, BUFFER_SIZE) <= 0) {
            perror("Error al recibir datos del servidor");
            break;
        }
        printf("%s", buffer);  // Muestra el menú recibido

        // Obtiene la selección del usuario
        printf("> ");
        fgets(input, sizeof(input), stdin);
        
        // Envía la selección al servidor
        if (send(socket_cliente, input, strlen(input), 0) <= 0) {
            perror("Error al enviar datos al servidor");
            break;
        }
        if(atoi(input)==4){
            memset(buffer, 0, BUFFER_SIZE);
            if (read(socket_cliente, buffer, BUFFER_SIZE) <= 0) {
                perror("Error al recibir datos del servidor");
                break;
            }
            printf("%s", buffer);  // Muestra el menú recibido
              

         }
        // Si el usuario elige salir (opción 4), termina el bucle
        if (atoi(input) == 5) {
            printf("Saliendo de la tienda...\n");
            break;
        }

        // Recibe y muestra la respuesta del servidor
        memset(buffer, 0, BUFFER_SIZE);
        if (read(socket_cliente, buffer, BUFFER_SIZE) <= 0) {
            perror("Error al recibir respuesta del servidor");
            break;
        }
        printf("%s\n", buffer);  // Muestra la respuesta del servidor
    }

    // Cierre limpio de la conexión
    close(socket_cliente);
    printf("Conexión cerrada. Hasta pronto!\n");
    return 0;
}