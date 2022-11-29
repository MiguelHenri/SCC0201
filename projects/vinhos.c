/* TRABALHO 1 - SCC0201 - Introducao a Ciencia de Computacao II */
/* Miguel Bragante Henriques NUSP 13671894 */

/* Catalogando vinhos*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

int quant_vinhos;

typedef struct{
	int id; 
	double citric_acid;
	double residual_sugar;
	double density;
	double pH;
	double alcohol;
}VINHO;

VINHO *importar();

double le_caracteristica(FILE *arq);

void imprime_vinhos(VINHO *catalogo);

void ordena_caracteristica(char *caracteristica, VINHO *catalogo);

void swap(VINHO *catalogo, int a, int b);

int busca_val(char *caracteristica, VINHO *catalogo, double val, int *num);

void imprime_um_vinho(VINHO *catalogo, int index, int num_vinhos);

double retorna_valor_caracteristica(VINHO *catalogo, int index, char *caracteristica);

int main(void){

	/*lendo arquivo csv*/
	VINHO* catalogo = importar();
	if(catalogo==NULL){
		printf("Erro ao alocar memoria\n");
		return 1;
	}

	/*numero de buscas*/
	int n_buscas; 
	scanf("%d", &n_buscas);

	/*loop de buscas*/
	for(int i=0; i<n_buscas; i++){
		/*lendo a caracteristica a ser buscada*/
		char caracteristica[30];
		scanf("%s", caracteristica);

		/*ordenando a caracteristica*/
		ordena_caracteristica(caracteristica, catalogo);

		/*buscando o valor pedido*/
		double val;
		scanf("%lf", &val);
		int num_vinhos;
		int index = busca_val(caracteristica, catalogo, val, &num_vinhos);
		imprime_um_vinho(catalogo, index, num_vinhos);

	}

	//imprime_vinhos(catalogo);

	/*desalocando memoria*/
	free(catalogo);

	return 0;
}

VINHO* importar(){
	/*funcao responsavel por ler o arquivo csv e fazer a traducao de char para double*/

	/*lendo o nome do arquivo*/
	char nome[50];
	scanf("%s", nome);

	/*abrindo arquivo*/
	FILE* arq = fopen(nome,"r");
	if(arq==NULL){
        printf("Erro ao abrir arquivo\n");
        exit(1);
    }

    /*contando numero de vinhos no catalogo*/
    char aux;
    quant_vinhos = 0;
    while(fscanf(arq, "%c", &aux)!=EOF){
    	if(aux=='\n'){
    		quant_vinhos++;
    	}
    }

    //printf("%d\n", quant_vinhos);

    /*posicionando ponteiro para comeco do arquivo para ler de fato os vinhos*/
    fseek(arq, 0, SEEK_SET);

    /*alocando memoria para o vetor de vinhos (catalogo de vinhos)*/
    VINHO* catalogo = (VINHO*)calloc(quant_vinhos, sizeof(VINHO)); 

    /*lendo o arquivo*/
    /*pulando o cabecalho*/
    do{
    	fscanf(arq, "%c", &aux);
    }while(aux!='\n');

    /*lendo*/
    for(int i=0; i<quant_vinhos; i++){
    	/*lendo o id*/
    	char id[10];
    	for(int j=0; j<10; j++){
    		fscanf(arq, "%c", &aux);
    		if(aux==','){ //le ate a virgula
    			break;
    		}
    		id[j] = aux;
    	}
    	catalogo[i].id = atoi(id);

    	/*lendo as caracteristicas*/
    	catalogo[i].citric_acid = le_caracteristica(arq);
    	catalogo[i].residual_sugar = le_caracteristica(arq);
    	catalogo[i].density = le_caracteristica(arq);
    	catalogo[i].pH = le_caracteristica(arq);
    	catalogo[i].alcohol = le_caracteristica(arq);
    }

    fclose(arq);

    return catalogo;
}

double le_caracteristica(FILE*arq){
	/*funcao para ler uma string ate achar , ou \n*/
	/*a string (valor da caracteristica) eh entao transformada para float e retornada*/
	char str[10];
	char ch;
	for(int j=0; j<10; j++){
    	fscanf(arq, "%c", &ch);
    	if((ch==',') || (ch=='\n')){ //le ate a virgula ou \n
    		str[j] = '\0';
    		break;
    	}
    	str[j] = ch;
    }
    double d = (double)atof(str);
    return d;
}

void imprime_vinhos(VINHO*catalogo){
	printf("%d\n", quant_vinhos);
	for(int i = 0; i<quant_vinhos; i++){
		printf("\n--------------------------------------------------------------\n");
		printf("%d ", catalogo[i].id);
		printf("%lf ", catalogo[i].citric_acid);
	    printf("%lf ", catalogo[i].residual_sugar);
	    printf("%lf ", catalogo[i].density);
	    printf("%lf ", catalogo[i].pH);
	    printf("%lf ", catalogo[i].alcohol);
	}
}

