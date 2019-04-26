#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

long size = 0;
int number_threads = 1;
long *arreglo;

void* func(void* i){
        const int ii = *(int*)&i;
        long job_quantity = size / number_threads;
        long init = (ii) * job_quantity;
        long fin = init + job_quantity -1;
        printf("Thread %i, desde -> %lld, hasta ->%lld\n",ii,init,fin);
        for(long j =  init; j <= fin;j++){
                arreglo[j] = j;
        }
}

int main(int argc, char const *argv[])
{
        //primer argumento es tamaño del arreglo
        //segundo argumento es cantidad de threads
        if(argv[1] != NULL){
                size = strtol( argv[1],NULL,10);
                printf("Size of Array: %ld\n",size);
        }
        if(argv[2] != NULL){
                number_threads = strtol(argv[2],NULL,10);
                printf("Number of Threads used: %d\n",number_threads);
        }

        pthread_t *threads = malloc(sizeof(pthread_t)*number_threads);
        arreglo = (long*)malloc(sizeof(long)*size);
        if(arreglo == NULL) {
                printf("Too much Memory\n");
                return -1;
        }
        for(int i = 0; i < number_threads; i++){
                pthread_create(&threads[i],NULL,func, (void*)i);
        }
        for(int i = 0; i < number_threads; i++) pthread_join(threads[i], NULL);
        // for(int i = 0; i < size; i++) printf("%d,",arreglo[i]);
        free(arreglo);
        return 0;
}
