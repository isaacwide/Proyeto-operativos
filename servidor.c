#include <unistd.h>          // Para fork(), close(), read(), write()
#include <netinet/in.h>      // Para struct sockaddr_in
#include <arpa/inet.h>       // Para inet_ntoa()
#include <sys/types.h>       // Para tipos de datos como pid_t
#include <sys/socket.h>      // Para funciones de sockets
#include <stdlib.h>          // Para exit(), atoi()
#include <string.h>          // Para strcpy(), strcat(), memset()
#include <stdio.h>           // Para printf(), perror()
#include <time.h> 
#include <ctype.h>


// Estructura que define un producto de la tienda
struct producto {
    char nombre[50];        // Nombre del producto (máx 50 caracteres)
    float precio;           // Precio del producto
};

//esto es para que aparezcan los articulos por marca
struct Subcategoria{
    char nombre[50];
    struct producto *productos;
    int num_productos;
};

struct Categoria{
    char nombre[50];
    struct Subcategoria *subcategorias;
    int num_subcategorias;
};

//Organizamos los productos por marcas para la parte de "Subcategoria"

struct producto samsung_telefonos[] = {
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
struct producto apple_telefonos[] = {
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
struct producto xiaomi_telefonos[] = {
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
struct producto huawei_telefonos[] = {
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
struct producto oppo_telefonos[] = {
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
struct producto lenovo_laptops[] = {
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
struct producto mac_laptops[] = {
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
struct producto msi_laptops[] = {
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
struct producto acer_laptops[] = {
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
struct producto hp_laptops[] = {
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
struct producto xbox_juegos[] = {
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
struct producto playstation_juegos[] = {
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
struct producto nintendo_juegos[] = {
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

// Aqui declaramos las configuracion de categorias y subcategorias 
struct Subcategoria subcategorias_telefonos[]={
    {"Samsung", samsung_telefonos, sizeof(samsung_telefonos)/sizeof(samsung_telefonos[0])},
    {"Apple", apple_telefonos, sizeof(apple_telefonos)/sizeof(apple_telefonos[0])},
    {"Xiaomi", xiaomi_telefonos, sizeof(xiaomi_telefonos)/sizeof(xiaomi_telefonos[0])},
    {"Huawei", huawei_telefonos, sizeof(huawei_telefonos)/sizeof(huawei_telefonos[0])},
    {"Oppo", oppo_telefonos, sizeof(oppo_telefonos)/sizeof(oppo_telefonos[0])}
};

struct Subcategoria subcategorias_laptops[]={
    {"Lenovo", lenovo_laptops, sizeof(lenovo_laptops)/sizeof(lenovo_laptops[0])},
    {"Mac", mac_laptops, sizeof(mac_laptops)/sizeof(mac_laptops[0])},
    {"MSI", msi_laptops, sizeof(msi_laptops)/sizeof(msi_laptops[0])},
    {"Acer", acer_laptops, sizeof(acer_laptops)/sizeof(acer_laptops[0])},
    {"HP", hp_laptops, sizeof(hp_laptops)/sizeof(hp_laptops[0])}
};

struct Subcategoria subcategorias_videojuegos[]={
    {"Xbox", xbox_juegos, sizeof(xbox_juegos)/sizeof(xbox_juegos[0])},
    {"PlayStation", playstation_juegos, sizeof(playstation_juegos)/sizeof(playstation_juegos[0])},
    {"Nintendo", nintendo_juegos, sizeof(nintendo_juegos)/sizeof(nintendo_juegos[0])}
};

struct Categoria categorias[]={
    {"Teléfonos", subcategorias_telefonos, sizeof(subcategorias_telefonos)/sizeof(subcategorias_telefonos[0])},
    {"Laptops", subcategorias_laptops, sizeof(subcategorias_laptops)/sizeof(subcategorias_laptops[0])},
    {"VideoJuegos", subcategorias_videojuegos, sizeof(subcategorias_videojuegos)/sizeof(subcategorias_videojuegos[0])}
};


struct producto carrito[30];
int total_carrito = 0;


float total_pagar=0;

/**
 * Función mejorada para enviar mensajes completos
 * Garantiza que todo el mensaje sea enviado, incluso si es largo
 */
int enviar_mensaje(int sock, const char *mensaje) {
    int total = 0;
    int longitud = strlen(mensaje);
    int restante = longitud;
    int n;
    
    // Enviar el mensaje en bloques si es necesario
    while(total < longitud) {
        n = send(sock, mensaje + total, restante, 0);
        
        if (n == -1) {
            break;
        }
        
        total += n;
        restante -= n;
    }
    
    return (n == -1 ? -1 : total);
}

/**
 * Función que envía el menú de productos al cliente
 * @param sock El descriptor de socket del cliente conectado
 */
void enviar_menu(int sock) {
    char menu[2048] = "\n\n=== MENÚ DE PRODUCTOS ===\n\n";
    
    // Construye el menú iterando sobre los productos
    for (int i = 0; i < sizeof(categorias)/sizeof(categorias[0]); i++) {
        char item[100];
        // Formatea cada producto con su número, nombre y precio
        snprintf(item, sizeof(item), "%d. %s\n", i+1, categorias[i].nombre);
        strcat(menu, item);  // Añade el producto al menú
    }
    
    // Añade la opción para salir
    strcat(menu, "4. Ver canasta\n");
    strcat(menu, "5. Salir\n\n");
    strcat(menu, "Seleccione una categoria (1-5): ");
    
    // Envía el menú completo al cliente usando la nueva función
    enviar_mensaje(sock, menu);
}

void enviar_subcategorias(int sock, int categoria_id) {
    char menu[2048];
    snprintf(menu, sizeof(menu), "=== %s ===\nMarcas Disponibles:\n", categorias[categoria_id].nombre);
    
    // Mostrar marcas numeradas desde 1
    for (int i = 0; i < categorias[categoria_id].num_subcategorias; i++) {
        char item[100];
        snprintf(item, sizeof(item), "%d. %s\n", i + 1, categorias[categoria_id].subcategorias[i].nombre);
        strcat(menu, item);
    }
    
    // Opción para volver siempre como 0
    strcat(menu, "\n0. Volver al menú principal\n\n");
    strcat(menu, "Seleccione una marca (0-");
    char rango[10];
    snprintf(rango, sizeof(rango), "%d", categorias[categoria_id].num_subcategorias);
    strcat(menu, rango);
    strcat(menu, "): ");
    
    enviar_mensaje(sock, menu);
}

void enviar_productos(int sock, int categoria_id, int subcategoria_id){
    char menu[2048];
    struct Subcategoria sub = categorias[categoria_id].subcategorias[subcategoria_id];
    snprintf(menu, sizeof(menu), "===%s-%s===\n", categorias[categoria_id].nombre, sub.nombre);
    for (int i = 0; i < sub.num_productos; i++){
        char item[100];
        snprintf(item, sizeof(item), "%d.%s-$%.2f\n", i+1, sub.productos[i].nombre, sub.productos[i].precio);
        strcat(menu, item);
    }
    strcat(menu, "\n0. Volver\n\n");
    strcat(menu, "Seleccione un producto (0-");
    char rango[10];
    snprintf(rango, sizeof(rango), "%d", sub.num_productos);
    strcat(menu, rango);
    strcat(menu, "): ");
    
    enviar_mensaje(sock, menu);
}

//funcion para manejar el cliente 
void mostrar_carrito(int sock) {
    char buffer[4096] = "=== TU CARRITO ===\n";
    float total = 0.0;
    
    if(total_carrito == 0) {
        strcat(buffer, "🛒 El carrito está vacío\n");
    } else {
        for(int i = 0; i < total_carrito; i++) {
            char item[100];
            snprintf(item, sizeof(item), "%2d. %-40s $%9.2f\n", 
                    i+1, carrito[i].nombre, carrito[i].precio);
            strcat(buffer, item);
            total += carrito[i].precio;
        }
        
        char total_msg[100];
        snprintf(total_msg, sizeof(total_msg), 
                "\n📌 Total productos: %d\n💵 Total a pagar: $%.2f\n", 
                total_carrito, total);
        strcat(buffer, total_msg);
    }
    
    strcat(buffer, "\n0. Volver\n1. Pagar\n2. Vaciar carrito\n\n");
    strcat(buffer, "Seleccione una opción (0-2): ");
    
    enviar_mensaje(sock, buffer);
}

void mostrar_metodos_pago(int sock) {
    char menu[512] = "\n=== MÉTODO DE PAGO ===\n";
    strcat(menu, "1. Tarjeta de crédito\n");
    strcat(menu, "2. Efectivo\n");
    strcat(menu, "3. Pago mensual\n");
    strcat(menu, "Seleccione una opción: ");
    
    enviar_mensaje(sock, menu);
}
void procesar_pago(int sock) {
    if(total_carrito == 0) {
        send(sock, "❌ No hay productos en el carrito para pagar\n", 45, 0);
        return;
    }
    // Calcular total
    total_pagar = 0.0;
    for(int i = 0; i < total_carrito; i++) {
        total_pagar += carrito[i].precio;
    }
    mostrar_metodos_pago(sock); 
}


void credito(int sock){
    char mensaje[2048] = "\n=== SECCIÓN DE PAGO CON TARJETA DE CRÉDITO ===\n";
    char buffer[100];
    char tarjeta[20] = {0};
    char fecha[6] = {0};
    char cvv[4] = {0};
    
    // Solicitar número de tarjeta
    strcat(mensaje, "Por favor ingrese los siguientes datos:\n");
    strcat(mensaje, "Número de tarjeta (XXXX-XXXX-XXXX-XXXX): ");
    send(sock, mensaje, strlen(mensaje), 0);
    
    // Recibir número de tarjeta
    memset(buffer, 0, sizeof(buffer));
    recv(sock, buffer, sizeof(buffer), 0);
    if(strchr(buffer, '\n')) *strchr(buffer, '\n') = '\0';
    strncpy(tarjeta, buffer, 19);
    
    // Solicitar fecha de expiración
    memset(mensaje, 0, sizeof(mensaje));
    strcpy(mensaje, "Fecha de expiración (MM/AA): ");
    send(sock, mensaje, strlen(mensaje), 0);
    
    // Recibir fecha
    memset(buffer, 0, sizeof(buffer));
    recv(sock, buffer, sizeof(buffer), 0);
    if(strchr(buffer, '\n')) *strchr(buffer, '\n') = '\0';
    strncpy(fecha, buffer, 5);
    
    // Solicitar CVV
    memset(mensaje, 0, sizeof(mensaje));
    strcpy(mensaje, "Código de seguridad (XXX): ");
    send(sock, mensaje, strlen(mensaje), 0);
    
    // Recibir CVV
    memset(buffer, 0, sizeof(buffer));
    recv(sock, buffer, sizeof(buffer), 0);
    if(strchr(buffer, '\n')) *strchr(buffer, '\n') = '\0';
    strncpy(cvv, buffer, 3);
    
    // Procesar el pago (simulación)
    memset(mensaje, 0, sizeof(mensaje));
    strcpy(mensaje, "\nProcesando pago con los siguientes datos:\n");
    char info[200];
    snprintf(info, sizeof(info), "Tarjeta: %s\nExpiración: %s\nCVV: %s\n\n", 
             tarjeta, fecha, cvv);
    strcat(mensaje, info);
    strcat(mensaje, "Procesando transacción...\n");
    send(sock, mensaje, strlen(mensaje), 0);
    
    // Simular procesamiento
    sleep(2);
    
    // Generar ticket
    memset(mensaje, 0, sizeof(mensaje));
    strcpy(mensaje, "✅ Pago aprobado!\n\n=== TICKET DE COMPRA ===\n");
    for (int i = 0; i < total_carrito; i++) {
        char item[100];
        snprintf(item, sizeof(item), "%-40s $%9.2f\n", carrito[i].nombre, carrito[i].precio);
        strcat(mensaje, item);
    }
    
    char total_msg[100];
    snprintf(total_msg, sizeof(total_msg), "\nTOTAL: $%.2f\n\n¡Gracias por su compra!\n", total_pagar);
    strcat(mensaje, total_msg);
    strcat(mensaje, "\nPresione ENTER para continuar...");
    
    send(sock, mensaje, strlen(mensaje), 0);
    
    // Vaciar carrito
    total_carrito = 0;
    
    // Esperar ENTER para continuar
    memset(buffer, 0, sizeof(buffer));
    recv(sock, buffer, sizeof(buffer), 0);

}

void efectivo(int sock){
    
    srand(time(NULL));

    char ticket[2048] = "=== TICKET DE COMPRA ===\n";
    for(int i = 0; i < total_carrito; i++) {
        char item[100];
        snprintf(item, sizeof(item), "%-40s $%9.2f\n", carrito[i].nombre, carrito[i].precio);
        strcat(ticket, item);
    }

    char total_msg[100];
    snprintf(total_msg, sizeof(total_msg),"\nTOTAL: $%.2f\n\n¡Gracias por su compra!\n", total_pagar);
    strcat(ticket, total_msg);

    int clave= 100000 + rand() % 900000;;
    char mesage[200];

    snprintf(mesage,sizeof(mesage),"\nEscane este codigo\n[%d]\n En la tienda de su preferencia\n",clave);
    strcat(ticket,mesage);

    send(sock, ticket, strlen(ticket), 0);

    total_carrito=0;

    char buffer[10];
    recv(sock, buffer, sizeof(buffer), 0);

}
void pagomes(int sock){

    char mensaje[2048] = "\n=== PAGO MENSUAL ===\n";
    char buffer[100];
    int meses = 12; // Valor predeterminado
    
    // Solicitar cantidad de meses
    strcat(mensaje, "¿En cuántos meses desea dividir su pago? (3, 6, 9 o 12): ");
    send(sock, mensaje, strlen(mensaje), 0);
    
    // Recibir cantidad de meses
    memset(buffer, 0, sizeof(buffer));
    recv(sock, buffer, sizeof(buffer), 0);
    if(strchr(buffer, '\n')) *strchr(buffer, '\n') = '\0';
    
    int opcion_meses = atoi(buffer);
    
    // Validar y asignar meses
    switch(opcion_meses) {
        case 3:
        case 6:
        case 9:
        case 12:
            meses = opcion_meses;
            break;
        default:
            meses = 12; // Valor predeterminado
            break;
    }
    
    // Calcular pago mensual
    float pago_mensual = total_pagar / (float)meses;
    
    // Solicitar datos de tarjeta para el plan
    memset(mensaje, 0, sizeof(mensaje));
    sprintf(mensaje, "Su compra será dividida en %d pagos mensuales sin intereses.\n\n", meses);
    char detalle[200];
    snprintf(detalle, sizeof(detalle), "Total de la compra: $%.2f\n", total_pagar);
    strcat(mensaje, detalle);
    snprintf(detalle, sizeof(detalle), "Pago mensual: $%.2f x %d meses\n\n", pago_mensual, meses);
    strcat(mensaje, detalle);
    
    strcat(mensaje, "Por favor ingrese los datos de su tarjeta para configurar el plan.\n");
    strcat(mensaje, "Número de tarjeta (XXXX-XXXX-XXXX-XXXX): ");
    send(sock, mensaje, strlen(mensaje), 0);
    
    // Recibir datos de tarjeta (similar a la función credito)
    char tarjeta[20] = {0};
    memset(buffer, 0, sizeof(buffer));
    recv(sock, buffer, sizeof(buffer), 0);
    if(strchr(buffer, '\n')) *strchr(buffer, '\n') = '\0';
    strncpy(tarjeta, buffer, 19);
    
    // Solicitar correo electrónico para notificaciones
    memset(mensaje, 0, sizeof(mensaje));
    strcpy(mensaje, "Correo electrónico para recibir notificaciones de pago: ");
    send(sock, mensaje, strlen(mensaje), 0);
    
    char correo[50] = {0};
    memset(buffer, 0, sizeof(buffer));
    recv(sock, buffer, sizeof(buffer), 0);
    if(strchr(buffer, '\n')) *strchr(buffer, '\n') = '\0';
    strncpy(correo, buffer, 49);
    
    // Procesar el plan de pagos (simulación)
    memset(mensaje, 0, sizeof(mensaje));
    strcpy(mensaje, "\nProcesando plan de pagos con los siguientes datos:\n");
    snprintf(detalle, sizeof(detalle), "Tarjeta: %s\nCorreo: %s\nPlan: %d meses\n\n", 
             tarjeta, correo, meses);
    strcat(mensaje, detalle);
    strcat(mensaje, "Configurando plan de pagos...\n");
    send(sock, mensaje, strlen(mensaje), 0);
    
    // Simular procesamiento
    sleep(2);
    
    // Generar ticket
    memset(mensaje, 0, sizeof(mensaje));
    strcpy(mensaje, "✅ Plan de pagos aprobado!\n\n=== RESUMEN DE COMPRA ===\n");
    for(int i = 0; i < total_carrito; i++) {
        char item[100];
        snprintf(item, sizeof(item), "%-40s $%9.2f\n", carrito[i].nombre, carrito[i].precio);
        strcat(mensaje, item);
    }
    
    char total_msg[200];
    snprintf(total_msg, sizeof(total_msg), 
             "\nTOTAL: $%.2f\nPAGO MENSUAL: $%.2f x %d meses\n\n¡Gracias por su compra!\n", 
             total_pagar, pago_mensual, meses);
    strcat(mensaje, total_msg);
    
    // Calendario de pagos
    strcat(mensaje, "\n=== CALENDARIO DE PAGOS ===\n");
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    
    for(int i = 1; i <= meses; i++) {
        // Avanzar un mes
        tm_info->tm_mon += 1;
        if(tm_info->tm_mon > 11) {
            tm_info->tm_mon = 0;
            tm_info->tm_year += 1;
        }
        
        char fecha[50];
        snprintf(fecha, sizeof(fecha), "Pago %d: %.2d/%.2d/%d - $%.2f\n", 
                 i, tm_info->tm_mon + 1, tm_info->tm_mday, 1900 + tm_info->tm_year, pago_mensual);
        strcat(mensaje, fecha);
    }
    
    strcat(mensaje, "\nPresione ENTER para continuar...");
    send(sock, mensaje, strlen(mensaje), 0);
    
    // Vaciar carrito
    total_carrito = 0;
    
    // Esperar ENTER para continuar
    memset(buffer, 0, sizeof(buffer));
    recv(sock, buffer, sizeof(buffer), 0);

}
/**
 * Función que maneja la interacción con un cliente
 * @param cliente El descriptor de socket del cliente
 * 
 */

void manejar_cliente(int cliente) {
    int estado = 0; // 0=menu principal, 1=marcas, 2=productos 3=estado del carrito, 4=método de pago 
    int categoria_id = -1;
    int subcategoria_id = -1;
    char buffer[1024];
    char respuesta[2048];

    // Enviar menú principal inicialmente
    enviar_menu(cliente);

    while(1) {
        // Limpieza exhaustiva de buffers
        memset(buffer, 0, sizeof(buffer));
        memset(respuesta, 0, sizeof(respuesta));

        // Recepción de datos con manejo mejorado de errores
        int bytes_recibidos = recv(cliente, buffer, sizeof(buffer) - 1, 0);
        if(bytes_recibidos <= 0) {
            printf("Cliente %d desconectado o error de recepción\n", cliente);
            break;
        }

        // Procesamiento de entrada con mejor validación
        buffer[bytes_recibidos] = '\0';
        if(strchr(buffer, '\n')) *strchr(buffer, '\n') = '\0';
        
        // Depuración para ver exactamente qué se recibió
        printf("Recibido del cliente: '%s'\n", buffer);
        
        // Tratamiento especial para mensajes de sincronización
        if(strcmp(buffer, "<SYNC_ACK>") == 0) {
            continue; // Ignorar mensajes de sincronización
        }
        
        // Validar que la entrada sea un número antes de convertirla
        int opcion = 0;
        if(strlen(buffer) > 0) {
            // Verificar que la entrada sea un número
            int es_numero = 1;
            for(int i = 0; i < strlen(buffer); i++) {
                if(!isdigit(buffer[i])) {
                    es_numero = 0;
                    break;
                }
            }
            
            if(es_numero) {
                opcion = atoi(buffer);
            } else {
                // Si no es un número, enviar mensaje de error
                enviar_mensaje(cliente, "⚠ Por favor ingrese un número válido.\n");
                
                // Reenviar el menú apropiado según el estado actual
                switch(estado) {
                    case 0: enviar_menu(cliente); break;
                    case 1: enviar_subcategorias(cliente, categoria_id); break;
                    case 2: enviar_productos(cliente, categoria_id, subcategoria_id); break;
                    case 3: mostrar_carrito(cliente); break;
                    case 4: mostrar_metodos_pago(cliente); break;
                }
                continue; // Continuar al siguiente ciclo
            }
        }
        
        printf("Estado: %d, Opción procesada: %d\n", estado, opcion); // Para depuración
        
        // Procesamiento según el estado actual con mejor manejo de errores
        switch(estado) {
            case 0: // Menú principal
                if(opcion == 5) {
                    enviar_mensaje(cliente, "🚪 Sesión finalizada. ¡Gracias por su visita!\n");
                    close(cliente);
                    return;
                }
                else if(opcion == 4) {
                    estado = 3; // Ir al carrito
                    mostrar_carrito(cliente);
                }
                else if(opcion >= 1 && opcion <= 3) {
                    categoria_id = opcion - 1;
                    estado = 1; // Ir a subcategorías
                    snprintf(respuesta, sizeof(respuesta),
                           "📱 Categoría '%s' seleccionada\n",
                           categorias[categoria_id].nombre);
                    enviar_mensaje(cliente, respuesta);
                    
                    // Pequeña pausa para asegurar que el cliente reciba correctamente
                    usleep(100000); // 100ms
                    
                    enviar_subcategorias(cliente, categoria_id);
                }
                else {
                    enviar_mensaje(cliente, "⚠ Opción no válida. Intente nuevamente.\n");
                    // Pequeña pausa para asegurar recepción correcta
                    usleep(100000);
                    enviar_menu(cliente);
                }
                break;
                
            case 1: // Subcategorías (marcas)
                if(opcion == 0) {
                    estado = 0; // Volver al menú principal
                    categoria_id = -1;
                    enviar_mensaje(cliente, "↩ Volviendo al menú principal...\n");
                    // Asegurar que el cliente reciba el mensaje antes del menú
                    usleep(200000); // 200ms
                    enviar_menu(cliente);
                }
                else if(opcion >= 1 && opcion <= categorias[categoria_id].num_subcategorias) {
                    subcategoria_id = opcion - 1;
                    estado = 2; // Ir a productos
                    snprintf(respuesta, sizeof(respuesta),
                           "🛍️ Mostrando productos de %s - %s\n",
                           categorias[categoria_id].nombre,
                           categorias[categoria_id].subcategorias[subcategoria_id].nombre);
                    enviar_mensaje(cliente, respuesta);
                    // Pequeña pausa
                    usleep(100000);
                    enviar_productos(cliente, categoria_id, subcategoria_id);
                }
                else {
                    enviar_mensaje(cliente, "⚠ Opción no válida. Intente nuevamente.\n");
                    usleep(100000);
                    enviar_subcategorias(cliente, categoria_id);
                }
                break;
                
            case 2: // Productos
                if(opcion == 0) {
                    estado = 1; // Volver a selección de marcas
                    enviar_mensaje(cliente, "↩ Volviendo a selección de marcas...\n");
                    usleep(100000); // Pequeña pausa
                    enviar_subcategorias(cliente, categoria_id);
                }
                else if(opcion >= 1 && opcion <= categorias[categoria_id].subcategorias[subcategoria_id].num_productos) {
                    // Añadir al carrito
                    if(total_carrito < 30) {
                        struct producto p = categorias[categoria_id].subcategorias[subcategoria_id].productos[opcion-1];
                        strcpy(carrito[total_carrito].nombre, p.nombre);
                        carrito[total_carrito].precio = p.precio;
                        total_carrito++;
                        
                        // Informar al usuario
                        snprintf(respuesta, sizeof(respuesta),
                               "✅ Añadido: %s ($%.2f)\n\n¿Qué desea hacer ahora?\n1. Seguir comprando\n2. Ver carrito\n3. Menú principal\nSeleccione una opción (1-3): ",
                               p.nombre, p.precio);
                        enviar_mensaje(cliente, respuesta);
                        
                        // Esperar respuesta para la navegación
                        memset(buffer, 0, sizeof(buffer));
                        bytes_recibidos = recv(cliente, buffer, sizeof(buffer) - 1, 0);
                        if(bytes_recibidos <= 0) break;
                        buffer[bytes_recibidos] = '\0';
                        if(strchr(buffer, '\n')) *strchr(buffer, '\n') = '\0';
                        
                        // Depuración para ver exactamente qué se recibió
                        printf("Después de añadir producto, recibido: '%s'\n", buffer);
                        
                        // Validar que sea un número
                        int nav_opcion = 0;
                        int es_nav_numero = 1;
                        
                        for(int i = 0; i < strlen(buffer); i++) {
                            if(!isdigit(buffer[i])) {
                                es_nav_numero = 0;
                                break;
                            }
                        }
                        
                        if(es_nav_numero && strlen(buffer) > 0) {
                            nav_opcion = atoi(buffer);
                        } else {
                            // Por defecto seguir comprando si no es número válido
                            nav_opcion = 1;
                        }
                        
                        if(nav_opcion == 2) {
                            estado = 3; // Ver carrito
                            mostrar_carrito(cliente);
                        }
                        else if(nav_opcion == 3) {
                            estado = 0; // Menú principal
                            enviar_menu(cliente);
                        }
                        else {
                            // Seguir comprando (por defecto)
                            enviar_productos(cliente, categoria_id, subcategoria_id);
                        }
                    } else {
                        enviar_mensaje(cliente, "❌ Carrito lleno (30/30 productos)\n");
                        usleep(100000);
                        enviar_productos(cliente, categoria_id, subcategoria_id);
                    }
                }
                else {
                    enviar_mensaje(cliente, "⚠ Opción no válida. Intente nuevamente.\n");
                    usleep(100000);
                    enviar_productos(cliente, categoria_id, subcategoria_id);
                }
                break;
                
            case 3: // Carrito
                if(opcion == 0) {
                    estado = 0; // Volver al menú principal
                    enviar_mensaje(cliente, "Volviendo al menú principal...\n");
                    usleep(100000);
                    enviar_menu(cliente);
                }
                else if(opcion == 1) {
                    if(total_carrito > 0) {
                        procesar_pago(cliente);
                        estado = 4; // Ir a métodos de pago
                    } else {
                        enviar_mensaje(cliente, "❌ No hay productos en el carrito para pagar\n");
                        usleep(100000);
                        mostrar_carrito(cliente);
                    }
                }
                else if(opcion == 2) {
                    total_carrito = 0;
                    enviar_mensaje(cliente, "🔄 Carrito vaciado correctamente\n");
                    estado = 0; // Volver al menú principal
                    usleep(100000);
                    enviar_menu(cliente);
                }
                else {
                    enviar_mensaje(cliente, "⚠ Opción no válida. Intente nuevamente.\n");
                    usleep(100000);
                    mostrar_carrito(cliente);
                }
                break;
                
            case 4: // Métodos de pago
                if(opcion == 1) {
                    credito(cliente);
                    estado = 0; // Volver al menú principal
                    usleep(200000); // Pausa más larga después del proceso de pago
                    enviar_menu(cliente);
                }
                else if(opcion == 2) {
                    efectivo(cliente);
                    estado = 0; // Volver al menú principal
                    usleep(200000);
                    enviar_menu(cliente);
                }
                else if(opcion == 3) {
                    pagomes(cliente);
                    estado = 0; // Volver al menú principal
                    usleep(200000);
                    enviar_menu(cliente);
                }
                else {
                    enviar_mensaje(cliente, "⚠ Opción no válida. Intente nuevamente.\n");
                    usleep(100000);
                    mostrar_metodos_pago(cliente);
                }
                break;
        }
    }
    

    close(cliente);
    printf("Conexión con cliente %d cerrada\n", cliente);
}
/**
 * Función principal del servidor
 */
int main(int argc, char *argv[]) {
    
    srand(time(NULL));
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
