#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>

#define BUFFER_SIZE 1024

// Estructura para pasar datos entre las callbacks de GTK
typedef struct {
    int socket_cliente;
    GtkWidget *text_view;
    GtkWidget *entry;
    GtkWidget *window;
} AppData;

// Función para enviar datos al servidor
void send_to_server(AppData *data, const char *message) {
    if (send(data->socket_cliente, message, strlen(message), 0) <= 0) {
        perror("Error al enviar datos al servidor");
        gtk_main_quit();
    }
}

// Función para recibir datos del servidor y mostrarlos en el TextView
void receive_from_server(AppData *data) {
    char buffer[BUFFER_SIZE] = {0};
    GtkTextBuffer *text_buffer;
    GtkTextIter end_iter;
    
    if (read(data->socket_cliente, buffer, BUFFER_SIZE) <= 0) {
        perror("Error al recibir datos del servidor");
        gtk_main_quit();
        return;
    }
    
    text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(data->text_view));
    gtk_text_buffer_get_end_iter(text_buffer, &end_iter);
    gtk_text_buffer_insert(text_buffer, &end_iter, buffer, -1);
    gtk_text_buffer_insert(text_buffer, &end_iter, "\n", -1);
    
    // Desplazar al final del texto
    GtkTextMark *mark = gtk_text_buffer_get_insert(text_buffer);
    gtk_text_view_scroll_to_mark(GTK_TEXT_VIEW(data->text_view), mark, 0.0, TRUE, 0.0, 1.0);
}

// Callback cuando se presiona Enter en el Entry
void on_entry_activate(GtkEntry *entry, gpointer user_data) {
    AppData *data = (AppData *)user_data;
    const char *text = gtk_entry_get_text(entry);
    
    send_to_server(data, text);
    send_to_server(data, "\n"); // Añadir salto de línea como fgets
    
    receive_from_server(data);
    
    gtk_entry_set_text(entry, "");
}

// Callback cuando se cierra la ventana
void on_window_destroy(GtkWidget *widget, gpointer user_data) {
    AppData *data = (AppData *)user_data;
    
    // Enviar comando de salida al servidor (opción 5)
    send_to_server(data, "5\n");
    
    // Cerrar el socket
    close(data->socket_cliente);
    printf("Conexión cerrada. Hasta pronto!\n");
    
    gtk_main_quit();
}

int main(int argc, char *argv[]) {
    GtkWidget *window, *scrolled_window, *vbox, *hbox, *button;
    GtkTextBuffer *buffer;
    AppData data;
    
    // Verificación de argumentos
    if (argc != 3) {
        printf("Uso correcto: %s <DIRECCION_IP> <PUERTO>\n", argv[0]);
        printf("Ejemplo: %s 127.0.0.1 8080\n", argv[0]);
        exit(-1);
    }

    // Configuración de la estructura del servidor
    struct sockaddr_in dir_servidor;
    dir_servidor.sin_family = AF_INET;
    dir_servidor.sin_port = htons(atoi(argv[2]));
    inet_aton(argv[1], &(dir_servidor.sin_addr));
    memset(&(dir_servidor.sin_zero), '\0', 8);

    // Creación del socket TCP
    data.socket_cliente = socket(AF_INET, SOCK_STREAM, 0);
    if (data.socket_cliente == -1) {
        perror("Error al crear el socket");
        exit(1);
    }

    // Conexión al servidor
    socklen_t tam = sizeof(struct sockaddr);
    printf("Conectando al servidor %s:%s...\n", argv[1], argv[2]);
    if (connect(data.socket_cliente, (struct sockaddr *)&dir_servidor, tam) == -1) {
        perror("Error al conectar con el servidor");
        exit(1);
    }
    printf("Conexión establecida.\n");

    // Inicializar GTK
    gtk_init(&argc, &argv);

    // Crear ventana principal
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Tienda de Videojuegos");
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);
    g_signal_connect(window, "destroy", G_CALLBACK(on_window_destroy), &data);
    data.window = window;

    // Crear contenedor vertical
    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Crear ScrolledWindow para el TextView
    scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_set_hexpand(scrolled_window, TRUE);
    gtk_widget_set_vexpand(scrolled_window, TRUE);
    gtk_box_pack_start(GTK_BOX(vbox), scrolled_window, TRUE, TRUE, 0);

    // Crear TextView para mostrar la salida del servidor
    data.text_view = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(data.text_view), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(data.text_view), FALSE);
    gtk_container_add(GTK_CONTAINER(scrolled_window), data.text_view);

    // Crear contenedor horizontal para el Entry y el botón
    hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);

    // Crear Entry para entrada de usuario
    data.entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(data.entry), "Introduce tu opción...");
    gtk_widget_set_hexpand(data.entry, TRUE);
    g_signal_connect(data.entry, "activate", G_CALLBACK(on_entry_activate), &data);
    gtk_box_pack_start(GTK_BOX(hbox), data.entry, TRUE, TRUE, 0);

    // Crear botón de enviar
    button = gtk_button_new_with_label("Enviar");
    g_signal_connect_swapped(button, "clicked", G_CALLBACK(on_entry_activate), data.entry);
    gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 0);

    // Mostrar todos los widgets
    gtk_widget_show_all(window);

    // Recibir el menú inicial del servidor
    receive_from_server(&data);

    // Ejecutar el bucle principal de GTK
    gtk_main();

    return 0;
}