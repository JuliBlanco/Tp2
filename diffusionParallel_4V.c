#include "common.h"


//Realizamos la escritura de las instrucciones que ejecutará el primer hilo que crearemos en un futuro, 
//al cual le decimos que realice la difusion del calor (sacar el promedio) comenzando en la fila 0 hasta el 
//final de estas, esta instruccion coincidirá con la de todos los hilos ya que la distribución es vertical, así
//que necesariamente todos los hilos deben recorrer todas las filas.
//La instruccion que le dimos con respecto a las columnas es que comience en la columna 0 (en realidad comienza en
//la 1) y realice la difusion hasta llegar a un cuarto de las columnas totales. El +1 lo pusimos para que luego, 
//el siguiente hilo pise la ultima columna de este hilo.
static void* diff1(void* arg){
    cicloDiffusion(0, MAXCOLUMNAS/4+1, 0, MAXFILAS);
    return 0;
}


//Realizamos la escritura de las instrucciones que ejecutará el segundo hilo que crearemos en un futuro.
//Como ya dijimos, al igual que los demas hilos, este recorerá desde la fila 0 hasta el final de las filas.
//Las columnas las recorrerá desde donde comienza el 2do cuarto y llegará hasta la mitad de las columnas totales.
//En los parametros escribimos un mas dos, porque al ser un hilo del centro, queremos que pise
//sus columnas exteriores con la de sus hilos limitrofes (como tiene 2, uno a la derecha y otro a la izquierda, le 
//sumamos 2) 
static void* diff2(void* arg){
    cicloDiffusion(MAXCOLUMNAS/4-1, MAXCOLUMNAS/4+2, 0, MAXFILAS);
    return 0;
}


//Realizamos la escritura de las instrucciones que ejecutará el tercer hilo que crearemos en un futuro.
//Hará la difusion de calor desde la fila 0 hasta el final de ellas.
//La difusion de calor la hará desde la mitad total de las columnas hasta terminar el tercer cuarto. Como
//queremos que se pase uno por derecha y otro por izquierda, le sumamos 2 al paramentro de ycount. Y asi 
//se pisará una columna con el hilo de la derecha de este y otra con el de la izquierda.
static void* diff3(void* arg){
    cicloDiffusion(MAXCOLUMNAS/2-1, MAXCOLUMNAS/4+2, 0, MAXFILAS);
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
        //Recorrerá todas las filas para hacer la difusion de calor e irá desde la columna 
        //donde comienza el cuarto cuarto y recorrerá hasta el final de las columnas.
        //Escribimos un + 1 para que pise la ultima fila del hilo anterior
        cicloDiffusion((MAXCOLUMNAS/4)*3-1, MAXCOLUMNAS/4+1, 0, MAXFILAS);

        //Realizamos los joins que lo que nos indican que la funcion no se detiene hasta que tanto el thread 1,
        //como el 2 y el 3 acaben su ejecucion.
        pthread_join(thread1, NULL);
        pthread_join(thread2, NULL);
        pthread_join(thread3, NULL);
    }

    // (3) Envio resultado a archivo
    imprimirArchivo("resultadoParallel_4V.txt");

    return 0;
}
