#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

//Cabeceras de las funciones
void test();
double microsegundos();
void imprimirTiempos(void (*algoritmo)(int[],int), void (*inicialización)(int*,int) , double cotaSubestimada,double cotaAjustada,double cotaSobreestimada);
void imprimirTablas();
double medirTiempo(void (*func)(int*,int), const int n,int v[], bool *promedio);
void inicializar_semilla();

void aleatorio(int v[], int n);
void ascendente(int v [], int n);
void descendente(int v [], int n);
void ord_ins(int v[], int n);
void ord_rap (int v [], int n);
int esta_ordenado(int v[], int n);
void imprimir_array(int v[], int n);


//Realiza las pruebas de los algoritmos Fibonacci y muestra las tablas de tiempos
int main(void) {
    //test();
    imprimirTablas();
    return 0;
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

void inicializar_semilla() {
    srand(time(NULL));
}

void aleatorio(int v[], int n) {/* se generan números pseudoaleatorio entre -n y +n */
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

// Verifica si el array esta ordenado
int esta_ordenado(int v[], int n) {
    int i;
    for (i = 0; i < n - 1; i++) {
        if (v[i] > v[i + 1]) {
            return 0;
        }
    }
    return 1;
}

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


void ordenacion(void (*inicialización)(int*,int),void (*algoritmo) (int*,int),int n, int v[n]) {
    inicialización(v, n);
    imprimir_array(v, n);
    printf("ordenado? %d\n", esta_ordenado(v, n));
    printf("ordenando...\n");
    algoritmo(v, n);
    imprimir_array(v, n);
    printf("ordenado? %d\n\n", esta_ordenado(v, n));
}


void imprimir_test(int n, int v[n],void (*algoritmo) (int*,int), char nombre_algoritmo[]) {
    printf("Ordenacion %s con inicializacion aleatoria\n",nombre_algoritmo);
    ordenacion(aleatorio,algoritmo,n, v);
    printf("Ordenacion %s con inicializacion descendente\n",nombre_algoritmo);
    ordenacion(descendente,algoritmo,n,v);
    printf("Ordenacion %s con inicializacion ascendente\n",nombre_algoritmo);
    ordenacion(ascendente,algoritmo,n,v);
}

// Función de prueba que realiza las ordenaciones
void test() {
    inicializar_semilla();
    int n = 17;
    int v[n];
    imprimir_test(n, v, ord_ins, "por inserción");
    imprimir_test(n, v, ord_rap ,"rapida auxiliar");
}

//Obtiene la hora actual en microsegundos
double microsegundos() {
    struct timeval t;
    if (gettimeofday(&t, NULL) < 0)
        return 0.0; //Maneja el error en la obtención del tiempo
    return (t.tv_usec + t.tv_sec * 1000000.0);
}

//Mide el tiempo de ejecución
double medirTiempo(void (*func)(int*,int), const int n,int v[], bool *promedio) {
    double t1, t2, t;
    int j, k = 1000;
    t1 = microsegundos();
    func(v,n);
    t2 = microsegundos();
    t = t2 - t1;

    //Si el tiempo es menor que 500 microsegundos
    if (t < 500) {
        t1 = microsegundos();
        for (j = 0; j < k; j++)
            func(v,n); //Llama a la función varias veces
        t2 = microsegundos();
        t = (t2 - t1) / k;
        *promedio = true; //Indica que se realizó un promedio
    }
    return t;
}


//Imprime los tiempos de ejecución de la función dada
void imprimirTiempos(void (*algoritmo)(int[],int), void (*inicialización)(int*,int) , double cotaSubestimada,double cotaAjustada,double cotaSobreestimada) {
    int i, v[32000];
    double t, x, y, z;
    bool promedio;

    for (i = 500; i <= 32000; i*= 2) {
        promedio = false;   //Indica si se calcula el promedio
        inicialización(v,i);
        t = medirTiempo(algoritmo, i,v, &promedio);
        x = t / pow(i,cotaSubestimada);
        y = t / pow(i,cotaAjustada);
        z = t / pow(i,cotaSobreestimada);

        if (promedio) {
            printf("*"); //Marca si se hizo un promedio
        } else {
            printf(" "); //Marca si no se hizo un promedio, para mantener la alineación
        }
        printf("%12d%17.4f%18.6f%18.6f%20.6f\n", i, t, x, y, z);
    }
}

// Imprime las tablas de tiempos para los diferentes algoritmos de ordenación
void imprimirTablas() {

    //Ordenación por Inserción
    printf("\nOrdenación por inserción con inicialización ascendente\n");
    printf("\n%13s%16s%20s%17s%20s\n", "n", "t(n)", "t(n)/n^0.8", "t(n)/n^1.0", "t(n)/n^1.2");
    imprimirTiempos(ord_ins,  ascendente, 0.8,1.0,1.2);

    printf("\nOrdenación por inserción con inicialización descendente\n");
    printf("\n%13s%16s%20s%17s%20s\n", "n", "t(n)", "t(n)/n^1.8", "t(n)/n^2.0", "t(n)/n^2.2");
    imprimirTiempos(ord_ins,  descendente, 1.8,2.0,2.2);

    printf("\nOrdenación por inserción con inicialización aleatorio\n");
    printf("\n%13s%16s%20s%17s%20s\n", "n", "t(n)", "t(n)/n^1.8", "t(n)/n^2.0", "t(n)/n^2.2");
    imprimirTiempos(ord_ins,  aleatorio, 1.8,2.0,2.2);

    //Ordenación rápida
    printf("\nOrdenación rápida con inicialización ascendente\n");
    printf("\n%13s%16s%20s%17s%20s\n", "n", "t(n)", "t(n)/n^1.0", "t(n)/n^1.15", "t(n)/n^1.3");
    imprimirTiempos(ord_rap,  ascendente, 1.0,1.15,1.3);

    printf("\nOrdenación rápida con inicialización descendente\n");
    printf("\n%13s%16s%20s%17s%20s\n", "n", "t(n)", "t(n)/n^1.0", "t(n)/n^1.15", "t(n)/n^1.3");
    imprimirTiempos(ord_rap,  descendente, 1.0,1.15,1.3);

    printf("\nOrdenación rápida con inicialización aleatoria\n");
    printf("\n%13s%16s%20s%17s%20s\n", "n", "t(n)", "t(n)/n^1.0", "t(n)/n^1.15", "t(n)/n^1.3");
    imprimirTiempos(ord_rap,  aleatorio, 1.0,1.15,1.3);
}


//Si la sucesion decrece sera cota sobreestimada
//Si la sucesión crece sera cota subestimada