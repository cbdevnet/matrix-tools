#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>

#include "vectors.c"

double det(_MATRIX mx);

int usage(){
	printf("determinant Utility\n");
	return 1;
}

int main(int argc, char** argv){
	FILE* in;
	_MATRIX mx;

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
		fclose(in);
		printf("Error.");
		exit(90012);
	}
	
	if(mx.header.height != mx.header.width){
	    printf("Not a quadratic matrix.");
	    exit(2013);
	}
	
	printf("%0.3lf\n", det(mx));
	
	fclose(in);
	freematrix(&mx);
	return 0;
}

double det(_MATRIX mx) {
    if (mx.header.height == 2) {
        return mx.data[0][0] * mx.data[1][1] - mx.data[0][1] * mx.data[1][0];
    }
    if (mx.header.height == 1) {
        return mx.data[0][0];
    }
    
    unsigned long i, j, k, m, n;
    double determinant = 0;
	
    _MATRIX new_mx;
    
	memcpy(new_mx.header.sig,"MATRIX\0\0",8);
    new_mx.header.height = mx.header.height - 1;
    new_mx.header.width = mx.header.width - 1;

	initmatrix(&new_mx);

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
        if (i % 2 == 0) {
            determinant += mx.data[i][0] * det(new_mx);
        } else {
            determinant -= mx.data[i][0] * det(new_mx);
        }
    }
    
	freematrix(&new_mx);
    return determinant;
}
