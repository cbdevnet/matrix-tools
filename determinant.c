#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <malloc.h>

#include "determinant.h"
#include "vectors.h"

int usage(){
	printf("determinant Utility\n");
	return 1;
}

int main(int argc, char** argv){
	FILE* in;
	char buf[2048];
	double** matrix;
	double elem;
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
	
	if(mx.header.height != mx.header.width){
	    printf("Not a quadratic matrix.");
	    exit(2013);
	}
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
	
	
	for (i = 0; i < mx.header.height; i++) {
	    fread(mx.data[i], sizeof(double), mx.header.width, in);
	}
	printf("%0.3lf\n", det(mx));
	
	fclose(in);
	for(i=0;i<mx.header.height;i++){
		free(mx.data[i]);
	}
	free(mx.data);
}

double det(_MATRIX mx) {
    if (mx.header.height == 2) {
        printf("det = %.0lf\n", mx.data[0][0] * mx.data[1][1] - mx.data[0][1] * mx.data[1][0]);
        return mx.data[0][0] * mx.data[1][1] - mx.data[0][1] * mx.data[1][0];
    }
    if (mx.header.height == 1) {
        return mx.data[0][0];
    }
    
    unsigned long i, j, k, m, n;
    double determinant = 0, tmp = 0;
    _MATRIX new_mx;
    memcpy(new_mx.header.sig,"MATRIX\0\0",8);
    new_mx.data = NULL;
    new_mx.header.height = mx.header.height - 1;
    new_mx.header.width = mx.header.width - 1;
    new_mx.data=malloc(new_mx.header.height*sizeof(double*));
	if(new_mx.data){
		for(i=0;i<new_mx.header.height;i++){
			new_mx.data[i]=malloc(new_mx.header.width*sizeof(double));
			if(!new_mx.data[i]){
				printf("Out of memory.");
				exit(90004);
			}
		}
	}
	else{
		printf("Out of memory.");
		exit(90005);
	}
    for (i = 0; i < mx.header.height; i++) { // traverse every row mx.data[i][*]
        m = 0;
        for (j = 0; j < mx.header.height; j++) { // traverse new row of new_mx
            n = 0;
            for (k = 1; k < mx.header.width; k++) { // traverse element of row
                if (j != i) {
                    new_mx.data[m][n] = mx.data[j][k];
                    n++;
                }
            }
            if (n > 0) {
                m++;
            }
        }
        for (m = 0; m < new_mx.header.height; m++) {
            for (n = 0; n < new_mx.header.width; n++) {
                printf("%.0lf ", new_mx.data[m][n]);
            }
            printf("\n");
        }
        //tmp = det(new_mx);
        if (i % 2 == 0) {
            determinant += mx.data[i][0] * det(new_mx);
        } else {
            determinant -= mx.data[i][0] * det(new_mx);
        }
        printf("\n");
    }
    for(i=0;i<new_mx.header.height;i++){
		free(new_mx.data[i]);
	}
    free(new_mx.data);
    return determinant;
}
