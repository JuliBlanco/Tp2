#include "common.h"


//Realizamos la escritura de las instrucciones que ejecutará el primer hilo que crearemos en un futuro, al cual 
//le decimos que realice la difusion del calor (sacar el promedio) comenzando en la columna 0 hasta la mitad de 
//las columnas totales (en realidad comienza en la columna 1). El + 1 lo indicamos porque queremos que se pase una
//columna a la hora de realizar la difusion de calor y se pise con el sigueinte hilo. 
//Le indicamos lo mismo para las filas: desde la fila 0 (en realidad 1) hasta la mitad de las filas totales +1, 
//para luego tambien pasarnos en 1 y lograr que se pise con otro hilo.
static void* diff1(void* arg){
    cicloDiffusion(0, MAXCOLUMNAS/2+1, 0, MAXFILAS/2+1);
    return 0;
}

//Realizamos la escritura de las instrucciones que ejecutará el segundo hilo que crearemos en un futuro.
//A este hilo le dimos las intrucciones de realizar el ciclo difusion del calor desde 
//la mitad de las columnas totales (para que pueda pisar una columna que haya hecho el hilo anterior)
//hasta el final de las columnas.
//Las instrucciones para las filas es: que vaya desde la fila 0, en realidad la 1, hasta la mitad total
//de filas +1 para lograr que en un futuro una fila sea pisada por otro hilo .
static void* diff2(void* arg){
    cicloDiffusion(MAXCOLUMNAS/2-1, MAXCOLUMNAS/2+1, 0, MAXFILAS/2+1);
    return 0;
}


//Realizamos la escritura de las instrucciones que ejecutará el tercer hilo que crearemos en un futuro.
//A este hilo le dimos las instrucciones de realizar la difusion se calor desde la columna 0 hasta la
//mitad total de las columnas mas 1 para que luego se pise con el otro thread.
//Las filas iran desde la mitad total de las filas hasta el final. Se pisarán la 1er fila de este hilo
//con la ultima del hilo 0, cuyas instrucciones estan por fuera del main. 
static void* diff3(void* arg){
    cicloDiffusion(0, MAXCOLUMNAS/2+1, MAXFILAS/2-1, MAXFILAS/2+1);
    return 0;
}

int main() {

    pthread_t thread1, thread2, thread3;
    
    // (1) Inicializo todo en temperatura ambiente
    init();

    // (2) Ecuacion de difusion, promedio de vecinos
    for (int iter = 0; iter < maxIter; iter++) {

        // (2.1) seteo las condiciones de contorno
        fijarCondicionesDeContorno();
        // (2.2) seteo condiciones iniciales
        fijarCondicionesiniciales();

        // (2.3) Hago la difusion en cuatro procesos distintos


        //Creamos los hilos(3) a los cuales se les asigan 1ro un puntero hacia donde se guardará su PID 
        //correspondientemente y tambien les pasamos un puntero a la rutina que deberan ejecutar; cada uno 
        //con su rutina correspondiente
        pthread_create(&thread1, NULL, diff1, NULL);
        pthread_create(&thread2, NULL, diff2, NULL);
        pthread_create(&thread3, NULL, diff3, NULL);

        //Realizamos el ultimo ciclo difusion que lo que hará será: la difusion del calor desde la mitad de las 
        //columnas y de las filas hasta el final de ambas dos; haciendo que se pisen en los bordes internos de
        //este cuadrante, con otros hilos. Especificamente el hilo 1 y el 2
        cicloDiffusion(MAXCOLUMNAS/2-1, MAXCOLUMNAS/2+1, MAXFILAS/2-1, MAXFILAS/2+1);


        //Realizamos los joins que lo que nos indican que la funcion no se detiene hasta que tanto el thread 1,
        //como el 2 y el 3 acaben su ejecucion.
        pthread_join(thread1, NULL);
        pthread_join(thread2, NULL);
        pthread_join(thread3, NULL);

    }

    // (3) Envio resultado a archivo
    imprimirArchivo("resultadoParallel_4Box.txt");

    return 0;
}
