#include <unistd.h>          
#include <netinet/in.h>      
#include <arpa/inet.h>       
#include <sys/types.h>       
#include <sys/socket.h>      
#include <stdlib.h>          
#include <string.h>          
#include <stdio.h>           
#define MAX_CARRITO 15 
#define MENU_PRINCIPAL 0
#define MENU_CATEGORIAS 1
#define MENU_MARCAS 2
#define MENU_PRODUCTOS 3
#define MENU_CARRITO 4
// Estructura que define un producto de la tienda
struct producto {
    char nombre[50];        // Nombre del producto (máx 50 caracteres)
    float precio;           // Precio del producto
};

//losta de productos que vendemos 
///agregado los productos ////////////////////////////////////celulares
struct producto samsung[] = {
    {"Galaxy S24 Ultra", 27999.00},
    {"Galaxy S24+", 21999.00},
    {"Galaxy S24", 17999.00},
    {"Galaxy S23 FE", 12999.00},
    {"Galaxy Z Fold 5", 39999.00},
    {"Galaxy Z Flip 5", 22999.00},
    {"Galaxy A34 5G", 7499.00},
    {"Galaxy A14 5G", 4999.00},
    {"Galaxy M54 5G", 8499.00},
    {"Galaxy M34 5G", 6499.00},
   
};
struct producto apple[] = {
    {"iPhone 15 Pro Max 256GB", 34999.00},  
    {"iPhone 15 Pro 128GB", 29999.00},
    {"iPhone 15 128GB", 24999.00},
    {"iPhone 14 Pro Max 128GB", 31999.00},
    {"iPhone 14 128GB", 21999.00},
    {"iPhone SE (3ra gen) 64GB", 12999.00}, 
    {"iPhone 13 128GB", 18999.00},            
    {"iPhone 12 64GB (Reacondicionado)", 11999.00},
    {"iPhone 11 64GB (Reacondicionado)", 8999.00},
    {"iPhone 15 Plus 256GB", 28999.00}  
};
struct producto xiaomi[] = {
    {"Redmi Note 13 Pro+ 5G (256GB)", 8999.00},  
    {"Redmi Note 13 Pro 5G (128GB)", 7499.00},
    {"Redmi Note 12 Pro 5G (128GB)", 6499.00},   
    {"Redmi 13C 5G (128GB)", 4999.00},          
    {"Redmi 12 5G (128GB)", 4499.00},
    {"POCO X6 Pro 5G (256GB)", 8499.00},        
    {"POCO M6 Pro 5G (128GB)", 5999.00},
    {"Xiaomi 13T 5G (256GB)", 14999.00},        
    {"Xiaomi 12 Lite 5G (128GB)", 8999.00},
    {"Redmi A3 (64GB)", 3499.00}                
};
struct producto huawei[] = {
    {"Huawei Mate 60 Pro (512GB)", 24999.00},   
    {"Huawei Mate 50 Pro (256GB)", 19999.00},   
    {"Huawei P60 Pro (256GB)", 21999.00},       
    {"Huawei P50 Pro (128GB)", 16999.00},
    {"Huawei Nova 11 (256GB)", 9999.00},        
    {"Huawei Nova Y91 (128GB)", 5999.00},       
    {"Huawei Enjoy 60X (128GB)", 5499.00},      
    {"Huawei Enjoy 50z (64GB)", 4499.00},
    {"Huawei P40 Pro (128GB Reacond.)", 8999.00}, 
    {"Huawei MatePad 11 (128GB+LTE)", 12999.00} 
};
struct producto oppo[] = {
    {"OPPO Find X6 Pro (256GB)", 24999.00},    
    {"OPPO Find N2 Flip (256GB)", 19999.00},   
    {"OPPO Reno 10 Pro+ 5G (256GB)", 14999.00}, 
    {"OPPO Reno 10 5G (128GB)", 10999.00},
    {"OPPO A98 5G (128GB)", 8499.00},          
    {"OPPO A78 5G (128GB)", 6999.00},
    {"OPPO A58 (128GB)", 5499.00},             
    {"OPPO K11x (128GB)", 5999.00},            
    {"OPPO Find X5 Lite (128GB Reacond.)", 7999.00},
    {"OPPO A38 (64GB)", 3999.00}              
};
/////laptops
struct producto lenovo[] = {
    {"ThinkPad X1 Carbon Gen 11 (i7, 16GB, 512GB)", 39999.00},  
    {"Legion Pro 7i (i9, RTX 4090, 32GB)", 69999.00},           
    {"Yoga Slim 7 Pro (Ryzen 7, 16GB, 1TB)", 27999.00},         
    {"ThinkPad T14 Gen 3 (i5, 16GB, 256GB)", 24999.00},         
    {"IdeaPad Gaming 3 (Ryzen 5, RTX 3050)", 18999.00},         
    {"Legion 5 (i7, RTX 4060, 16GB)", 32499.00},                
    {"Yoga 9i (4K, i7, 16GB)", 45999.00},                       
    {"ThinkBook 14 (i5, 8GB, 512GB)", 16499.00},                
    {"IdeaPad 5 (Ryzen 5, 12GB, 512GB)", 15999.00},            
    {"Legion Slim 5 (Ryzen 7, RTX 4070)", 38999.00}             
};
struct producto mac[] = {
    {"MacBook Pro 16\" M3 Max (48GB, 1TB)", 89999.00},          
    {"MacBook Air 15\" M2 (16GB, 512GB)", 34999.00},            
    {"MacBook Pro 14\" M3 Pro (36GB, 512GB)", 59999.00},        
    {"MacBook Air 13\" M1 (8GB, 256GB)", 19999.00},             
    {"MacBook Pro 13\" M2 (24GB, 1TB)", 42999.00},              
    {"MacBook Air M2 (8GB, 256GB)", 22999.00},                 
    {"MacBook Pro 16\" M1 Max (64GB, 8TB)", 120999.00},         
    {"MacBook Air 13\" M2 (16GB, 512GB)", 29999.00},           
    {"MacBook Pro 14\" M2 Pro (32GB, 1TB)", 54999.00},          
    {"MacBook Air 15\" M2 (8GB, 256GB)", 31999.00}              
};
struct producto msi[] = {
    {"Raider GE78 (i9, RTX 4090, 64GB)", 99999.00},             
    {"Stealth 16 Studio (i7, RTX 4070)", 54999.00},           
    {"Katana 15 (i7, RTX 4060, 16GB)", 32999.00},               
    {"Vector GP68 (i9, RTX 4080)", 72499.00},                   
    {"Crosshair 15 (Ryzen 7, RTX 3060)", 28999.00},            
    {"Prestige 16 (i7, RTX 4050)", 41999.00},                  
    {"Sword 15 (i5, RTX 3050)", 23999.00},                      
    {"Titan GT77 (i9, RTX 4090, 128GB)", 150999.00},            
    {"Cyborg 15 (i7, RTX 4050)", 29499.00},                     
    {"Bravo 15 (Ryzen 5, RX 5500M)", 18999.00}                  
};
struct producto acer[] = {
    {"Predator Helios 16 (i9, RTX 4080)", 65999.00},            
    {"Swift X 14 (Ryzen 7, RTX 3050)", 24999.00},               
    {"Nitro 5 (i5, RTX 3050, 144Hz)", 21999.00},                
    {"Aspire 5 (i5, Iris Xe, 16GB)", 15999.00},                 
    {"Spin 5 (i7, 16GB, 2K)", 32999.00},                        
    {"Predator Triton 14 (i7, RTX 4060)", 48999.00},            
    {"Chromebook 314 (Celeron, 4GB)", 6999.00},                 
    {"ConceptD 7 (i7, RTX 3060)", 55999.00},                    
    {"Swift Go 14 (i5, 16GB)", 19999.00},                       
    {"Extensa 15 (i3, 8GB)", 10999.00}                          
};
struct producto hp[] = {
    {"Spectre x360 16 (i7, OLED, 32GB)", 49999.00},            
    {"Omen 17 (i9, RTX 4090)", 79999.00},                       
    {"Envy 16 (i9, RTX 4060)", 54999.00},                       
    {"Victus 16 (Ryzen 7, RTX 3050)", 22999.00},               
    {"Pavilion Plus 14 (i5, 2.8K OLED)", 23999.00},            
    {"ZBook Firefly G10 (i7, RTX A500)", 42999.00},           
    {"ProBook 440 G10 (i5, 16GB)", 20999.00},                 
    {"EliteBook 840 G9 (i7, 32GB)", 38999.00},                 
    {"15-fd0099la (Ryzen 5, 12GB)", 14999.00},                
    {"Chromebook 14a (Pentium, 8GB)", 7499.00}                  
};
////videojuegos 
struct producto xbox[] = {
    {"Starfield ", 1199.00},          
    {"Forza Motorsport ", 1299.00},               
    {"Halo Infinite ", 999.00},                 
    {"Senua's Saga: Hellblade II", 1199.00},           
    {"Microsoft Flight Simulator 2024", 1499.00},       
    {"Gears 5 (Edición Definitiva)", 799.00},           
    {"Hi-Fi RUSH ", 499.00},                   
    {"Sea of Thieves (Edición Deluxe)", 699.00},        
    {"Psychonauts 2 ", 599.00},         
    {"Ori and the Will of the Wisps", 449.00}         
};
struct producto playstation[] = {
    {"Marvel's Spider-Man 2 (Edición Estándar)", 1499.00},  
    {"God of War: Ragnarok", 1299.00},                      
    {"The Last of Us Part I (Remake)", 1199.00},            
    {"Final Fantasy XVI ", 1399.00},    
    {"Horizon Forbidden West", 1099.00},                    
    {"Demon's Souls (Remake)", 1199.00},                  
    {"Gran Turismo 7 ", 1199.00},       
    {"Ratchet & Clank: Rift Apart", 999.00},        
    {"Returnal (Edición Digital)", 899.00},                 
    {"Ghost of Tsushima: Director's Cut", 1099.00}         
};
struct producto nintendo[] = {
    {"The Legend of Zelda: Tears of the Kingdom", 1499.00}, 
    {"Super Mario Bros. Wonder", 1199.00},                  
    {"Pokémon Escarlata/Violeta (DLC incluido)", 1399.00}, 
    {"Animal Crossing: New Horizons", 1099.00},            
    {"Metroid Prime 4 ", 1499.00},           
    {"Splatoon 3 (Edición Definitiva)", 1099.00},          
    {"Super Smash Bros. Ultimate", 1199.00},             
    {"Mario Kart 8 Deluxe ", 1099.00},                
    {"Xenoblade Chronicles 3", 1199.00},                  
    {"Luigi's Mansion 3", 999.00}                           
};

