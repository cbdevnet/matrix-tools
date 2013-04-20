#include "vectors.h"

int readmatrix(FILE* infile, _MATRIX* mx){
	uint32_t i;
	
	fread(&mx->header,sizeof(MATRIX_HEADER),1,infile);
	
	if(memcmp(mx->header.sig,"MATRIX\0\0",8)){
		return ERR_NOTAMATRIX;
	}
	
	mx->data=malloc(mx->header.height*sizeof(double*));
	if(mx->data){
		for(i=0;i<mx->header.height;i++){
			mx->data[i]=malloc(mx->header.width*sizeof(double));
			if(!mx->data[i]){
				return ERR_OUTOFMEM;
			}
		}
	}
	else{
		return ERR_OUTOFMEM;
	}
	
	for(i=0;i<mx->header.height;i++){
		fread(mx->data[i],sizeof(double),mx->header.width,infile);
	}
	
	return ERR_OK;
}

int freematrix(_MATRIX* mx){
	uint32_t i;
	for(i=0;i<mx->header.height;i++){
		free(mx->data[i]);
	}
	free(mx->data);
}