#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <gtk/gtk.h>
#include <pango/pango.h>

#define BUFFER_SIZE 8192

// Variables globales para los elementos de la interfaz
GtkWidget *window;
GtkWidget *text_view;
GtkTextBuffer *text_buffer;
GtkWidget *entry;
GtkWidget *send_button;
GtkWidget *header_bar;
int socket_cliente;

// Función para aplicar estilos CSS
void aplicar_estilos() {
    GtkCssProvider *provider = gtk_css_provider_new();
    const gchar *css = 
        "window {"
        "   background-color: #f5f5f5;"
        "}"
        "textview {"
        "   background-color: white;"
        "   border-radius: 8px;"
        "   padding: 10px;"
        "   box-shadow: 0 2px 4px rgba(0,0,0,0.1);"
        "}"
        "textview text {"
        "   color: #333333;"
        "}"
        "entry {"
        "   border-radius: 20px;"
        "   padding: 8px 15px;"
        "   border: 1px solid #dddddd;"
        "   background-color: white;"
        "   box-shadow: inset 0 1px 3px rgba(0,0,0,0.1);"
        "}"
        "entry:focus {"
        "   border-color: #4a90e2;"
        "}"
        "button {"
        "   background-color: #4a90e2;"
        "   color: white;"
        "   border-radius: 20px;"
        "   padding: 8px 20px;"
        "   border: none;"
        "   box-shadow: 0 2px 4px rgba(0,0,0,0.1);"
        "   transition: all 0.3s ease;"
        "}"
        "button:hover {"
        "   background-color: #3a7bc8;"
        "   box-shadow: 0 4px 6px rgba(0,0,0,0.15);"
        "}"
        "button:active {"
        "   background-color: #2a6bb0;"
        "}"
        ".header {"
        "   background-color: #4a90e2;"
        "   color: white;"
        "   padding: 10px 15px;"
        "   box-shadow: 0 2px 4px rgba(0,0,0,0.1);"
        "}";

    gtk_css_provider_load_from_data(provider, css, -1, NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                                            GTK_STYLE_PROVIDER(provider),
                                            GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}

// Función para manejar mensajes de sincronización
void manejar_sync(int socket_cliente) {
    char buffer[10];
    memset(buffer, 0, sizeof(buffer));
    recv(socket_cliente, buffer, sizeof(buffer), 0);
    if(strcmp(buffer, "<SYNC>") == 0) {
        send(socket_cliente, "<SYNC_ACK>", 10, 0);
    }
}

// Función para recibir datos del servidor
int recibir_datos(int socket_cliente, char *buffer, int buffer_size) {
    memset(buffer, 0, buffer_size);
    fd_set read_fds;
    struct timeval timeout;
    
    FD_ZERO(&read_fds);
    FD_SET(socket_cliente, &read_fds);
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;
    
    int ret = select(socket_cliente + 1, &read_fds, NULL, NULL, &timeout);
    if(ret <= 0) return -1;
    
    int bytes_recibidos = read(socket_cliente, buffer, buffer_size - 1);
    if(bytes_recibidos > 0) buffer[bytes_recibidos] = '\0';
    
    return bytes_recibidos;
}

// Función para actualizar el texto en la interfaz
void actualizar_interfaz(const char *texto, gboolean es_usuario) {
    GtkTextIter end;
    gtk_text_buffer_get_end_iter(text_buffer, &end);
    
    // Configurar tags para diferentes estilos
    if(es_usuario) {
        // Mensaje del usuario (derecha, fondo azul)
        gtk_text_buffer_insert_with_tags_by_name(text_buffer, &end, "\n", -1, "user_space", NULL);
        gtk_text_buffer_insert_with_tags_by_name(text_buffer, &end, texto, -1, "user_msg", NULL);
    } else {
        // Mensaje del servidor (centrado)
        gtk_text_buffer_insert_with_tags_by_name(text_buffer, &end, texto, -1, "server_msg", NULL);
        gtk_text_buffer_insert(text_buffer, &end, "\n", -1);
    }
    
    // Auto-scroll
    GtkAdjustment *adj = gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(
        gtk_widget_get_parent(text_view)));
    gtk_adjustment_set_value(adj, gtk_adjustment_get_upper(adj));
}

// Función para enviar datos al servidor
void enviar_datos(GtkWidget *widget, gpointer data) {
    const char *input = gtk_entry_get_text(GTK_ENTRY(entry));
    
    if(strlen(input) == 0) return;
    
    // Mostrar entrada del usuario
    char user_msg[256];
    snprintf(user_msg, sizeof(user_msg), "Tú: %s", input);
    actualizar_interfaz(user_msg, TRUE);
    
    // Enviar al servidor
    send(socket_cliente, input, strlen(input), 0);
    gtk_entry_set_text(GTK_ENTRY(entry), "");
    
    // Recibir respuesta
    char buffer[BUFFER_SIZE];
    int bytes_recibidos = recibir_datos(socket_cliente, buffer, BUFFER_SIZE);
    
    if(bytes_recibidos > 0) {
        if(strncmp(buffer, "<SYNC>", 6) == 0) {
            send(socket_cliente, "<SYNC_ACK>", 10, 0);
        } else {
            actualizar_interfaz(buffer, FALSE);
            
            if(strstr(buffer, "Sesión finalizada") != NULL || 
               strstr(buffer, "Gracias por su compra") != NULL) {
                gtk_widget_set_sensitive(send_button, FALSE);
                gtk_widget_set_sensitive(entry, FALSE);
            }
        }
    } else {
        actualizar_interfaz("Error: Conexión con el servidor perdida", FALSE);
        gtk_widget_set_sensitive(send_button, FALSE);
        gtk_widget_set_sensitive(entry, FALSE);
    }
}

