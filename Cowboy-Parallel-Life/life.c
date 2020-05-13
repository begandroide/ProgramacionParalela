#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "life.h"
#include "grid.h"

Grid *life_load_board(FILE *fp)
{
	int display;
	int *rows = malloc(sizeof(int));
	int *cols = malloc(sizeof(int));
	fscanf(fp,"%d %d\n",rows,cols);
	// printf("%d, %d\n",*rows,*cols);
	Grid *grid = grid_alloc(*rows, *cols);
	int actualRow = 0;
	int actualCol = 0;
	while(1){
		//reading file
		display = fgetc(fp);
		if(feof(fp)){
			break;
		}
		if( display != ' ' && display != '\n'){
			grid_set_next(grid,actualRow,actualCol,(uint8_t)display);
			actualCol++;
			if(actualCol == (*cols)){
				actualRow++;
				actualCol = 0;
			}
		}
	}
	// printf("\n");
	grid_flip(grid);
	// for (int i = 0; i < (*rows)*(*cols); i++)
	// {
	// 	printf("%c",grid->buf1[i]);
	// 	if(i!= 0 && ( ( (i+1) % *cols) == 0 ) ){
	// 		printf("\n");
	// 	}
	// }
	// printf("\n");
	return grid; // replace this
}
/*
 * Get number of lives neighbors of a cel
 * at position index in grid 
 */
