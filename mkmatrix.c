#include <stdio.h>
#include <stdbool.h>
#include <malloc.h>
#include <stdlib.h>

#include "vectors.c"

int usage(){
	printf("mkmatrix Utility\n");
	return 1;
}

int main(int argc, char** argv){
	char buf[2048];
	_MATRIX mx;
	memcpy(mx.header.sig,"MATRIX\0\0",8);
	char* matrixName=NULL;
	mx.data=NULL;
	unsigned long i=0;
	unsigned long c=0;
	bool usearg=false;
	FILE* out;
	
	if(argc>1){
		if(argv[1][0]=='-'){
			exit(usage());
		}
		else{
			matrixName=argv[1];
			usearg=true;
		}
	}
	else{
		printf("Enter name of matrix: ");
		if(fgets(buf,sizeof(buf)-1,stdin)){
			matrixName=malloc((strlen(buf)+1)*sizeof(char));
			if(matrixName){
				strncpy(matrixName,buf,strlen(buf)-1);
			}
			else{
				printf("Out of memory.");
				exit(ERR_OUTOFMEM);
			}
		}
		else{
			printf("Error reading.");
			exit(9002);
		}
	}
	
	out=fopen(matrixName,"w");
	if(!out){
		printf("No write access.");
		exit(9012);
	}

	printf("Width of Matrix %s: ",matrixName);
	if(fgets(buf,sizeof(buf)-1,stdin)){
		mx.header.width=strtoul(buf,NULL,10);
	}
	else{
		printf("Error reading.");
		exit(90010);
	}
	
	printf("Height of Matrix %s: ",matrixName);
	if(fgets(buf,sizeof(buf)-1,stdin)){
		mx.header.height=strtoul(buf,NULL,10);
	}
	else{
		printf("Error reading.");
		exit(9005);
	}
	
	if(mx.header.width<=0||mx.header.height<=0){
		printf("Invalid dimensions.");
		exit(ERR_NOTAMATRIX);
	}
	
	printf("Creating data fields for %s [%dx%d]\n",matrixName,mx.header.width,mx.header.height);
	
	if(initmatrix(&mx)!=ERR_OK){
		printf("Out of memory");
		exit(ERR_OUTOFMEM);
	}
	
	//single ask mode
	//TODO row mode
	for(i=0;i<mx.header.height;i++){
		for(c=0;c<mx.header.width;c++){
			printf("%s[%ld][%ld]=",matrixName,i,c);
			if(fgets(buf,sizeof(buf)-1,stdin)){
				mx.data[i][c]=strtof(buf,NULL);
			}
			else{
				printf("Error reading.");
				exit(90011);
			}
		}
	}
	
	//write
	writematrix(out,&mx);
	
	//cleanup
	fclose(out);
	if(!usearg){
		free(matrixName);
	}
	
	freematrix(&mx);
	return 0;
}