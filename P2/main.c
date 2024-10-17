#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>


//Cabeceras de las funciones
double microsegundos();
void imprimirTiempos(void (*func)(int[], int), const int *valores, const char *tipoOrden, int tipoAlgoritmo);
double medirTiempo(void (*func)(int[], int), int v[], int n, bool *promedio, int k);
void inicializar_semilla();
void aleatorio(int v [], int n);
void ascendente(int v [], int n);
void descendente(int v [], int n);
void ord_ins(int v[], int n);
void ord_rap_aux (int v [], int iz, int dr);
void ord_rap (int v [], int n);
double cotaAjustada(int n, int tipo);
double cotaSubestimada(int n, int tipo);
double cotaSobreestimada(int n, int tipo);
void testInsercion(int *v, int tamaño, const char *tipoInicializacion);
void testRapida(int *v, int tamaño, const char *tipoInicializacion);
void test();
bool estaOrdenado(int v[], int n);
void imprimirArray(int v[], int n);
void imprimirTablas();


//Realiza las pruebas de los algoritmos Fibonacci y muestra las tablas de tiempos
int main(void) {
    inicializar_semilla();
    test();
    imprimirTablas();
    return 0;
}

//Obtiene la hora actual en microsegundos
double microsegundos() {
    struct timeval t;
    if (gettimeofday(&t, NULL) < 0)
        return 0.0; //Maneja el error en la obtención del tiempo
    return (t.tv_usec + t.tv_sec * 1000000.0);
}

double medirTiempo(void (*func)(int[], int), int v[], int n, bool *promedio, int k) {
    double t1, t2, t;
    int j;
    t1 = microsegundos();
    func(v, n);
    t2 = microsegundos();
    t = t2 - t1;

    if (t < 500) {
        t1 = microsegundos();
        for (j = 0; j < k; j++) {
            func(v, n);
        }
        t2 = microsegundos();
        t = (t2 - t1) / k;
        *promedio = true;
    }
    return t;
}

void inicializar_semilla() {
    srand(time(NULL));
}
void aleatorio(int v [], int n) {/* se generan números pseudoaleatorio entre -n y +n */
    int i, m=2*n+1;
    for (i=0; i < n; i++)
        v[i] = (rand() % m) - n;
}
void ascendente(int v [], int n) {
    int i;
    for (i=0; i < n; i++)
        v[i] = i;
}

void descendente(int v [], int n) {
    int i;
    for (i=0; i < n; i++)
        v[i] = n-i;
}

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

