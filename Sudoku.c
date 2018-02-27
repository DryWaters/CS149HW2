/*
 * Sudoku.c
 *
 *  Created on: Feb 27, 2018
 *      Author: daniel
 */

#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>

int puzzle[] = {
			6, 5, 3, 1, 2, 8, 7, 9, 4,
			1, 7, 4, 3, 5, 9, 6, 8, 2,
			9, 2, 8, 4, 6, 7, 5, 3, 1,
			2, 8, 6, 5, 1, 4, 3, 7, 9,
			3, 9, 1, 7, 8, 2, 4, 5, 6,
			5, 4, 7, 6, 9, 3, 2, 1, 8,
			8, 6, 5, 2, 3, 1, 9, 4, 7,
			4, 1, 2, 9, 7, 5, 8, 6, 3,
			7, 3, 9, 8, 4, 6, 1, 2, 5
	};

int validity[81] = { 0 };

void *checkRow(void *param);
void *checkCol(void *param);
void *checkBox(void *param);

int main() {

	pthread_t tid[3][9];
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	int rowLocations[] = {0, 9, 18, 27, 36, 45, 54, 63, 72};
	int colLocations[] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
	int boxLocations[] = {0, 3, 6, 27, 30, 33, 54, 47, 60};

	for (int i = 0; i < 9; i++) {
		pthread_create(&tid[0][i], &attr, checkRow, &rowLocations[i]);
		pthread_create(&tid[1][i], &attr, checkCol, &colLocations[i]);
		pthread_create(&tid[2][i], &attr, checkBox, &boxLocations[i]);
	}

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 9; j++) {
			pthread_join(tid[i][j], NULL);
		}
	}

	for (int i = 1; i < 82; i++) {
		printf("%d ", validity[i-1]);
		if (i != 0 && i % 9 == 0) {
			printf("\n");
		}
	}
	return 0;
}

void *checkRow(void *params) {

	int location = *(int*) params;
	for (int i = location; i < location+9; i++) {
		int occurances = 0;
		for (int j = location; j < location+9; j++) {
			if(puzzle[i] == puzzle[j]) {
				occurances++;
			}
		}
		if (occurances != 1) {
			validity[i] = 1;
		}
	}
	pthread_exit(0);
}

void *checkCol(void *params) {
	int location = *(int*) params;
	for (int i = location; i < 81; i+=9) {

		int occurances = 0;

		for (int j = location; j < 81; j+=9) {
			if(puzzle[i] == puzzle[j]) {
				occurances++;
			}
		}
		if (occurances != 1) {
			validity[i] = 1;
		}
	}
	pthread_exit(0);
}

void *checkSquare(void *params) {

	int location = *(int*) params;

	pthread_exit(0);
}



