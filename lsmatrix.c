#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <malloc.h>

#include "vectors.c"

int usage(){
	printf("lsmatrix Utility\n");
	return 1;
}

int main(int argc, char** argv){
	FILE* in;
	char buf[2048];
	double* row;
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
	
	if(readmatrix(in,&mx)!=ERR_OK){
		printf("Failed.");
	}
	
	for(i=0;i<mx.header.height;i++){
		for(c=0;c<mx.header.width;c++){
			printf("%0.3lf ",mx.data[i][c]);
		}
		printf("\n");
	}
	freematrix(&mx);
	
	fclose(in);
}