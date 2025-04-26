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


enum Categoria{
    TELEFONOS,
    LAPTOPS,
    VIDEOJUEGOS
};



//Estructura para agrupar productos por categoria
struct CategoriaProductos{
    char nombre[50];
    struct producto *productos;
    int num_productos;
};


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
    {"Galaxy M34 5G", 6499.00}
   
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


struct CategoriaProductos categorias[]={
    {"Telefonos", NULL, 0},
    {"Laptops", NULL, 0},
    {"Videojuegos", NULL, 0}
}

/*
// Lista de productos disponibles en la tienda
struct producto products[] = {
    {"Zelda BOTW", 1399},   // Producto 1
    {"Sekiro", 799},        // Producto 2
    {"Ghost of tsushima", 999} // Producto 3
};
// Calcula el número de productos en el array
const int num_products = sizeof(products)/sizeof(products[0]);
*/
struct producto carrito[30]={};

int total = 0 ;
float total_pagar=0;
/**
 * Función que envía el menú de productos al cliente
 * @param sock El descriptor de socket del cliente conectado
 */
void enviar_menu(int sock) {
    char menu[2048] = "\n=== MENÚ DE PRODUCTOS ===\n";
    strcat(menu, "Categorias disponibles: \n");
    // Construye el menú iterando sobre los productos
    for (int i = 0; i < 3; i++) {
        char item[100];
        // Formatea cada producto con su número, nombre y precio
        snprintf(item, sizeof(item), "%d. %s\n", i+1, categorias[i].nombre);
        strcat(menu, item);  // Añade el producto al menú
    }
    // Añade la opción para salir
    strcat(menu,"4.-ver canasta \n");
    strcat(menu, "5. Salir\nSeleccione una categoria: ");
    
    // Envía el menú completo al cliente
    send(sock, menu, strlen(menu), 0);
}

void enviar_productos(int sock, int categoria){
    char menu[2048];
    snprintf(menu, sizeof (menu), "\n=== %s ===\n", categorias[categoria].nombre);
    //Se hace la lista de productos de la categoria que se seleccionó
    for (int i = 0; i < categorias[categoria].num_productos; i++){
        char item[100];
        snprintf(item, sizeof(item), "%d. %s - $%.2f\n", i+1, categorias[categoria].productos[i].nombre, 
                categorias[categoria].productos[i].precio);
        strcat(menu, item);  
    }
    strcat(menu, "0. Volver al menu principal\n");
    strcat(menu, "Seleccione un producto: ");
    send (sock, menu, strlen(menu), 0);
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
    char p[2048] ="tus productos\n";
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
        char total_msg[100];
        snprintf(total_msg, sizeof(total_msg), "\nTotal a pagar: $%.2f\n", t);
        strcat(p, total_msg);

    }
    send(cliente, p, strlen(p), 0);
 }



/*void manejar_cliente(int cliente) {
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
} */

void manejar_cliente(int cliente) {
    char buffer[1024];
    int categoria_seleccionada = -1; // -1 = en menú principal

    while (1) {
        if (categoria_seleccionada == -1) {
            enviar_menu(cliente);
        } else {
            enviar_productos(cliente, categoria_seleccionada);
        }

        memset(buffer, 0, sizeof(buffer));
        if (recv(cliente, buffer, sizeof(buffer), 0) <= 0) break;

        int opcion = atoi(buffer);

        if (categoria_seleccionada == -1) {
            // Menú principal
            if (opcion >= 1 && opcion <= 3) {
                categoria_seleccionada = opcion - 1; // Entrar a categoría
            } else if (opcion == 4) {
                mostrar_carrito(cliente);
            } else if (opcion == 5) {
                break; // Salir
            }
        } else {
            // Dentro de una categoría
            if (opcion >= 1 && opcion <= categorias[categoria_seleccionada].num_productos) {
                // Añadir al carrito
                if (total < 30) {
                    strcpy(carrito[total].nombre, categorias[categoria_seleccionada].productos[opcion-1].nombre);
                    carrito[total].precio = categorias[categoria_seleccionada].productos[opcion-1].precio;
                    total++;
                    
                    char respuesta[100];
                    snprintf(respuesta, sizeof(respuesta), "Añadido: %s\n", carrito[total-1].nombre);
                    send(cliente, respuesta, strlen(respuesta), 0);
                } else {
                    send(cliente, "Carrito lleno.\n", 15, 0);
                }
            } else if (opcion == 0) {
                categoria_seleccionada = -1; // Volver al menú principal
            }
        }
    }
    close(cliente);
}

/**
 * Función principal del servidor
 */
int main(int argc, char *argv[]) {
  // Configuracion de las categorias
  
  //Telefonos
    categorias[TELEFONOS].productos = samsung;
   categorias[TELEFONOS].num_productos = sizeof(samsung) / sizeof(samsung[0]);

   categorias[TELEFONOS].productos = apple;
   categorias[TELEFONOS].num_productos = sizeof(apple) / sizeof(apple[0]);

   categorias[TELEFONOS].productos = xiaomi
   categorias[TELEFONOS].num_productos = sizeof(xiaomi) / sizeof(xiaomi[0]);

   categorias[TELEFONOS].productos = huawei;
   categorias[TELEFONOS].num_productos = sizeof(huawei) / sizeof(huawei[0]);

   categorias[TELEFONOS].productos = oppo;
   categorias[TELEFONOS].num_productos = sizeof(oppo) / sizeof(oppo[0]);

   //Laptops
   categorias[LAPTOPS].productos = lenovo;
   categorias[LAPTOPS].num_productos = sizeof(lenovo) / sizeof(lenovo[0]);

   categorias[LAPTOPS].productos = mac
   categorias[LAPTOPS].num_productos = sizeof(mac) / sizeof(mac[0]);

   categorias[LAPTOPS].productos = msi;
   categorias[LAPTOPS].num_productos = sizeof(msi) / sizeof(msi[0]);

   categorias[LAPTOPS].productos = acer;
   categorias[LAPTOPS].num_productos = sizeof(acer) / sizeof(acer[0]);

   categorias[LAPTOPS].productos = hp;
   categorias[LAPTOPS].num_productos = sizeof(hp) / sizeof(hp[0]);

   //Videojuegos
   categorias[VIDEOJUEGOS].productos = xbox;
   categorias[VIDEOJUEGOS].num_productos = sizeof(xbox) / sizeof(xbox[0]);

   categorias[VIDEOJUEGOS].productos = playstation
   categorias[VIDEOJUEGOS].num_productos = sizeof(playstation) / sizeof(playstation[0]);

   categorias[VIDEOJUEGOS].productos = nintendo;
   categorias[VIDEOJUEGOS].num_productos = sizeof(nintendo) / sizeof(nintendo[0]);




   
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
