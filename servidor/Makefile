# Makefile para el servidor

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
          $(SRC_DIR)/core/business_logic.c \
          $(SRC_DIR)/data/store.c \
          $(SRC_DIR)/net/server_manager.c \
          $(SRC_DIR)/net/client_handler.c

# Archivos objeto
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# Nombre del ejecutable
TARGET = servidor

# Regla principal
all: directories $(TARGET)

# Crear directorios de build si no existen
directories:
	mkdir -p $(BUILD_DIR)/common
	mkdir -p $(BUILD_DIR)/core
	mkdir -p $(BUILD_DIR)/data
	mkdir -p $(BUILD_DIR)/net

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

# Para ejecutar el servidor (opcional)
run: all
	./$(TARGET)

.PHONY: all clean run directories