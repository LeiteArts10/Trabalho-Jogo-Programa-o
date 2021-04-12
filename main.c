#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <conio.h>
#include <conio2.h>
#include <windows.h>
#include <time.h>

#define NUMERODEVIDAS 3

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

#define COR_PAREDE 0
#define COR_OBSTACULO 3
#define COR_JOGADOR 5
#define COR_TESOURO 7
#define COR_ZUMBI 9
#define COR_TROLL 12
#define COR_VAZIO 14

#define MAXIMODEMONSTROS1 15

typedef struct Jogador  // definicoes do jogador
{
    int PosX;
    int PosY;
    int Vidas;
} PLAYER;

typedef struct Inimigo  // definicoes dos inimigos
{
    int PosX;
    int PosY;
    int Direcao;
    int Tipo;
    int steps;
} INIMIGO;

void Introducao()  // mensagem de introducao
    {
     printf("\t\t\tBem vindo ao jogo.\n\n\tNeste jogo voce representa um mago que tem como \n\tobjetivo recuperar o tesouro de seu povo.\n\n");
     printf("\tPara isso devera atravesar todos os niveis superando os obstaculos \n\tdo calabouco.\n\n\tEntre os obstaculos estao trols e zumbi que podem \n\tdevora-lo e espinhos que podem machuca-lo.\n\n");
     printf("\tAlem de superar os obstaculos, pode marcar pontos matando os monstros \n\tutilizando sua aura magica,\n\tseu super poder.\n\n");
     printf("Para fazer vai ter 3 vidas para atravessar todas as salas do calabouco.\n");
     printf("\tComandos:\n");
     printf("\tMovimento\t\tW/S/A/D\n\tAura Magica\t\tF\n\tMenu\t\t\tTAB\n\n");
     printf("\t\t\t\t\tBoa Sorte!!!\n\n");
     printf("Aperte qualquer tecla");
     fflush(stdin);
     getchar();
     clrscr();
    }

void Menu(char *Selecaopont) // mostra menu e retorna resposta
    {
    char Resposta;
    printf("\n\t(N) Novo jogo\n");
    printf("\t(C) Carregar jogo salvo (se houver)\n");
    printf("\t(S) Salvar jogo\n");
    printf("\t(Q) Sair do jogo\n");
    printf("\t(V) Voltar\n");
    do
        {
        scanf(" %c", &Resposta);
        Resposta=toupper(Resposta);
        }
    while(Resposta!='N' && Resposta!='C' && Resposta!='S' && Resposta!='Q' && Resposta!='V');
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
                    case 'z':
                    case 'Z': if(*QuantInimigos<=MaximodeInimigos){
                              Inimigo[*QuantInimigos].PosX = J;
                              Inimigo[*QuantInimigos].PosY = I;
                              Inimigo[*QuantInimigos].Direcao = 0;
                              Inimigo[*QuantInimigos].steps = 0;
                              Inimigo[*QuantInimigos].Tipo = TIPO_ZUMBI;
                              *QuantInimigos = *QuantInimigos+1;
                              break;
                            }
                    case 't':
                    case 'T': if(*QuantInimigos<=MaximodeInimigos){
                              Inimigo[*QuantInimigos].PosX = J;
                              Inimigo[*QuantInimigos].PosY = I;
                              Inimigo[*QuantInimigos].Direcao = 0;
                              Inimigo[*QuantInimigos].steps = 0;
                              Inimigo[*QuantInimigos].Tipo = TIPO_TROLL;
                              *QuantInimigos = *QuantInimigos+1;
                              break;
                            }
                    case 'j':
                    case 'J': Jogador -> PosX = J;
                              Jogador -> PosY = I;
                              break;
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
    }

}


int SelecionaDirecao(){  // selecione direcao do monstro

    int N;
    N = rand()%4;

    return N;

}


void MoveInimigos(char MapadoJogo[LINHAS][COLUNAS], INIMIGO Inimigos[MAXIMODEMONSTROS1], int Quant, PLAYER *Jogador, int *Controle){

    int I, X = 0, Y = 0;
    char Simbolo;
    printf("\nVidas restantes: %d", Jogador->Vidas);

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
            }
            textbackground(Cor);
            gotoxy(X+1, Y+1);
            cprintf(" ");
        }
    }
}

void HideCursor()   // acho que apaga o ponteiro
{
  CONSOLE_CURSOR_INFO cursor = {1, FALSE};
  SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor);
}

void  Execucao(int Mapa, int Vidas)
    {
    int I;
    char MapadoJogo[LINHAS][COLUNAS];
    int QuantInimigos;
    int Controle = 0;
    int Colisao=0;
    char Tecla = 'u';
    int timerint = 0;
    INIMIGO Inimigos[MAXIMODEMONSTROS1];
    PLAYER Jogador;
    clock_t timer, timerend, timebegin;
    timebegin = clock();
    timer = clock();
    srand(time(NULL));
    Jogador.Vidas=Vidas;

    while(Jogador.Vidas > 0 && Colisao != 2)
    {
        if(LerMapa("mapa1.txt", MapadoJogo, LINHAS, COLUNAS, MAXIMODEMONSTROS1, &QuantInimigos, Inimigos, &Jogador))
        {
            PintaMapa(MapadoJogo, LINHAS, COLUNAS);
            HideCursor();

            if(Controle == 1)
            {
                Jogador.Vidas--;
            }
            Controle = 0;
            for(I = 0; I < QuantInimigos; I++)
            {
                Inimigos[I].Direcao = SelecionaDirecao();
            }
            while((Colisao = (ControledeColisao(MapadoJogo, &Jogador))) != 1 && (Colisao = (ControledeColisao(MapadoJogo, &Jogador))) != 2 && (Colisao = (ControledeColisao(MapadoJogo, &Jogador))) != 3 && (Colisao = (ControledeColisao(MapadoJogo, &Jogador))) != 4)
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
                    MoveInimigos(MapadoJogo, Inimigos, QuantInimigos, &Jogador, &Controle);
                    timerint++;
                    timer = clock();
                }
                switch(Tecla = toupper(Tecla))
                {
                    case 'W': Move(MapadoJogo, &Jogador, 0, -1, &Controle);
                            break;
                    case 'A': Move(MapadoJogo, &Jogador, -1, 0, &Controle);
                            break;
                    case 'S': Move(MapadoJogo, &Jogador, 0, +1, &Controle);
                            break;
                    case 'D': Move(MapadoJogo, &Jogador, +1, 0, &Controle);
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
    if(Colisao == 2)
    {
        clrscr();
        gotoxy(20,10);
        textcolor(GREEN);
        printf("VICTORY");
    }
    else
    {
        clrscr();
        gotoxy(20,10);
        textcolor(RED);
        printf("GAME OVER");
    }

    Sleep(200);
    clrscr();
    }


void Carregamento()
{

}

void Salvamento()
{

}




int main()
    {
    char MapadoJogo[LINHAS][COLUNAS];
    int Sair=0;
    char Selecao;
    Introducao();
    Execucao(1, NUMERODEVIDAS);
    while(Sair==0)
    {
     Menu(&Selecao);
     switch(Selecao){
    case 'N':
        Execucao(1, NUMERODEVIDAS); break;

    case 'C':

        Carregamento();
        Execucao(1, NUMERODEVIDAS);

    case 'S': Salvamento();  break;

    case 'Q': Sair=1;
                break;

    case 'V':
        Execucao(1, NUMERODEVIDAS); break;
    }

    }
    return (0);
    }

