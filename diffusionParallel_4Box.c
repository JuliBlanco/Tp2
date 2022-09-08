#include "common.h"


//En esta parte del codigo damos las instrucciones que deberá ejecutar el 1er hilo, al cual le decimos que
//realice la difusion del calor (sacar el promedio) comenzando en la columna 0 hasta la mitad de las columnas
//totales (en realidad comienza en la columna 1). El + 1 lo indicamos porque queremos que se pase una columna 
//a la hora de realizar la difusion de calor. 
//Le indicamos lo mismo para las filas: desde la fila 0 (en realidad 1) hasta la mitad de las filas totales +1, 
//para luego tambien pasarnos en 1 y lograr que se pise con otro hilo.
static void* diff1(void* arg){
    cicloDiffusion(0, MAXCOLUMNAS/2+1, 0, MAXFILAS/2+1);
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

        //Aqui creamos otro hilo cuyo PID se guardará en la direccion de thread1, sin atributo y direccionandose 
        //a la rutina descripta por fuera del main (diff1).
        //A este hilo le dimos las intrucciones de realizar el ciclo difusion del calor desde 
        //la mitad de las columnas totales (para que pueda pisar una columna que haya hecho el hilo anterior)
        //hasta el final de las columnas
        //Las instrucciones para las filas era: que vaya desde la fila 0, en realidad la 1, hasta la mitad total
        //de filas +1 para lograr que en un futuro una fila sea pisada por otro hilo 
        pthread_create(&thread1, NULL, diff1, NULL);
        cicloDiffusion(MAXCOLUMNAS/2-1, MAXCOLUMNAS/2+1, 0, MAXFILAS/2+1);

        //Aqui le decimos al hilo que espere que termine de ejecutarse para seguir con las proximas instrucciones
        pthread_join(thread1, NULL);

        //Aqui creamos otro hilo cuyo PID se guardará en la direccion de thread2, sin atributo y direccionandose 
        //a la rutina descripta por fuera del main (diff1).
        //A este hilo le dimos las instrucciones de realizar la difusion se calor desde la columna 0 hasta la
        //mitad total de las columnas mas 1 para que luego se pise con el otro thread.
        //Las filas iran desde la mitad total de las filas hasta el final. Se pisaran la 1er fila de este hilo
        //con la ultima del hilo 0, cuyas instrucciones estan por fuera del main. 
        pthread_create(&thread2, NULL, diff1, NULL);
        cicloDiffusion(0, MAXCOLUMNAS/2+1, MAXFILAS/2-1, MAXFILAS/2+1);

        //El hilo espera a que terminen para seguir con las proximas instrucciones
        pthread_join(thread2, NULL);

        //Creamos un ultimo hilo con las caracteristicas de los anteriores pero ahora el PID apunta a la direccion
        //de thread3.
        //Este hilo comenzará la difusion del calor desde la mitad de las columnas y de las filas hasta el final 
        //de ambas dos; haciendo que se pisen en los bordes internos de este cuadrante, con otros hilos. Especifi-
        //camente el hilo 1 y el 2
        pthread_create(&thread3, NULL, diff1, NULL);
        cicloDiffusion(MAXCOLUMNAS/2-1, MAXCOLUMNAS/2+1, MAXFILAS/2-1, MAXFILAS/2+1);

        //Espera a que el hilo 3 termine su ejecucion para acabar el ciclo
        pthread_join(thread3, NULL);

    }

    // (3) Envio resultado a archivo
    imprimirArchivo("resultadoParallel_4Box.txt");

    return 0;
}