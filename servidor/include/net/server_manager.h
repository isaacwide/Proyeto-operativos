#ifndef SERVER_MANAGER_H
#define SERVER_MANAGER_H

/**
 * Inicializa y configura el servidor
 * @param puerto Puerto en el que escuchará el servidor
 * @return Descriptor del socket o -1 en caso de error
 */
int server_manager_iniciar(int puerto);

/**
 * Cierra el servidor
 * @param socket_servidor Descriptor del socket del servidor
 */
void server_manager_cerrar(int socket_servidor);

#endif // SERVER_MANAGER_H
