#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "grid.h"
#include "life.h"
#include <errno.h>
#include <stdbool.h>
#include <string.h>

typedef struct arg_struct {
    int id;
    int initialIndexRow;
    int finishIndexRow;
    int initialIndexCol;
    int finishIndexCol;
} CustomArgs;

// global
static int numGens = 0;
static Grid* grid = NULL;
pthread_mutex_t *mutexes;

void *threadFun(void* argTh){
    CustomArgs *num = (CustomArgs*)argTh;

    int numberRows = num->finishIndexRow - num->initialIndexRow;
    int numberCols = num->finishIndexCol - num->initialIndexCol;
    bool hasLeftNeighbor = false;
    bool hasRightNeighbor = false;
    
    //saber si el thread tiene vecinos
    if (num->initialIndexCol == 0){
        //no tiene vecinos izquierdos
        if (num->finishIndexCol < grid->cols){
            // tiene vecinos derechos
            numberCols++;
            hasRightNeighbor = true;
        }
    } else {
        //tiene vecino izquierdo
        numberCols++; //OJO tiene q ser a la izq.
        hasLeftNeighbor = true;
        num->initialIndexCol--;
    }
    // TODO
    // if (num->initialIndexRow == 0){
    //     //no tiene vecinos arriba
    // } else {
    //     //tiene vecino arriba
    // }

    Grid* gridThread = grid_alloc(numberRows, numberCols); // sumamos padding en col
    // setear valores a grilla
    for(int i = num->initialIndexRow; i < num->initialIndexRow + numberRows; i++){
        for(int j = num->initialIndexCol; j < ( num->initialIndexCol + numberCols); j++){
            int tmpIndexJ = j - num->initialIndexCol;
            uint8_t val = grid_get_current(grid, i, j);
            if(strncmp((const char*)&(val), "", 1) == 0){
                val = (uint8_t)'0';
                printf("WARNING val null\n",val);
            }

            grid_set_current(gridThread, i, tmpIndexJ, val);
        }
    }

    printf("Grilla de thread %d, GENERACIÓN 0:\n",num->id);

	life_save_board(stdout, gridThread);

    life_compute_next_gen(gridThread);
    grid_flip(gridThread);
    
    // life_save_board(stdout, gridThread);

    for(int z = 0; z < numGens; z++){

        if(hasRightNeighbor){
            // ESPERA
            //necesito saber el valor de su primera columna, del proceso a la derecha
            //Espero por su valor
            for (int i = 0; i < numberRows; i++)
            {
                int retVal = pthread_mutex_lock(&mutexes[(numberCols) ]);
                grid_set_current(grid, i, numberCols - 2,gridThread->buf1[ (i+1)*numberCols - 1]);
                // life_save_board(stdout, grid);
                if (retVal == 0) {
                    life_compute_next_gen(gridThread);
                    pthread_mutex_unlock(&mutexes[(numberCols)]);
                }
            }
        }
        // Syncro
        if(hasLeftNeighbor){
            //necesito saber el valor de su ultima columna, del proceso a la izquierda
            for (int i = 0; i < numberRows; i++)
            {
                int retVal = pthread_mutex_lock(&mutexes[((i+1)*numberCols)]);
                grid_set_current(grid, 0, numberCols - 2,gridThread->buf1[ (i+1)*numberCols - 1]);
                if (retVal == 0) {
                    life_compute_next_gen(gridThread);
                    pthread_mutex_unlock(&mutexes[(i+1)*numberCols]);
                }
            }
        }

    }
    
    printf("ASI QUEDA: id: {%d}\n", num->id);
    //setear valores a grilla general y salir
    life_save_board(stdout, gridThread);
    
    pthread_exit(NULL);
}

int main(int argc, char **argv){
    if (argc != 5) {
		fprintf(stderr, "Usage: ./runpar <filename> <numgens> <N> <M>\n");
		exit(1);
	}


	const char *filename = argv[1];
	numGens = atoi(argv[2]);
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
        
        actualCol = tmpFinishIndexCol;
    }
    for(int i = 0; i < numbThreads; i++) pthread_join(tid[i],NULL);

    return 0;
}