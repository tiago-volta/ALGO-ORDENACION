#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <stdbool.h>
#include <math.h>
// #include <string.h>

// Definición de la estructura para las cotas
typedef struct {
    double cotaSubestimada;
    double cotaAjustada;
    double cotaSobreestimada;
    char strCotaSubestimada[10];
    char strCotaAjustada[10];
    char strCotaSobreestimada[10];
} Cotas;

// Cabeceras de las funciones
void test();
double microsegundos();
void imprimirTiempos(void (*algoritmo)(int[], int), void (*inicialización)(int*, int), Cotas *cotas);
void imprimirTablas();
double medirTiempo(void (*func)(int*, int), void (*inicialización)(int*, int), const int n, int v[], bool *promedio);
void inicializar_semilla();

void aleatorio(int v[], int n);
void ascendente(int v[], int n);
void descendente(int v[], int n);
void ord_ins(int v[], int n);
void ord_rap(int v[], int n);
int esta_ordenado(int v[], int n);
void imprimir_array(int v[], int n);
void imprimirEncabezado(const char *nombreAlgoritmo, const char *initType, Cotas *cotas);
void imprimirFila(int n, double t, double x, double y, double z, bool promedio);

// Realiza las pruebas de los algoritmos de ordenación y muestra las tablas de tiempos
int main(void) {
    inicializar_semilla();
    imprimirTablas();
    return 0;
}

// Algoritmo de ordenación por inserción
void ord_ins(int v[], int n) {
    int x, j, i;
    for (i = 1; i < n; i++) {
        x = v[i];
        j = i - 1;
        while (j >= 0 && v[j] > x) {
            v[j + 1] = v[j];
            j--;
        }
        v[j + 1] = x;
    }
}

// Algoritmo auxiliar de ordenación rápida
void ord_rap_aux(int v[], int iz, int dr) {
    int i, j, x, y;
    i = iz;
    j = dr;
    x = v[(iz + dr) / 2];
    do {
        while (v[i] < x && i < dr) i++;
        while (x < v[j] && j > iz) j--;
        if (i <= j) {
            y = v[i];
            v[i] = v[j];
            v[j] = y;
            i++;
            j--;
        }
    } while (i <= j);
    if (iz < j) ord_rap_aux(v, iz, j);
    if (i < dr) ord_rap_aux(v, i, dr);
}

// Algoritmo de ordenación rápida
void ord_rap(int v[], int n) {
    ord_rap_aux(v, 0, n - 1);
}

// Inicializa la semilla para la generación de números aleatorios
void inicializar_semilla() {
    srand(time(NULL));
}

// Inicializa el array con valores aleatorios
void aleatorio(int v[], int n) {
    int i, m = 2 * n + 1;
    for (i = 0; i < n; i++)
        v[i] = (rand() % m) - n;
}

// Inicializa el array en orden ascendente
void ascendente(int v[], int n) {
    int i;
    for (i = 0; i < n; i++)
        v[i] = i;
}

// Inicializa el array en orden descendente
void descendente(int v[], int n) {
    int i;
    for (i = 0; i < n; i++)
        v[i] = n - i;
}

// Verifica si el array está ordenado
int esta_ordenado(int v[], int n) {
    int i;
    for (i = 0; i < n - 1; i++) {
        if (v[i] > v[i + 1]) {
            return 0;
        }
    }
    return 1;
}

// Imprime un array
void imprimir_array(int v[], int n) {
    for (int i = 0; i < n; i++) {
        if (i == n - 1) {
            printf("%d", v[i]);  // Último elemento sin coma
        } else {
            printf("%d, ", v[i]);  // Elementos anteriores con coma
        }
    }
    printf("\n");
}

// Obtiene la hora actual en microsegundos
double microsegundos() {
    struct timeval t;
    if (gettimeofday(&t, NULL) < 0)
        return 0.0;
    return (t.tv_usec + t.tv_sec * 1000000.0);
}

