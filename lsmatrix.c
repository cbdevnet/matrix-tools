#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>

#include "vectors.c"

int usage(){
	printf("lsmatrix Utility\n");
	return 1;
}

int main(int argc, char** argv){
	FILE* in;
	_MATRIX mx;
	uint32_t i;
	uint32_t c;

	if(argc>1){
		if(argv[1][0]=='-'){
			exit(usage());
		}
		else{
			in=fopen(argv[1],"r");
		}
	}
	else{
		in=stdin;
	}
	
	if(!in){
		printf("Could not open.");
		exit(9003);
	}
	
	switch(readmatrix(in,&mx)){
		case ERR_OUTOFMEM:
			printf("Out of mem.");
			exit(ERR_OUTOFMEM);
		
		case ERR_NOTAMATRIX:
			printf("Not a Matrix");
			exit(ERR_NOTAMATRIX);
		
		case ERR_OK:
			break;
		
		default:
			printf("Bad things are happening");
			exit(9000);
	}
	
	for(i=0;i<mx.header.height;i++){
		for(c=0;c<mx.header.width;c++){
			printf("%0.3lf ",mx.data[i][c]);
		}
		printf("\n");
	}
	freematrix(&mx);
	
	fclose(in);
	return 0;
}