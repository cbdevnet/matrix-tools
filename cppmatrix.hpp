/*
                .---. .---. 
               :     : o   :    me want cookie!
           _..-:   o :     :-.._    /
       .-''  '  `---' `---' "   ``-.    
     .'   "   '  "  .    "  . '  "  `.  
    :   '.---.,,.,...,.,.,.,..---.  ' ;
    `. " `.                     .' " .'
     `.  '`.                   .' ' .'
      `.    `-._           _.-' "  .'  .----.
        `. "    '"--...--"'  . ' .'  .'  o   `.
        .'`-._'    " .     " _.-'`. :       o  :
      .'      ```--.....--'''    ' `:_ o       :
    .'    "     '         "     "   ; `.;";";";'
   ;         '       "       '     . ; .' ; ; ;
  ;     '         '       '   "    .'      .-'
  '  "     "   '      "           "    _.-'

  Matrixclass for cbdev matrix format by null_ptr
*/
#pragma once

#include "vectors.h"
#include <stdint.h>
#include <cstring>
#include <iostream>
#include <exception>

typedef uint32_t mat_size_t;
const int NOSQMATRIX = 43;
const int NOINVFOUND = 44;
const double EPSILON = 0.000000000000001;

class cppmatrix;
std::ostream& operator<<(std::ostream& stream, const cppmatrix& input);
std::istream& operator>>(std::istream& stream, cppmatrix& output);

class cppmatrix
{
public:
	
private:
	friend std::ostream& operator<<(std::ostream& stream, const cppmatrix& input);
	friend std::istream& operator>>(std::istream& stream, cppmatrix& matrix);
private:
	_MATRIX matrix;

public:
	cppmatrix()
	{
		memset(&matrix, 0, sizeof(_MATRIX));
	}

	cppmatrix(mat_size_t height, mat_size_t width)
	{
		memset(&matrix, 0, sizeof(_MATRIX));

		matrix.header.height = height;
		matrix.header.width = width;

		matrix.data = new double*[height];
		for(mat_size_t i = 0; i < height; ++i)
		{
			matrix.data[i] = new double[width];
		}
	}

	cppmatrix(const cppmatrix& cpy)
	{
		(*this) = cpy;
	}

	cppmatrix(cppmatrix&& mv)
	{
		(*this) = mv;
	}

	cppmatrix& operator=(const cppmatrix& cpy)
	{
		if(matrix.data != 0)
		{
			for(mat_size_t i = 0; i < matrix.header.height; ++i)
			{
				delete[] matrix.data[i];
			}
			delete[] matrix.data;
			matrix.data = 0;
		}

		std::memcpy(&matrix.header, &cpy.matrix.header, sizeof(MATRIX_HEADER));
		matrix.data = new double*[matrix.header.height];
		for(mat_size_t i = 0; i < matrix.header.height; ++i)
		{
			matrix.data[i] = new double[matrix.header.width];
			memcpy(matrix.data[i], cpy.matrix.data[i], sizeof(mat_size_t)*matrix.header.width);
		}

		return (*this);
	}

	cppmatrix& operator=(cppmatrix&& mv)
	{
		std::memcpy(&matrix.header, &mv.matrix.header, sizeof(MATRIX_HEADER));
		std::memcpy(&matrix, &mv.matrix, sizeof(_MATRIX));
		mv.matrix.data = 0;

		return (*this);
	}

	~cppmatrix()
	{
		if(matrix.data != 0)
		{
			for(mat_size_t i = 0; i < matrix.header.height; ++i)
			{
				delete[] matrix.data[i];
			}
			delete[] matrix.data;
		}
	}

	mat_size_t height() { return matrix.header.height; }
	mat_size_t width() { return matrix.header.width; }

	double * const operator[] (mat_size_t index)
	{
		return matrix.data[index];
	}

