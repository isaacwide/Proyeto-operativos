#include <unistd.h>          // Para fork(), close(), read(), write()
#include <netinet/in.h>      // Para struct sockaddr_in
#include <arpa/inet.h>       // Para inet_ntoa()
#include <sys/types.h>       // Para tipos de datos como pid_t
#include <sys/socket.h>      // Para funciones de sockets
#include <stdlib.h>          // Para exit(), atoi()
#include <string.h>          // Para strcpy(), strcat(), memset()
#include <stdio.h>           // Para printf(), perror()

// Estructura que define un producto de la tienda
struct producto {
    char nombre[50];        // Nombre del producto (máx 50 caracteres)
    float precio;           // Precio del producto
};

// Lista de productos disponibles en la tienda
struct producto products[] = {
    {"Zelda BOTW", 1399},   // Producto 1
    {"Sekiro", 799},        // Producto 2
    {"Ghost of tsushima", 999} // Producto 3
};
// Calcula el número de productos en el array
const int num_products = sizeof(products)/sizeof(products[0]);

struct producto carrito[30]={};

int total = 0 ;
float total_pagar=0;
/**
 * Función que envía el menú de productos al cliente
 * @param sock El descriptor de socket del cliente conectado
 */
void enviar_menu(int sock) {
    char menu[1024] = "\n=== MENÚ DE PRODUCTOS ===\n";
    
    // Construye el menú iterando sobre los productos
    for (int i = 0; i < num_products; i++) {
        char item[100];
        // Formatea cada producto con su número, nombre y precio
        snprintf(item, sizeof(item), "%d. %s - $%.2f\n", i+1, products[i].nombre, products[i].precio);
        strcat(menu, item);  // Añade el producto al menú
    }
    // Añade la opción para salir
    strcat(menu,"4.-ver canasta \n");
    strcat(menu, "5. Salir\nSeleccione una opción: ");
    
    // Envía el menú completo al cliente
    send(sock, menu, strlen(menu), 0);
}

/**
 * Función que maneja la interacción con un cliente
 * @param cliente El descriptor de socket del cliente
 * 
 * 
 * 
 */
//funcion para manejar el cliente 
 void mostrar_carrito(int cliente){
    char p[1024] ="tus productos\n";
    float t=0.0;
    if(total==0){
        strcat(p, "El carrito está vacío\n");
    }else{
        for (int i = 0; i <= total; i++) {
            char item[100];
            snprintf(item, sizeof(item), "%d. %s - $%.2f\n", 
                    i+1, carrito[i].nombre, carrito[i].precio);
            strcat(p, item);
            t += carrito[i].precio;
        }
        char total_msg[50];
        snprintf(total_msg, sizeof(total_msg), "\nTotal a pagar: $%.2f\n", t);
        strcat(p, total_msg);

    }
    send(cliente, p, strlen(p), 0);
 }



void manejar_cliente(int cliente) {
    char buffer[1024];  // Buffer para almacenar datos recibidos
    
    while (1) {  // Bucle infinito hasta que el cliente elija salir
        enviar_menu(cliente);  // Envía el menú al cliente

        // Limpia el buffer antes de recibir datos
        memset(buffer, 0, sizeof(buffer));
        
        // Recibe la selección del cliente
        int recibido = recv(cliente, buffer, sizeof(buffer) - 1, 0);
        if (recibido <= 0) break;  // Si hay error o cierre, sale del bucle

        int opcion = atoi(buffer);  // Convierte la entrada a número
        printf("Cliente %d seleccionó la opción: %d\n", cliente, opcion);//Muestra qué opción seleccionó el cliente
        char respuesta[256];       // Buffer para la respuesta al cliente

        // Procesa la opción seleccionada
        if (opcion >= 1 && opcion <= num_products) {

            if (total < 30) {
                //copiamos el nombre del producto
                strcpy(carrito[total].nombre, products[opcion-1].nombre);
                carrito[total].precio = products[opcion-1].precio;
                total++;
                
                snprintf(respuesta, sizeof(respuesta), 
                       "Añadido: %s - $%.2f\n", 
                       products[opcion-1].nombre, 
                       products[opcion-1].precio);
                //Muestra qué producto agregó
                printf("Cliente %d agregó al carrito: %s\n", cliente, products[opcion - 1].nombre);
            } else {
                strcpy(respuesta, "Carrito lleno. No se pueden añadir más productos.\n");
            }


        } else if (opcion == num_products + 1) {  // opcion para ver lo que se ha guardo

                //Muestra que pidió ver la canasta
                printf("Cliente %d solicitó ver la canasta.\n", cliente);
                mostrar_carrito(cliente);
                continue;
            
            // aqui se veran los productos que se han comprado
        } else if (opcion == num_products + 2) {
            
            strcpy(respuesta, "Opción inválida. Intente nuevamente.\n");
        }else if (opcion == 5) {//el cliente se desconectó y lo muestra
            printf("Cliente %d se desconectó.\n", cliente);
            break;
        }

        

        // Envía la respuesta al cliente
        send(cliente, respuesta, strlen(respuesta), 0);
    }

    // Cierra el socket del cliente y termina el proceso hijo
    close(cliente);
    exit(0);
}

/**
 * Función principal del servidor
 */
int main(int argc, char *argv[]) {
    int mi_socket, nuevo;      // Descriptores de socket
    socklen_t tam;            // Tamaño de la estructura de dirección
    struct sockaddr_in mi_estructura;  // Estructura para la dirección del servidor

    // Configuración de la dirección del servidor
    mi_estructura.sin_family = AF_INET;         // Familia IPv4
    mi_estructura.sin_port = htons(8080);       // Puerto 8080
    mi_estructura.sin_addr.s_addr = INADDR_ANY; // Acepta conexiones de cualquier interfaz
    memset(&(mi_estructura.sin_zero), '\0', 8); // Rellena con ceros el resto de la estructura

    // Crea el socket del servidor
    mi_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (mi_socket == -1) {
        perror("Error al crear socket");
        exit(1);
    }

    // Configura el socket para reutilizar la dirección (evita errores al reiniciar)
    int opt = 1;
    if (setsockopt(mi_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt");
        exit(1);
    }

    // Vincula el socket a la dirección y puerto
    if (bind(mi_socket, (struct sockaddr *)&mi_estructura, sizeof(struct sockaddr)) == -1) {
        perror("Error en bind");
        exit(1);
    }

    // Pone el socket en modo escucha, con cola máxima de 5 conexiones
    if (listen(mi_socket, 5) == -1) {
        perror("Error en listen");
        exit(1);
    }

    printf("Servidor esperando conexiones en puerto 8080...\n");

    // Bucle principal del servidor
    while (1) {
        tam = sizeof(struct sockaddr_in);
        
        // Acepta una nueva conexión
        nuevo = accept(mi_socket, (struct sockaddr *)&mi_estructura, &tam);
        if (nuevo == -1) {
            perror("Error en accept");
            continue;  // Continúa esperando conexiones a pesar del error
        }

        // Crea un proceso hijo para manejar al cliente
        pid_t pid = fork();
        if (pid == 0) { // Proceso hijo
            close(mi_socket);  // El hijo no necesita el socket principal
            manejar_cliente(nuevo);  // Maneja la conexión con el cliente
        } else if (pid > 0) { // Proceso padre
            close(nuevo);  // El padre no necesita el socket del cliente
        } else {
            perror("fork");  // Error al crear proceso hijo
            close(nuevo);
        }
    }

    // Cierra el socket principal (aunque en este código nunca se alcanza)
    close(mi_socket);
    return 0;
}