// Función para manejar la tecla Enter
gboolean on_key_press(GtkWidget *widget, GdkEventKey *event, gpointer user_data) {
    if(event->keyval == GDK_KEY_Return || event->keyval == GDK_KEY_KP_Enter) {
        enviar_datos(NULL, NULL);
        return TRUE;
    }
    return FALSE;
}

// Función para inicializar la conexión con el servidor
void iniciar_conexion(const char *ip, const char *puerto) {
    struct sockaddr_in dir_servidor;
    dir_servidor.sin_family = AF_INET;
    dir_servidor.sin_port = htons(atoi(puerto));
    inet_aton(ip, &(dir_servidor.sin_addr));
    memset(&(dir_servidor.sin_zero), '\0', 8);

    // Crear socket
    socket_cliente = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_cliente == -1) {
        perror("Error al crear el socket");
        exit(1);
    }

    // Configurar timeout
    struct timeval tv;
    tv.tv_sec = 10;
    tv.tv_usec = 0;
    setsockopt(socket_cliente, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);

    // Conectar
    if(connect(socket_cliente, (struct sockaddr *)&dir_servidor, sizeof(struct sockaddr)) == -1) {
        perror("Error al conectar con el servidor");
        exit(1);
    }
    
    // Recibir mensaje de bienvenida
    char buffer[BUFFER_SIZE];
    int bytes_recibidos = recibir_datos(socket_cliente, buffer, BUFFER_SIZE);
    if(bytes_recibidos > 0) {
        actualizar_interfaz(buffer, FALSE);
    }
}

int main(int argc, char *argv[]) {
    // Verificar argumentos
    if(argc != 3) {
        printf("Uso correcto: %s <DIRECCION_IP> <PUERTO>\n", argv[0]);
        printf("Ejemplo: %s 127.0.0.1 8080\n", argv[0]);
        exit(-1);
    }

    // Inicializar GTK
    gtk_init(&argc, &argv);
    aplicar_estilos();

    // Crear ventana principal
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Tienda Interactiva");
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 500);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Crear header bar
    header_bar = gtk_header_bar_new();
    gtk_header_bar_set_title(GTK_HEADER_BAR(header_bar), "Tienda Interactiva");
    gtk_header_bar_set_show_close_button(GTK_HEADER_BAR(header_bar), TRUE);
    gtk_header_bar_set_decoration_layout(GTK_HEADER_BAR(header_bar), "menu:close");
    gtk_window_set_titlebar(GTK_WINDOW(window), header_bar);

    // Crear contenedor principal
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), vbox);
    gtk_container_set_border_width(GTK_CONTAINER(vbox), 10);

    // Crear área de texto con scroll
    GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),
                                  GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(scrolled_window), GTK_SHADOW_IN);
    gtk_box_pack_start(GTK_BOX(vbox), scrolled_window, TRUE, TRUE, 0);

    text_view = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(text_view), FALSE);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view), GTK_WRAP_WORD_CHAR);
    
    // Configurar fuente
    PangoFontDescription *font_desc = pango_font_description_from_string("Sans 11");
    gtk_widget_override_font(text_view, font_desc);
    pango_font_description_free(font_desc);
    
    // Configurar márgenes
    gtk_text_view_set_left_margin(GTK_TEXT_VIEW(text_view), 15);
    gtk_text_view_set_right_margin(GTK_TEXT_VIEW(text_view), 15);
    gtk_text_view_set_top_margin(GTK_TEXT_VIEW(text_view), 10);
    gtk_text_view_set_bottom_margin(GTK_TEXT_VIEW(text_view), 10);
    
    gtk_container_add(GTK_CONTAINER(scrolled_window), text_view);
    text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
    
    // Crear tags para formato de texto
    gtk_text_buffer_create_tag(text_buffer, "server_msg",
                             "justification", GTK_JUSTIFY_CENTER,
                             "weight", PANGO_WEIGHT_NORMAL,
                             NULL);
                             
    gtk_text_buffer_create_tag(text_buffer, "user_msg",
                             "justification", GTK_JUSTIFY_RIGHT,
                             "background", "#e3f2fd",
                             "foreground", "#0d47a1",
                             "weight", PANGO_WEIGHT_BOLD,
                             "margin-left", "100",
                             "margin-right", "10",
                             "border-radius", "10px",
                             "padding", "5px",
                             NULL);
                             
    gtk_text_buffer_create_tag(text_buffer, "user_space",
                             "pixels-above-lines", "5",
                             NULL);

    // Crear área de entrada
    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);

    entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "Escribe tu opción aquí...");
    gtk_widget_set_hexpand(entry, TRUE);
    gtk_box_pack_start(GTK_BOX(hbox), entry, TRUE, TRUE, 0);
    g_signal_connect(entry, "key-press-event", G_CALLBACK(on_key_press), NULL);

    send_button = gtk_button_new_with_label("Enviar");
    gtk_widget_set_can_default(send_button, TRUE);
    gtk_box_pack_start(GTK_BOX(hbox), send_button, FALSE, FALSE, 0);
    g_signal_connect(send_button, "clicked", G_CALLBACK(enviar_datos), NULL);

    // Mostrar todos los widgets
    gtk_widget_show_all(window);

    // Iniciar conexión con el servidor
    iniciar_conexion(argv[1], argv[2]);

    // Ejecutar el bucle principal de GTK
    gtk_main();

    // Cerrar el socket al salir
    close(socket_cliente);
    return 0;
}