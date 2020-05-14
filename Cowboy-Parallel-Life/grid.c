#include <stdlib.h>
#include <string.h>
#include "grid.h"

Grid *grid_alloc(int rows, int cols)
{
	Grid *grid = malloc(sizeof(Grid));
	uint8_t * g1 = malloc(rows*cols);
	uint8_t * g2 = malloc(rows*cols);
	for(int i = 0; i < rows*cols ; i++){
		g1[i] = 0;
		g2[i] = 0;
	}
	grid->buf1 = g1;
	grid->buf2 = g2;
	grid->rows = rows;
	grid->cols = cols;
	return grid;
}

void grid_destroy(Grid *grid)
{
	free(grid->buf1);
	free(grid->buf2);
	free(grid);
}

void grid_flip(Grid *grid)
{
	int length = grid->cols*grid->rows;
	uint8_t *buf1Tmp = malloc(sizeof(uint8_t)*length);
	memcpy(buf1Tmp,grid->buf1, length);
	uint8_t *buf2Tmp = malloc(sizeof(uint8_t)*length);
	memcpy(buf2Tmp,grid->buf2, length);
	memcpy(grid->buf1, buf2Tmp, length);
	memcpy(grid->buf2, buf1Tmp, length);
}

uint8_t grid_get_current(Grid *grid, int row, int col)
{
	int cols = grid->cols;
	int position = cols*(row) + col;
	return grid->buf1[position];
}

void grid_set_current(Grid *grid, int row, int col, uint8_t data)
{
	int cols = grid->cols;
	int position = cols*(row) + col;
	grid->buf1[position] = data;
}

void grid_set_next(Grid *grid, int row, int col, uint8_t val)
{
	int cols = grid->cols;
	int position = cols*(row) + col;
	grid->buf2[position] = val;
}
