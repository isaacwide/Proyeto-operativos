#ifndef STORE_H
#define STORE_H

// Estructura que define un producto de la tienda
struct producto {
    char nombre[50];        // Nombre del producto (máx 50 caracteres)
    float precio;           // Precio del producto
};

// Estructura para subcategorías (marcas)
struct Subcategoria {
    char nombre[50];
    struct producto *productos;
    int num_productos;
};

// Estructura para categorías
// Estructura para categorías (debería estar al principio del archivo)
struct Categoria {
    char nombre[50];
    struct Subcategoria *subcategorias;
    int num_subcategorias;
};

// Declaración externa del array categorias (debería estar después de la definición de la estructura)
extern struct Categoria categorias[];

// Variable global para el carrito
extern struct producto carrito[30];
extern int total_carrito;
extern float total_pagar;
// Añade esta línea en include/data/store.h


// Inicializa todas las categorías, subcategorías y productos
void store_inicializar();

// Obtiene una categoría por su ID
struct Categoria* store_obtener_categoria(int categoria_id);

// Obtiene una subcategoría
struct Subcategoria* store_obtener_subcategoria(int categoria_id, int subcategoria_id);

// Obtiene un producto
struct producto* store_obtener_producto(int categoria_id, int subcategoria_id, int producto_id);

// Añade un producto al carrito
int store_anadir_al_carrito(struct producto p);

// Vacía el carrito
void store_vaciar_carrito();

// Calcula el total a pagar
float store_calcular_total();

#endif // STORE_H