/******************************************************************************
* FILE: mpi_mm_skel.c
* DESCRIPTION:  
*   MPI Matrix Multiply - C Version
*   In this code, the master task distributes a matrix multiply
*   operation to numtasks-1 worker tasks.
* ******************************************************************************/
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>


#define NRA 8                 /* number of rows in matrix A */
#define NCA 4                 /* number of columns in matrix A */
#define NCB 6                  /* number of columns in matrix B */
#define MASTER 0               /* taskid of first task */
#define FROM_MASTER 1          /* setting a message type */
#define FROM_WORKER 2          /* setting a message type */
#define TOL  0.001             /* Error tolerance in matrix product */

int main (int argc, char *argv[]) {
	int	numtasks,              /* number of tasks in partition. i.e., number of processes */
		taskid,                /* a task identifier. i.e., process identifier*/
		numworkers,            /* number of worker tasks */
		source,                /* task id of message source */
		dest,                  /* task id of message destination */
		mtype,                 /* message type */
		rows,                  /* rows of matrix A sent to each worker */
		averow, extra, offset, /* used to determine rows sent to each worker */
		i, j, k, rc;           /* misc */

	 double **a,                   /* matrix A[NRA][NCA] to be multiplied */
		    **b,                   /* matrix B[NCA][NCB] to be multiplied */
		    **c,                   /* result matrix C[NRA][NCB] */
			aval,bval,cval,        /* init values for each matrix */
		    err, errsq;            /* error and square error of the matrix product */

	MPI_Status status;
	// status of all request
	MPI_Request* requests;

	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&taskid);
	MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
	
	if (numtasks < 2 ) {
		printf("Need at least two MPI tasks. Quitting...\n");
		MPI_Abort(MPI_COMM_WORLD, rc);
	 	exit(1);
	}
	numworkers = numtasks-1;


	/**************************** master task ************************************/
	if (taskid == MASTER) {
		printf("mpi_mm has started with %d tasks.\n",numtasks);

		a = (double**)malloc(NRA*sizeof(double*));
		double *ta = (double *)malloc(NRA * NCA * sizeof(double));	

		for (i=0;i<NRA;i++) {
			a[i] = &(ta[i*NCA]);
		}

		b = (double**)malloc(NCA*sizeof(double*));	
		double *tb = (double *)malloc(NCA * NCB * sizeof(double));	

		for (i=0;i<NCA;i++) {
			b[i] = &(tb[i*NCB]);
		}

		aval = 3.0;
		bval = 2.0;

		for (i=0; i<NRA; i++) {
			for (j=0; j<NCA; j++) {
				a[i][j]= aval;
			}
		}

		for (i=0; i<NCA; i++) {
			for (j=0; j<NCB; j++) {
				b[i][j]= bval;
			}
		}

		/* Send matrix data to the worker tasks: */

		/* 1. Calculate how many rows per worker */
		int worker_task[NRA];
		int worker_num = 0;
		for (i=0; i<NRA; i++) {
			if (i == MASTER) {
				// Move in table
				worker_num = (++worker_num) % numtasks;
			}
			worker_task[i] = worker_num;
			// Move in table
			worker_num = (++worker_num) % numtasks;
		}
		/* 2. For each worker, send:
		 offset, 
		 number of rows, 
		 chunk of a of size "number of rows", 
		 a whole copy of b -> broadcast
		*/
		mtype = FROM_MASTER;
		worker_num = 0;
		// Send in broadcast B
		// MPI_Broad();
		// Send a portition of A to each process
		for (i=0; i < NCA; i++) {
			worker_num++;
			if (i == MASTER) {
				worker_num--;
				continue;
			}
			rows = NCA;
			printf("Task %d\twork on %d rows\n", i, rows);
			MPI_Send(&rows, 1, MPI_INT, i, mtype, MPI_COMM_WORLD);
			double* worker_task = (double*)malloc(rows * sizeof(double));
			MPI_Send(worker_task, rows, MPI_DOUBLE, i, mtype, MPI_COMM_WORLD);
		}

		// Wait for all requests to finish
		// for requests == FINISH

		/* Receive results from worker tasks */
		mtype = FROM_WORKER;

		// Allocation matrice C
		c = (double**)malloc(NRA*sizeof(double*));
		double *tc = (double *)malloc(NRA * NCB * sizeof(double));	
		for (i=0;i<NRA;i++) {
			c[i] = &(tc[i*NCB]);
		}

		/* Receive from each worker:
			offset,
			number of rows to receive,
			chunk of c of size "rows to receive",
		*/
		// Receive all C part from workers
		// Place all workers result into a
		// c[nb_row] = matrice_from_worker

		/* Print results */
		printf("******************************************************\n");
		printf("Result Matrix:\n");
		for (i=0; i<NRA; i++) {
			printf("\n"); 
			for (j=0; j<NCB; j++) {
				printf("%6.2f   ", c[i][j]);
			}
		}
		printf("\n******************************************************\n");
		printf ("Done.\n");

		/* Check results */
		cval = aval*bval*(double)NCA;	
		errsq = 0.0; 
		for (i=0; i<NRA; i++) {
			for (j=0; j<NCB; j++) {
				err = c[i][j] - cval; 
				errsq += err * err;
			}
		}
	
		if (errsq > TOL) {
			printf("\n Errors in multiplication: %f\n",errsq);
		} else {
			printf("\n The result is correct\n");
		}
	}


	/**************************** worker task ************************************/
	else {
		mtype = FROM_MASTER;
		/* 
		Receive from master:
		   offset,
		   number of rows
      	*/
		MPI_Recv(&rows, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD, &status);

		// Allocate space for the chunk of a received and for the whole b matrix
		a = (double**)malloc(rows*sizeof(double*));
		double *t = (double *)malloc(rows * NCA * sizeof(double));	

		for (i=0;i<rows;i++) {
			a[i] = &t[i*NCA];
		}

		b = (double**)malloc(NCA*sizeof(double*));	
		double *tb = (double *)malloc(NCA * NCB * sizeof(double));	

		for (i=0;i<NCA;i++) {
			b[i] = &(tb[i*NCB]);
		}

		/* Receive chunk of a and whole b */
		// Temporary use of t due to data type
		MPI_Recv(t, rows, MPI_DOUBLE, MASTER, mtype, MPI_COMM_WORLD, &status);
		printf("%d\tReceive data from %d of size %d\n", taskid, MASTER, rows);

		// Allocate space for the chunk of c to calculate 
		c = (double**)malloc(rows*sizeof(double*));
		double *tc = (double *)malloc(rows * NCB * sizeof(double));	

		for (i=0;i<rows;i++) {
			c[i] = &(tc[i*NCB]);
		}
		/* Calculate the matrix product */
		// To calculate, go into each a element and multiply by b column
		for(i=0;i<rows;i++){
			for(j=0;j<NCB;k++) {
				for(k=0;k<NCA;k++) {
					c[i][j] += a[i][k] + b[k][j];
				}
			}
		}

		/* Send offset, number of rows and chunk of c to the master*/ 
		mtype = FROM_WORKER;
	}
	MPI_Finalize();
}
