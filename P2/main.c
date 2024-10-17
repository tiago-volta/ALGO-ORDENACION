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
void imprimirTablas();


//Realiza las pruebas de los algoritmos Fibonacci y muestra las tablas de tiempos
int main(void) {
    inicializar_semilla();
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

/*
//Devuelve la cota subestimada para el algoritmo correspondiente
double cotaSubestimada(int n, int tipo) {
    switch (tipo) {
        case 1: //Para fib1
            return pow(1.1, n);
        case 2: //Para fib2
            return pow(n, 0.8);
        case 3: //Para fib3
            return sqrt(log(n));
        default:
            return 1.0; //Valor por defecto
    }
}

//Devuelve la cota ajustada para el algoritmo correspondiente
double cotaAjustada(int n, int tipo) {
    switch (tipo) {
        case 1: // Para fib1
            return pow((1 + sqrt(5)) / 2, n);
        case 2: // Para fib2
            return n;
        case 3: // Para fib3
            return log(n);
        default:
            return 1.0; //Valor por defecto
    }
}

//Devuelve la cota sobreestimada para el algoritmo correspondiente
double cotaSobreestimada(int n, int tipo) {
    switch (tipo) {
        case 1: //Para fib1
            return pow(2, n);
        case 2: //Para fib2
            return n * log(n);
        case 3: //Para fib3
            return pow(n, 0.5);
        default:
            return 1.0; //Valor por defecto
    }
}
*/
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

// Imprime los tiempos de ejecución y las cotas de complejidad
void imprimirTiempos(void (*func)(int[], int), const int *valores, const char *tipoOrden, int tipoAlgoritmo) {
    int v[32000];
    double t, x, y, z;
    bool promedio;

    // Encabezado de la tabla
    printf("\nOrdenación %s con inicialización %s\n", tipoAlgoritmo == 1 ? "por inserción" : "rápida (QuickSort)", tipoOrden);
    printf("%-8s%15s%15s%15s%15s\n", "n", "t(n)", "t(n)/n^1.8", "t(n)/n^2", "t(n)/n^2.2");

    for (int i = 0; i < 6; i++) {
        int n = valores[i];
        promedio = false;

        // Inicializa el vector de acuerdo con el tipo de orden
        if (strcmp(tipoOrden, "Ascendente") == 0) {
            ascendente(v, n);
        } else if (strcmp(tipoOrden, "Descendente") == 0) {
            descendente(v, n);
        } else if (strcmp(tipoOrden, "Aleatorio") == 0) {
            aleatorio(v, n);
        }

        t = medirTiempo(func, v, n, &promedio, 1000);

        // Cálculo de las cotas de complejidad
        x = t / pow(n, 1.8);
        y = t / pow(n, 2);
        z = t / pow(n, 2.2);

        // Imprimir fila de la tabla
        if (promedio)
            printf("*");
        printf("%-8d%15.2f%15.6f%15.6f%15.6f\n", n, t, x, y, z);
    }
}

// Imprime las tablas de tiempos para los diferentes algoritmos de ordenación
void imprimirTablas() {
    int valores[6] = {500, 1000, 2000, 4000, 8000, 16000};

    // Ordenación por Inserción
    imprimirTiempos(ord_ins, valores, "Ascendente", 1);
    imprimirTiempos(ord_ins, valores, "Descendente", 1);
    imprimirTiempos(ord_ins, valores, "Aleatorio", 1);

    // Ordenación rápida
    imprimirTiempos(ord_rap, valores, "Ascendente", 2);
    imprimirTiempos(ord_rap, valores, "Descendente", 2);
    imprimirTiempos(ord_rap, valores, "Aleatorio", 2);
}

