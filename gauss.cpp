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
#include "cppmatrix.hpp"
#include "runtime.hpp"
#include <iostream>

DECLARE_MAIN();

void announce_yourself(std::ostream& output)
{
	output << "Gauss calculator for cbdev matrix format." << std::endl;
	output << std::endl;
	output << "Usage:" << std::endl;
	output << "gauss [<FILE>]" << std::endl;
	output << "Reads from stdin or file and writes gaussed matrix" << std::endl;
	output << "to stdout" << std::endl;
}
void announce_version(std::ostream& output)
{
	output << "Gausscalculator 0.0.2 Alpha without cookies" << std::endl;
}
int do_calculation(std::istream& input, std::ostream& output)
{
	cppmatrix matrix;
	input >> matrix;

	if(!matrix.is_valid())
	{
		return ERR_NOTAMATRIX;
	}
	
	matrix.gauss();

	output << matrix;
	return 0;
}
