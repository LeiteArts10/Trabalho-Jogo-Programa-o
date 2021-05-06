#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <conio.h>
#include <conio2.h>
#include <windows.h>
//#include <linux.h> necessario mudar funcoes dependentes de OS
//#include <MACOS.h> necessario mudar funcoes dependentes de OS
#include <time.h>

#define TAMSTR 32//tamanho de strings para salvamento de arquivos

#define NUMERODEVIDASFACIL 3
#define NUMERODEVIDASDIFICIL 1
#define TAMANHOAURA 2
#define DANOAURAFACIL 2
#define DANO AURADIFICIL 1

#define VIDAZUMBI 1
#define VIDATROLL 2

#define UP_DIRECTION 1
#define RIGHT_DIRECTION 2
#define DOWN_DIRECTION 3
#define LEFT_DIRECTION 4

#define TIPO_ZUMBI 5
#define TIPO_TROLL 6

#define COLUNAS 60
#define LINHAS 23

#define PAREDE '#'
#define OBSTACULO 'X'
#define JOGADOR 'J'
#define TESOURO 'G'
#define ZUMBI 'Z'
#define TROLL 'T'
#define VAZIO ' '
#define AURA 'O'
//#define AURA_CHEFAO_PREPARO 'C' "telegrafa" ataque do chefao, dando tempo de reacao pro jogador, nao causa dano
//#define AURA_CHEFAO_ATIVA 'D' aura ativa, causa perda de vida se jogador entrar em contato

#define COR_PAREDE 8
#define COR_OBSTACULO 2
#define COR_JOGADOR 9
#define COR_TESOURO 14
#define COR_ZUMBI 12
#define COR_TROLL 16
#define COR_VAZIO 7
#define COR_AURA 15
//#define COR_AURACHEFAO_PREPARO X
//#define COR_AURACHEFAO_ATIVA X

#define MAXIMODEMONSTROS 15

typedef struct Dificuldade //definicoes de dificuldade
{
    int VidasIniciais;//vidas iniciais do jogador
    int DanoDaAura;//dano aplicado nos monstros
    int RecargaAura;//tempo de recarga da aura magica

}DIFICULDADE;

typedef struct Jogador  // definicoes do jogador
{
    int PosX;
    int PosY;
} PLAYER;

typedef struct Inimigo  // definicoes dos inimigos
{
    int PosX;
    int PosY;
    int Direcao;
    int Tipo;
    int steps;
    int vida;

} INIMIGO;

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

void Introducao()// mensagem de introducao
    {
     printf("\t\t\tBem vindo ao jogo.\n\n\tNeste jogo voce representa um mago que tem como \n\tobjetivo recuperar o tesouro de seu povo.\n\n");
     printf("\tPara isso devera atravesar todos os niveis superando os obstaculos \n\tdo calabouco.\n\n\tEntre os obstaculos estao trols e zumbi que podem \n\tdevora-lo e espinhos que podem machuca-lo.\n\n");
     printf("\tAlem de superar os obstaculos, pode marcar pontos matando os monstros \n\tutilizando sua aura magica, seu super poder.\n\n");
     printf("\tPara fazer vai ter 3 vidas para atravessar as salas do calabouco em\n\tmodo facil e 1 vida em modo dificil\n");
     printf("\tComandos:\n");
     printf("\tMovimento\t\tW/S/A/D\n\tAura Magica\t\tF\n\tMenu\t\t\tTAB\n\n");
     printf("\t\t\t\t\tBoa Sorte!!!\n\n");
     printf("Aperte Enter para continuar");
     fflush(stdin);
     getchar();
     clrscr();

    }

void ConclusaoNormal()  //Mensagem dada após terminar o jogo na dificuldade padrao
{
     printf("\n\t\t\tParabens, Voce Venceu!!!\n\n");
     printf("\tVoce recuperou o tesouro de seu povo e agora sua cidade podera viver em paz\n\n");
     printf("\tSe desejar, pode reviver sua aventura jogando com uma outra dificuldade\n");

     printf("\nAperte qualquer tecla para ir ao menu inicial");
     fflush(stdin);
     getchar();
     clrscr();
}

