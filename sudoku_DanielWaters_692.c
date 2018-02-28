/*
 * sudoku_DanielWaters_692.c
 *
 *  Created on: Feb 27, 2018
 *      Author: Daniel Waters
 *      CSUID:  011872692
 */
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>

#define MAX_THREADS 27

int puzzle[9][9] = {
			{6, 5, 3, 1, 2, 8, 7, 9, 4},
			{1, 7, 4, 3, 5, 9, 6, 8, 2},
			{9, 2, 8, 4, 6, 7, 5, 3, 1},
			{2, 8, 6, 5, 1, 4, 3, 7, 9},
			{3, 9, 1, 7, 8, 2, 4, 5, 6},
			{5, 4, 7, 6, 9, 3, 2, 1, 8},
			{8, 6, 5, 2, 3, 1, 9, 4, 7},
			{4, 1, 2, 9, 7, 5, 8, 6, 3},
			{7, 3, 9, 8, 4, 6, 1, 2, 5}
	};

typedef struct {
	int row;
	int col;
	int result;
} parameters;

void *checkRow(void*);
void *checkCol(void*);
void *checkBox(void*);
int checkLocation(int, int, int);
void printArray();
int checkResults(parameters*);

int main() {
	parameters *data = (parameters *) malloc(MAX_THREADS * sizeof(parameters));

	pthread_t tid[MAX_THREADS];
	pthread_attr_t attr;
	pthread_attr_init(&attr);

	for (int i = 0; i < 9; i++) {
		data[i].result = 0;
		data[i].row = i;
		data[i+9].col = i;
		data[i+9].result = 0;
		pthread_create(&tid[i], &attr, checkRow, &data[i]);
		pthread_create(&tid[i+9], &attr, checkCol, &data[i+9]);
	}

	int index = 18;
	for (int i = 0; i < 9; i+=3) {
		for (int j = 0; j < 9; j+=3) {
			data[index].result = 0;
			data[index].row = i;
			data[index].col = j;
			pthread_create(&tid[index], &attr, checkBox, &data[index]);
			index++;
		}
	}

	for (int i = 0; i < 27; i++) {
		pthread_join(tid[i], NULL);
	}

	printArray();

	if (checkResults(data)) {
		printf("Invalid Sudoku.\n");
	}

	free(data);

	return 0;
}

void *checkRow(void *params) {
	parameters *param = (parameters*) params;
	int row = param->row;

	for (int i = 0; i < 9; i++) {
		int occurances = 0;
		for (int j = 0; j < 9; j++) {
			if(puzzle[row][i] == puzzle[row][j]) {
				occurances++;
			}
		}
		if (occurances != 1) {
			param->result = 1;
		}
	}
	pthread_exit(0);
}

void *checkCol(void *params) {
	parameters *param = (parameters*) params;
	int col = param->col;

	for (int i = 0; i < 9; i++) {
		int occurances = 0;
		for (int j = 0; j < 9; j++) {
			if(puzzle[i][col] == puzzle[j][col]) {
				occurances++;
			}
		}
		if (occurances != 1) {
			param->result = 1;
		}
	}
	pthread_exit(0);
}

void *checkBox(void *params) {
	parameters *param = (parameters*) params;
	int row = param->row;
	int col = param->col;

	for (int i = row; i < (row+3); i++) {
		for (int j = col; j < (col+3); j++) {
			if(checkLocation(puzzle[i][j], row, col) != 1) {
				param->result = 1;
			}
		}
	}
	pthread_exit(0);
}

int checkLocation(int value, int row, int col) {
	int occurances = 0;
	for (int i = row; i < (row + 3); i++) {
		for (int j = col; j < (col +3); j++) {
			if (value == puzzle[i][j]) {
				occurances++;
			}
		}
	}
	return occurances;
}

void printArray() {
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			printf("%d ", puzzle[i][j]);
		}
		printf("\n");
	}
}

int checkResults(parameters *data) {
	for (int i = 0; i < 27; i++) {
		if (data[i].result != 0) {
			return 1;
		}
	}
	return 0;
}
