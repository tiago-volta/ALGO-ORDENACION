#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <stdbool.h>
#include <math.h>

//Cabeceras de las funciones
void test();
double microsegundos();
void imprimirTiempos(int (*func)(int), const int * valores, const int tipo, const int repeticiones);
void imprimirTablas();
double cotaAjustada(int n, int tipo);
double medirTiempo(int (*func)(int), const int n, bool *promedio, int k);
double cotaSubestimada(int n, int tipo);
double cotaSobreestimada(int n, int tipo);
void inicializar_semilla();
void aleatorio(int v [], int n);
void ascendente(int v [], int n);
void descendente(int v [], int n);
void ord_ins(int v[], int n);
void ord_rap_aux (int v [], int iz, int dr);
void ord_rap (int v [], int n);


//Realiza las pruebas de los algoritmos Fibonacci y muestra las tablas de tiempos
int main(void) {
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

//Mide el tiempo de ejecución
double medirTiempo(int (*func)(int), const int n, bool *promedio, int k) {
    double t1, t2, t;
    t1 = microsegundos();
    func(n);
    t2 = microsegundos();
    t = t2 - t1;

    //Si el tiempo es menor que 500 microsegundos
    if (t < 500) {
        t1 = microsegundos();
        for (int j = 0; j < k; j++)
            func(n); //Llama a la función varias veces
        t2 = microsegundos();
        t = (t2 - t1) / k;
        *promedio = true; //Indica que se realizó un promedio
    }
    return t;
}
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

// Verifica si un arreglo está ordenado
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
    int i;
    for (i = 0; i < n; i++) {
        printf("%d, ", v[i]);
    }
    printf("\n");
}

// Función de prueba que realiza las ordenaciones y verifica si los arreglos están ordenados
void test() {
    inicializar_semilla();
    int n = 17;
    int v[n];

    printf("Ordenacion por insercion con inicializacion aleatoria\n");
    aleatorio(v, n);
    imprimir_array(v, n);
    printf("ordenado? %d\n", esta_ordenado(v, n));
    printf("ordenando...\n");
    ord_ins(v, n);
    imprimir_array(v, n);
    printf("ordenado? %d\n", esta_ordenado(v, n));

    printf("Ordenacion por insercion con inicializacion descendente\n");
    descendente(v, n);
    imprimir_array(v, n);
    printf("ordenado? %d\n", esta_ordenado(v, n));
    printf("ordenando...\n");
    ord_ins(v, n);
    imprimir_array(v, n);
    printf("ordenado? %d\n", esta_ordenado(v, n));

    printf("Ordenacion por insercion con inicializacion ascendente\n");
    ascendente(v, n);
    imprimir_array(v, n);
    printf("ordenado? %d\n", esta_ordenado(v, n));
    printf("ordenando...\n");
    ord_ins(v, n);
    imprimir_array(v, n);
    printf("ordenado? %d\n", esta_ordenado(v, n));
    
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

//Imprime los tiempos de ejecución de la función dada
void imprimirTiempos(int (*func)(int), const int * valores, const int tipo, const int repeticiones) {
    double t, x, y, z;
    for (int i = 0; i < 5; i++) {
        bool promedio = false; //Indica si se calculó un promedio
        int n = valores[i];
        t = medirTiempo(func, n, &promedio, repeticiones);
        x = t / cotaSubestimada(n, tipo);
        y = t / cotaAjustada(n, tipo);
        z = t / cotaSobreestimada(n, tipo);

        if (promedio) {
            printf("*"); //Marca si se hizo un promedio
        } else {
            printf(" "); //Marca si no se hizo un promedio, para mantener la alineación
        }
        printf("%12d%17.4f%18.6f%18.6f%20.8f\n", n, t, x, y, z);
    }
}

//Imprime las tablas de tiempos para los diferentes algoritmos de Fibonacci
void imprimirTablas() {
    int valoresFib1[5] = {2, 4, 8, 16, 32}; //Valores para el algoritmo 1
    int valoresFib2Fib3[5] = {1000, 10000, 100000, 1000000, 10000000}; //Valores para los algoritmos 2 y 3
    printf("\n%55s\n\n", "Tablas de tiempos Fibonacci");

    //Fibonacci 1
    printf("\nAlgoritmo fibonacci 1\n");
    printf("\n%13s%16s%20s%25s%11s\n", "n", "t(n)", "t(n)/1.1^n", "t(n)/((1+sqrt(5))/2)^n", "t(n)/2^n");
    imprimirTiempos(fib1, valoresFib1, 1, 100000);

    //Fibonacci 2
    printf("\nAlgoritmo fibonacci 2\n");
    printf("\n%12s%17s%20s%16s%22s\n", "n", "t(n)", "t(n)/n^0.8", "t(n)/n", "t(n)/n*log(n)");
    imprimirTiempos(fib2, valoresFib2Fib3, 2, 1000);

    //Fibonacci 3
    printf("\nAlgoritmo fibonacci 3\n");
    printf("\n%12s%17s%23s%16s%18s\n", "n", "t(n)", "t(n)/sqrt(log(n))", "t(n)/log(n)", "t(n)/n^0,5");
    imprimirTiempos(fib3, valoresFib2Fib3 ,3,10000);
}

