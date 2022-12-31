#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/*SCC0201 - Introducao a Ciencia de Computacao II*/
/*Trabalho 3 - Login*/
/*Aluno: Miguel Bragante Henriques*/
/*NUSP: 13671894*/

/*sistema de login com senha para alunos de uma disciplina*/

#define NOTAS 4 //numero de notas

typedef struct aluno_ ALUNO;

struct aluno_{
	char NUSP[100];
	unsigned int senha;
	double notas[NOTAS];
};

typedef struct no *NO_ADJ; //evitar uso excessivo de *

typedef struct no NO;

struct no{
	ALUNO aluno; //lista encadeada
	NO_ADJ proximo;
};

unsigned int cria_hash(char *s);

bool no_nao_existe(NO_ADJ *table, ALUNO aluno, int hash);

void no_insere(NO_ADJ *table, ALUNO aluno, int hash);

void no_procura(NO_ADJ *table, ALUNO aluno, int hash);

void oblitera_table(NO_ADJ **table, int num_alunos);

int main(void){
	/*lendo numero de alunos*/
	int num_alunos;
	scanf("%d", &num_alunos);

	/*alocando espaco para vetor de alunos*/
	NO_ADJ *table = (NO_ADJ *)malloc(num_alunos * sizeof(NO_ADJ));
	if(table == NULL){
		printf("Erro na alocacao de memoria!\n");
		return 1;
	}
	/*inicializando*/
	for(int i=0; i<num_alunos; i++){
		table[i] = NULL;
	}

	/*lendo dados dos alunos*/
	for(int i=0; i<num_alunos; i++){
		ALUNO aluno;

		//lendo NUSP
		scanf("%s", aluno.NUSP);

		//lendo senha
		char senha[100];
		scanf("%s", senha);

		//criando hash da senha informada
		aluno.senha = cria_hash(senha);

		//lendo notas
		for(int j=0; j<NOTAS; j++){
			scanf("%lf", &aluno.notas[j]);
		}

		//criando hash do aluno dado seu NUSP (string)
		int hash = cria_hash(aluno.NUSP)%num_alunos;

		//exige que o NUSP seja unico
		if(no_nao_existe(table, aluno, hash)){
			no_insere(table, aluno, hash);
		}
	}

	/*lendo numero de logins*/
	int num_logins;
	scanf("%d", &num_logins);

	for(int i=0; i<num_logins; i++){
		ALUNO aluno;

		//lendo NUSP
		scanf("%s", aluno.NUSP);

		//lendo senha
		char senha[100];
		scanf("%s", senha);

		//criando hash a partir da senha informada
		aluno.senha = cria_hash(senha);
		
		//procurando dados
		no_procura(table, aluno, num_alunos);
	}

	/*desalocando memoria*/
	oblitera_table(&table, num_alunos);

	return 0;
}

unsigned int cria_hash(char *s) {

	//a. criando dois unsigned ints de 16 bits (high = 0, low = 1)
	unsigned short int high = 0; unsigned short int low = 1;

	/*b. para cada caracter de S
	i. low = (low + S[i]) mod 65521
	ii. high = (high + low) mod 65521*/
	for(int i=0; i<strlen(s); i++){
		low = ((low+s[i])%65521);
		high = ((low+high)%65521);
	}

	/*c. retornando um unsigned int de 32 bits, sendo que os 16 bits mais significativos
	correspondem a high e os 16 bits menos significativos correspondem a low.*/
	unsigned int sum = 0;

	sum = sum|(high << 16);
	sum = sum|low;

	return sum;
}

bool no_nao_existe(NO_ADJ *table, ALUNO aluno, int hash){
	/*funcao que testa se um no (NUSP) ja existe ou nao*/

	for(NO_ADJ temp = table[hash]; temp != NULL; temp = temp->proximo){
		if(strcmp(temp->aluno.NUSP, aluno.NUSP)==0){ //nusp ja existe
			printf("NUSP ja cadastrado\n");
			return false;
		}
	}
	//nusp nao eh repetido
	printf("Cadastro efetuado com sucesso\n");
	return true;

}

void no_insere(NO_ADJ *table, ALUNO aluno, int hash) {
	//funcao para inserir nos em uma lista encadeada de uma posicao hash da table
	NO_ADJ novo = (NO_ADJ)malloc(sizeof(NO));
	if(novo == NULL){
		printf("Erro ao alocar memoria para o no\n");
		exit(1);
	}
	//inserindo
	novo->aluno = aluno;
	novo->proximo = table[hash];
	table[hash] = novo;

}

void no_procura(NO_ADJ *table, ALUNO aluno, int num_alunos) {

	if(num_alunos == 0){
		/*dado nao encontrado*/
		printf("NUSP invalido\n");
		return;
	}

	int hash = cria_hash(aluno.NUSP)%num_alunos;

	for(NO_ADJ temp = table[hash]; temp != NULL; temp = temp->proximo){

		if(strcmp(temp->aluno.NUSP, aluno.NUSP) == 0){ //encontrou dado
			if(temp->aluno.senha == aluno.senha){ //senha correta
				printf("Notas: P1=%.1lf", temp->aluno.notas[0]);
				printf(", P2=%.1lf", temp->aluno.notas[1]);
				printf(", T1=%.1lf", temp->aluno.notas[2]);
				printf(", T2=%.1lf\n", temp->aluno.notas[3]);
			}
			else{ //senha incorreta
				printf("Senha incorreta para o NUSP digitado\n");
			}
			return;
		}
	}
	/*dado nao encontrado*/
	printf("NUSP invalido\n");
	
}

void oblitera_table(NO_ADJ **table, int num_alunos) {
	/*desalocando memoria*/
	NO_ADJ aux;
	NO_ADJ temp;
	for(int i=0; i<num_alunos; i++){
		aux = (*table)[i];
		while(aux != NULL){
			temp = aux->proximo;
			free(aux);
			aux = temp;
		}
	}
	free(temp);
	free(*table);
}