#include "../include/data/store.h"
#include <string.h>
#include <stdlib.h>

// Definiciones de productos por categorías y subcategorías
// Samsung Teléfonos
struct producto samsung_telefonos[] = {
    {"Galaxy S24 Ultra", 27999.00},
    {"Galaxy S24+", 21999.00},
    {"Galaxy S24", 17999.00},
    {"Galaxy S23 FE", 12999.00},
    {"Galaxy Z Fold 5", 39999.00},
    {"Galaxy Z Flip 5", 22999.00},
    {"Galaxy A34 5G", 7499.00},
    {"Galaxy A14 5G", 4999.00},
    {"Galaxy M54 5G", 8499.00},
    {"Galaxy M34 5G", 6499.00}
};

// Apple Teléfonos
struct producto apple_telefonos[] = {
    {"iPhone 15 Pro Max 256GB", 34999.00},  
    {"iPhone 15 Pro 128GB", 29999.00},
    {"iPhone 15 128GB", 24999.00},
    {"iPhone 14 Pro Max 128GB", 31999.00},
    {"iPhone 14 128GB", 21999.00},
    {"iPhone SE (3ra gen) 64GB", 12999.00}, 
    {"iPhone 13 128GB", 18999.00},            
    {"iPhone 12 64GB (Reacondicionado)", 11999.00},
    {"iPhone 11 64GB (Reacondicionado)", 8999.00},
    {"iPhone 15 Plus 256GB", 28999.00}  
};

// Xiaomi Teléfonos
struct producto xiaomi_telefonos[] = {
    {"Redmi Note 13 Pro+ 5G (256GB)", 8999.00},  
    {"Redmi Note 13 Pro 5G (128GB)", 7499.00},
    {"Redmi Note 12 Pro 5G (128GB)", 6499.00},   
    {"Redmi 13C 5G (128GB)", 4999.00},          
    {"Redmi 12 5G (128GB)", 4499.00},
    {"POCO X6 Pro 5G (256GB)", 8499.00},        
    {"POCO M6 Pro 5G (128GB)", 5999.00},
    {"Xiaomi 13T 5G (256GB)", 14999.00},        
    {"Xiaomi 12 Lite 5G (128GB)", 8999.00},
    {"Redmi A3 (64GB)", 3499.00}                
};

// Huawei Teléfonos
struct producto huawei_telefonos[] = {
    {"Huawei Mate 60 Pro (512GB)", 24999.00},   
    {"Huawei Mate 50 Pro (256GB)", 19999.00},   
    {"Huawei P60 Pro (256GB)", 21999.00},       
    {"Huawei P50 Pro (128GB)", 16999.00},
    {"Huawei Nova 11 (256GB)", 9999.00},        
    {"Huawei Nova Y91 (128GB)", 5999.00},       
    {"Huawei Enjoy 60X (128GB)", 5499.00},      
    {"Huawei Enjoy 50z (64GB)", 4499.00},
    {"Huawei P40 Pro (128GB Reacond.)", 8999.00}, 
    {"Huawei MatePad 11 (128GB+LTE)", 12999.00} 
};

// OPPO Teléfonos
struct producto oppo_telefonos[] = {
    {"OPPO Find X6 Pro (256GB)", 24999.00},    
    {"OPPO Find N2 Flip (256GB)", 19999.00},   
    {"OPPO Reno 10 Pro+ 5G (256GB)", 14999.00}, 
    {"OPPO Reno 10 5G (128GB)", 10999.00},
    {"OPPO A98 5G (128GB)", 8499.00},          
    {"OPPO A78 5G (128GB)", 6999.00},
    {"OPPO A58 (128GB)", 5499.00},             
    {"OPPO K11x (128GB)", 5999.00},            
    {"OPPO Find X5 Lite (128GB Reacond.)", 7999.00},
    {"OPPO A38 (64GB)", 3999.00}              
};

// Lenovo Laptops
struct producto lenovo_laptops[] = {
    {"ThinkPad X1 Carbon Gen 11 (i7, 16GB, 512GB)", 39999.00},  
    {"Legion Pro 7i (i9, RTX 4090, 32GB)", 69999.00},           
    {"Yoga Slim 7 Pro (Ryzen 7, 16GB, 1TB)", 27999.00},         
    {"ThinkPad T14 Gen 3 (i5, 16GB, 256GB)", 24999.00},         
    {"IdeaPad Gaming 3 (Ryzen 5, RTX 3050)", 18999.00},         
    {"Legion 5 (i7, RTX 4060, 16GB)", 32499.00},                
    {"Yoga 9i (4K, i7, 16GB)", 45999.00},                       
    {"ThinkBook 14 (i5, 8GB, 512GB)", 16499.00},                
    {"IdeaPad 5 (Ryzen 5, 12GB, 512GB)", 15999.00},            
    {"Legion Slim 5 (Ryzen 7, RTX 4070)", 38999.00}             
};

