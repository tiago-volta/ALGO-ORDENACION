#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))
#define LONGITUD_CLAVE 30
#define LONGITUD_SINONIMOS 300

typedef struct entrada_ {
    int ocupada;  // 0 si está vacío, 1 si está ocupado
    char clave[LONGITUD_CLAVE];
    char sinonimos[LONGITUD_SINONIMOS];
} entrada;

typedef entrada* tabla_cerrada;
typedef int pos;

typedef struct item_ {
    char clave[LONGITUD_CLAVE];
    char sinonimos[LONGITUD_SINONIMOS];
} item;

//Declaración de funciones
unsigned int dispersionA(char *clave, int tamTabla);
unsigned int dispersionB(char *clave, int tamTabla);
void inicializar_cerrada(tabla_cerrada *diccionario, int tam);
int insertar_cerrada(char *clave, char *sinonimos, tabla_cerrada *diccionario, int tam, unsigned int (*dispersion)(char *, int));
pos buscar_cerrada(char *clave, tabla_cerrada diccionario, int tam, int *colisiones, unsigned int (*dispersion)(char *, int));
void mostrar_cerrada(tabla_cerrada diccionario, int tam);
int leer_sinonimos(item datos[]);

//Función de dispersión A
unsigned int dispersionA(char *clave, int tamTabla) {
    int i, n = MIN(8, strlen(clave));
    unsigned int valor = clave[0];
    for (i = 1; i < n; i++) {
        valor += clave[i];
    }
    return valor % tamTabla;
}

// Función de dispersión B
unsigned int dispersionB(char *clave, int tamTabla) {
    int i, n = MIN(8, strlen(clave));
    unsigned int valor = clave[0];
    for (i = 1; i < n; i++) {
        valor = (valor << 5) + clave[i]; // Desplazamiento de 5 bits
    }
    return valor % tamTabla;
}

// Inicializar la tabla
void inicializar_cerrada(tabla_cerrada *diccionario, int tam) {
    for (int i = 0; i < tam; i++) {
        (*diccionario)[i].ocupada = 0; // Indica que la posición está vacía
    }
}

// Función para insertar en la tabla cerrada
int insertar_cerrada(char *clave, char *sinonimos, tabla_cerrada *diccionario, int tam,
                     unsigned int (*dispersion)(char *, int)) {
    unsigned int index = dispersion(clave, tam);
    int colisiones = 0;

    while ((*diccionario)[index].ocupada) { // Mientras haya colisiones
        colisiones++;
        index = (index + 1) % tam; // Exploración lineal
    }

    // Insertar la clave y sinónimos
    (*diccionario)[index].ocupada = 1;
    strcpy((*diccionario)[index].clave, clave);
    strcpy((*diccionario)[index].sinonimos, sinonimos);

    return colisiones; // Retorna el número de colisiones
}

// Función para buscar en la tabla cerrada
pos buscar_cerrada(char *clave, tabla_cerrada diccionario, int tam, int *colisiones, unsigned int (*dispersion)(char *, int)) {
    unsigned int index = dispersion(clave, tam);

    while (diccionario[index].ocupada) {
        if (strcmp(diccionario[index].clave, clave) == 0) {
            return index; // Clave encontrada
        }
        *colisiones += 1;
        index = (index + 1) % tam; // Exploración lineal
    }
    return -1; // Clave no encontrada
}

// Mostrar el contenido de la tabla
void mostrar_cerrada(tabla_cerrada diccionario, int tam) {
    for (int i = 0; i < tam; i++) {
        if (diccionario[i].ocupada) {
            printf("%d - (%s): %s\n", i, diccionario[i].clave, diccionario[i].sinonimos);
        }
    }
}

// Leer sinónimos desde el archivo
int leer_sinonimos(item datos[]) {
    char c;
    int i, j;
    FILE *archivo;
    if ((archivo = fopen("sinonimos.txt", "r")) == NULL) {
        printf("Error al abrir ’sinonimos.txt’\n");
        return(EXIT_FAILURE);
    }
    for (i = 0; fscanf(archivo, "%s", datos[i].clave) != EOF; i++) {
        if ((c = fgetc(archivo)) != '\t') {
            printf("Error al leer el tabulador\n");
            return(EXIT_FAILURE);
        }
        for (j = 0; (c = fgetc(archivo)) != '\n'; j++) {
            if (j < LONGITUD_SINONIMOS - 1)
                datos[i].sinonimos[j] = c;
        }
        datos[i].sinonimos[MIN(j, LONGITUD_SINONIMOS -1)] = '\0';
    }
    if (fclose(archivo) != 0) {
        printf("Error al cerrar el fichero\n");
        return(EXIT_FAILURE);
    }
    return(i);
}

// Programa principal
int main(void) {
    tabla_cerrada diccionario = malloc(38197 * sizeof(entrada));
    if (diccionario == NULL) {
        printf("Error al asignar memoria para el diccionario\n");
        return EXIT_FAILURE;
    }
    inicializar_cerrada(&diccionario, 38197);

    item datos[19062];
    int n = leer_sinonimos(datos);

    int total_colisiones = 0;
    for (int i = 0; i < n; i++) {
        total_colisiones += insertar_cerrada(datos[i].clave, datos[i].sinonimos, &diccionario, 38197, dispersionB);
    }

    printf("Número total de colisiones al insertar: %d\n", total_colisiones);
    mostrar_cerrada(diccionario, 38197);

    // Ejemplo de búsqueda
    char clave_buscar[LONGITUD_CLAVE];
    printf("Introduce la clave a buscar: ");
    scanf("%s", clave_buscar);

    int colisiones_busqueda = 0;
    pos index = buscar_cerrada(clave_buscar, diccionario, 38197, &colisiones_busqueda, dispersionB);

    if (index != -1) {
        printf("Encontrado: %s -> %s, colisiones: %d\n", diccionario[index].clave, diccionario[index].sinonimos, colisiones_busqueda);
    } else {
        printf("No encontrado: colisiones: %d\n", colisiones_busqueda);
    }

    free(diccionario);
    return 0;
}
