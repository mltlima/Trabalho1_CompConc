#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define MAX_N 100000000 //tamanho maximo sequencia
#define MAX_THREADS 25

long int N; //dimensao da sequencia
int nthreads; //numero de threads
int primo[MAX_N+1]; //primo[i] = 1 se i for primo
int proxBase; // proximo multiplacar do crivo a ser usado

// lock para a variavel proxBase
pthread_mutex_t proxBaseLock = PTHREAD_MUTEX_INITIALIZER;
// ID as threads
pthread_t id[MAX_THREADS];

// elimina todos os multiplos impares de x
void elimina(int x){  

    int i;

    for (i = 3; i*x <= N; i += 2)  {
        primo[i*x] = 0;
    }
}


/**
 * A ser rodada por todas as threads
 * 
 * pega o proximo crivo a ser multiplicado, muda a proxBase somente com o 
 * lock ativado
 * se o lock esta desativado, a chamada da funcao ira para o lock e 
 * retornara o valor
 * Se o lock estiver ativo, a funcao nao ira retornar o valor ate que o 
 * thread bloquedo descloqueie
 * 
 * threadN e o numero da thread
 */
void *trabalho(void* threadN)  {  
    int lim,base;
    int trabalho = 0;  // trabalho feito pela thread
    lim = sqrt(N);
    do  {

      pthread_mutex_lock(&proxBaseLock);
      base = proxBase;
      proxBase += 2;
      // unlock
      pthread_mutex_unlock(&proxBaseLock);

      if (base <= lim)  {
         if (primo[base])  {
            elimina(base);
            trabalho++;  
         }
      }
      else{
          return (void *) trabalho;
      }
   } while (1);
}


int main(int argc, char *argv[]){

    int nPrimos; // numero de primos achados
    int resultado;

    
	//recebe e valida os parametros de entrada (dimensao do vetor, numero de threads)
    if(argc < 3) {
        fprintf(stderr, "Digite: %s <dimensao sequencia> <numero threads>\n", argv[0]);
        return 1; 
    }
    N = atoi(argv[1]);
    nthreads = atoi(argv[2]);
    
    clock_t tempo1 = clock();
    // marca todos os numeros pares ja que nao sao primos
    // primos 1 ate serem validados
    for (int i = 3; i <= N; i++)  {
       if (i%2 == 0) primo[i] = 0;
       else {
           primo[i] = 1;
       }
    }

    
    proxBase = 3;
    // inicia as threads
    for (int i = 0; i < nthreads; i++){
        pthread_create(&id[i],NULL,trabalho,NULL);
    }



    
    //espera as threads terminarem
    for (int i = 0; i < nthreads; i++){
        pthread_join(id[i],&resultado);
        //printf("%d valor retornado pela thread\n",resultado);
    }
    tempo1 = clock() - tempo1;
    double tempoExecucao1 = (double)(tempo1) / CLOCKS_PER_SEC;
    printf("\nTempo concorrente: %f\n", tempoExecucao1);


    // resultados
    nPrimos = 1;
    for (int i = 3; i <= N; i++)
       if (primo[i])  {
          nPrimos++;
       }
    printf("o numero de primos achados foi: %d\n",nPrimos);
    
}