const int producs=10;

// el maximo del carrito sera de 15 productos 
struct producto carrito[MAX_CARRITO]={};

int total = 0 ;
float total_pagar=0;
/**
 * Función que envía el menú de productos al cliente
 * @param sock El descriptor de socket del cliente conectado
 */
void enviar_menu(int sock, int estado, int categoria_actual, int marca_actual) {
    char menu[2048] = ""; // Buffer más grande para los menús
    
    switch(estado) {
        case MENU_PRINCIPAL:
            strcat(menu, "\n=== MENÚ PRINCIPAL ===\n");
            strcat(menu, "1. Celulares\n");
            strcat(menu, "2. Laptops\n");
            strcat(menu, "3. Videojuegos\n");
            strcat(menu, "4. Ver carrito\n");
            strcat(menu, "5. Salir\n");
            strcat(menu, "Seleccione una opción: ");
            break;
            
        case MENU_CATEGORIAS:
            if(categoria_actual == 0) { // Celulares
                strcat(menu, "\n=== MARCAS DE CELULARES ===\n");
                strcat(menu, "1. Samsung\n");
                strcat(menu, "2. Apple\n");
                strcat(menu, "3. Xiaomi\n");
                strcat(menu, "4. Huawei\n");
                strcat(menu, "5. Oppo\n");
                strcat(menu, "0. Volver\n");
                strcat(menu, "Seleccione una marca: ");
            }
            else if(categoria_actual == 1) { // Laptops
                strcat(menu, "\n=== MARCAS DE LAPTOPS ===\n");
                strcat(menu, "1. Lenovo\n");
                strcat(menu, "2. Mac\n");
                strcat(menu, "3. MSI\n");
                strcat(menu, "4. Acer\n");
                strcat(menu, "5. HP\n");
                strcat(menu, "0. Volver\n");
                strcat(menu, "Seleccione una marca: ");
            }
            else if(categoria_actual == 2) { // Videojuegos
                strcat(menu, "\n=== PLATAFORMAS DE VIDEOJUEGOS ===\n");
                strcat(menu, "1. Xbox\n");
                strcat(menu, "2. PlayStation\n");
                strcat(menu, "3. Nintendo\n");
                strcat(menu, "0. Volver\n");
                strcat(menu, "Seleccione una plataforma: ");
            }
            break;
            
        case MENU_PRODUCTOS:
            strcat(menu, "\n=== PRODUCTOS ===\n");
            if(categoria_actual == 0) { // Celulares
                switch(marca_actual) {
                    case 0: // Samsung
                        for(int i = 0; i < 10; i++) {
                            char item[100];
                            snprintf(item, sizeof(item), "%d. %s - $%.2f\n", 
                                    i+1, samsung[i].nombre, samsung[i].precio);
                            strcat(menu, item);
                        }
                        break;
                    case 1: // Apple
                        for(int i = 0; i < 10; i++) {
                            char item[100];
                            snprintf(item, sizeof(item), "%d. %s - $%.2f\n", 
                                    i+1, apple[i].nombre, apple[i].precio);
                            strcat(menu, item);
                        }
                        break;
                    // Añadir casos para otras marcas de celulares
                }
            }
            else if(categoria_actual == 1) { // Laptops
                switch(marca_actual) {
                    case 0: // Lenovo
                        for(int i = 0; i < 10; i++) {
                            char item[100];
                            snprintf(item, sizeof(item), "%d. %s - $%.2f\n", 
                                    i+1, lenovo[i].nombre, lenovo[i].precio);
                            strcat(menu, item);
                        }
                        break;
                    // Añadir casos para otras marcas de laptops
                }
            }
            else if(categoria_actual == 2) { // Videojuegos
                switch(marca_actual) {
                    case 0: // Xbox
                        for(int i = 0; i < 10; i++) {
                            char item[100];
                            snprintf(item, sizeof(item), "%d. %s - $%.2f\n", 
                                    i+1, xbox[i].nombre, xbox[i].precio);
                            strcat(menu, item);
                        }
                        break;
                    // Añadir casos para otras plataformas de videojuegos
                }
            }
            strcat(menu, "0. Volver\n");
            strcat(menu, "Seleccione un producto: ");
            break;
            
        case MENU_CARRITO:
            mostrar_carrito(sock);  // Cambiado de cliente a sock
            return 0 ;  // Salir para evitar el send() después del switch
    }
    
    send(sock, menu, strlen(menu), 0);
}
//manejar carrito 
void mostrar_carrito(int cliente) {
    char buffer[2048] = "\n=== TU CARRITO ===\n";
    float total_pagar = 0.0;
    
    if(total == 0) {
        strcat(buffer, "El carrito está vacío\n");
    } else {
        for(int i = 0; i < total; i++) {
            char item[120];
            snprintf(item, sizeof(item), "%d. %s - $%.2f\n", i+1, carrito[i].nombre, carrito[i].precio);
            strcat(buffer, item);
            total_pagar += carrito[i].precio;
        }
        
        char total_msg[100];
        snprintf(total_msg, sizeof(total_msg), "\nTotal: $%.2f\n", total_pagar);
        strcat(buffer, total_msg);
        
        strcat(buffer, "1. Finalizar compra\n");
        strcat(buffer, "2. Seguir comprando\n");
        strcat(buffer, "Seleccione una opción: ");
    }
    
    send(cliente, buffer, strlen(buffer), 0);
}
void manejar_cliente(int cliente) {
    char buffer[1024];
    int estado = MENU_PRINCIPAL;
    int categoria_actual = -1;
    int marca_actual = -1;
    
    while(1) {
        enviar_menu(cliente, estado, categoria_actual, marca_actual);
        
        memset(buffer, 0, sizeof(buffer));
        int recibido = recv(cliente, buffer, sizeof(buffer) - 1, 0);
        if(recibido <= 0) break;
        
        int opcion = atoi(buffer);
        
        switch(estado) {
            case MENU_PRINCIPAL:
                if(opcion >= 1 && opcion <= 3) {
                    categoria_actual = opcion - 1;
                    estado = MENU_CATEGORIAS;
                } 
                else if(opcion == 4) {
                    estado = MENU_CARRITO;
                } 
                else if(opcion == 5) {
                    close(cliente);
                    exit(0);
                }
                break;
                
            case MENU_CATEGORIAS:
                if(opcion == 0) {
                    estado = MENU_PRINCIPAL;
                } 
                else {
                    // Validar según categoría
                    if(categoria_actual == 0 && opcion >= 1 && opcion <= 5) { // Celulares
                        marca_actual = opcion - 1;
                        estado = MENU_PRODUCTOS;
                    }
                    else if(categoria_actual == 1 && opcion >= 1 && opcion <= 5) { // Laptops
                        marca_actual = opcion - 1;
                        estado = MENU_PRODUCTOS;
                    }
                    else if(categoria_actual == 2 && opcion >= 1 && opcion <= 3) { // Videojuegos
                        marca_actual = opcion - 1;
                        estado = MENU_PRODUCTOS;
                    }
                }
                break;
                
            case MENU_PRODUCTOS:
                if(opcion == 0) {
                    estado = MENU_CATEGORIAS;
                } 
                else if(opcion >= 1 && opcion <= 10) {
                    if(total < MAX_CARRITO) {
                        if(categoria_actual == 0) { // Celulares
                            switch(marca_actual) {
                                case 0: // Samsung
                                    strcpy(carrito[total].nombre, samsung[opcion-1].nombre);
                                    carrito[total].precio = samsung[opcion-1].precio;
                                    break;
                                case 1: // Apple
                                    strcpy(carrito[total].nombre, apple[opcion-1].nombre);
                                    carrito[total].precio = apple[opcion-1].precio;
                                    break;
                                // Añadir casos para otras marcas
                            }
                            total++;
                            char msg[100];
                            snprintf(msg, sizeof(msg), "Producto añadido: %s\n", carrito[total-1].nombre);
                            send(cliente, msg, strlen(msg), 0);
                        }
                        else if(categoria_actual == 1) { // Laptops
                            switch(marca_actual) {
                                case 0: // Lenovo
                                    strcpy(carrito[total].nombre, lenovo[opcion-1].nombre);
                                    carrito[total].precio = lenovo[opcion-1].precio;
                                    break;
                                // Añadir casos para otras marcas de laptops
                            }
                            total++;
                            char msg[100];
                            snprintf(msg, sizeof(msg), "Producto añadido: %s\n", carrito[total-1].nombre);
                            send(cliente, msg, strlen(msg), 0);
                        }
                        // Similar para videojuegos
                    } 
                    else {
                        send(cliente, "Carrito lleno!\n", 14, 0);
                    }
                }
                break;
                
            case MENU_CARRITO:
                if(opcion == 1) {
                    // Finalizar compra
                    char msg[100];
                    snprintf(msg, sizeof(msg), "Gracias por su compra. Total: $%.2f\n", total_pagar);
                    send(cliente, msg, strlen(msg), 0);
                    total = 0; // Vaciar carrito
                    total_pagar = 0;
                }
                estado = MENU_PRINCIPAL;
                break;
        }
    }
    
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