	void gauss()
	{
		// The interesting part
		for(mat_size_t k = 0; k < matrix.header.width && k < matrix.header.height; ++k)
		{
			// Swap row if matrix.data[k][k] is 0
			if(-EPSILON < matrix.data[k][k] && matrix.data[k][k] < EPSILON)
			{
				for(mat_size_t i = k; i < matrix.header.height; ++i)
				{
					if(matrix.data [i][k] < -EPSILON || EPSILON < matrix.data[i][k])
					{
						swap_rows(i, k);
						break;
					}
				}
			}
			if(-EPSILON < matrix.data[k][k] && matrix.data[k][k] < EPSILON)
			{
				continue;
			}

			// Subtract multiple of row from other rows
			for(mat_size_t i = 0; i < matrix.header.height; ++i)
			{
				if(i != k && (matrix.data[i][k] < -EPSILON || EPSILON < matrix.data[i][k]))
				{
					double factor = matrix.data[i][k] / matrix.data[k][k];
					for(mat_size_t m = k; m < matrix.header.width; ++m)
					{
						matrix.data[i][m] -= matrix.data[k][m] * factor;
					}
				}
			}
		}

		shift_zeros_down();
	}

	void swap_rows(mat_size_t first, mat_size_t second)
	{
		double* tmp = matrix.data[first];
		matrix.data[first] = matrix.data[second];
		matrix.data[second] = tmp;
	}

	bool invert()
	{
		mat_size_t rowsize = matrix.header.width * sizeof(double);

		cppmatrix inverted(matrix.header.height, matrix.header.width * 2);
		for(mat_size_t i = 0; i < matrix.header.height; ++i)
		{
			memcpy(inverted.matrix.data[i], matrix.data[i], rowsize);
			memset(inverted.matrix.data[i] + matrix.header.width, 0, rowsize);
			inverted.matrix.data[i][matrix.header.width + i] = 1.;
		}
		inverted.gauss();

		for(mat_size_t i = 0; i < matrix.header.height; ++i)
		{
			if(-EPSILON < inverted.matrix.data[i][i] && inverted.matrix.data[i][i] < EPSILON)
				return false;
		}
		
		for(mat_size_t i = 0; i < matrix.header.height; ++i)
		{
			memcpy(matrix.data[i], inverted.matrix.data[i] + matrix.header.width, rowsize);
		}
		return true;
	}

	bool is_square()
	{
		return matrix.header.height == matrix.header.width;
	}
	
	bool is_valid()
	{
		return !(memcmp(matrix.header.sig, MATRIX_MAGIC, sizeof(MATRIX_MAGIC)-1));
	}
private:
	void shift_zeros_down()
	{
		// Normalize
		mat_size_t ptr = 0;
		for(mat_size_t i = 0; i < matrix.header.width && i < matrix.header.height; ++i)
		{
			if(matrix.data[i][i] < -EPSILON || EPSILON < matrix.data[i][i])
			{
				double factor = 1 / matrix.data[i][i];
				for(mat_size_t k = 0; k < matrix.header.width; ++k)
					matrix.data[i][k] *= factor;

				if(ptr != i)
				{
					swap_rows(ptr, i);
				}
				++ptr;
			}
		}
	}
};

std::ostream& operator<<(std::ostream& stream, const cppmatrix& input)
{
	stream.write(reinterpret_cast<const char*>(&input.matrix.header), sizeof(MATRIX_HEADER));
	for(mat_size_t i = 0; i < input.matrix.header.height; ++i)
	{
		stream.write(reinterpret_cast<const char*>(input.matrix.data[i]), input.matrix.header.width*sizeof(double));
	}

	return stream;
}
std::istream& operator>>(std::istream& stream, cppmatrix& output)
{
	cppmatrix buffer;
	stream.read(reinterpret_cast<char*>(&buffer.matrix.header), sizeof (MATRIX_HEADER));

	buffer.matrix.data = new double*[buffer.matrix.header.height];
	for(mat_size_t i = 0; i < buffer.matrix.header.height; ++i)
	{
		buffer.matrix.data[i] = new double[buffer.matrix.header.width];
		stream.read(reinterpret_cast<char*>(buffer.matrix.data[i]), buffer.matrix.header.width*sizeof(double));
	}

	output = std::move(buffer);

	return stream;
}