void ord_rap_aux (int v [], int iz, int dr) {
    int i, j, x, y;
    i = iz;
    j = dr;
    x = v[(iz+dr)/2];
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
void ord_rap (int v [], int n) {
    ord_rap_aux(v, 0, n-1);
}

// Devuelve la cota subestimada para el algoritmo correspondiente
double cotaSubestimada(int n, int tipo) {
    switch (tipo) {
        case 1: // Para la ordenación por inserción
            return pow(n, 2); // O(n^2)
        case 2: // Para la ordenación rápida (QuickSort)
            return n * log(n); // O(n log n)
        default:
            return 1.0; // Valor por defecto
    }
}

// Devuelve la cota ajustada para el algoritmo correspondiente
double cotaAjustada(int n, int tipo) {
    switch (tipo) {
        case 1: // Para la ordenación por inserción
            return pow(n, 1.8); // O(n^1.8)
        case 2: // Para la ordenación rápida (QuickSort)
            return pow(n, 2); // O(n^2)
        default:
            return 1.0; // Valor por defecto
    }
}

// Devuelve la cota sobreestimada para el algoritmo correspondiente
double cotaSobreestimada(int n, int tipo) {
    switch (tipo) {
        case 1: // Para la ordenación por inserción
            return pow(n, 2.2); // O(n^2.2)
        case 2: // Para la ordenación rápida (QuickSort)
            return pow(n, 2); // O(n^2)
        default:
            return 1.0; // Valor por defecto
    }
}

// Función para verificar si un array está ordenado
bool estaOrdenado(int v[], int n) {
    int i;
    for (i = 1; i < n; i++) {
        if (v[i - 1] > v[i]) {
            return false;
        }
    }
    return true;
}

// Función para imprimir un array
void imprimirArray(int v[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        printf("%d", v[i]);
        if (i < n - 1) {
            printf(", ");
        }
    }
    printf("\n");
}

// Función auxiliar para probar ordenación rápida (QuickSort)
void testRapida(int *v, int tamaño, const char *tipoInicializacion) {
    printf("Ordenacion rapida con inicializacion %s\n", tipoInicializacion);

    // Inicializa el array según el tipo
    if (strcmp(tipoInicializacion, "aleatoria") == 0) {
        aleatorio(v, tamaño);
    } else if (strcmp(tipoInicializacion, "ascendente") == 0) {
        ascendente(v, tamaño);
    } else if (strcmp(tipoInicializacion, "descendente") == 0) {
        descendente(v, tamaño);
    }

    // Imprime el array inicial
    imprimirArray(v, tamaño);
    printf("ordenado? %d\n", estaOrdenado(v, tamaño));
    printf("ordenando...\n");

    // Ordena el array
    ord_rap(v, tamaño);

    // Imprime el array después de ordenar
    imprimirArray(v, tamaño);
    printf("ordenado? %d\n\n", estaOrdenado(v, tamaño));
}

// Función auxiliar para probar ordenación por inserción
void testInsercion(int *v, int tamaño, const char *tipoInicializacion) {
    printf("Ordenacion por insercion con inicializacion %s\n", tipoInicializacion);

    // Inicializa el array según el tipo
    if (strcmp(tipoInicializacion, "aleatoria") == 0) {
        aleatorio(v, tamaño);
    } else if (strcmp(tipoInicializacion, "ascendente") == 0) {
        ascendente(v, tamaño);
    } else if (strcmp(tipoInicializacion, "descendente") == 0) {
        descendente(v, tamaño);
    }

    // Imprime el array inicial
    imprimirArray(v, tamaño);
    printf("ordenado? %d\n", estaOrdenado(v, tamaño));
    printf("ordenando...\n");

    // Ordena el array
    ord_ins(v, tamaño);

    // Imprime el array después de ordenar
    imprimirArray(v, tamaño);
    printf("ordenado? %d\n\n", estaOrdenado(v, tamaño));
}

// Función de prueba para los algoritmos de ordenación
void test() {
    int v[17];  // Vector de tamaño 17
    int v2[10]; // Vector de tamaño 10

    // Pruebas con ordenación por inserción
    testInsercion(v, 17, "aleatoria");
    testInsercion(v2, 10, "ascendente");
    testInsercion(v2, 10, "descendente");

    // Pruebas con ordenación rápida (QuickSort)
    testRapida(v, 17, "aleatoria");
    testRapida(v2, 10, "ascendente");
    testRapida(v2, 10, "descendente");
}

// Imprime los tiempos de ejecución y las cotas de complejidad
void imprimirTiempos(void (*func)(int[], int), const int *valores, const char *tipoOrden, int tipoAlgoritmo) {
    int v[32000];
    int i, n;
    double t, x, y, z;
    bool promedio;

    // Encabezado de la tabla
    printf("\nOrdenación %s con inicialización %s\n", tipoAlgoritmo == 1 ? "por inserción" : "rápida (QuickSort)", tipoOrden);
    printf("\n%13s%16s%20s%17s%20s\n", "n", "t(n)", "t(n)/n^1.8", "t(n)/n^2", "t(n)/n^2.2");

    for (i = 0; i < 6; i++) {
        n = valores[i];
        promedio = false;

        //Inicializa el vector de acuerdo con el tipo de orden
        if (strcmp(tipoOrden, "Ascendente") == 0) {
            ascendente(v, n);
        } else if (strcmp(tipoOrden, "Descendente") == 0) {
            descendente(v, n);
        } else if (strcmp(tipoOrden, "Aleatorio") == 0) {
            aleatorio(v, n);
        }

        t = medirTiempo(func, v, n, &promedio, 1000);

        // Cálculo de las cotas de complejidad
        x = t / cotaSubestimada(n, tipoAlgoritmo);
        y = t / cotaAjustada(n, tipoAlgoritmo);
        z = t / cotaSobreestimada(n, tipoAlgoritmo);

        // Imprimir fila de la tabla
        if (promedio) {
            printf("*");
        } else {
            printf(" ");
        }
        printf("%12d%17.4f%18.6f%18.6f%20.8f\n", n, t, x, y, z);
    }
}

// Imprime las tablas de tiempos para los diferentes algoritmos de ordenación
void imprimirTablas() {
    int valores[6] = {100, 200, 400, 800, 1600, 3200};


    //Ordenación por Inserción
    imprimirTiempos(ord_ins, valores, "Ascendente", 1);
    imprimirTiempos(ord_ins, valores, "Descendente", 1);
    imprimirTiempos(ord_ins, valores, "Aleatorio", 1);

    //Ordenación rápida
    imprimirTiempos(ord_rap, valores, "Ascendente", 2);
    imprimirTiempos(ord_rap, valores, "Descendente", 2);
    imprimirTiempos(ord_rap, valores, "Aleatorio", 2);
}


