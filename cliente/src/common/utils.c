#include "../include/common/utils.h"
#include <stdio.h>

/**
 * Función para limpiar el buffer de entrada
 */
void utils_limpiar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}