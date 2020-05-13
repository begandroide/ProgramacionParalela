#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "grid.h"
#include "life.h"

typedef struct arg_struct {
    int id;
    int initialIndexRow;
    int finishIndexRow;
    int initialIndexCol;
    int finishIndexCol;
} CustomArgs;

// global
int g = 0;
static Grid* grid = NULL;

pthread_mutex_t *mutexes;

void *threadFun(void* argTh){
    CustomArgs *num = (CustomArgs*)argTh;
    printf("{ [ID: %d], %d, %d, %d, %d}\n",
        num->id,num->initialIndexRow,num->finishIndexRow,
        num->initialIndexCol,num->finishIndexCol);
    
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
	grid = life_load_board(fp);
	fclose(fp);

    //inicializar mutexs
    int elem = grid->cols*grid->rows;
    mutexes = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t)*elem);
    for(int ii = 0; ii < elem; ii++){
        pthread_mutex_init(&mutexes[ii], 0);
    }

	// simulation
	for (int i = 0; i < numgens; i++) {
		life_compute_next_gen(grid);
		grid_flip(grid);
	}
    int i;
    pthread_t tid[numbThreads];

    int actualRow = 0;
    int actualCol = 0;
    int loadWorkRow = grid->rows / N;
    int loadWorkCol = grid->cols / M;
    for(int i = 0; i < numbThreads; i++){
        
        int tmpFinishIndexRow = actualRow + loadWorkRow;
        int tmpFinishIndexCol = actualCol + loadWorkCol;

        CustomArgs *arrfs = (CustomArgs*)malloc(sizeof(CustomArgs));
        arrfs->id = i;
        arrfs->initialIndexRow = actualRow;
        arrfs->finishIndexRow = tmpFinishIndexRow;
        arrfs->initialIndexCol = actualCol;
        arrfs->finishIndexCol = tmpFinishIndexCol;
        pthread_create(&tid[i],NULL,threadFun,(void*)arrfs);
        
        // actualRow = tmpFinishIndexRow;
        actualCol = tmpFinishIndexCol;
    }
    for(int i = 0; i < numbThreads; i++) pthread_join(tid[i],NULL);

	// print result
	life_save_board(stdout, grid);
    return 0;
}