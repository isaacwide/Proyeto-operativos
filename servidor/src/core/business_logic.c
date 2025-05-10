#include "../include/core/business_logic.h"
#include "../include/data/store.h"
#include "../include/net/client_handler.h"
#include "../include/config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/socket.h>



int business_logic_enviar_menu(int sock) {
    char menu[2048] = "\n\n=== MENÚ DE PRODUCTOS ===\n\n";
    
    // Construye el menú iterando sobre las categorías
    int num_categorias = 3;  // Fijo en 3 (teléfonos, laptops, videojuegos)
    for (int i = 0; i < num_categorias; i++) {
        char item[100];
        snprintf(item, sizeof(item), "%d. %s\n", i+1, categorias[i].nombre);
        strcat(menu, item);
    }
    
    // Añade opciones adicionales
    strcat(menu, "4. Ver canasta\n");
    strcat(menu, "5. Salir\n\n");
    strcat(menu, "Seleccione una categoria (1-5): ");
    
    return client_handler_enviar_mensaje(sock, menu);
}

int business_logic_enviar_subcategorias(int sock, int categoria_id) {
    char menu[2048];
    struct Categoria* cat = store_obtener_categoria(categoria_id);
    
    if (cat == NULL) {
        return client_handler_enviar_mensaje(sock, "Categoría no válida\n");
    }
    
    snprintf(menu, sizeof(menu), "=== %s ===\nMarcas Disponibles:\n", cat->nombre);
    
    // Mostrar marcas numeradas desde 1
    for (int i = 0; i < cat->num_subcategorias; i++) {
        char item[100];
        snprintf(item, sizeof(item), "%d. %s\n", i + 1, cat->subcategorias[i].nombre);
        strcat(menu, item);
    }
    
    // Opción para volver
    strcat(menu, "\n0. Volver al menú principal\n\n");
    strcat(menu, "Seleccione una marca (0-");
    char rango[10];
    snprintf(rango, sizeof(rango), "%d", cat->num_subcategorias);
    strcat(menu, rango);
    strcat(menu, "): ");
    
    return client_handler_enviar_mensaje(sock, menu);
}

int business_logic_enviar_productos(int sock, int categoria_id, int subcategoria_id) {
    char menu[2048];
    struct Categoria* cat = store_obtener_categoria(categoria_id);
    
    if (cat == NULL || subcategoria_id < 0 || subcategoria_id >= cat->num_subcategorias) {
        return client_handler_enviar_mensaje(sock, "Categoría o subcategoría no válida\n");
    }
    
    struct Subcategoria* sub = &(cat->subcategorias[subcategoria_id]);
    
    snprintf(menu, sizeof(menu), "===%s-%s===\n", cat->nombre, sub->nombre);
    
    for (int i = 0; i < sub->num_productos; i++) {
        char item[100];
        snprintf(item, sizeof(item), "%d.%s-$%.2f\n", i+1, sub->productos[i].nombre, sub->productos[i].precio);
        strcat(menu, item);
    }
    
    strcat(menu, "\n0. Volver\n\n");
    strcat(menu, "Seleccione un producto (0-");
    char rango[10];
    snprintf(rango, sizeof(rango), "%d", sub->num_productos);
    strcat(menu, rango);
    strcat(menu, "): ");
    
    return client_handler_enviar_mensaje(sock, menu);
}

int business_logic_mostrar_carrito(int sock) {
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
    
    return client_handler_enviar_mensaje(sock, buffer);
}

int business_logic_mostrar_metodos_pago(int sock) {
    char menu[512] = "\n=== MÉTODO DE PAGO ===\n";
    strcat(menu, "1. Tarjeta de crédito\n");
    strcat(menu, "2. Efectivo\n");
    strcat(menu, "3. Pago mensual\n");
    strcat(menu, "Seleccione una opción: ");
    
    return client_handler_enviar_mensaje(sock, menu);
}

int business_logic_procesar_pago(int sock) {
    if(total_carrito == 0) {
        return client_handler_enviar_mensaje(sock, "❌ No hay productos en el carrito para pagar\n");
    }
    
    // Calcular total
    store_calcular_total();
    return business_logic_mostrar_metodos_pago(sock);
}

