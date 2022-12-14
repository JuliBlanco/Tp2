#include "common.h"


static void* diff1(void* arg) {
    cicloDiffusion(0, MAXCOLUMNAS/2+1, 0 ,MAXFILAS);
    return 0;
}

int main() {

    pthread_t thread1;
    
    // (1) Inicializo todo en temperatura ambiente
    init();

    // (2) Ecuacion de difusion, promedio de vecinos
    for (int iter = 0; iter < maxIter; iter++) {

        // (2.1) seteo las condiciones de contorno
        fijarCondicionesDeContorno();
        // (2.2) seteo condiciones iniciales
        fijarCondicionesiniciales();

        // (2.3) Hago la difusion en dos procesos distintos

        pthread_create(&thread1, NULL, diff1, NULL);
        cicloDiffusion(MAXCOLUMNAS/2-1, MAXCOLUMNAS/2+1, 0, MAXFILAS);

        pthread_join(thread1, NULL);
    }

    // (3) Envio resultado a archivo
    imprimirArchivo("resultadoParallel_2V.txt");

    return 0;
}
