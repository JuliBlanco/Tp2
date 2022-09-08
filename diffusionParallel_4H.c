#include "common.h"

//Aclaración: **para clarificar terminos**, dividimos la matriz en 4 partes horizontales. Al referirnos al primer
//cuarto, hacemos referencia al 1ro contando desde arriba hacia abajo. A medida que vamos bajando, 
//cambiamos de cuarto y al siguiente lo llamamos segundo, al siguente tercero y al ultimo de todos cuarto.

//En esta parte del codigo damos las instrucciones que deberá ejecutar el 1er hilo, al cual le decimos que
//realice la difusion del calor (sacar el promedio) comenzando en la columna 0 hasta el final de estas, esta 
//instruccion coincidira con la de todos los hilos ya que la distribución es horizontal, así que necesariamente
//todos los hilos deben recorrer todas las columnas.
//La instruccion que le dimos con respecto a las filas es que comience en la fila 0 (en realidad comienza en la 1)
//y realice la difusion hasta llegar a un cuarto de las filas totales. El +1 lo pusimos para que luego, el
//siguiente hilo pise la ultima fila de este hilo.
static void* diff1(void* arg){
    cicloDiffusion(0, MAXCOLUMNAS, 0, MAXFILAS/4+1);
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

        //Creamos otro hilo cuyo PID se guardará en la direccion de thread1, sin atributo y direccionandose 
        //a la rutina descripta por fuera del main (diff1).
        //Como ya dijimos, al igual que los demas hilos, este recoreá desde la columna 0 hasta el final de las columnas.
        //Las filas las recorrerá desde donde comienza el 2do cuarto y llegara hasta la mitad de las filas totales.
        //En los parametros escribimos un mas dos, porque al ser un termino del centro, queremos que pise
        //sus filas exteriores con la de sus hilos limitrofes (como tiene 2, uno por arriba y otro por abajo, le 
        //sumamos 2) 
        pthread_create(&thread1, NULL, diff1, NULL);
        cicloDiffusion(0, MAXCOLUMNAS, MAXFILAS/4-1, MAXFILAS/4+2);

        //Realizamos un join para que espere a que el thread 1 acabe su ejecucion para seguir.
        pthread_join(thread1, NULL);

        //Aqui creamos otro hilo cuyo PID se guardará en la direccion de thread2, sin atributo y direccionandose 
        //a la rutina descripta por fuera del main (diff1).
        //Hará la difusion de calor desde la columana 0 hasta el final de ellas.
        //La difusion de calor la hará desde la mitad total de las filas hasta terminar el tercer cuarto. Como
        //queremos que se pase uno por arriba y otro por debajo, le sumamos 2 al paramentro de ycount. Y asi 
        //se pisara una fila con el hilo 1 y otra con el 3.
        pthread_create(&thread2, NULL, diff1, NULL);
        cicloDiffusion(0, MAXCOLUMNAS, MAXFILAS/2-1, MAXFILAS/4+2);


        //Reakizamos un join para que espere a que el thread 2 acabe su ejecucion para seguir.
        pthread_join(thread2, NULL);


        //Creamos un ultimo hilo con las caracteristicas de los anteriores pero ahora el PID apunta a la direccion
        //de thread3.
        //Recorrerá todas las columnas para hacer la difusion de calor e irá desde donde comienza el cuarto cuarto
        //y recorrerá hasta el final de las filas. Escribimos un + 1 para que pise la ultima fila del hilo anterior
        pthread_create(&thread3, NULL, diff1, NULL);
        cicloDiffusion(0, MAXCOLUMNAS, (MAXFILAS/4)*3-1, MAXFILAS/4+1);

        //Espera a que el hilo 3 termine su ejecucion para acabar el ciclo
        pthread_join(thread3, NULL);



    }

    // (3) Envio resultado a archivo
    imprimirArchivo("resultadoParallel_4H.txt");

    return 0;
}