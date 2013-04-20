#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <malloc.h>

#include "vectors.h"

int usage(){
	printf("lsmatrix Utility\n");
	return 1;
}

int main(int argc, char** argv){
	FILE* in;
	char buf[2048];
	double* row;
	_MATRIX mx;
	unsigned long i;
	unsigned long c;

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
	
	fread(&mx.header,sizeof(MATRIX_HEADER),1,in);
	
	if(memcmp(mx.header.sig,"MATRIX\0\0",8)){
		fclose(in);
		printf("Not a matrix.");
		exit(90012);
	}
	
	row=malloc(mx.header.width*sizeof(double));
	if(!row){
		printf("Out of memory.");
		exit(9013);
	}
	
	for(i=0;i<mx.header.height;i++){
		fread(row,sizeof(double),mx.header.width,in);
		for(c=0;c<mx.header.width;c++){
			printf("%0.3lf ",row[c]);
		}
		printf("\n");
	}
	
	free(row);
	fclose(in);
}