int business_logic_procesar_credito(int sock) {
    char mensaje[2048] = "\n=== SECCIÓN DE PAGO CON TARJETA DE CRÉDITO ===\n";
    char buffer[100];
    char tarjeta[20] = {0};
    char fecha[6] = {0};
    char cvv[4] = {0};
    
    // Solicitar número de tarjeta
    strcat(mensaje, "Por favor ingrese los siguientes datos:\n");
    strcat(mensaje, "Número de tarjeta (XXXX-XXXX-XXXX-XXXX): ");
    client_handler_enviar_mensaje(sock, mensaje);
    
    // Recibir número de tarjeta
    memset(buffer, 0, sizeof(buffer));
    recv(sock, buffer, sizeof(buffer), 0);
    if(strchr(buffer, '\n')) *strchr(buffer, '\n') = '\0';
    strncpy(tarjeta, buffer, 19);
    
    // Solicitar fecha de expiración
    memset(mensaje, 0, sizeof(mensaje));
    strcpy(mensaje, "Fecha de expiración (MM/AA): ");
    client_handler_enviar_mensaje(sock, mensaje);
    
    // Recibir fecha
    memset(buffer, 0, sizeof(buffer));
    recv(sock, buffer, sizeof(buffer), 0);
    if(strchr(buffer, '\n')) *strchr(buffer, '\n') = '\0';
    strncpy(fecha, buffer, 5);
    
    // Solicitar CVV
    memset(mensaje, 0, sizeof(mensaje));
    strcpy(mensaje, "Código de seguridad (XXX): ");
    client_handler_enviar_mensaje(sock, mensaje);
    
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
    client_handler_enviar_mensaje(sock, mensaje);
    
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
    
    client_handler_enviar_mensaje(sock, mensaje);
    
    // Vaciar carrito
    store_vaciar_carrito();
    
    // Esperar ENTER para continuar
    memset(buffer, 0, sizeof(buffer));
    recv(sock, buffer, sizeof(buffer), 0);
    
    return 1;
}

int business_logic_procesar_efectivo(int sock) {
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

    int clave = 100000 + rand() % 900000;
    char message[200];

    snprintf(message, sizeof(message), "\nEscane este codigo\n[%d]\n En la tienda de su preferencia\n", clave);
    strcat(ticket, message);

    client_handler_enviar_mensaje(sock, ticket);

    store_vaciar_carrito();

    char buffer[10];
    recv(sock, buffer, sizeof(buffer), 0);
    
    return 1;
}

int business_logic_procesar_pagomes(int sock) {
    char mensaje[2048] = "\n=== PAGO MENSUAL ===\n";
    char buffer[100];
    int meses = 12; // Valor predeterminado
    
    // Solicitar cantidad de meses
    strcat(mensaje, "¿En cuántos meses desea dividir su pago? (3, 6, 9 o 12): ");
    client_handler_enviar_mensaje(sock, mensaje);
    
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
    client_handler_enviar_mensaje(sock, mensaje);
    
    // Recibir datos de tarjeta (similar a la función credito)
    char tarjeta[20] = {0};
    memset(buffer, 0, sizeof(buffer));
    recv(sock, buffer, sizeof(buffer), 0);
    if(strchr(buffer, '\n')) *strchr(buffer, '\n') = '\0';
    strncpy(tarjeta, buffer, 19);
    
    // Solicitar correo electrónico para notificaciones
    memset(mensaje, 0, sizeof(mensaje));
    strcpy(mensaje, "Correo electrónico para recibir notificaciones de pago: ");
    client_handler_enviar_mensaje(sock, mensaje);
    
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
    client_handler_enviar_mensaje(sock, mensaje);
    
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
    client_handler_enviar_mensaje(sock, mensaje);
    
    // Vaciar carrito
    store_vaciar_carrito();
    
    // Esperar ENTER para continuar
    memset(buffer, 0, sizeof(buffer));
    recv(sock, buffer, sizeof(buffer), 0);
    
    return 1;
}
