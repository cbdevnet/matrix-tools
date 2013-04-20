#pragma once

#include <stdint.h>

typedef struct /*_MATRIX_HEADER*/{
	int8_t sig[8];
	uint32_t width;
	uint32_t height;
} MATRIX_HEADER;

typedef struct /*__MATRIX*/ {
	char* name;
	MATRIX_HEADER header;
	double** data;
} _MATRIX;

//TODO standardize error/exitcodes