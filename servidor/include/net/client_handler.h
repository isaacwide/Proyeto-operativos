#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H

/**
 * Función para enviar mensajes completos a un cliente
 * @param sock Socket del cliente
 * @param mensaje Mensaje a enviar
 * @return Número de bytes enviados o -1 en caso de error
 */
int client_handler_enviar_mensaje(int sock, const char *mensaje);

/**
 * Maneja la interacción con un cliente
 * @param cliente Socket del cliente
 */
void client_handler_manejar_cliente(int cliente);

#endif // CLIENT_HANDLER_H