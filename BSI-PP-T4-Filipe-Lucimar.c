// Filipe - Lucimar

// Bibliotecas
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

// Definindo constantes
#define LINHA 3
#define COLUNA 3

// typedef para semaforo
sem_t sem_0, sem_1;

// Exemplo do slide
int A[LINHA][COLUNA] = {{3, 1,0}, {0, 2, -1}, {0, 0, 3}};
int B[LINHA] = {4, 2, 0};
// Resultado esperado X = {1, 1, 0}

// Exemplo 1 de teste
//int A[LINHA][COLUNA] = {{2, 1, 3}, {0, 1, -1}, {0, 0, 2}};
//int B[LINHA] = {11, 1, 4};
// Resultado esperado  X = {1, 3, 2}

// Exemplo 2 de teste
//int A[LINHA][COLUNA] = {{1, 1, 2}, {0, 2, 1}, {0, 0, 5}};
//int B[LINHA] = {1, -3, -5};
// Resultado esperado  X = {4, -1, -1}

// Variaves globais compartilhadas
int X[LINHA];
int soma;

// Assinatura de funcoes
void printm(int mat[LINHA][COLUNA]); // Funcao para imprimir matriz
void printv(int vet[LINHA]); // Funcao para imprimir vetor
void* func_0(void* args); // Funcao thread calculo de X
void* func_1(void* args); // Funcao thread calculo somatorio

int main(void)
{
    system("clear"); // Limpar tela

    // Imprimindo a matriz A e o vetor B
    printf("Matriz A:\n");
    printm(A); 
    printf("Vetor B:\n");
    printv(B);

    // typedef para thread
    pthread_t t0, t1;

    // Iniciando semaforos
    sem_init(&sem_0, 0, 0); // Travando o semaforo referente ao calculo de X
    sem_init(&sem_1, 0, 1); // Liberando o semaforo referente ao somatorio

    // Criando threads
    pthread_create(&t0, NULL, &func_0, NULL);
    pthread_create(&t1, NULL, &func_1, NULL);

    // Aguardando execucao das threads
    pthread_join(t0, NULL);
    pthread_join(t1, NULL);

    // Imprimindo o resuldado esperado do vetor X
    printf("Vetor X:\n");
    printv(X);

    // Destruindo os semaforos
    sem_destroy(&sem_0);
    sem_destroy(&sem_1);

    return 0;
}

void printm(int mat[LINHA][COLUNA])
{
    for (int l = 0; l < LINHA; l++)
    {
        for (int c = 0; c < COLUNA; c++)
        {
            printf("%d\t", mat[l][c]);
        }
        printf("\n");        
    }
    
}

void printv(int vet[LINHA])
{
    for (int l = 0; l < LINHA; l++)
    {
        printf("%d\n", vet[l]);
    }
    
}

void* func_0(void* args) // Thread funcao que calcula X linha
{
    for (int i = LINHA - 1; i >= 0; i--)
    {        
        sem_wait(&sem_0); // Semaforo aguardando valor de soma
        X[i] = (B[i] - soma) / A[i][i]; // calculando X linha
        sem_post(&sem_1); // Semaforo liberando somatorio    
    }
    pthread_exit(NULL);
}

void* func_1(void* args) //Thread funcao que calcula o somatorio
{
    for (int i = LINHA - 1; i >= 0; i--)
    {
        sem_wait(&sem_1); // Primeiro semaforo liberado para somatorio
        soma = 0;
        for (int j = i; j < LINHA; j++)
        {
            soma += A[i][j] * X[j]; // calculando o somatorio
        }
        sem_post(&sem_0); // Liberando semaforo para calculo de X  
    }    
    pthread_exit(NULL);
}