void ConclusaoDificil() //Mensagem dada após terminar o jogo na dificuldade Dicifil
{
    printf("\n\t\t\tParabens, Voce Venceu!!!\n\n");

    printf("\tPor ter ganho o jogo no modo Dificil agora voce e digno de conhecer\n");
    printf("\tnovos poderes capazes de manipular as leis do universo\n");
    printf("\tPara acessa-los basta apertar a tecla apostrofe(') quando abrir o menu\n");
    printf("\te dizer o seguinte:\n\t\tcovarde\t\t-\tPara ter vidas infinitas\n");
    printf("\t\tgreyskull\t-\tPara aumentar o poder de sua aura magica\n");

    printf("\n\n\tMuito Obrigado por jogar, boa sorte em sua proxima aventura!\n");

    fflush(stdin);
    getchar();
    clrscr();

}

int SelecionarDificuldade(){
    int nivelDeDificuldade, valido;
    clrscr();
    printf("\tSelecione a dificuldade que voce deseja jogar\n\t(1)-Facil\n\t(2)-Dificil\n");
    scanf("%d", &nivelDeDificuldade);
    valido = 0;
    do{
        switch (nivelDeDificuldade){
            case 1: printf("\n\tVoce selecionou dificuldade facil, voce tera 3 vidas para completar o jogo\nPressione qualquer tecla para continuar");
                    valido = 1;
                    getch();
                    break;
            case 2: printf("\n\tVoce selecionou a dificuldade dificil, voce tem apenas uma vida para completar o jogo e sua aura e mais fraca\nPressione qualquer tecla para continuar");
                    valido = 1;
                    getch();
                    break;
            default: printf("\n\tErro, selecione uma dificuldade valida\n");
                    valido = 0;
                    scanf("%d", &nivelDeDificuldade);
                    break;
        }
    }while (valido == 0);
    if (nivelDeDificuldade == 1){
        clrscr();
        return 1;
    }
    else if (nivelDeDificuldade == 2){
        clrscr();
        return 2;
    }
}

void Menu(char *Selecaopont) // mostra menu e retorna resposta
    {
    char Resposta;
    textcolor(WHITE);
    printf("\n\t(N) Novo jogo\n");
    printf("\t(C) Carregar jogo salvo (se houver)\n");
    printf("\t(S) Salvar jogo\n");
    printf("\t(Q) Sair do jogo\n");
    printf("\t(V) Voltar\n");
    //printf("\t(') Abrir Console\n");
    do
        {
        Resposta = getch();
        }
    while(Resposta!='N' && Resposta!='n' && Resposta!='C' && Resposta!='c' && Resposta!='S' && Resposta!='s' && Resposta!='Q' && Resposta!='q' && Resposta!='V' && Resposta!='v' && Resposta!= 39);
    *Selecaopont=Resposta;
    clrscr();
    }

