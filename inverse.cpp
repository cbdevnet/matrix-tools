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
#include "runtime.hpp"
#include "cppmatrix.hpp"
#include <iostream>

DECLARE_MAIN();

void announce_yourself(std::ostream& output)
{
	output << "Inverse calculator for cbdev matrix format." << std::endl;
	output << std::endl;
	output << "Usage:" << std::endl;
	output << "inverse [<FILE>]" << std::endl;
	output << "Reads from stdin or and writes inversed matrix" << std::endl;
	output << "to stdout" << std::endl;
}
void announce_version(std::ostream& output)
{
	output << "Inverse 0.0.2 Alpha special edition" << std::endl;
}

int do_calculation(std::istream& input, std::ostream& output)
{
	cppmatrix matrix;
	input >> matrix;
	if(!matrix.is_valid())
	{
		return ERR_NOTAMATRIX;
	}
	if(!matrix.is_square())
	{
		return NOSQMATRIX;
	}

	if(!matrix.invert())
	{
		return NOINVFOUND;
	}
	output << matrix;

	return 0;
}
