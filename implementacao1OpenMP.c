#include<stdio.h>
#include<math.h>
#include<omp.h>
#include<windows.h> //para calcular tempo de execução

#define NUM_THR 8 //número de threads a serem utilizadas

float RegraTrapezio(){
    double h = 0.0001; //intervalo de discretização
    int a = 0; //início do intervalo
    int b = 100; //final do intervalo
    double trapezios = (b-a)/h; //número de trapézios (quantas vezes iterar)
    long double fxa = 0.0; //valor da função no ponto x inicial do intervalo
    float xa = 0.0; //ponto x inicial no qual calcular a função
    long double fxb = 0.0; //valor da função no ponto x final do intervalo
    float xb = 0.0; //ponto x final no qual calcular a função
    long double fxdx = 0.0; //valor da integral
#pragma omp parallel for private(xa, fxa, xb, fxb) reduction(+:fxdx) num_threads(NUM_THR) //variáveis privadas para cálculos locais, 
                                                                                        //reduction: forma mais otimizada para evitar 
                                                                                        //condição de corrida, num_threads para definir
                                                                                        //quantas threads executar de acordo com a atividade
        for(int i = 1; i<=(int)trapezios-1; i++) //do primeiro trapézio (x0) ao último trapézio 
        {
            xa = i*h; //calcula o ponto xa atual
            fxa = sqrt(100.0*100.0 - xa*xa); //função a ser integrada no ponto xa
            fxdx += 2*fxa; //adiciona fxa na soma final 2 vezes, seguindo a simplificação da fórmula do trapézio
            //xa = i*h; //calcula o ponto xa seguindo o intervalo definido
            //fxa = sqrt(100.0*100.0 - xa*xa); //função a ser integrada no ponto xa
            //xb = (i+1)*h; //calcula o ponto xb seguindo o intervalo definido
            //fxb = sqrt(100.0*100.0 - xb*xb); //função a ser integrada no ponto xb
            //fxdx += (fxa+fxb)/2*h; //calculo da integral com regra do trapezio
        }
        fxdx = h/2*(100.0+fxdx); //soma das funções e dos pontos inicial e final [f(0)=100, f(100)=0] e finalização da regra do trapézio
    return fxdx;
}


int main() {
    SYSTEMTIME tempoI, tempoF; // variáveis para armazenar tempo do sistema
    float tempoIms, tempoFms; // variáveis para conversão do tempo do sistema para milisegundos
    GetSystemTime(&tempoI); //marcador de início de execução
    float a = RegraTrapezio();
    GetSystemTime(&tempoF); //marcador de fim de execução
    tempoIms = tempoI.wSecond*1000.0 + tempoI.wMilliseconds; //transformando o tempo para milisegundos
    tempoFms = tempoF.wSecond*1000.0 + tempoF.wMilliseconds; //transformando o tempo para milisegundos
    printf("Valor da integral: %f\n Tempo de execucao: %f segundos\n", a, (tempoFms-tempoIms)/1000.0); //retorna valor da integral e 
                                                                                                    //tempo de execução em segundos
    return 0;
}