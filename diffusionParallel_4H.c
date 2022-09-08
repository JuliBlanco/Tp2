#include "common.h"


//Realizamos la escritura de las instrucciones que ejecutará el primer hilo que crearemos en un futuro, 
//al cual le decimos que realice la difusion del calor (sacar el promedio) comenzando en la columna 0 hasta el 
//final de estas, esta instruccion coincidirá con la de todos los hilos ya que la distribución es horizontal, así
//que necesariamente todos los hilos deben recorrer todas las columnas.
//La instruccion que le dimos con respecto a las filas es que comience en la fila 0 (en realidad comienza en la 1)
//y realice la difusion hasta llegar a un cuarto de las filas totales. El +1 lo pusimos para que luego, el
//siguiente hilo pise la ultima fila de este hilo.
static void* diff1(void* arg){
    cicloDiffusion(0, MAXCOLUMNAS, 0, MAXFILAS/4+1);
    return 0;
}


//Realizamos la escritura de las instrucciones que ejecutará el segundo hilo que crearemos en un futuro.
//Como ya dijimos, al igual que los demas hilos, este recoreá desde la columna 0 hasta el final de las columnas.
//Las filas las recorrerá desde donde comienza el 2do cuarto y llegara hasta la mitad de las filas totales.
//En los parametros escribimos un mas dos, porque al ser un hilo del centro, queremos que pise
//sus filas exteriores con la de sus hilos limitrofes (como tiene 2, uno por arriba y otro por abajo, le 
//sumamos 2) 
static void* diff2(void* arg){
    cicloDiffusion(0, MAXCOLUMNAS, MAXFILAS/4-1, MAXFILAS/4+2);
    return 0;
}


//Realizamos la escritura de las instrucciones que ejecutará el tercer hilo que crearemos en un futuro.
//Hará la difusion de calor desde la columana 0 hasta el final de ellas.
//La difusion de calor la hará desde la mitad total de las filas hasta terminar el tercer cuarto. Como
//queremos que se pase uno por arriba y otro por debajo, le sumamos 2 al paramentro de ycount. Y asi 
//se pisará una fila con el hilo de arriba de este y otra con el de debajo.
static void* diff3(void* arg){
    cicloDiffusion(0, MAXCOLUMNAS, MAXFILAS/2-1, MAXFILAS/4+2);
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

        //Realizamos el ultimo ciclo difusion que:
        //Recorrerá todas las columnas para hacer la difusion de calor e irá desde donde comienza el cuarto cuarto
        //y recorrerá hasta el final de las filas. Escribimos un + 1 para que pise la ultima fila del hilo anterior
        cicloDiffusion(0, MAXCOLUMNAS, (MAXFILAS/4)*3-1, MAXFILAS/4+1);


        //Realizamos los joins que lo que nos indican que la funcion no se detiene hasta que tanto el thread 1,
        //como el 2 y el 3 acaben su ejecucion.
        pthread_join(thread1, NULL);
        pthread_join(thread2, NULL);
        pthread_join(thread3, NULL);


    }

    // (3) Envio resultado a archivo
    imprimirArchivo("resultadoParallel_4H.txt");

    return 0;
}
