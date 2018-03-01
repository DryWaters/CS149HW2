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

#define MAX_THREADS 27		// Define the maximum number of threads
#define OCCURRENCE_SIZE 10	// Define the size of the array to hold number occurances
#define PUZZLE_SIZE 9		// Define the size of the puzzle row/col

// hard-coded valid puzzle
int puzzle[PUZZLE_SIZE][PUZZLE_SIZE] = {
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

// struct to hold the parameters to threads and the result of that thread
typedef struct {
	int row;
	int col;
	int result;
} parameters;

// function prototypes
void *checkRow(void*);
void *checkCol(void*);
void *checkBox(void*);

// prints the hard-coded sudoku puzzle to stdout
void printArray();

// checks array of structs result field and
// returns 0 (false) if all are equal to 0
// returns 1 (true) if any are equal not equal 0
int errorInResults(parameters*);

// counts the number of occurrences of each number (1 to OCCURRENCE_SIZE-1)
// returns 0 (false) if each number (1 to OCCURRENCE_SIZE-1) occurs only once
// return 1 (true) if they occur more than once
int exceedsOccurrences(int[]);

int main(void) {
	printf("CS149 Sudoku from Daniel Waters\n\n");

	// hold return code for error checking threads
	int threadErrorCode;

	// alloc an array of parameters structs
	parameters *data = (parameters *) malloc(MAX_THREADS * sizeof(parameters));

	// if malloc fails to allocate memory, print error code message and return 1;
	if (data == NULL) {
		perror("Error");
		return 1;
	}

	// 0-8 row threads
	// 9-17 col threads
	// 18-27 box threads
	pthread_t tid[MAX_THREADS];
	pthread_attr_t attr;
	pthread_attr_init(&attr);

	// create threads to check rows and columns
	// one for each col and row (18 total)
	for (int i = 0; i < PUZZLE_SIZE; i++) {
		data[i].result = 0;
		data[i].row = i;
		data[i+PUZZLE_SIZE].col = i;
		data[i+PUZZLE_SIZE].result = 0;

		// create threads to check rows
		threadErrorCode = pthread_create(&tid[i], &attr, checkRow, &data[i]);

		// if thread creation fails, print error code message and return 2;
		if (threadErrorCode) {
			perror("Error");
			return 2;
		}

		// create threads to check columns
		threadErrorCode = pthread_create(&tid[i+PUZZLE_SIZE], &attr, checkCol, &data[i+9]);

		// if thread creation fails, print error code message and return 2;
		if (threadErrorCode) {
			perror("Error");
			return 2;
		}
	}

	// create threads to check boxes (9 total)
	int index = 18;	// index for offsetting thread ids for box threads
	for (int i = 0; i < PUZZLE_SIZE; i+=3) {
		for (int j = 0; j < PUZZLE_SIZE; j+=3) {
			data[index].result = 0;
			data[index].row = i;
			data[index].col = j;

			// create threads to check boxes
			threadErrorCode = pthread_create(&tid[index], &attr, checkBox, &data[index]);

			// if thread creation fails, print error code message and return 2;
			if (threadErrorCode) {
				perror("Error");
				return 2;
			}
			index++;
		}
	}

	// Wait for all threads to complete
	for (int i = 0; i < MAX_THREADS; i++) {
		threadErrorCode = pthread_join(tid[i], NULL);

		// if thread join fails, print error code message and return 3;
		if (threadErrorCode) {
			perror("Error");
			return 3;
		}
	}

	// Print the hard-coded sudoku puzzle
	printArray();

	if (errorInResults(data)) {
		printf("\nInvalid Sudoku.\n");
	} else {
		printf("\nValid Sudoku!\n");
	}

	free(data);	// free malloc of struct array parameters
	return 0;
}

// check a row of numbers by counting the number of occurrences
void *checkRow(void *params) {
	int occurrences[OCCURRENCE_SIZE] = {0};

	// cast void* params back to a pointer to parameters struct
	parameters *param = (parameters*) params;
	int row = param->row;

	// Count the number of occurrences of each number
	for (int i = 0; i < PUZZLE_SIZE; i++) {
		occurrences[puzzle[row][i]]++;
	}

	// place 1 (true) in struct if a number occurs more than once
	param->result = exceedsOccurrences(occurrences);

	// return with no return (return value is stored in param struct)
	pthread_exit(0);
}

// check a col of numbers by counting the number of occurrences
void *checkCol(void *params) {
	int occurrences[OCCURRENCE_SIZE] = {0};

	// cast void* params back to a pointer to parameters struct
	parameters *param = (parameters*) params;
	int col = param->col;

	// Count the number of occurrences of each number
	for (int i = 0; i < PUZZLE_SIZE; i++) {
		occurrences[puzzle[i][col]]++;
	}

	// place 1 (true) in struct if a number occurs more than once
	param->result = exceedsOccurrences(occurrences);

	// return with no return (return value is stored in param struct)
	pthread_exit(0);
}

void *checkBox(void *params) {
	int occurrences[OCCURRENCE_SIZE] = {0};

	// cast void* params back to a pointer to parameters struct
	parameters *param = (parameters*) params;
	int row = param->row;
	int col = param->col;

	// Count the number of occurrences of each number
	for (int i = row; i < (row+3); i++) {
		for (int j = col; j < (col+3); j++) {
			occurrences[puzzle[i][j]]++;
		}
	}

	// place 1 (true) in struct if a number occurs more than once
	param->result = exceedsOccurrences(occurrences);

	pthread_exit(0);
}

// return 1 (true) if any number from 1 to OCCURRENCE_SIZE - 1
// occurred more than once
int exceedsOccurrences(int occurances[]) {
	for (int i = 1; i < OCCURRENCE_SIZE; i++) {
		if (occurances[i] > 1) {
			return 1;
		}
	}
	return 0;
}

// print the hard-coded puzzle to stdout
void printArray() {
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			printf("%d ", puzzle[i][j]);
		}
		printf("\n");
	}
}

// if any thread returned a result not equal to 0
// return 1 (true) found bad puzzle
int errorInResults(parameters *data) {
	for (int i = 0; i < MAX_THREADS; i++) {
		if (data[i].result != 0) {
			return 1;
		}
	}
	return 0;
}