// Apple Laptops
struct producto mac_laptops[] = {
    {"MacBook Pro 16\" M3 Max (48GB, 1TB)", 89999.00},          
    {"MacBook Air 15\" M2 (16GB, 512GB)", 34999.00},            
    {"MacBook Pro 14\" M3 Pro (36GB, 512GB)", 59999.00},        
    {"MacBook Air 13\" M1 (8GB, 256GB)", 19999.00},             
    {"MacBook Pro 13\" M2 (24GB, 1TB)", 42999.00},              
    {"MacBook Air M2 (8GB, 256GB)", 22999.00},                 
    {"MacBook Pro 16\" M1 Max (64GB, 8TB)", 120999.00},         
    {"MacBook Air 13\" M2 (16GB, 512GB)", 29999.00},           
    {"MacBook Pro 14\" M2 Pro (32GB, 1TB)", 54999.00},          
    {"MacBook Air 15\" M2 (8GB, 256GB)", 31999.00}              
};

// MSI Laptops
struct producto msi_laptops[] = {
    {"Raider GE78 (i9, RTX 4090, 64GB)", 99999.00},             
    {"Stealth 16 Studio (i7, RTX 4070)", 54999.00},           
    {"Katana 15 (i7, RTX 4060, 16GB)", 32999.00},               
    {"Vector GP68 (i9, RTX 4080)", 72499.00},                   
    {"Crosshair 15 (Ryzen 7, RTX 3060)", 28999.00},            
    {"Prestige 16 (i7, RTX 4050)", 41999.00},                  
    {"Sword 15 (i5, RTX 3050)", 23999.00},                      
    {"Titan GT77 (i9, RTX 4090, 128GB)", 150999.00},            
    {"Cyborg 15 (i7, RTX 4050)", 29499.00},                     
    {"Bravo 15 (Ryzen 5, RX 5500M)", 18999.00}                  
};

// Acer Laptops
struct producto acer_laptops[] = {
    {"Predator Helios 16 (i9, RTX 4080)", 65999.00},            
    {"Swift X 14 (Ryzen 7, RTX 3050)", 24999.00},               
    {"Nitro 5 (i5, RTX 3050, 144Hz)", 21999.00},                
    {"Aspire 5 (i5, Iris Xe, 16GB)", 15999.00},                 
    {"Spin 5 (i7, 16GB, 2K)", 32999.00},                        
    {"Predator Triton 14 (i7, RTX 4060)", 48999.00},            
    {"Chromebook 314 (Celeron, 4GB)", 6999.00},                 
    {"ConceptD 7 (i7, RTX 3060)", 55999.00},                    
    {"Swift Go 14 (i5, 16GB)", 19999.00},                       
    {"Extensa 15 (i3, 8GB)", 10999.00}                          
};

// HP Laptops
struct producto hp_laptops[] = {
    {"Spectre x360 16 (i7, OLED, 32GB)", 49999.00},            
    {"Omen 17 (i9, RTX 4090)", 79999.00},                       
    {"Envy 16 (i9, RTX 4060)", 54999.00},                       
    {"Victus 16 (Ryzen 7, RTX 3050)", 22999.00},               
    {"Pavilion Plus 14 (i5, 2.8K OLED)", 23999.00},            
    {"ZBook Firefly G10 (i7, RTX A500)", 42999.00},           
    {"ProBook 440 G10 (i5, 16GB)", 20999.00},                 
    {"EliteBook 840 G9 (i7, 32GB)", 38999.00},                 
    {"15-fd0099la (Ryzen 5, 12GB)", 14999.00},                
    {"Chromebook 14a (Pentium, 8GB)", 7499.00}                  
};

// Xbox Juegos
struct producto xbox_juegos[] = {
    {"Starfield ", 1199.00},          
    {"Forza Motorsport ", 1299.00},               
    {"Halo Infinite ", 999.00},                 
    {"Senua's Saga: Hellblade II", 1199.00},           
    {"Microsoft Flight Simulator 2024", 1499.00},       
    {"Gears 5 (Edición Definitiva)", 799.00},           
    {"Hi-Fi RUSH ", 499.00},                   
    {"Sea of Thieves (Edición Deluxe)", 699.00},        
    {"Psychonauts 2 ", 599.00},         
    {"Ori and the Will of the Wisps", 449.00}         
};

// PlayStation Juegos
struct producto playstation_juegos[] = {
    {"Marvel's Spider-Man 2 (Edición Estándar)", 1499.00},  
    {"God of War: Ragnarok", 1299.00},                      
    {"The Last of Us Part I (Remake)", 1199.00},            
    {"Final Fantasy XVI ", 1399.00},    
    {"Horizon Forbidden West", 1099.00},                    
    {"Demon's Souls (Remake)", 1199.00},                  
    {"Gran Turismo 7 ", 1199.00},       
    {"Ratchet & Clank: Rift Apart", 999.00},        
    {"Returnal (Edición Digital)", 899.00},                 
    {"Ghost of Tsushima: Director's Cut", 1099.00}         
};

