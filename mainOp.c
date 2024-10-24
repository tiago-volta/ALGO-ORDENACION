#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))

#define LONGITUD_CLAVE 30
#define LONGITUD_SINONIMOS 300

typedef struct entrada_ {
    int ocupada;
    char clave [LONGITUD_CLAVE];
    char sinonimos [LONGITUD_SINONIMOS];
} entrada;
typedef int pos;

typedef struct {
    char clave [LONGITUD_CLAVE];
    char sinonimos [LONGITUD_SINONIMOS];
} item;


//Función
typedef entrada *tabla_cerrada;
tabla_cerrada d = malloc (38197 * sizeof(entrada));

unsigned int dispersionB(char *clave, int tamTabla);

unsigned int dispersionA(char *clave, int tamTabla);

void inicializar_cerrada(tabla_cerrada *diccionario, int tam);
//Posicion en la que se encuentra la clave, no buscar en el orden de inserción de las claves ya que las primeras no va a haber colisiones y los datos para n pequeños no van a aparecer coliciones y los datos no saldrán lineales
//Por ello escoger al azar, generar número aleatorio y buscar ese elemento
pos buscar_cerrada(char *clave, tabla_cerrada diccionario, int tam,int *colisiones, unsigned int (*dispersion)(char *, int),unsigned int (*resol_colisiones)(int pos_ini, int num_intento));
//Numero de coliciones que se produjeron al introducir la clave
int insertar_cerrada(char *clave, char *sinonimos,tabla_cerrada *diccionario, int tam,unsigned int (*dispersion)(char *, int),unsigned int (*resol_colisiones)(int pos_ini, int num_intento));

void mostrar_cerrada(tabla_cerrada diccionario, int tam);

int ndispersion(char *clave, int tamTabla);



int main(void) {
    printf("Hello, World!\n");
    return 0;
}

//Test
int ndispersion(char *clave, int tamTabla) {
    if (strcmp(clave, "ANA")==0) return 7;
    if (strcmp(clave, "JOSE")==0) return 7;
    if (strcmp(clave, "OLGA")==0) return 7;
    return 6;
}

unsigned int dispersionA(char *clave, int tamTabla) {
    int i, n = MIN(8, strlen(clave));
    unsigned int valor = clave[0];
    for (i = 1; i < n; i++)
        valor += clave[i];
    return valor % tamTabla;
}

unsigned int dispersionB(char *clave, int tamTabla) {
    int i, n = MIN(8, strlen(clave));
    unsigned int valor = clave[0];
    for (i = 1; i < n; i++)
        valor = (valor<<5) + clave[i]; /* el desplazamiento de 5 bits equivale a */
    return valor % tamTabla;
    /* multipicar por 32 */
}

/*
 *TEST
* ***TABLA CERRADA LINEAL
{
0- (IVAN ) //Nombre en la posición en la que se encuentra en la tabla
1-
2-
3-
4-
5-
6- (LUIS )
7- (ANA )
8- (JOSE )
9- (OLGA )
10- (ROSA )
}
Numero total de colisiones al insertar los elementos: 12
Al buscar: ANA, encuentro: ANA, colisiones: 0
Al buscar: LUIS, encuentro: LUIS, colisiones: 0
Al buscar: JOSE, encuentro: JOSE, colisiones: 1
Al buscar: OLGA, encuentro: OLGA, colisiones: 2
Al buscar: ROSA, encuentro: ROSA, colisiones: 4
Al buscar: IVAN, encuentro: IVAN, colisiones: 5
No encuentro: CARLOS, colisiones: 6     //Devuelve una celda vacía => carlos no está
***TABLA CERRADA CUADRATICA
{0- (OLGA )
1-
2-
3-
4- (IVAN )
5-
6- (LUIS )
7- (ANA )
8- (JOSE )
9-
10- (ROSA )}
Numero total de colisiones al insertar los elementos: 8
Al buscar: ANA, encuentro: ANA, colisiones: 0
Al buscar: LUIS, encuentro: LUIS, colisiones: 0
Al buscar: JOSE, encuentro: JOSE, colisiones: 1
Al buscar: OLGA, encuentro: OLGA, colisiones: 2
Al buscar: ROSA, encuentro: ROSA, colisiones: 2
Al buscar: IVAN, encuentro: IVAN, colisiones: 3
No encuentro: CARLOS, colisiones: 5
***TABLA CERRADA DOBLE
 */

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

//Comprobar empíricamente la complejidad O(n) del algoritmo que se utiliza para buscar
