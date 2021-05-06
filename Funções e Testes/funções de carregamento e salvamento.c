//funções de salvamento e carregamento de estado de jogo

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define TAMSTR 32

typedef struct Dificuldade{
    int VidasIniciais;
    int RecargaAura;
    int DanoDaAura;
}DIFICULDADE;

typedef struct Estado_de_Jogo //definições do estado de jogo, usado para escrever e ler arrquivo binario
{
    int VidasRestantes;
    int MapaAtual;
    DIFICULDADE Dificuldade;
    int RecargaAura; //tempo até poder usar a aura novamente

    //trapaças
    int CovardeAtivado;     //0-desativado      1-ativado
    int GreyskullAtivado;   //0-desativado      1-ativado

}ESTADODEJOGO;

int Salvamento(ESTADODEJOGO *Estado_de_Jogo)
{
    FILE *ArquivoNomesDosSaves;//arquivo com o nome de todos os saves
    FILE *ArquivoSave;//save especifico
    char nomeDoSave[TAMSTR];
    char nomeArquivoMapa[TAMSTR+4];//mudar constantes por defines
    char nomeArquivoBin[TAMSTR+4];
    printf("\nDigite o nome com que deseja salvar o arquivo: ");
    gets(nomeDoSave);

    snprintf(nomeArquivoMapa, sizeof nomeArquivoMapa, "%s.txt", nomeDoSave);
    snprintf(nomeArquivoBin, sizeof nomeArquivoBin, "%s.dat", nomeDoSave);

    printf("\nArquivoTexto: %s", nomeArquivoMapa);
    printf ("\nArquivoBinario: %s", nomeArquivoBin);

    //salva nome do save atual na lista de saves
    if (!(ArquivoNomesDosSaves = fopen("ListaDeSaves.txt","a")))
    {
        printf("Erro de abertura lista de saves\n");
    }
    else {
        fprintf(ArquivoNomesDosSaves,"%s\n",nomeDoSave);
    }
    fclose(ArquivoNomesDosSaves);


    //salva estado de jogo em arquivo binário com o nome do save
    if (!(ArquivoSave = fopen(nomeArquivoBin,"wb")))
    {
        printf("Erro de abertura binario\n");
    }
    else
    {
        if(fwrite(&Estado_de_Jogo,sizeof(ESTADODEJOGO),1,ArquivoSave) ==1)
        {
        fflush(ArquivoSave);//pre-salvamento
        }
        else{
            printf("erro escrita binario\n");
        }
    }
    fclose(ArquivoSave);

    clrscr();

    //salva arquivo de mapa do save?

    return 1;
}

//Carregamento
int Carregamento(ESTADODEJOGO *Estado_de_Jogo){
    ESTADODEJOGO EstadoArquivo;
    FILE *ArquivoNomesDosSaves;
    FILE *SaveBinEscolhido;
    char str[TAMSTR];
    char NomeEscolhido[TAMSTR];
    char NomeArquivoBin[TAMSTR+4];

    //ler ArquivosNomesdosSaves
    if (!(ArquivoNomesDosSaves = fopen("ListaDeSaves.txt","r")))
    {
        printf("Erro de abertura lista de saves\n");
    }
    else {
        printf("\nLISTA DE SAVES\n");//mostrar os nomes numa lista
        while (fgets(str, TAMSTR, ArquivoNomesDosSaves) != NULL)
        printf("%s", str);
    }
    fclose(ArquivoNomesDosSaves);

    //Usuario escreve o nome do save que deseja carregar
    printf("\nEscreva o nome do save que deseja carregar\n");
    gets(NomeEscolhido);

    snprintf(NomeArquivoBin, sizeof NomeArquivoBin, "%s.dat", NomeEscolhido);

    printf("%s",NomeArquivoBin);

    //abrir arquivo.dat com os dados do save
    if (!(SaveBinEscolhido = fopen(NomeArquivoBin,"rb")))
    {
        printf("Erro na abertura do arquivo de save binario\n");
    }
    else
    {
        if(fread(&EstadoArquivo,sizeof(ESTADODEJOGO),1,NomeArquivoBin) == 1)
        {
            printf("sucesso na leitura de save binario!!\n");
        }
        else{
            printf("Erro na leitura de save binario!!\n");
        }
    }
    printf("\nteste");
    //alterar estado de jogo


    //saida correta permite iniciar loop de jogo na main

}

//int SalvamentoTemporario(){}


int main()
{
    ESTADODEJOGO Estado_de_Jogo;
    Estado_de_Jogo.CovardeAtivado = 0;
    Estado_de_Jogo.Dificuldade.DanoDaAura = 2;
    Estado_de_Jogo.Dificuldade.RecargaAura = 3;
    Estado_de_Jogo.Dificuldade.VidasIniciais = 3;
    Estado_de_Jogo.MapaAtual = 1;
    Estado_de_Jogo.VidasRestantes = 2;
    Estado_de_Jogo.GreyskullAtivado = 0;
    int funcsave, funcsavetemp, funcload;
    int sair =0;

    funcsave = Salvamento(&Estado_de_Jogo);
    if (funcsave == 0){
        printf("\nERRO NA FUNCAO SAVE\n");
    }
    else {
        printf("\nsucesso!!!");

    }

    Carregamento(&Estado_de_Jogo);


return 0;
}