int LerMapa(char *NomedoMapa, char MapadoJogo[LINHAS][COLUNAS], int Linhas, int Colunas, int MaximodeInimigos, int *QuantInimigos, INIMIGO *Inimigo, PLAYER *Jogador){  //le e transforma em matriz, atribui as posicoes do jogador e inimigos, controla o numero
    int I,J;
    FILE *Arquivo;
    int Status = 0;
    *QuantInimigos = 0;
    if((Arquivo=fopen(NomedoMapa, "r")))
    {
        for (I=0; I<Linhas; I++)
        {
            for (J=0; J<Colunas; J++)
            {
                MapadoJogo[I][J]= getc(Arquivo);
                switch(MapadoJogo[I][J]){
                    case 'z'://ZUMBI
                    case 'Z': if(*QuantInimigos<=MaximodeInimigos){
                              Inimigo[*QuantInimigos].PosX = J;
                              Inimigo[*QuantInimigos].PosY = I;
                              Inimigo[*QuantInimigos].Direcao = 0;
                              Inimigo[*QuantInimigos].steps = 0;
                              Inimigo[*QuantInimigos].Tipo = TIPO_ZUMBI;
                              Inimigo[*QuantInimigos].vida = VIDAZUMBI;
                              *QuantInimigos = *QuantInimigos+1;
                              break;
                            }
                    case 't'://TROLL
                    case 'T': if(*QuantInimigos<=MaximodeInimigos){
                              Inimigo[*QuantInimigos].PosX = J;
                              Inimigo[*QuantInimigos].PosY = I;
                              Inimigo[*QuantInimigos].Direcao = 0;
                              Inimigo[*QuantInimigos].steps = 0;
                              Inimigo[*QuantInimigos].Tipo = TIPO_TROLL;
                              Inimigo[*QuantInimigos].vida = VIDATROLL;
                              *QuantInimigos = *QuantInimigos+1;
                              break;
                            }
                    case 'j':
                    case 'J': Jogador -> PosX = J;
                              Jogador -> PosY = I;
                              break;
                    /*case 'o':
                    case 'O': AURA ATIVA, para no caso de salvamento de mapa ter aura ativada, COLOCAR funcao para substituir 'O' por espaço em branco   */

                    /*case 'c':
                    case 'C': AURA DO CHEFAO ATIVA */
                }
            }
            getc(Arquivo);
        }
        Status = 1;
        fclose(Arquivo);
    }
    return Status;
}

int ControledeColisao(char MapadoJogo[LINHAS][COLUNAS], PLAYER *Jogador){  // verifica se vai haver colisao e informa o tipo

    int Cod = 5;

    switch(MapadoJogo[Jogador -> PosY][Jogador -> PosX]){
        case(PAREDE): Cod = 0; //se a posição a ser ocupada pelo jogador é uma parade ele retorna um código 0 para a função de movimentação
                           break;

        case(OBSTACULO): Cod = 1; //se a posição a ser ocupada pelo jogador é um obstáculo ele retorna um código 1 para a função de movimentação
                            break;

        case(TESOURO): Cod = 2; //se a posição a ser ocupada pelo jogador é o tesouro ele retorna um código 2 para a função de movimentação
                            break;

        case(ZUMBI): Cod = 3; //se a posição a ser ocupada pelo jogador é um zombie ele retorna um código 3 para a função de movimentação
                            break;

        case(TROLL): Cod = 4; //se a posição a ser ocupada pelo jogador é um troll ele retorna um código 4 para a função de movimentação
                            break;
        case(AURA): Cod = 6; //se a posição a ser ocupada é uma Aura ele retorna código 6 para a função de movimentação
                            break;
        default: break;
    }

    return Cod;
}

void Move(char MapadoJogo[LINHAS][COLUNAS], PLAYER *Jogador, int X, int Y, int *Controle){  // move jogador, se colisao retorna

     MapadoJogo[Jogador -> PosY][Jogador -> PosX] = VAZIO;
    (Jogador -> PosY) = (Jogador -> PosY) + Y;
    (Jogador -> PosX) = (Jogador -> PosX) + X;

    switch(ControledeColisao(MapadoJogo, Jogador)){
        case 0: (Jogador -> PosY) = (Jogador -> PosY) - Y;
                (Jogador -> PosX) = (Jogador -> PosX) - X;
                MapadoJogo[Jogador -> PosY][Jogador -> PosX] = JOGADOR;
                 break;

        case 1: *Controle = 1;
                MapadoJogo[Jogador -> PosY][Jogador -> PosX] = OBSTACULO;
                break;

        case 2: MapadoJogo[Jogador -> PosY][Jogador -> PosX] = TESOURO;
                break;

        case 3: *Controle = 1;
                MapadoJogo[Jogador -> PosY][Jogador -> PosX] = ZUMBI;
                break;

        case 4: *Controle = 1;
                MapadoJogo[Jogador ->PosY][Jogador -> PosX] = TROLL;
                break;

        case 5: MapadoJogo[Jogador -> PosY][Jogador -> PosX] = JOGADOR;
                break;
        case 6: MapadoJogo[Jogador -> PosY][Jogador -> PosX] = AURA;
    }

}


int SelecionaDirecao(){  // selecione direcao do monstro

    int N;
    N = rand()%4;

    return N;

}


