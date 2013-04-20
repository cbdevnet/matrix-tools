#include <stdio.h>
#include <stdbool.h>
#include <malloc.h>

#include "vectors.h"

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
				exit(9001);
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
		exit(9003);
	}
	
	printf("Creating data fields for %s [%dx%d]\n",matrixName,mx.header.width,mx.header.height);
	
	mx.data=malloc(mx.header.height*sizeof(double*));
	if(mx.data){
		for(i=0;i<mx.header.height;i++){
			mx.data[i]=malloc(mx.header.width*sizeof(double));
			if(!mx.data[i]){
				printf("Out of memory.");
				exit(9004);
			}
		}
	}
	else{
		printf("Out of memory.");
		exit(9005);
	}
	
	//single ask mode
	//TODO row mode
	for(i=0;i<mx.header.height;i++){
		for(c=0;c<mx.header.width;c++){
			printf("%s[%d][%d]=",matrixName,i,c);
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
	fwrite(&mx.header,sizeof(MATRIX_HEADER),1,out);
	for(i=0;i<mx.header.height;i++){
		fwrite(mx.data[i],sizeof(double),mx.header.width,out);
	}
	
	//cleanup
	fclose(out);
	if(!usearg){
		free(matrixName);
	}
	
	for(i=0;i<mx.header.height;i++){
		free(mx.data[i]);
	}
	free(mx.data);
}