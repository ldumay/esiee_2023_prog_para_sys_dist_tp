#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <sys/time.h>
// Dimension par defaut de la taille des matrices
#ifndef VAL_M
#define VAL_M 101
#endif
#ifndef VAL_N
#define VAL_N 201
#endif

double get_time() {
  struct timeval tv;
  gettimeofday(&tv, (void *)0);
  return (double) tv.tv_sec + tv.tv_usec*1e-6;

}

int main(int argc, char **argv){   
  double t,start,stop;
  int nb, i , j, k;
  int m=VAL_M, n=VAL_N;

  double matrice_A[m][n];
  double matrice_B[n][m];
  double matrice_res[m][m];

// init matrice A
  for(int i=0; i < m; i++) {
    for(int j=0; j < n; j++) {
      matrice_A[i][j] = (i+1)+(j+1);
    }
  }
// init matrice B
  for(int i=0; i < n; i++) {
    for(int j=0; j < m; j++) {
      matrice_B[i][j] = (i+1)-(j+1);
    }
  }

  //printf("Nb.threads\tTps.\n");
  start = get_time(); 
  #pragma omp parallel for num_threads(100) private(j,k) schedule(static,10000)
  for(i = 0; i < m; i++){      
    for(j = 0; j < n; j++){

      #pragma omp atomic write
      matrice_res[i][j] = 0.0;

      for(k = 0; k < n; k++){
        #pragma omp atomic
        matrice_res[i][j] += matrice_A[i][k]*matrice_B[k][j];
      }
    }
  }
  stop=get_time();
  t=stop-start;
  printf("%f",t);

   // Impression du resultat.
  fprintf(stdout, "\n\n"
	  "   Valeurs de m et n   : %5d %5d\n"
	  "   Resultat partiel    : %10.3E %10.3E ... %10.3E %10.3E\n\n",
	  m, n, matrice_res[1][1], matrice_res[2][2], matrice_res[m-3][m-3], matrice_res[m-2][m-2]
	  );

  return EXIT_SUCCESS;
}