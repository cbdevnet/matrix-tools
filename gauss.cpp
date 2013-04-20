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
 
  Gausscalculator by null_ptr
*/
#include "vectors.h"

#include <iostream>
#include <fstream>
#include <exception>
#include <cstring>

const int IOERR = 42;
const double EPSILON = 0.000000000000001;
typedef unsigned long mat_size;

void write_matrix(const MATRIX_HEADER& head, const _MATRIX& matrix, std::ostream& stream)
{
	stream.write(reinterpret_cast<const char*>(&head), sizeof(MATRIX_HEADER));
	for(mat_size i = 0; i < head.height; ++i)
	{
		stream.write(reinterpret_cast<const char*>(matrix.data[i]), head.width*sizeof(double));
	}
}
void read_matrix(MATRIX_HEADER& head, _MATRIX& matrix, std::istream& stream)
{
	stream.read(reinterpret_cast<char*>(&head), sizeof (MATRIX_HEADER));
	matrix.data = new double*[head.height];
	for(mat_size i = 0; i < head.height; ++i)
	{
		matrix.data[i] = new double[head.width];
		stream.read(reinterpret_cast<char*>(matrix.data[i]), head.width*sizeof(double));
	}
}

void announce_yourself()
{
	std::cout << "Gausscalculator" << std::endl;
	std::cout << std::endl;
	std::cout << "This is a gauss calculator for cbdev matrix format." << std::endl;
	std::cout << "Usage:" << std::endl;
	std::cout << "Reads from stdin and writes to stdout" << std::endl;
}
void announce_version()
{
	std::cout << "Gausscalculator 0.0.1 Alpha bugless" << std::endl;
}

int main(int argc, char** argv)
{
	std::istream* input = &std::cin;
	std::ifstream ifs;
	std::ostream* output = &std::cout;

	if(argc > 1)
	{
		// TODO: More beer err... flags
		if(!strcmp(argv[1], "-h"))
		{
			announce_yourself();
			return 0;
		}
		else if(!strcmp(argv[1], "-v"))
		{
			announce_version();
			return 0;
		}
		else
		{
			ifs.open(argv[1],std::ifstream::in);
			input=&ifs;
		}
	}
	try
	{
		input->exceptions(std::ios_base::badbit | std::ios_base::failbit | std::ios_base::eofbit);
		output->exceptions(std::ios_base::badbit | std::ios_base::failbit | std::ios_base::eofbit);
		MATRIX_HEADER head;
		_MATRIX matrix;
		read_matrix(head, matrix, *input);

		// The interesting part
		for(mat_size k = 0; k < head.width && k < head.height; ++k)
		{
			// Swap row if matrix.data[k][k] is 0
			if(-EPSILON < matrix.data[k][k] && matrix.data[k][k] < EPSILON)
			{
				for(mat_size i = k; i < head.height; ++i)
				{
					if(matrix.data [i][k] < -EPSILON || EPSILON < matrix.data[i][k])
					{
						double* tmp = matrix.data[i];
						matrix.data[i] = matrix.data[k];
						matrix.data[k] = tmp;
						break;
					}
				}
			}
			if(-EPSILON < matrix.data[k][k] && matrix.data[k][k] < EPSILON)
			{
				continue;
			}

			// Subtract multiple of row from other rows
			for(mat_size i = 0; i < head.height; ++i)
			{
				if(i != k && (matrix.data[i][k] < -EPSILON || EPSILON < matrix.data[i][k]))
				{
					double factor = matrix.data[i][k] / matrix.data[k][k];
					for(mat_size m = k; m < head.width; ++m)
					{
						matrix.data[i][m] -= matrix.data[k][m] * factor;
					}
				}
			}
		}

		// Normalize
		mat_size ptr = 0;
		double* row0 = 0;
		for(mat_size i = 0; i < head.width && i < head.height; ++i)
		{
			if(matrix.data[i][i] < -EPSILON || EPSILON < matrix.data[i][i])
			{
				double factor = 1 / matrix.data[i][i];
				for(mat_size k = 0; k < head.width; ++k)
					matrix.data[i][k] *= factor;

				if(ptr != i)
				{
					matrix.data[ptr] = matrix.data[i];
					matrix.data[i] = row0;
				}
				++ptr;
			}
			else
			{
				row0 = matrix.data[i];
			}
		}

		write_matrix(head, matrix, *output);

		// Should call delete but... fuck it OS takes care!
	}
	catch(std::ios_base::failure exc)
	{
		return IOERR;
	}
}