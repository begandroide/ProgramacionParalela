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
static Grid* grid = NULL;
pthread_mutex_t *mutexes;

void *threadFun(void* argTh){
    CustomArgs *num = (CustomArgs*)argTh;

    printf("{ [ID: %d], %d, %d, %d, %d}\n", num->id,num->initialIndexRow,num->finishIndexRow, num->initialIndexCol,num->finishIndexCol);
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
    // if (num->initialIndexRow == 0){
    //     //no tiene vecinos arriba
    // } else {
    //     //tiene vecino arriba
    // }

    Grid* gridThread = grid_alloc(numberRows, numberCols); // sumamos padding en col
    // setear valores a grilla
    for(int i = num->initialIndexRow; i < numberRows; i++){
        for(int j = num->initialIndexCol; j < ( num->initialIndexCol + numberCols); j++){
            int tmpIndexJ = j - num->initialIndexCol;
            uint8_t val = grid_get_current(grid, i, j);
            if(strncmp((const char*)&(val), "", 1) == 0){
                val = (uint8_t)'0';
                printf("WARNING val null\n",val);
            }
            
            printf("val-> %d\n",j);

            grid_set_current(gridThread, i, tmpIndexJ, val);
            // if (hasRightNeighbor) {
            //     printf("tiene vecino der\n");
            //     if ( (j+1) == num->finishIndexCol ) {
            //         printf("padding-index: %d\n", i*(numberCols) + j +1 );
            //         int retVal = pthread_mutex_trylock(&mutexes[i*(num->finishIndexCol+1)]);
            //         if (retVal == 0) {
            //             printf("retVal -> %d\n", retVal);
            //         } else if (retVal == EBUSY) {
            //             printf("EBUSY\n");

            //         } else if (retVal == EINVAL){
            //             printf("EINVAL\n");

            //         } else if (retVal == EFAULT){
            //             printf("EFAULT\n");
            //         }
            //     } else {
            //         uint8_t val = grid_get_current(grid, i, j);
            //         grid_set_current(gridThread, i, j, val);
            //     }

            // } else if(hasLeftNeighbor) {
            //     printf("tiene vecino izq\n");
            //     if ( j == num->initialIndexCol) {
            //         printf("padding-index: %d\n", i*(numberCols));
            //         int retVal = pthread_mutex_trylock(&mutexes[i*(numberCols+1)]);
            //         if (retVal == 0) {
            //             printf("retVal -> %d\n", retVal);
            //         } else if (retVal == EBUSY) {
            //             printf("EBUSY\n");

            //         } else if (retVal == EINVAL){
            //             printf("EINVAL\n");

            //         } else if (retVal == EFAULT){
            //             printf("EFAULT\n");
            //         }
            //     }
            // }


        }
    }

    printf("Grilla del thread id: %d:\n",num->id);

	life_save_board(stdout, gridThread);

    // life_compute_next_gen(gridThread);
    // grid_flip(gridThread);
    
    // printf("Grilla del threadV.2:\n--------------------------\n");
    // // life_save_board(stdout, gridThread);

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
	// for (int i = 0; i < numgens; i++) {
	// 	life_compute_next_gen(grid);
	// 	grid_flip(grid);
	// }
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
	// life_save_board(stdout, grid);
    return 0;
}