// Mide el tiempo de ejecución de una función
double medirTiempo(void (*func)(int*, int), void (*inicialización)(int*, int), const int n, int v[], bool *promedio) {
    double t1, t2, t;
    int j, k = 1000;

    inicialización(v, n);

    t1 = microsegundos();
    func(v, n);
    t2 = microsegundos();
    t = t2 - t1;

    if (t < 500) {
        t1 = microsegundos();
        for (j = 0; j < k; j++) {
            inicialización(v, n);
            func(v, n);
        }
        t2 = microsegundos();
        t = (t2 - t1) / k;
        *promedio = true;
    }
    return t;
}

// Función para imprimir el encabezado de las tablas
void imprimirEncabezado(const char *nombreAlgoritmo, const char *initType, Cotas *cotas) {
    printf("\nOrdenación %s con inicialización %s\n", nombreAlgoritmo, initType);
    printf("\n%13s%16s%20s%17s%20s\n", "n", "t(n)", cotas->strCotaSubestimada, cotas->strCotaAjustada,
        cotas->strCotaSobreestimada);
}

// Función para imprimir una fila de la tabla
void imprimirFila(int n, double t, double x, double y, double z, bool promedio) {
    if (promedio) {
        printf("*");
    } else {
        printf(" ");
    }
    printf("%12d%17.4f%18.6f%18.6f%20.6f\n", n, t, x, y, z);
}

// Imprime los tiempos de ejecución de la función dada usando el struct de cotas
void imprimirTiempos(void (*algoritmo)(int[], int), void (*inicialización)(int*, int), Cotas *cotas) {
    int i, v[32000];
    double t, x, y, z;
    bool promedio;

    for (i = 500; i <= 32000; i *= 2) {
        promedio = false;
        t = medirTiempo(algoritmo, inicialización, i, v, &promedio);
        x = t / pow(i, cotas->cotaSubestimada);
        y = t / pow(i, cotas->cotaAjustada);
        z = t / pow(i, cotas->cotaSobreestimada);

        imprimirFila(i, t, x, y, z, promedio);
    }
}

// Imprime las tablas de tiempos para los diferentes algoritmos de ordenación
void imprimirTablas() {
    Cotas cotas;

    // Ordenación por Inserción con inicialización ascendente
    cotas = (Cotas){0.75, 0.95, 1.15, "n^0.75",
        "n^0.95", "n^1.15"};
    imprimirEncabezado("por inserción", "ascendente", &cotas);
    imprimirTiempos(ord_ins, ascendente, &cotas);

    // Ordenación por Inserción con inicialización descendente
    cotas = (Cotas){1.8, 2.0, 2.2, "n^1.8",
        "n^2.0", "n^2.2"};
    imprimirEncabezado("por inserción", "descendente", &cotas);
    imprimirTiempos(ord_ins, descendente, &cotas);

    // Ordenación por Inserción con inicialización aleatoria
    cotas = (Cotas){1.8, 2.0, 2.2, "n^1.8",
        "n^2.0", "n^2.2"};
    imprimirEncabezado("por inserción", "aleatoria", &cotas);
    imprimirTiempos(ord_ins, aleatorio, &cotas);

    // Ordenación rápida con inicialización ascendente
    cotas = (Cotas){0.9, 1.1, 1.3, "n^0.9",
        "n^1.1", "n^1.3"};
    imprimirEncabezado("rápida", "ascendente", &cotas);
    imprimirTiempos(ord_rap, ascendente, &cotas);

    // Ordenación rápida con inicialización descendente
    cotas = (Cotas){0.87, 1.136, 1.27, "n^0.87",
        "n^1.136", "n^1.27"};
    imprimirEncabezado("rápida", "descendente", &cotas);
    imprimirTiempos(ord_rap, descendente, &cotas);

    // Ordenación rápida con inicialización aleatoria
    cotas = (Cotas){0.84, 1.04, 1.24, "n^0.84",
        "n^1.04", "n^1.24"};
    imprimirEncabezado("rápida", "aleatoria", &cotas);
    imprimirTiempos(ord_rap, aleatorio, &cotas);
}
