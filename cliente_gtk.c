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
#include <pthread.h>
#include <fcntl.h>

#define BUFFER_SIZE 8192

// Variables globales para los elementos de la interfaz
GtkWidget *window;
GtkWidget *text_view;
GtkTextBuffer *text_buffer;
GtkWidget *entry;
GtkWidget *send_button;
GtkWidget *header_bar;
int socket_cliente;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
gboolean connection_active = TRUE;

// Estructura para pasar datos a los hilos
typedef struct {
    char mensaje[BUFFER_SIZE];
    gboolean es_usuario;
} MensajeInfo;

// Función para aplicar estilos CSS - versión mejorada
void aplicar_estilos() {
    GtkCssProvider *provider = gtk_css_provider_new();
    const gchar *css = 
        "window, #main-box {"
        "   background-color: #ffffff;"
        "}"
        "textview {"
        "   background-color: #ffffff;"
        "   color: #333333;"
        "   border-radius: 8px;"
        "   border: 1px solid #e0e0e0;"
        "}"
        "textview text {"
        "   background-color: #ffffff;"
        "   color: #333333;"
        "}"
        "entry {"
        "   border-radius: 20px;"
        "   padding: 8px 15px;"
        "   border: 1px solid #dddddd;"
        "   background-color: white;"
        "   color: #333333;"
        "}"
        "button {"
        "   background-color: #4a90e2;"
        "   color: white;"
        "   border-radius: 20px;"
        "   padding: 8px 20px;"
        "   border: none;"
        "}";

    GError *error = NULL;
    gtk_css_provider_load_from_data(provider, css, -1, &error);
    
    if (error) {
        g_warning("Error al cargar CSS: %s", error->message);
        g_error_free(error);
    } else {
        gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                                                GTK_STYLE_PROVIDER(provider),
                                                GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    }
    
    g_object_unref(provider);
}

// Función para recibir datos del servidor con timeout
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
    
    int bytes_recibidos = recv(socket_cliente, buffer, buffer_size - 1, 0);
    if(bytes_recibidos > 0) buffer[bytes_recibidos] = '\0';
    
    return bytes_recibidos;
}

// Función para actualizar la interfaz desde el hilo principal
gboolean actualizar_interfaz_idle(gpointer data) {
    MensajeInfo *info = (MensajeInfo *)data;
    
    GtkTextIter end;
    pthread_mutex_lock(&mutex);
    gtk_text_buffer_get_end_iter(text_buffer, &end);
    
    if(info->es_usuario) {
        // Mensaje del usuario (derecha, fondo azul)
        gtk_text_buffer_insert(text_buffer, &end, "\n", -1);
        gtk_text_buffer_insert_with_tags_by_name(text_buffer, &end, info->mensaje, -1, "user_msg", NULL);
    } else {
        // Mensaje del servidor (izquierda)
        gtk_text_buffer_insert(text_buffer, &end, "\n", -1);
        gtk_text_buffer_insert_with_tags_by_name(text_buffer, &end, info->mensaje, -1, "server_msg", NULL);
    }
    
    // Auto-scroll al final
    GtkAdjustment *adj = gtk_scrolled_window_get_vadjustment(
        GTK_SCROLLED_WINDOW(gtk_widget_get_parent(text_view)));
    gtk_adjustment_set_value(adj, gtk_adjustment_get_upper(adj) - gtk_adjustment_get_page_size(adj));
    
    pthread_mutex_unlock(&mutex);
    
    free(info);
    return FALSE; // Para eliminar el idle
}

// Función para actualizar el texto en la interfaz
void actualizar_interfaz(const char *texto, gboolean es_usuario) {
    MensajeInfo *info = g_malloc(sizeof(MensajeInfo));
    strncpy(info->mensaje, texto, BUFFER_SIZE - 1);
    info->mensaje[BUFFER_SIZE - 1] = '\0';
    info->es_usuario = es_usuario;
    
    // Añadir a la cola de eventos para ser procesado en el hilo principal
    gdk_threads_add_idle(actualizar_interfaz_idle, info);
}

