#include <stdio.h>
#include <stdlib.h>

/*SCC0201 - Introducao a Ciencia de Computacao II*/
/*Trabalho 2 - Tabela de Notas*/
/*Aluno: Miguel Bragante Henriques*/
/*NUSP: 13671894*/

/*tipo de dado utilizado para armazenar as notas e informacoes de cada aluno*/
typedef struct{
    char nome[50];
    int nota[50];
    double media;
    int index_arvorebin;
}ALUNO;

void print_classe(ALUNO *classe, int num_alunos, int num_provas);

void print_resposta(ALUNO *classe, int num_alunos, int num_provas);

void print_aluno(ALUNO a, int num_provas);

void calcula_media(ALUNO *classe, int num_alunos, int num_provas);

ALUNO maior(ALUNO a, ALUNO b);

ALUNO acha_pai(ALUNO *classe, ALUNO *arvore_bin, int num_alunos, int posicao);

ALUNO *torneioGeralSort(ALUNO *classe, int num_alunos, int num_provas);

int main()
{
    /*lendo numero de alunos e de provas*/
    int num_alunos, num_provas;
    scanf("%d %d", &num_alunos, &num_provas);

    /*alocando espaco na memoria para o vetor de alunos, isto eh, a classe*/
    ALUNO *classe = (ALUNO *)calloc(num_alunos, sizeof(ALUNO));
    if(classe == NULL){ //checando alocacao
        printf("Erro na alocacao de memoria.\n");
        return 1;
    }

    /*lendo as informacoes de cada aluno (entrada)*/
    for(int i=0; i<num_alunos; i++){
        scanf("%s", classe[i].nome);
        for(int j=0; j<num_provas; j++){
            scanf("%d", &classe[i].nota[j]);
        }
    }

    /*calculando as medias de cada aluno*/
    calcula_media(classe, num_alunos, num_provas);

    /*ordenando a classe em novo vetor e desalocando o antigo*/
    ALUNO *classe_ordenada = torneioGeralSort(classe, num_alunos, num_provas);
    free(classe);
    if(classe_ordenada == NULL){ //checando alocacao
        printf("Erro na alocacao de memoria.\n");
        return 1;
    }

    /*imprimindo o resultado, ja calculando os criterios de desempate*/
    print_resposta(classe_ordenada, num_alunos, num_provas);

    /*desalocando memoria do vetor ordenado*/
    free(classe_ordenada);

    return 0;
}

/*funcao que imprime os nomes, notas e medias dos alunos do vetor de alunos*/
/*foi usada apenas para debugar o codigo*/
void print_classe(ALUNO *classe, int num_alunos, int num_provas){
    for(int i=0; i<num_alunos; i++){
        printf("aluno: %s, notas:", classe[i].nome);
        for(int j=0; j<num_provas; j++){
            printf(" %d", classe[i].nota[j]);
        }
        printf("\nmedia: %.2lf\n", classe[i].media);
    }
}

/*funcao que imprime a resposta pedida, isto eh, os alunos ordenados e os criterios de desempate*/
void print_resposta(ALUNO *classe, int num_alunos, int num_provas){
    printf("Maior media: %.3f\n", classe[0].media); //imprime maior media
    for(int i=0; i<num_alunos-1; i++){

        printf("%d. %s - ", i+1, classe[i].nome); //imprime nome

        /*encontrar criterio de desempate com o individuo i+1*/
        if(classe[i].media > classe[i+1].media){
            printf("media\n"); //desempate = media
        }
        else{ //desempate = nota
            /*loop ate achar nota desempate*/
            int j = 0;
            while(classe[i].nota[j] == classe[i+1].nota[j]) j++;
            printf("desempate: nota %d\n", j+1);
        }
    }
    printf("%d. %s", num_alunos, classe[num_alunos-1].nome); //imprime ultimo aluno (nao ha criterio de desempate)
}

/*funcao que imprime os dados de um aluno, usada apenas para debugar o codigo*/
void print_aluno(ALUNO a, int num_provas){
    printf("aluno: %s, notas:", a.nome);
    for(int j=0; j<num_provas; j++){
        printf(" %d", a.nota[j]);
    }
    printf("\nmedia %.2f, index arvore bin %d\n", a.media, a.index_arvorebin);
}