void MoveInimigos(char MapadoJogo[LINHAS][COLUNAS], INIMIGO Inimigos[MAXIMODEMONSTROS], int Quant, PLAYER *Jogador, int *Controle, ESTADODEJOGO *estadodejogo){

    int I, X = 0, Y = 0;
    char Simbolo;
    if (estadodejogo->VidasRestantes>1)
    {
        textcolor(BLACK);
        printf("\nVidas restantes: %d", estadodejogo->VidasRestantes);
    }
    else
    {
        textcolor(RED);
        printf("\nVidas restantes: %d", estadodejogo->VidasRestantes);
    }

    for(I = 0; I < Quant; I++)
    {
        switch(Inimigos[I].Direcao){
            case 0: Y = -1;
                    X = 0;
                    break;
            case 1: X = -1;
                    Y = 0;
                    break;
            case 2: Y = 1;
                    X = 0;
                    break;
            case 3: X = 1;
                    Y = 0;
                    break;
        }
        Simbolo = MapadoJogo[Inimigos[I].PosY + Y][Inimigos[I].PosX + X];
        if((Simbolo == OBSTACULO && Inimigos[I].Tipo == TIPO_TROLL) || (Inimigos[I].steps > 4) || Simbolo == PAREDE){
            while(Inimigos[I].Direcao == rand()%4){
                Inimigos[I].Direcao = rand()%4;
            }
            Inimigos[I].steps = 0;
        }
        else if(Inimigos[I].Tipo == TIPO_ZUMBI && MapadoJogo[Inimigos[I].PosY][Inimigos[I].PosX] == OBSTACULO){

            MapadoJogo[Inimigos[I].PosY][Inimigos[I].PosX] = OBSTACULO;
            (Inimigos[I].PosY) = (Inimigos[I].PosY) + Y;
            (Inimigos[I].PosX) = (Inimigos[I].PosY) + X;
            Inimigos[I].steps = Inimigos[I].steps + 1;
        }
        else if((Inimigos[I].Tipo == TIPO_ZUMBI && MapadoJogo[Inimigos[I].PosY][Inimigos[I].PosX] == TESOURO) || (Inimigos[I].Tipo == TIPO_ZUMBI && MapadoJogo[Inimigos[I].PosY][Inimigos[I].PosX] == TESOURO)){
            MapadoJogo[Inimigos[I].PosY][Inimigos[I].PosX] = TESOURO;
            (Inimigos[I].PosY) = (Inimigos[I].PosY) + Y;
            (Inimigos[I].PosX) = (Inimigos[I].PosX) + X;
            Inimigos[I].steps = Inimigos[I].steps + 1;

        }
        else if(MapadoJogo[Inimigos[I].PosY][Inimigos[I].PosX] == MapadoJogo[Jogador -> PosY][Jogador -> PosX]){
            *Controle = 1;
            switch(Inimigos[I].Tipo){
                case TIPO_TROLL: MapadoJogo[Jogador -> PosY][Jogador -> PosX] = TROLL;
                                       break;
                case TIPO_ZUMBI: MapadoJogo[Jogador -> PosY][Jogador -> PosX] = ZUMBI;
                                        break;
            }

        }
        else{
            MapadoJogo[Inimigos[I].PosY][Inimigos[I].PosX] = VAZIO;
            (Inimigos[I].PosY) = (Inimigos[I].PosY) + Y;
            (Inimigos[I].PosX) = (Inimigos[I].PosX) + X;
            Inimigos[I].steps = Inimigos[I].steps + 1;
        }


        switch(Inimigos[I].Tipo){
            case TIPO_TROLL: if(MapadoJogo[Inimigos[I].PosY][Inimigos[I].PosX] == TESOURO){
                                        MapadoJogo[Inimigos[I].PosY][Inimigos[I].PosX] = TESOURO;
                                    }
                                   MapadoJogo[Inimigos[I].PosY][Inimigos[I].PosX] = TROLL;
                                   break;
            case TIPO_ZUMBI: if(MapadoJogo[Inimigos[I].PosY][Inimigos[I].PosX] == TESOURO){
                                        MapadoJogo[Inimigos[I].PosY][Inimigos[I].PosX] = TESOURO;
                                    }
                                    else if(MapadoJogo[Inimigos[I].PosY][Inimigos[I].PosX] != OBSTACULO)
                                        MapadoJogo[Inimigos[I].PosY][Inimigos[I].PosX] = ZUMBI;
                                    break;
        }
    }
}

