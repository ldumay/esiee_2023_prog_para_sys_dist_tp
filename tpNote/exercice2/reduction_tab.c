////////////////////////////////////////////////////////////////////////////////
// reduction_tab.c ---  réduction d’un tableau
//
////////////////////////////////////////////////////////////////////////////////

#define _POSIX_SOURCE // pour rand_r()

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <string.h>


// Dimension par defaut de la taille des matrices
#ifndef VAL_NMOLEC
#define VAL_NMOLEC 10000
#endif
#ifndef VAL_NMOL
#define VAL_NMOL 10000
#endif
#ifndef VAL_N
#define VAL_N 10
#endif

// Initialisation a zero d'un tableau
void clear(double* array, int size) {
	for(int l=0; l<size; l++) array[l] = 0;
}

int main() {

	const int nmolec=VAL_NMOLEC, nmol=VAL_NMOL, n=VAL_N;
	double tab[nmolec][n][nmol];

	// Initialisation du tableau
	for(int k=0; k<nmolec; k++) {
		for(int j=0; j<n; j++) {
			for(int i=0; i<nmol; i++) {
				tab[k][j][i] = i + j + k;
			}
		}
	}

	double tab1[nmol], tab2[nmol];
	clear(tab2, nmol);


	// Nid de boucle a paralleliser
	#pragma omp parallel for private(i,j) {
		for(int k=0; k<nmolec; k++) {
			clear(tab1, nmol);
			for(int j=0; j<n; j++) {
				for(int i=0; i<nmol; i++) {
					tab1[i] += tab[k][j][i];
					tab1[i] = 2*tab1[i];
				}
			}

		}
	}

	// Verification des resultats
	double tab1c[nmol], tab2c[nmol];
	clear(tab2c, nmol);

	for(int k=0; k<nmolec; k++) {
		clear(tab1c, nmol);
		for(int j=0; j<n; j++) {
			for(int i=0; i<nmol; i++) {
				tab1c[i] += tab[k][j][i];
			}
		}
		for(int l=0; l<nmol; l++) tab2c[l] += 2*tab1c[l];
	}

	double err = 0;
	for(int i=0; i<nmol; i++) {
	double curr = fabs(tab2c[i] - tab2[i]) / fabs(tab2c[i]);
	if (curr > err)
		err = curr;
	}

	// Impression du resultat
	return EXIT_SUCCESS;
}