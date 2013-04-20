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
 
	Matrixcalculationruntime by null_ptr
*/
#include "vectors.h"

#include <iostream>
#include <fstream>
#include <exception>
#include <cstring>

#define DECLARE_MAIN() int main(int argc, char** argv) {\
	return do_main(argc, argv);\
}

void announce_yourself(std::ostream&);
void announce_version(std::ostream&);
int do_calculation(std::istream& input, std::ostream& output);

int do_main(int argc, char** argv)
{
	std::istream* input = &std::cin;
	std::ifstream ifs;
	std::ostream* output = &std::cout;

	if(argc > 1)
	{
		// TODO: More beer err... flags
		if(!std::strcmp(argv[1], "-h"))
		{
			announce_yourself(std::cout);
			return 0;
		}
		else if(!std::strcmp(argv[1], "-v"))
		{
			announce_version(std::cout);
			return 0;
		}
		else
		{
			ifs.open(argv[1], std::ifstream::in);
			input=&ifs;
		}
	}
	try
	{
		input->exceptions(std::ios_base::badbit | std::ios_base::failbit | std::ios_base::eofbit);
		output->exceptions(std::ios_base::badbit | std::ios_base::failbit | std::ios_base::eofbit);

		return do_calculation((*input), (*output));
	}
	catch(const std::ios_base::failure& exc)
	{
		return ERR_NOTAMATRIX;
	}
}
