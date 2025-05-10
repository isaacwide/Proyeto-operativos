#ifndef APP_MANAGER_H
#define APP_MANAGER_H

/**
 * Ejecuta el bucle principal de la aplicación
 * @param socket_cliente Descriptor del socket conectado al servidor
 * @return 0 si terminó correctamente, otro valor en caso de error
 */
int app_manager_ejecutar(int socket_cliente);

#endif // APP_MANAGER_H