void ordena_caracteristica(char *caracteristica, VINHO *catalogo){

	/*encontrando a caracteristica*/

	/*valor maior e index*/
	double maior;
	int index;

	for(int j=0; j<quant_vinhos; j++){
		/*atribuindo o maior como o primeiro*/
		maior = retorna_valor_caracteristica(catalogo, 0, caracteristica);
		index = 0;
		for(int i=0; i<quant_vinhos-j; i++){
			/*procurando o maior*/
			if(retorna_valor_caracteristica(catalogo, i, caracteristica)>maior){
				maior = retorna_valor_caracteristica(catalogo, i, caracteristica);
				index = i;
			}
			/*se igual, busca maior indice*/
			if(retorna_valor_caracteristica(catalogo, i, caracteristica)==maior){
				if(catalogo[i].id>catalogo[index].id){
					maior = retorna_valor_caracteristica(catalogo, i, caracteristica);
					index = i;
				}
			}
		}
		/*colocando o maior para o fim do vetor - n*/
		swap(catalogo, index, quant_vinhos-j-1);
	}
}


void swap(VINHO *catalogo, int a, int b){
	/*trocando os valores dos vinhos de indice a e b*/
	int auxint;
	double auxfloat; 

	//id
	auxint = catalogo[a].id;
	catalogo[a].id = catalogo[b].id;
	catalogo[b].id = auxint;

	//citric acid
	auxfloat = catalogo[a].citric_acid;
	catalogo[a].citric_acid = catalogo[b].citric_acid;
	catalogo[b].citric_acid = auxfloat;

	//residual_sugar
	auxfloat = catalogo[a].residual_sugar;
	catalogo[a].residual_sugar = catalogo[b].residual_sugar;
	catalogo[b].residual_sugar = auxfloat;

	//density
	auxfloat = catalogo[a].density;
	catalogo[a].density = catalogo[b].density;
	catalogo[b].density = auxfloat;

	//pH
	auxfloat = catalogo[a].pH;
	catalogo[a].pH = catalogo[b].pH;
	catalogo[b].pH = auxfloat;

	//alcohol
	auxfloat = catalogo[a].alcohol;
	catalogo[a].alcohol = catalogo[b].alcohol;
	catalogo[b].alcohol = auxfloat;
}

int busca_val(char *caracteristica, VINHO *catalogo, double val, int *num){

	int inicio = -1; int fim = quant_vinhos;

	while(inicio < fim-1){
		if(retorna_valor_caracteristica(catalogo, (inicio+fim)/2, caracteristica) > val){
			fim = (inicio+fim)/2;
		}
		else if(retorna_valor_caracteristica(catalogo, (inicio+fim)/2, caracteristica) < val){
			inicio = (inicio+fim)/2;
		}
		else{ //encontrou!
			/*testando os proximos e anteriores no vetor*/
			int i = 0;
			while(retorna_valor_caracteristica(catalogo, ((inicio+fim)/2)-i, caracteristica) == val){
				i++;
			}
			int r = ((inicio+fim)/2)-i+1; //indice resposta
			/*contando numero de vinhos com mesmo valor*/
			int cont = 0;
			while(retorna_valor_caracteristica(catalogo, ((inicio+fim)/2)-i+1+cont, caracteristica) == val){
				cont++;
			}
			*num = cont;
			return r;
		}
	}
	return -1;
}

void imprime_um_vinho(VINHO *catalogo, int index, int num_vinhos){
	if(index >= 0){
		printf("ID: %d, Citric Acid: %.5lf, Residual Sugar %.5lf, Density %.5lf, pH %.5lf, Alcohol %.5lf\n", catalogo[index].id, catalogo[index].citric_acid, catalogo[index].residual_sugar, catalogo[index].density, catalogo[index].pH, catalogo[index].alcohol);
		printf("Total de vinhos encontrados: %d\n", num_vinhos);
	}
	else{
		printf("Nenhum vinho encontrado\n");
	}
}

double retorna_valor_caracteristica(VINHO *catalogo, int index, char *caracteristica){

	/*dado o uso de struct, essa funcao retorna o valor de uma caracteristica de um valor de indice index no vetor de struct
	ou seja, retorna o valor da caracteristica de um vinho (index) no catalogo*/

	if(strcmp(caracteristica, "citric_acid")==0){
		return catalogo[index].citric_acid;
	}

	if(strcmp(caracteristica, "residual_sugar")==0){
		return catalogo[index].residual_sugar;
	}

	if(strcmp(caracteristica, "density")==0){
		return catalogo[index].density;
	}

	if(strcmp(caracteristica, "pH")==0){
		return catalogo[index].pH;
	}

	if(strcmp(caracteristica, "alcohol")==0){
		return catalogo[index].alcohol;
	}

	return -1;
}
