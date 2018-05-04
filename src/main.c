#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/types.h>


#define N_MAX_PROCESSOS 4

void verificaPrimo(unsigned long int NUMBER, int INDEX, int *contador){
	// VERIFICA SE EH PRIMO
	unsigned long int divisor;
	unsigned int count = 0;


	if(NUMBER == 0 || NUMBER == 1){
		contador[INDEX] = 0;
	}else if((NUMBER % 2 == 0) && NUMBER != 2){
		contador[INDEX] = 0;
	}else{
		for(divisor=1;divisor<=NUMBER;divisor++){
			if(NUMBER % divisor == 0) count++;
		}	
			// SE FOR PRIMO
			// INSERE '1' NA POSICAO 'INDEX' 1;
			// INSERE '0' C.C
			if(count == 2){
				contador[INDEX] = 1;
			}else{
				contador[INDEX] = 0;
		}
	}


	exit(0);	// Sai do processo filho...
}

int main() {
	unsigned int i = 0, n = 0;
	unsigned int j;
	unsigned int TOTAL_PRIMOS = 0;
  	char str[1000], *s = str, *t = NULL;
	unsigned long int vet[64];
	int *contador;
	unsigned int VSIZE;
	pid_t pid[64];


	/*
	* ---------------------------------------------------------------------------------------
	* ------------------------------- LEITURA DAS ENTRADAS ----------------------------------
	* ---------------------------------------------------------------------------------------
	*/

	fgets(s, 1000, stdin);

	// Remove '\n' do final da string
	while(str[i] != '\n') i++;
	str[i] = '\0';

	// Tokeniza e insere no vetor de unsigned long int - vet[]
	while ((t = strtok(s, " ")) != NULL) {
		s = NULL;
		vet[n++] = atoi(t);
	}
	VSIZE = n;

	// Memoria compartilhada entre os processos filhos
	contador = (int*) mmap(NULL, sizeof(int)*VSIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, 0, 0);




	/*
	* ---------------------------------------------------------------------------------------
	* ---------------------------- PROCESSAMENTO DAS ENTRADAS -------------------------------
	* ---------------------------------------------------------------------------------------
	*/



	/*!
	 * Para cada elemento 'i' do vetor de numeros entrados, cria um processo paralelo que verifica
	 * se vet[i] eh primo. O loop vai percorrer todo o vetor, que tem tamanho VSIZE.
	 * Se o vetor tem mais de 4 elementos (condicao verificada pelo if(i<4)), entao aguarda-se um processo
	 * acabar para iniciar outro, mantendo-se assim, 4 processos paralelos por vez.
	 */
	i = 0;
	while(i < VSIZE){

		// Se ate 4 processos...
		if(i < N_MAX_PROCESSOS){
			pid[i] = fork();

			if(pid[i] == 0)	verificaPrimo(vet[i], i, contador);
			i++;
		}
		else{
		// Se ja usou N_MAX_PROCESSOS processos, espera um deles terminar e inicia outro...
			int status;
			wait(&status);	// Espera algum processo acabar...
			pid[i] = fork();
			if(pid[i] == 0){
				verificaPrimo(vet[i], i, contador);
			}
			i++;
		}
		

	}

	// Finalizando todo processo
	for(i = 0; i < N_MAX_PROCESSOS ; i++){
	int status;
	wait(&status);
	}	


	// CONTA QUANTOS PRIMOS FORAM ACHADOS...
	for(i=0;i < VSIZE;i++){
		TOTAL_PRIMOS += contador[i];
	}

	// Exibe o total de primos na saida padrao.
	printf("%d\n", TOTAL_PRIMOS);

	return 0;
}

