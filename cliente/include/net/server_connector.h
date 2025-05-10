#ifndef SERVER_CONNECTOR_H
#define SERVER_CONNECTOR_H

/**
 * Inicializa la conexión con el servidor
 * @param ip_servidor Dirección IP del servidor
 * @param puerto Puerto del servidor
 * @return Descriptor del socket o -1 en caso de error
 */
int server_connector_iniciar(const char *ip_servidor, const char *puerto);

/**
 * Cierra la conexión con el servidor
 * @param socket_cliente Descriptor del socket
 */
void server_connector_cerrar(int socket_cliente);

/**
 * Manejar mensajes de sincronización del servidor
 * @param socket_cliente Descriptor del socket
 */
void server_connector_manejar_sync(int socket_cliente);

/**
 * Recibe datos del servidor con manejo de errores
 * @param socket_cliente Descriptor del socket
 * @param buffer Buffer donde se almacenarán los datos
 * @param buffer_size Tamaño del buffer
 * @return Número de bytes recibidos o -1 en caso de error
 */
int server_connector_recibir_datos(int socket_cliente, char *buffer, int buffer_size);

/**
 * Envía datos al servidor con reintentos en caso de fallo
 * @param socket_cliente Descriptor del socket
 * @param datos Datos a enviar
 * @return 1 si se enviaron los datos correctamente, 0 en caso contrario
 */
int server_connector_enviar_datos(int socket_cliente, const char *datos);

#endif // SERVER_CONNECTOR_H