void Aura(char MapadoJogo[LINHAS][COLUNAS], PLAYER *Jogador)
{
    int recarga;//recarga em segundos, varia de acordo com dificuldade selecionada
    int dano; //dano implicado aos inimigos, varia com dificuldade ou trapaça
    int raio;

    //pegar posição do jogador
    //checar tempo restante até ser utilizada novamente
    //checar o impacto dela no slot
        //se for monstro aplica dano ao monstro(ISSO OU O MONSTRO CHECHA SE ELE BATE NA AURA, POSSIVELMENTE MELHOR)
        //se for parede ou obstaculo não faz nada
    //pintar no mapa por 0.5 até 1 segundo(a decidir)
    //colocar delay de 3 segundos após ser executado


    printf("teste");

}

void PintaMapa(char MapadoJogo[LINHAS][COLUNAS], int Linhas, int Colunas){   // imprime o mapa
    int X, Y, Cor;
    for(X=0; X<Colunas; X++){
        for(Y=0; Y<Linhas; Y++){
            switch(MapadoJogo[Y][X]){
                case(PAREDE): Cor = COR_PAREDE;
                                   break;

                case(OBSTACULO): Cor = COR_OBSTACULO;
                                       break;

                case(JOGADOR): Cor = COR_JOGADOR;
                                     break;

                case(TESOURO): Cor = COR_TESOURO;
                                   break;

                case(ZUMBI): Cor = COR_ZUMBI;
                                     break;

                case(TROLL): Cor = COR_TROLL;
                                    break;

                case(VAZIO): Cor = COR_VAZIO;
                                         break;
                case(AURA): Cor = COR_AURA; //serve pra corrigir problemas de um save em que o player acabou de usar a aura dele
                                         break;
            }
            textbackground(Cor);
            gotoxy(X+1, Y+1);
            cprintf(" ");
        }
    }
}

void SalvamentoTemp(char MapadoJogo, ESTADODEJOGO Estado_de_Jogo)//exporta estado atual do mapa para arquivo texto e ESTADO DE JOGO para arquivo binario
{

    //fopen(mapa temp)//escreve em arquivo txt o estado atual do mapa


    //fopen(estado temp)//escreve estado de jogo em arquivo binario

}

void HideCursor()   //apaga o ponteiro
{
  CONSOLE_CURSOR_INFO cursor = {1, FALSE};
  SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor);
}

