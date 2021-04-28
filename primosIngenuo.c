#include <stdio.h>
#include <stdlib.h>
#include <time.h>
 
#define limite 10000000 
 
int main(){
    unsigned long long int i,j;
    int *primes;
    int z = 1;
 

    clock_t tempo1 = clock();
    primes = malloc(sizeof(int) * limite);
 
    for (i = 2;i < limite; i++)
        primes[i] = 1;
 
    for (i = 2;i < limite; i++)
        if (primes[i])
            for (j = i;i * j < limite; j++)
                primes[i * j] = 0;
 
    int count = 0;
    for (i = 2;i < limite; i++)
        if (primes[i])
            count++;
 
    printf("\nNumero de primos: %d \n",count);

    tempo1 = clock() - tempo1;
    double tempoExecucao1 = (double)(tempo1) / CLOCKS_PER_SEC;
    printf("\nTempo sequencial: %f\n", tempoExecucao1);

return 0;
}