// Función para manejar eventos de conexión desde un hilo separado
gpointer escuchar_servidor(gpointer data) {
    char buffer[BUFFER_SIZE];
    
    while(connection_active) {
        int bytes_recibidos = recibir_datos(socket_cliente, buffer, BUFFER_SIZE);
        
        if(bytes_recibidos > 0) {
            if(strncmp(buffer, "<SYNC>", 6) == 0) {
                send(socket_cliente, "<SYNC_ACK>", 10, 0);
            } else {
                actualizar_interfaz(buffer, FALSE);
                
                // Verificar si debemos deshabilitar la entrada
                if(strstr(buffer, "Sesión finalizada") != NULL || 
                   strstr(buffer, "Gracias por su compra") != NULL) {
                    // Usar gdk_threads_add_idle para modificar la UI desde el hilo principal
                    gdk_threads_add_idle((GSourceFunc)gtk_widget_set_sensitive, send_button);
                    gdk_threads_add_idle((GSourceFunc)gtk_widget_set_sensitive, entry);
                    connection_active = FALSE;
                    break;
                }
            }
        } else if (bytes_recibidos == 0) {
            // Conexión cerrada por el servidor
            actualizar_interfaz("Conexión cerrada por el servidor", FALSE);
            connection_active = FALSE;
            break;
        } else {
            // Solo reportar error si la conexión sigue activa
            if(connection_active) {
                sleep(1); // Evitar bucle infinito de CPU
            }
        }
    }
    
    return NULL;
}

// Función para enviar datos al servidor
void enviar_datos(GtkWidget *widget, gpointer data) {
    const char *input = gtk_entry_get_text(GTK_ENTRY(entry));
    
    if(strlen(input) == 0 || !connection_active) return;
    
    // Mostrar entrada del usuario
    char user_msg[BUFFER_SIZE];
    snprintf(user_msg, sizeof(user_msg), "Tú: %s", input);
    actualizar_interfaz(user_msg, TRUE);
    
    // Enviar al servidor
    if(send(socket_cliente, input, strlen(input), 0) < 0) {
        actualizar_interfaz("Error: No se pudo enviar el mensaje", FALSE);
        connection_active = FALSE;
        gtk_widget_set_sensitive(send_button, FALSE);
        gtk_widget_set_sensitive(entry, FALSE);
    }
    
    // Limpiar el campo de entrada
    gtk_entry_set_text(GTK_ENTRY(entry), "");
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
gboolean iniciar_conexion(const char *ip, const char *puerto) {
    struct sockaddr_in dir_servidor;
    dir_servidor.sin_family = AF_INET;
    dir_servidor.sin_port = htons(atoi(puerto));
    
    if(inet_aton(ip, &(dir_servidor.sin_addr)) == 0) {
        return FALSE;
    }
    
    memset(&(dir_servidor.sin_zero), '\0', 8);

    // Crear socket
    socket_cliente = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_cliente == -1) {
        return FALSE;
    }

    // Configurar para operaciones no bloqueantes
    int flags = fcntl(socket_cliente, F_GETFL, 0);
    fcntl(socket_cliente, F_SETFL, flags | O_NONBLOCK);

    // Configurar timeout
    struct timeval tv;
    tv.tv_sec = 10;
    tv.tv_usec = 0;
    setsockopt(socket_cliente, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);

    // Conectar con timeout
    connect(socket_cliente, (struct sockaddr *)&dir_servidor, sizeof(struct sockaddr));
    
    // Esperar a que el socket esté listo para escritura (conectado)
    fd_set write_fds;
    FD_ZERO(&write_fds);
    FD_SET(socket_cliente, &write_fds);
    
    tv.tv_sec = 5;
    tv.tv_usec = 0;
    
    if(select(socket_cliente + 1, NULL, &write_fds, NULL, &tv) <= 0) {
        close(socket_cliente);
        return FALSE;
    }
    
    // Verificar si la conexión tuvo éxito
    int error = 0;
    socklen_t len = sizeof(error);
    if(getsockopt(socket_cliente, SOL_SOCKET, SO_ERROR, &error, &len) < 0 || error != 0) {
        close(socket_cliente);
        return FALSE;
    }
    
    // Volver a modo bloqueante para operaciones normales
    flags = fcntl(socket_cliente, F_GETFL, 0);
    fcntl(socket_cliente, F_SETFL, flags & ~O_NONBLOCK);
    
    // Iniciar hilo para escuchar al servidor
    pthread_t thread_id;
    if(pthread_create(&thread_id, NULL, escuchar_servidor, NULL) != 0) {
        close(socket_cliente);
        return FALSE;
    }
    
    pthread_detach(thread_id);
    return TRUE;
}

