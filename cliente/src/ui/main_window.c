#include "../include/ui/main_window.h"
#include "../include/common/utils.h"
#include <stdio.h>
#include <string.h>

void ui_mostrar_mensaje(const char *mensaje) {
    printf("%s", mensaje);
    fflush(stdout);
}

int ui_obtener_entrada(const char *prompt, char *buffer, int buffer_size) {
    printf("%s", prompt);
    fflush(stdout);
    
    if (fgets(buffer, buffer_size, stdin) == NULL) {
        return 0;
    }
    
    // Limpiar buffer de entrada si es necesario
    if (strchr(buffer, '\n') == NULL) {
        utils_limpiar_buffer();
    } else {
        buffer[strcspn(buffer, "\n")] = 0; // Eliminar salto de línea
    }
    
    return 1;
}

void ui_mostrar_error(const char *mensaje) {
    fprintf(stderr, "Error: %s\n", mensaje);
}

void ui_mostrar_despedida() {
    printf("\nConexión cerrada. Hasta pronto!\n");
}