// Nintendo Juegos
struct producto nintendo_juegos[] = {
    {"The Legend of Zelda: Tears of the Kingdom", 1499.00}, 
    {"Super Mario Bros. Wonder", 1199.00},                  
    {"Pokémon Escarlata/Violeta (DLC incluido)", 1399.00}, 
    {"Animal Crossing: New Horizons", 1099.00},            
    {"Metroid Prime 4 ", 1499.00},           
    {"Splatoon 3 (Edición Definitiva)", 1099.00},          
    {"Super Smash Bros. Ultimate", 1199.00},             
    {"Mario Kart 8 Deluxe ", 1099.00},                
    {"Xenoblade Chronicles 3", 1199.00},                  
    {"Luigi's Mansion 3", 999.00}                           
};


// Declaración de subcategorías
struct Subcategoria subcategorias_telefonos[] = {
    {"Samsung", samsung_telefonos, sizeof(samsung_telefonos)/sizeof(samsung_telefonos[0])},
    {"Apple", apple_telefonos, sizeof(apple_telefonos)/sizeof(apple_telefonos[0])},
    {"Xiaomi", xiaomi_telefonos, sizeof(xiaomi_telefonos)/sizeof(xiaomi_telefonos[0])},
    {"Huawei", huawei_telefonos, sizeof(huawei_telefonos)/sizeof(huawei_telefonos[0])},
    {"Oppo", oppo_telefonos, sizeof(oppo_telefonos)/sizeof(oppo_telefonos[0])}
};

struct Subcategoria subcategorias_laptops[] = {
    {"Lenovo", lenovo_laptops, sizeof(lenovo_laptops)/sizeof(lenovo_laptops[0])},
    {"Mac", mac_laptops, sizeof(mac_laptops)/sizeof(mac_laptops[0])},
    {"MSI", msi_laptops, sizeof(msi_laptops)/sizeof(msi_laptops[0])},
    {"Acer", acer_laptops, sizeof(acer_laptops)/sizeof(acer_laptops[0])},
    {"HP", hp_laptops, sizeof(hp_laptops)/sizeof(hp_laptops[0])}
};

struct Subcategoria subcategorias_videojuegos[] = {
    {"Xbox", xbox_juegos, sizeof(xbox_juegos)/sizeof(xbox_juegos[0])},
    {"PlayStation", playstation_juegos, sizeof(playstation_juegos)/sizeof(playstation_juegos[0])},
    {"Nintendo", nintendo_juegos, sizeof(nintendo_juegos)/sizeof(nintendo_juegos[0])}
};

// Definición de categorías
// Definición del array categorias (sin la palabra extern)
struct Categoria categorias[] = {
    {"Teléfonos", subcategorias_telefonos, sizeof(subcategorias_telefonos)/sizeof(subcategorias_telefonos[0])},
    {"Laptops", subcategorias_laptops, sizeof(subcategorias_laptops)/sizeof(subcategorias_laptops[0])},
    {"VideoJuegos", subcategorias_videojuegos, sizeof(subcategorias_videojuegos)/sizeof(subcategorias_videojuegos[0])}
};

// Variables globales para el carrito
struct producto carrito[30];
int total_carrito = 0;
float total_pagar = 0;

// Inicializa todas las categorías, subcategorías y productos
void store_inicializar() {
    // Ya están inicializados estáticamente, pero aquí podríamos
    // cargar datos desde archivos o base de datos en el futuro
    total_carrito = 0;
    total_pagar = 0;
}

// Obtiene una categoría por su ID
struct Categoria* store_obtener_categoria(int categoria_id) {
    if (categoria_id < 0 || categoria_id >= sizeof(categorias)/sizeof(categorias[0])) {
        return NULL;
    }
    return &categorias[categoria_id];
}

// Obtiene una subcategoría
struct Subcategoria* store_obtener_subcategoria(int categoria_id, int subcategoria_id) {
    struct Categoria* cat = store_obtener_categoria(categoria_id);
    if (cat == NULL || subcategoria_id < 0 || subcategoria_id >= cat->num_subcategorias) {
        return NULL;
    }
    return &(cat->subcategorias[subcategoria_id]);
}

// Obtiene un producto
struct producto* store_obtener_producto(int categoria_id, int subcategoria_id, int producto_id) {
    struct Subcategoria* sub = store_obtener_subcategoria(categoria_id, subcategoria_id);
    if (sub == NULL || producto_id < 0 || producto_id >= sub->num_productos) {
        return NULL;
    }
    return &(sub->productos[producto_id]);
}

// Añade un producto al carrito
int store_anadir_al_carrito(struct producto p) {
    if (total_carrito >= 30) {
        return 0; // Carrito lleno
    }
    
    strcpy(carrito[total_carrito].nombre, p.nombre);
    carrito[total_carrito].precio = p.precio;
    total_carrito++;
    return 1;
}

// Vacía el carrito
void store_vaciar_carrito() {
    total_carrito = 0;
    total_pagar = 0;
}

// Calcula el total a pagar
float store_calcular_total() {
    total_pagar = 0.0;
    for (int i = 0; i < total_carrito; i++) {
        total_pagar += carrito[i].precio;
    }
    return total_pagar;
}