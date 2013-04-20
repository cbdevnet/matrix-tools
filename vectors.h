#pragma once

#include <stdint.h>

typedef struct /*_MATRIX_HEADER*/{
	int8_t sig[8];
	uint32_t width;
	uint32_t height;
} MATRIX_HEADER;

typedef struct /*__MATRIX*/ {
	MATRIX_HEADER header;
	double** data;
} _MATRIX;

#define ERR_OUTOFMEM 9004
#define ERR_NOTAMATRIX 9003
#define ERR_OK 0