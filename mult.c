#include <stdio.h>
#include <stdbool.h>

#include "vectors.c"

int usage(){
	printf("matrix multiplication utility\n");
	printf("Usage: mult [-m] <scalar|matrix> [<matrix>]\n");
	printf("\nFirst argument may be matrix or scalar, second argument must be matrix\n");
	printf("To enable usage of integral matrix names (discouraged),\n");
	printf("Use -m to force interpretation of the first argument as matrix");
	//TODO make better
	return 1;
}

int main(int argc, char** argv){
	//arguments
	// 1. scalar or matrix
	// 2. matrix or from stdin
	//if -m force matrix
	char* first=NULL;
	char* second=NULL;
	_MATRIX arg1;
	_MATRIX arg2;
	_MATRIX result;
	bool force=false;
	bool scalarMult=true;
	long scalar;
	FILE* in;
	int32_t i,c,r;
	
	if(argc<2){
		exit(usage());
	}
	
	for(i=1;i<argc;i++){
		if(!strcmp(argv[i],"-m")){
			force=true;
		}
		else{
			if(first){
				second=argv[i];
			}
			else{
				first=argv[i];
			}
		}
	}
	
	scalar=strtol(first,NULL,10);
	
	if(force||scalar==0){
		//open
		in=fopen(first,"r");
		if(in){
			scalarMult=false;
			if(readmatrix(in,&arg1)!=ERR_OK){
				printf("Error reading first arg.");
				exit(9001);
			}
			fclose(in);
		}
	}
	
	if(second){
		in=fopen(second,"r");
	}
	else{
		in=stdin;
	}
	
	if(!in){
		printf("No such second arg.");
		exit(9003);
	}
	
	if(readmatrix(in,&arg2)!=ERR_OK){
		printf("Error reading second arg.");
		exit(9002);
	}
	if(in!=stdin){
		fclose(in);
	}
	
	if(!scalarMult){
		//check dimensions
		if(arg1.header.width==arg2.header.height){
			result.header.height=arg1.header.height;
			result.header.width=arg2.header.width;
			memcpy(result.header.sig,MATRIX_MAGIC,8);
			initmatrix(&result);
			
			for(r=0;r<arg2.header.width;r++){
				for(i=0;i<arg2.header.height;i++){
					for(c=0;c<arg1.header.height;c++){
						result.data[c][r]+=arg1.data[c][i]*arg2.data[i][r];
					}
				}
			}
				
			writematrix(stdout,&result);
			freematrix(&result);
		}
		else{
			printf("Incorrect dimensions.");
		}
	}
	else{
		//do scalar multiplication
		for(i=0;i<arg2.header.height;i++){
			for(c=0;c<arg2.header.width;c++){
				arg2.data[i][c]=arg2.data[i][c]*((double)scalar);
			}
		}
		writematrix(stdout,&arg2);
	}
	
	if(!scalarMult){
		freematrix(&arg1);
	}
	freematrix(&arg2);
	return 0;
}