int Execucao(ESTADODEJOGO *estadodejogo){//mudar parametros para ESTADO DE JOGO
    int I;
    int Menu = 0;
    ESTADODEJOGO estadoparamover;
    int testeproximomapa;
    char MapadoJogo[LINHAS][COLUNAS];
    int QuantInimigos;
    int Controle = 0;
    int Colisao=0;
    char Tecla = 'u';
    int timerint = 0;
    char nomeArquivo[10];
    INIMIGO Inimigos[MAXIMODEMONSTROS];
    PLAYER Jogador;
    clock_t timer, timerend, timebegin;
    timebegin = clock();
    timer = clock();
    srand(time(NULL));

    snprintf(nomeArquivo, sizeof nomeArquivo, "mapa%d.txt", estadodejogo->MapaAtual);//transforma numero mapa em string pra carregar diferentes mapas

    while(estadodejogo->VidasRestantes >=1 && Colisao != 2  &&  Menu != 1)
    {
        if(LerMapa(nomeArquivo, MapadoJogo, LINHAS, COLUNAS, MAXIMODEMONSTROS, &QuantInimigos, Inimigos, &Jogador))
        {
            PintaMapa(MapadoJogo, LINHAS, COLUNAS);
            HideCursor();

            if(Controle == 1)
            {
                if(estadodejogo->CovardeAtivado==0)
                {
                    estadodejogo->VidasRestantes--;
                }
                else
                {
                estadodejogo->VidasRestantes=estadodejogo->VidasRestantes;
                }
            }
            estadoparamover= *estadodejogo;
            Controle = 0;
            for(I = 0; I < QuantInimigos; I++)
            {
                Inimigos[I].Direcao = SelecionaDirecao();
            }
            while((Colisao = (ControledeColisao(MapadoJogo, &Jogador))) != 1 && (Colisao = (ControledeColisao(MapadoJogo, &Jogador))) != 2 && (Colisao = (ControledeColisao(MapadoJogo, &Jogador))) != 3 && (Colisao = (ControledeColisao(MapadoJogo, &Jogador))) != 4  && Menu != 1)
            {
                if(kbhit())
                {
                    Tecla = getch();
                }
                else
                {
                    Tecla = 'u';
                }
                timerend = clock();
                if(((float)(timerend - timer)/ CLOCKS_PER_SEC)>0.2)
                {
                    MoveInimigos(MapadoJogo, Inimigos, QuantInimigos, &Jogador, &Controle, &estadoparamover);
                    timerint++;
                    timer = clock();
                }
                switch(Tecla)
                {   case 'w':
                    case 'W': Move(MapadoJogo, &Jogador, 0, -1, &Controle);
                            break;
                    case 'a':
                    case 'A': Move(MapadoJogo, &Jogador, -1, 0, &Controle);
                            break;
                    case 's':
                    case 'S': Move(MapadoJogo, &Jogador, 0, +1, &Controle);
                            break;
                    case 'd':
                    case 'D': Move(MapadoJogo, &Jogador, +1, 0, &Controle);
                            break;
                    case 'f':
                    case 'F': Aura(MapadoJogo, &Jogador);
                            break;
                    case 9 : Menu=1;
                            clrscr();
                            return 3;//código de retorno aqui ou o programa buga, e passa as fazes adiante quando se aperta TAB
                             //SalvamentoTemp(MapadoJogo, %Jogador.Vidas);
                            break;

                }
                PintaMapa(MapadoJogo, LINHAS, COLUNAS);

            }
        }
        else
        {
            printf("Erro na leitura do mapa\n");
        }
    }
    if(Colisao == 2){
            //IF VIDA DE TODOS OS MONSTROS == 0
            //ELSE PRINTF("\nNAO EH SEGURO COLETAR O TESOURO\nAINDA HA MONSTROS VIVOS\n")
        clrscr();
        gotoxy(20,10);
        textcolor(GREEN);
        printf("PASSOU DE FASE");
        Sleep(1200);//tempo de espera
        clrscr();


        testeproximomapa=estadodejogo->MapaAtual+1;
        snprintf(nomeArquivo, sizeof nomeArquivo, "mapa%d.txt", testeproximomapa);
        if(LerMapa(nomeArquivo, MapadoJogo, LINHAS, COLUNAS, MAXIMODEMONSTROS, &QuantInimigos, Inimigos, &Jogador)){
            return 1;
        }
        else{
            return 2;
        }
    }
    else if (Menu == 1)
    {
    gotoxy(20,10);
    }
    else
    {
        clrscr();
        gotoxy(20,10);
        textcolor(RED);
        printf("DERROTA");
        Sleep(1200);//tempo de espera
        clrscr();
        return 0; // retornar codigo de derrota
    }

clrscr();
}

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

    //abrir arquivo.dat com os dados do save
    if (!(SaveBinEscolhido = fopen(NomeArquivoBin,"rb")))
    {
        printf("Erro na abertura do arquivo de save binario\n");
        getch();
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
    //alterar estado de jogo


    //saida correta permite iniciar loop de jogo na main
    clrscr();
}

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
        printf("Arquivo Binario Salvo\n");
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

