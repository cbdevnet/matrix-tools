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
 
	Inversecalculator by null_ptr
*/
#include "vectors.h"

#include "cppmatrix.hpp"
#include <iostream>
#include <fstream>
#include <exception>
#include <string.h>

void announce_yourself()
{
	std::cout << "Inverse" << std::endl;
	std::cout << std::endl;
	std::cout << "This is a inverse calculator for cbdev matrix format." << std::endl;
	std::cout << "Usage:" << std::endl;
	std::cout << "Reads from stdin and writes to stdout" << std::endl;
}
void announce_version()
{
	std::cout << "Inverse 0.0.1 Alpha bugless" << std::endl;
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
		
		cppmatrix matrix;

		(*input) >> matrix;

		if(!matrix.is_square())
		{
			return NOSQMATRIX;
		}

		if(!matrix.invert())
		{
			return NOINVFOUND;
		}

		(*output) << matrix;
	}
	catch(std::ios_base::failure exc)
	{
		return IOERR;
	}
}
