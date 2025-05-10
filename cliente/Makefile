# Makefile para el cliente

# Compilador
CC = gcc

# Flags de compilación
CFLAGS = -Wall -Wextra -g

# Directorios
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build

# Archivos fuente
SOURCES = $(SRC_DIR)/common/main.c \
          $(SRC_DIR)/common/utils.c \
          $(SRC_DIR)/core/app_manager.c \
          $(SRC_DIR)/net/server_connector.c \
          $(SRC_DIR)/ui/main_window.c

# Archivos objeto
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# Nombre del ejecutable
TARGET = cliente

# Regla principal
all: directories $(TARGET)

# Crear directorio build si no existe
directories:
	mkdir -p $(BUILD_DIR)/common
	mkdir -p $(BUILD_DIR)/core
	mkdir -p $(BUILD_DIR)/net
	mkdir -p $(BUILD_DIR)/ui

# Construir el ejecutable
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

# Regla para compilar archivos .c a .o
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# Limpiar archivos compilados
clean:
	rm -rf $(BUILD_DIR)
	rm -f $(TARGET)

# Para ejecutar el cliente (opcional)
run: all
	./$(TARGET) 127.0.0.1 8080

.PHONY: all clean run directories