int get_neightbors(Grid *grid, int index){
	int rows = grid->rows;
	int cols = grid->cols;
	int rowActual = (index)/cols;
	int colActual = index % cols;
	int alives = 0;
	if( rowActual == 0 ){
		//primera fila
		// printf("Primera fila\n");
		if(colActual == 0){
			if( strncmp((const char*)&(grid->buf1[index + 1]), "1", 1) == 0 )
			{
				alives++;
			}
			if( strncmp((const char*)&(grid->buf1[index + cols]), "1", 1) == 0){
				alives++;
			}
			if( strncmp((const char*)&(grid->buf1[index + cols + 1]), "1", 1) == 0){
				alives++;
			}
		} else if( colActual == (cols - 1) ){
			// printf("Ultima col\n");
			if( strncmp((const char*)&(grid->buf1[index - 1]), "1", 1) == 0 )
			{
				alives++;
			}
			if( strncmp((const char*)&(grid->buf1[index + cols - 1]), "1", 1) == 0){
				alives++;
			}
			if( strncmp((const char*)&(grid->buf1[index + cols]), "1", 1) == 0){
				alives++;
			}

		} else {
			// printf("media col\n");
			if( strncmp((const char*)&(grid->buf1[index - 1]), "1", 1) == 0 )
			{
				alives++;
			}
			if( strncmp((const char*)&(grid->buf1[index + 1]), "1", 1) == 0 )
			{
				alives++;
			}
			if( strncmp((const char*)&(grid->buf1[index + cols + - 1]), "1", 1) == 0){
				alives++;
			}
			if( strncmp((const char*)&(grid->buf1[index + cols]), "1", 1) == 0){
				alives++;
			}
			if( strncmp((const char*)&(grid->buf1[index + cols + 1]), "1", 1) == 0){
				alives++;
			}
		}
	} else if (rowActual == (rows - 1) ){
		//ultima fila
		// printf("ultima fila\n");
		if(colActual == 0){
			if( strncmp((const char*)&(grid->buf1[index + 1]), "1", 1) == 0 )
			{
				alives++;
			}
			if( strncmp((const char*)&(grid->buf1[index - cols]), "1", 1) == 0){
				alives++;
			}
			if( strncmp((const char*)&(grid->buf1[index - cols + 1]), "1", 1) == 0){
				alives++;
			}
		} else if( colActual == (cols - 1) ){
			// printf("Ultima col\n");
			if( strncmp((const char*)&(grid->buf1[index - 1]), "1", 1) == 0 )
			{
				alives++;
			}
			if( strncmp((const char*)&(grid->buf1[index - cols - 1]), "1", 1) == 0){
				alives++;
			}
			if( strncmp((const char*)&(grid->buf1[index - cols]), "1", 1) == 0){
				alives++;
			}
		} else {
			if( strncmp((const char*)&(grid->buf1[index - 1]), "1", 1) == 0 )
			{
				alives++;
			}
			if( strncmp((const char*)&(grid->buf1[index + 1]), "1", 1) == 0 )
			{
				alives++;
			}
			if( strncmp((const char*)&(grid->buf1[index - cols + - 1]), "1", 1) == 0){
				alives++;
			}
			if( strncmp((const char*)&(grid->buf1[index - cols]), "1", 1) == 0){
				alives++;
			}
			if( strncmp((const char*)&(grid->buf1[index - cols + 1]), "1", 1) == 0){
				alives++;
			}
		}
	} else{
		//cualquier otra fila
		// printf("Otra fila\n");
		if(colActual == 0){
			if( strncmp((const char*)&(grid->buf1[index + 1]), "1", 1) == 0 )
			{
				alives++;
			}
			if( strncmp((const char*)&(grid->buf1[index - cols]), "1", 1) == 0){
				alives++;
			}
			if( strncmp((const char*)&(grid->buf1[index - cols + 1]), "1", 1) == 0){
				alives++;
			}
			if( strncmp((const char*)&(grid->buf1[index + cols]), "1", 1) == 0){
				alives++;
			}
			if( strncmp((const char*)&(grid->buf1[index + cols + 1]), "1", 1) == 0){
				alives++;
			}
		} else if( colActual == (cols - 1) ){
			// printf("Ultima col\n");
			if( strncmp((const char*)&(grid->buf1[index - 1]), "1", 1) == 0 )
			{
				alives++;
			}
			if( strncmp((const char*)&(grid->buf1[index - cols]), "1", 1) == 0){
				alives++;
			}
			if( strncmp((const char*)&(grid->buf1[index - cols - 1]), "1", 1) == 0){
				alives++;
			}
			if( strncmp((const char*)&(grid->buf1[index + cols]), "1", 1) == 0){
				alives++;
			}
			if( strncmp((const char*)&(grid->buf1[index + cols - 1]), "1", 1) == 0){
				alives++;
			}
		} else {
			if( strncmp((const char*)&(grid->buf1[index - 1]), "1", 1) == 0 )
			{
				alives++;
			}
			if( strncmp((const char*)&(grid->buf1[index + 1]), "1", 1) == 0 )
			{
				alives++;
			}
			if( strncmp((const char*)&(grid->buf1[index - cols + - 1]), "1", 1) == 0){
				alives++;
			}
			if( strncmp((const char*)&(grid->buf1[index - cols]), "1", 1) == 0){
				alives++;
			}
			if( strncmp((const char*)&(grid->buf1[index - cols + 1]), "1", 1) == 0){
				alives++;
			}
			if( strncmp((const char*)&(grid->buf1[index + cols + - 1]), "1", 1) == 0){
				alives++;
			}
			if( strncmp((const char*)&(grid->buf1[index + cols]), "1", 1) == 0){
				alives++;
			}
			if( strncmp((const char*)&(grid->buf1[index + cols + 1]), "1", 1) == 0){
				alives++;
			}
		}
	}
	return alives;
}
void life_compute_next_gen(Grid *grid)
{
	int rows = grid->rows;
	int cols = grid->cols;
	for (int i = 0; i < (rows)*(cols); i++)
	{
		// printf("%c",grid->buf1[i]);
		int rowActual = (i)/cols;
		int colActual = i % cols;
		int alives = get_neightbors(grid,i);
		// printf("alives = %d\n", alives);
		if(strncmp( (const char*)&grid->buf1[i], "0",1) == 0){
			if(alives == 3){
				// celda muerta -> revive!
				grid_set_next(grid,rowActual,colActual,(uint8_t)'1');
				// grid->buf2[i] = (uint8_t)1;
				// printf("REVIVIO!\n");
			} else {
				grid_set_next(grid,rowActual,colActual,(uint8_t)'0');
			}
		} else {
			if(alives == 2 || alives == 3){
				grid_set_next(grid,rowActual,colActual,(uint8_t)'1');
				// printf("VIVE!\n");
			} else {
				grid_set_next(grid,rowActual,colActual,(uint8_t)'0');
			}
		}

		// if(i!= 0 && ( ( (i+1) % cols) == 0 ) ){
		// 	printf("\n");
		// }
	}
}



void life_save_board(FILE *fp, Grid *grid)
{

	int rows = grid->rows;
	int cols = grid->cols;

	for(int index = 0; index < rows*cols; index++){
		fputc(grid->buf1[index],fp);
		if(index!= 0 && ( ( (index+1) % cols) == 0 ) ){
			fputc('\n', fp);
		}
	}
}