/*funcao utilizada para calcular as medias dos alunos de uma classe*/
void calcula_media(ALUNO *classe, int num_alunos, int num_provas){
    for(int i=0; i<num_alunos; i++){
        double soma = 0;
        for(int j=0; j<num_provas; j++){
            soma+=classe[i].nota[j];
        }
        classe[i].media = soma/num_provas; //media = soma das notas/total de provas
    }
}

/*funcao que, dados dois alunos a e b, retorna o com melhor desempenho entre os dois*/
ALUNO maior(ALUNO a, ALUNO b){
    if(a.media > b.media) return a;
    if(b.media > a.media) return b;

    int i = 0;
    while(b.nota[i] == a.nota[i]) i++;

    if(a.nota[i] > b.nota[i]) return a;
    else return b;
}

/*funcao recursiva que constroi a arvore binaria do torneioSort, utilizada uma vez*/
ALUNO constroi_arvorebin(ALUNO *classe, ALUNO *arvore_bin, int k, int posicao){
    /*funcao que cria a arvore*/
    /*comecaremos com posicao = 1 (raiz ou maior elemento) e iremos ate as folhas*/
    int p1 = posicao*2;
    int p2 = (posicao*2) + 1;
    ALUNO f1, f2;
    if(p1 <= 2*k){
        f1 = constroi_arvorebin(classe, arvore_bin, k, p1);
        if(p2 < 2*k){
            f2 = constroi_arvorebin(classe, arvore_bin, k, p2);
            arvore_bin[posicao] = maior(f1, f2);
        }
        else{
            arvore_bin[posicao] = f1;
            return f1;
        }
    }
    return arvore_bin[posicao];
}

/*funcao recursiva que atualiza a arvore binaria, dado a posicao do individuo removido anteriormente*/
ALUNO atualiza_arvorebin(ALUNO *classe, ALUNO *arvore_bin, int num_alunos, int posicao){
    /*funcao que atualiza a arvore*/
    /*iremos atualizar a partir da posicao inicial*/
    if(posicao == 1){
        return arvore_bin[1]; //retorna o novo vencedor do torneio
    }

    /*procura os duelos dos torneios e acha o novo vencedor*/
    if(posicao%2 == 0){
        arvore_bin[posicao/2] = maior(arvore_bin[posicao], arvore_bin[posicao+1]);
    }
    else{
        arvore_bin[posicao/2] = maior(arvore_bin[posicao], arvore_bin[posicao-1]);
    }

    return atualiza_arvorebin(classe, arvore_bin, num_alunos, posicao/2);
}

/*funcao sort que ordena o vetor de alunos, utiliza uma arvore binaria, e depende das funcoes que constroem e atualizam ela*/
ALUNO *torneioGeralSort(ALUNO *classe, int num_alunos, int num_provas){
    /*criando o novo vetor, que recebera os alunos ordenados*/
    ALUNO *vet_ordenado = (ALUNO *)calloc(num_alunos, sizeof(ALUNO));
    if(vet_ordenado == NULL){
        return NULL; //checando alocacao
    }

    /*calculando o tamanho 2k*/
    int k = 1;
    do{
        k = k*2;
    }while (k <= num_alunos);

    /*criando e alocando espaco para a arvore binaria*/
    ALUNO *arvore_bin = (ALUNO *)calloc(2*k, sizeof(ALUNO)); //+1 pois raiz = posicao 1
    if(arvore_bin == NULL){
        return NULL; //checando alocacao
    }

    /*preenchendo as folhas*/
    int aux = 0;
    for(int i=num_alunos-1; i>=0; i--){
        arvore_bin[(2*k)-aux-1] = classe[i];
        arvore_bin[(2*k)-aux-1].index_arvorebin = (2*k)-aux-1;
        aux++;
    }

    /*loop de funcao recursiva que constroi e atualiza a nossa arvore binaria, encontrando o melhor elemento*/
    ALUNO melhor;
    for(int i=0; i<num_alunos; i++){
        if(i==0){ //para a primeira iteracao, constroi a arvore
            melhor = constroi_arvorebin(classe, arvore_bin, k, 1);
        }
        else{
            melhor = atualiza_arvorebin(classe, arvore_bin, num_alunos, melhor.index_arvorebin);
        }
        arvore_bin[melhor.index_arvorebin].media = -1; //flag no melhor aluno
        vet_ordenado[i] = melhor; //atualizando o vetor ordenado
    }

    /*desalocando memoria utilizada na arvore*/
    free(arvore_bin);

    return vet_ordenado;
}
