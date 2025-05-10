#ifndef BUSINESS_LOGIC_H
#define BUSINESS_LOGIC_H

int business_logic_enviar_menu(int sock);
int business_logic_enviar_subcategorias(int sock, int categoria_id);
int business_logic_enviar_productos(int sock, int categoria_id, int subcategoria_id);
int business_logic_mostrar_carrito(int sock);
int business_logic_procesar_pago(int sock);
int business_logic_procesar_credito(int sock);
int business_logic_procesar_efectivo(int sock);
int business_logic_procesar_pagomes(int sock);
int business_logic_mostrar_metodos_pago(int sock);

#endif // BUSINESS_LOGIC_H