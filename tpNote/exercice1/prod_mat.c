////////////////////////////////////////////////////////////////////////////////
// prod_mat.c --- produit de matrices : C = A * B
//
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>

// Dimension par defaut de la taille des matrices
#ifndef VAL_M
#define VAL_M 101
#endif
#ifndef VAL_N
#define VAL_N 201
#endif

int main() {
	int m=VAL_M, n=VAL_N;
	double a[m][n], b[n][m], c[m][m];
	//Initialisation des matrices A, B et C.
	//--Matrice A
	for(int i=0; i < m; i++) {
		for(int j=0; j < n; j++) {
			a[i][j] = (i+1)+(j+1);
		}
	}
	//--Matrice B
	for(int i=0; i < n; i++) {
		for(int j=0; j < m; j++) {
			b[i][j] = (i+1)-(j+1);
		}
	}
	//--Matrice C
	for(int i=0; i < m; i++) {
		for(int j=0; j < m; j++) {
			c[i][j] = 0;
		}
	}
	//Produit de matrices
	for(int i=0; i < m; i++) {
		for(int j=0; j < m; j++) {
			for(int k=0; k < n; k++) {
				c[i][j] = c[i][j] + a[i][k] * b[k][j];
			}
		}
	}
	//Impression du resultat.
	fprintf(stdout, "\n\n"
		"   Valeurs de m et n   : %5d %5d\n"
		"   Resultat partiel    : %10.3E %10.3E ... %10.3E %10.3E\n\n",
		m, n, c[1][1], c[2][2], c[m-3][m-3], c[m-2][m-2]
	);
	return EXIT_SUCCESS;
}