// Función para mostrar diálogo de error
void mostrar_error(const char *mensaje) {
    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(window),
                                             GTK_DIALOG_DESTROY_WITH_PARENT,
                                             GTK_MESSAGE_ERROR,
                                             GTK_BUTTONS_CLOSE,
                                             "%s", mensaje);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

// Función de limpieza al cerrar
void on_window_destroy(GtkWidget *widget, gpointer data) {
    connection_active = FALSE;
    if(socket_cliente > 0) {
        close(socket_cliente);
    }
    gtk_main_quit();
}

int main(int argc, char *argv[]) {
    // Verificar argumentos
    if(argc != 3) {
        printf("Uso correcto: %s <DIRECCION_IP> <PUERTO>\n", argv[0]);
        printf("Ejemplo: %s 127.0.0.1 8080\n", argv[0]);
        exit(-1);
    }

    // Forzar tema claro en GTK
    g_setenv("GTK_THEME", "Adwaita:light", TRUE);

    // Inicializar GTK con soporte para hilos
    gtk_init(&argc, &argv);
    aplicar_estilos();

    // Crear ventana principal
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Tienda Interactiva");
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 500);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    g_signal_connect(window, "destroy", G_CALLBACK(on_window_destroy), NULL);

    // Crear header bar
    header_bar = gtk_header_bar_new();
    gtk_header_bar_set_title(GTK_HEADER_BAR(header_bar), "Tienda Interactiva");
    gtk_header_bar_set_show_close_button(GTK_HEADER_BAR(header_bar), TRUE);
    gtk_header_bar_set_decoration_layout(GTK_HEADER_BAR(header_bar), "menu:close");
    gtk_window_set_titlebar(GTK_WINDOW(window), header_bar);
    
    // Aplicar color a la header bar
    GtkStyleContext *header_context = gtk_widget_get_style_context(header_bar);
    gtk_style_context_add_class(header_context, "header");

    // Crear contenedor principal
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_name(vbox, "main-box");
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
                             "background", "#f0f0f0",
                             "foreground", "#333333",
                             "weight", PANGO_WEIGHT_NORMAL,
                             "left-margin", 10,
                             "right-margin", 100,
                             "pixels-above-lines", 5,
                             "pixels-below-lines", 5,
                             NULL);
                             
    gtk_text_buffer_create_tag(text_buffer, "user_msg",
                             "background", "#e3f2fd",
                             "foreground", "#0d47a1",
                             "weight", PANGO_WEIGHT_BOLD,
                             "left-margin", 100,
                             "right-margin", 10,
                             "pixels-above-lines", 5,
                             "pixels-below-lines", 5,
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

    // Mostrar mensaje inicial
    actualizar_interfaz("Conectando al servidor...", FALSE);
    
    // Mostrar todos los widgets
    gtk_widget_show_all(window);

    // Iniciar conexión con el servidor
    if(!iniciar_conexion(argv[1], argv[2])) {
        mostrar_error("Error al conectar con el servidor. Verifica la dirección IP y el puerto.");
        gtk_widget_set_sensitive(send_button, FALSE);
        gtk_widget_set_sensitive(entry, FALSE);
    } else {
        actualizar_interfaz("Conexión establecida. Esperando respuesta del servidor...", FALSE);
    }

    // Ejecutar el bucle principal de GTK
    gtk_main();

    return 0;
}