#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>

#include "vectors.c"

int usage(){
	printf("transpose Utility\n");
	return 1;
}

int main(int argc, char** argv){
	FILE* stream_in;
	_MATRIX in;
	_MATRIX out;
	
	uint32_t i;
	uint32_t c;

	if(argc>1){
		if(argv[1][0]=='-'){
			exit(usage());
		}
		else{
			stream_in=fopen(argv[1],"r");
		}
	}
	else{
		stream_in=stdin;
	}
	
	if(!stream_in){
		printf("Could not open.");
		exit(9003);
	}
	
	switch(readmatrix(stream_in,&in)){
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
	
	fclose(stream_in);
	
	memcpy(out.header.sig,MATRIX_MAGIC,8);
	out.header.width=in.header.height;
	out.header.height=in.header.width;
	
	if(initmatrix(&out)==ERR_OK){
		for(i=0;i<in.header.height;i++){
			for(c=0;c<in.header.width;c++){
				out.data[c][i]=in.data[i][c];
			}
		}
		writematrix(stdout,&out);
		freematrix(&out);
	}
	else{
		printf("Failed.");
	}
	
	freematrix(&in);
	
	return 0;
}