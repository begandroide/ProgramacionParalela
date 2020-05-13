#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "grid.h"
#include "life.h"

typedef struct arg_struct {
    int arg1;
    int arg2;
    int arg3;
} CustomArgs;

// global
int g = 0;

void *threadFun(void* argTh){
    CustomArgs *num = (CustomArgs*)argTh;
    // int *num = (int*)argTh;
    static int s = 0;
    ++s;++g;
    printf("Thread ID:%d, arg2: %d, Global: %d\n",num->arg1,num->arg2,num->arg3);
    
    pthread_exit(NULL);
}

int main(int argc, char **argv){
    if (argc != 5) {
		fprintf(stderr, "Usage: ./runpar <filename> <numgens> <N> <M>\n");
		exit(1);
	}

	const char *filename = argv[1];
	int numgens = atoi(argv[2]);
	int N = atoi(argv[3]);  // number of rows of processes
	int M = atoi(argv[4]);  // number of columns of processes
    
    int numbThreads = N*M;
    
    // read file
	FILE *fp = fopen(filename, "r");
	Grid *grid = life_load_board(fp);
	fclose(fp);

	// simulation
	for (int i = 0; i < numgens; i++) {
		life_compute_next_gen(grid);
		grid_flip(grid);
	}
    int i;
    pthread_t tid[numbThreads];
    for(int i = 0; i < numbThreads; i++){
        CustomArgs *arrfs = (CustomArgs*)malloc(sizeof(CustomArgs));
        arrfs->arg1 = i;
        arrfs->arg2 = grid->cols/M;
        arrfs->arg3 = grid->rows/N;
        // printf("Creando thread -> %d",i);
        pthread_create(&tid[i],NULL,threadFun,(void*)arrfs);
    }
    for(int i = 0; i < numbThreads; i++){
        pthread_join(tid[i],NULL);
    }     

	// print result
	life_save_board(stdout, grid);
    return 0;
}