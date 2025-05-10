#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

/**
 * Muestra un mensaje al usuario
 * @param mensaje Mensaje a mostrar
 */
void ui_mostrar_mensaje(const char *mensaje);

/**
 * Obtiene una entrada del usuario
 * @param prompt Texto a mostrar antes de la entrada
 * @param buffer Buffer donde se almacenará la entrada
 * @param buffer_size Tamaño del buffer
 * @return 1 si se obtuvo la entrada correctamente, 0 en caso contrario
 */
int ui_obtener_entrada(const char *prompt, char *buffer, int buffer_size);

/**
 * Muestra un mensaje de error
 * @param mensaje Mensaje de error a mostrar
 */
void ui_mostrar_error(const char *mensaje);

/**
 * Muestra un mensaje de despedida
 */
void ui_mostrar_despedida();

#endif // MAIN_WINDOW_H

