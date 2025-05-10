#include "../include/net/client_handler.h"
#include "../include/core/business_logic.h"
#include "../include/data/store.h"
#include "../include/config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/socket.h>  // Añadido para send y recv

int client_handler_enviar_mensaje(int sock, const char *mensaje) {
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

void client_handler_manejar_cliente(int cliente) {
    int estado = 0; // 0=menu principal, 1=marcas, 2=productos 3=estado del carrito, 4=método de pago 
    int categoria_id = -1;
    int subcategoria_id = -1;
    char buffer[1024];
    char respuesta[2048];

    // Enviar menú principal inicialmente
    business_logic_enviar_menu(cliente);

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
                client_handler_enviar_mensaje(cliente, "⚠ Por favor ingrese un número válido.\n");
                
                // Reenviar el menú apropiado según el estado actual
                switch(estado) {
                    case 0: business_logic_enviar_menu(cliente); break;
                    case 1: business_logic_enviar_subcategorias(cliente, categoria_id); break;
                    case 2: business_logic_enviar_productos(cliente, categoria_id, subcategoria_id); break;
                    case 3: business_logic_mostrar_carrito(cliente); break;
                    case 4: business_logic_mostrar_metodos_pago(cliente); break;
                }
                continue; // Continuar al siguiente ciclo
            }
        }
        
        printf("Estado: %d, Opción procesada: %d\n", estado, opcion); // Para depuración
        
        // Procesamiento según el estado actual con mejor manejo de errores
        switch(estado) {
            case 0: // Menú principal
                if(opcion == 5) {
                    client_handler_enviar_mensaje(cliente, "🚪 Sesión finalizada. ¡Gracias por su visita!\n");
                    close(cliente);
                    return;
                }
                else if(opcion == 4) {
                    estado = 3; // Ir al carrito
                    business_logic_mostrar_carrito(cliente);
                }
                else if(opcion >= 1 && opcion <= 3) {
                    categoria_id = opcion - 1;
                    estado = 1; // Ir a subcategorías
                    snprintf(respuesta, sizeof(respuesta),
                           "📱 Categoría '%s' seleccionada\n",
                           categorias[categoria_id].nombre);
                    client_handler_enviar_mensaje(cliente, respuesta);
                    
                    // Pequeña pausa para asegurar que el cliente reciba correctamente
                    usleep(100000); // 100ms
                    
                    business_logic_enviar_subcategorias(cliente, categoria_id);
                }
                else {
                    client_handler_enviar_mensaje(cliente, "⚠ Opción no válida. Intente nuevamente.\n");
                    // Pequeña pausa para asegurar recepción correcta
                    usleep(100000);
                    business_logic_enviar_menu(cliente);
                }
                break;
                
            case 1: // Subcategorías (marcas)
                if(opcion == 0) {
                    estado = 0; // Volver al menú principal
                    categoria_id = -1;
                    client_handler_enviar_mensaje(cliente, "↩ Volviendo al menú principal...\n");
                    // Asegurar que el cliente reciba el mensaje antes del menú
                    usleep(200000); // 200ms
                    business_logic_enviar_menu(cliente);
                }
                else if(opcion >= 1 && opcion <= categorias[categoria_id].num_subcategorias) {
                    subcategoria_id = opcion - 1;
                    estado = 2; // Ir a productos
                    snprintf(respuesta, sizeof(respuesta),
                           "🛍️ Mostrando productos de %s - %s\n",
                           categorias[categoria_id].nombre,
                           categorias[categoria_id].subcategorias[subcategoria_id].nombre);
                    client_handler_enviar_mensaje(cliente, respuesta);
                    // Pequeña pausa
                    usleep(100000);
                    business_logic_enviar_productos(cliente, categoria_id, subcategoria_id);
                }
                else {
                    client_handler_enviar_mensaje(cliente, "⚠ Opción no válida. Intente nuevamente.\n");
                    usleep(100000);
                    business_logic_enviar_subcategorias(cliente, categoria_id);
                }
                break;
                
            case 2: // Productos
                if(opcion == 0) {
                    estado = 1; // Volver a selección de marcas
                    client_handler_enviar_mensaje(cliente, "↩ Volviendo a selección de marcas...\n");
                    usleep(100000); // Pequeña pausa
                    business_logic_enviar_subcategorias(cliente, categoria_id);
                }
                else if(opcion >= 1 && opcion <= categorias[categoria_id].subcategorias[subcategoria_id].num_productos) {
                    // Añadir al carrito
                    if(total_carrito < 30) {
                        struct producto p = categorias[categoria_id].subcategorias[subcategoria_id].productos[opcion-1];
                        store_anadir_al_carrito(p);
                        
                        // Informar al usuario
                        snprintf(respuesta, sizeof(respuesta),
                               "✅ Añadido: %s ($%.2f)\n\n¿Qué desea hacer ahora?\n1. Seguir comprando\n2. Ver carrito\n3. Menú principal\nSeleccione una opción (1-3): ",
                               p.nombre, p.precio);
                        client_handler_enviar_mensaje(cliente, respuesta);
                        
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
                            business_logic_mostrar_carrito(cliente);
                        }
                        else if(nav_opcion == 3) {
                            estado = 0; // Menú principal
                            business_logic_enviar_menu(cliente);
                        }
                        else {
                            // Seguir comprando (por defecto)
                            business_logic_enviar_productos(cliente, categoria_id, subcategoria_id);
                        }
                    } else {
                        client_handler_enviar_mensaje(cliente, "❌ Carrito lleno (30/30 productos)\n");
                        usleep(100000);
                        business_logic_enviar_productos(cliente, categoria_id, subcategoria_id);
                    }
                }
                else {
                    client_handler_enviar_mensaje(cliente, "⚠ Opción no válida. Intente nuevamente.\n");
                    usleep(100000);
                    business_logic_enviar_productos(cliente, categoria_id, subcategoria_id);
                }
                break;
                
            case 3: // Carrito
                if(opcion == 0) {
                    estado = 0; // Volver al menú principal
                    client_handler_enviar_mensaje(cliente, "Volviendo al menú principal...\n");
                    usleep(100000);
                    business_logic_enviar_menu(cliente);
                }
                else if(opcion == 1) {
                    if(total_carrito > 0) {
                        business_logic_procesar_pago(cliente);
                        estado = 4; // Ir a métodos de pago
                    } else {
                        client_handler_enviar_mensaje(cliente, "❌ No hay productos en el carrito para pagar\n");
                        usleep(100000);
                        business_logic_mostrar_carrito(cliente);
                    }
                }
                else if(opcion == 2) {
                    store_vaciar_carrito();
                    client_handler_enviar_mensaje(cliente, "🔄 Carrito vaciado correctamente\n");
                    estado = 0; // Volver al menú principal
                    usleep(100000);
                    business_logic_enviar_menu(cliente);
                }
                else {
                    client_handler_enviar_mensaje(cliente, "⚠ Opción no válida. Intente nuevamente.\n");
                    usleep(100000);
                    business_logic_mostrar_carrito(cliente);
                }
                break;
                
            case 4: // Métodos de pago
                if(opcion == 1) {
                    business_logic_procesar_credito(cliente);
                    estado = 0; // Volver al menú principal
                    usleep(200000); // Pausa más larga después del proceso de pago
                    business_logic_enviar_menu(cliente);
                }
                else if(opcion == 2) {
                    business_logic_procesar_efectivo(cliente);
                    estado = 0; // Volver al menú principal
                    usleep(200000);
                    business_logic_enviar_menu(cliente);
                }
                else if(opcion == 3) {
                    business_logic_procesar_pagomes(cliente);
                    estado = 0; // Volver al menú principal
                    usleep(200000);
                    business_logic_enviar_menu(cliente);
                }
                else {
                    client_handler_enviar_mensaje(cliente, "⚠ Opción no válida. Intente nuevamente.\n");
                    usleep(100000);
                    business_logic_mostrar_metodos_pago(cliente);
                }
                break;
        }
    }
    
    // Limpieza final
    close(cliente);
    printf("Conexión con cliente %d cerrada\n", cliente);
}