void Cheat_Code(int *covarde, int *greyskull)
{
//declare local variables//
    char cheatcode[20];
    int repeticao=0, validacao = 0;


//Run the code to check the password//
    do
    {
        fflush(stdin);
        printf("Insira o Cheat Code: ");
        scanf("%s", cheatcode);
        validacao = strcmp(cheatcode,"covarde");//torna vidas infinitas
        if (validacao == 0)
        {
            printf("\n Accepted type 1");
            *covarde=1;
        }
        else
        {
            validacao = strcmp(cheatcode,"greyskull");//buff na aura
            if (validacao == 0)
            {
                printf("\n Accepted type 2");
                *greyskull=1;
            }
            else
            {
                printf("Wrong password\n");//codigo errado
            }
        }
        printf("\n %s", cheatcode);
        printf("\nTentar Novamente?\n(S)im ou (N)ao?\n");
        fflush(stdin);
        switch (getch())//laço de repetição
        {
            case 's':
            case 'S': repeticao = 0; break;
            case 'n':
            case 'N': repeticao = 1; break;
            default: repeticao = 0; break;
        }
    }while (repeticao == 0);
    clrscr();

}

void LoopDeJogo(ESTADODEJOGO *Estado_de_Jogo, int Dificuldade){
    //função responsável por iniciar o loop de jogo da execução, fazendo a progressão dos mapas
    //o loop na main vem pra cá
    int retorno;

    do{
        retorno=Execucao(Estado_de_Jogo);
        if(retorno==1){
        Estado_de_Jogo->MapaAtual++;
        }
        else if (retorno == 3){
            printf("\n Abrindo Menu...\n");
        }
    }while(retorno==1);
    if(retorno==2){
        if(Dificuldade==1){
        ConclusaoNormal();
        }
    else{
        ConclusaoDificil();
        }
    }

}

int main()
    {
    char MapadoJogo[LINHAS][COLUNAS];
    int Sair=0;
    char Selecao;
    int mapainicial=0;
    int Dificuldade;
    ESTADODEJOGO Estado_de_Jogo;

    //inicialização de estado de jogo
    Estado_de_Jogo.CovardeAtivado=0;
    Estado_de_Jogo.GreyskullAtivado=0;
    Estado_de_Jogo.MapaAtual=0;
    Estado_de_Jogo.RecargaAura=0;

    Introducao();//ARRUMAR CORES
    while(Sair==0)
    {
        Menu(&Selecao);
        switch(Selecao){
        case 'n':
        case 'N':   Dificuldade = SelecionarDificuldade(); //seleciona dificuldade 1-normal 2-dificil
                    if (Dificuldade == 1){//FACIL
                        Estado_de_Jogo.Dificuldade.VidasIniciais = 3;
                        Estado_de_Jogo.Dificuldade.DanoDaAura = 2;
                        Estado_de_Jogo.Dificuldade.RecargaAura = 2; //2 segundos
                    }
                    else if (Dificuldade == 2){//DIFICIL
                        Estado_de_Jogo.Dificuldade.VidasIniciais = 1;
                        Estado_de_Jogo.Dificuldade.DanoDaAura = 1;
                        Estado_de_Jogo.Dificuldade.RecargaAura = 5; //5 segundos
                    }

                        Estado_de_Jogo.VidasRestantes = Estado_de_Jogo.Dificuldade.VidasIniciais;
                        Estado_de_Jogo.MapaAtual=mapainicial;

                    LoopDeJogo(&Estado_de_Jogo, Dificuldade);

            break;

        case 'c':
        case 'C': Carregamento(&Estado_de_Jogo);
            break;

        case 's':
        case 'S': Salvamento(&Estado_de_Jogo);
            break;

        case 'q':
        case 'Q': Sair=1;
            break;

        case 'v':   LoopDeJogo(&Estado_de_Jogo, Dificuldade);
        case 'V': //Carregamento(); // com arquivo temp
            break;

        case 39: Cheat_Code(&Estado_de_Jogo.CovardeAtivado, &Estado_de_Jogo.GreyskullAtivado); //(APERTOU APOSTROFE '), ESCONDER NA RELEASE
        //função leitura de cheat code, exemplos : covarde = vida infinita, greyskull = aumento do dano da aura, camaleao = muda cor do jogador
            break;
        }
    }
    return (0);
    }
