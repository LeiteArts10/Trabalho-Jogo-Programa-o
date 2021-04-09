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

#define ENEMY_TYPE_ZOMBIE 5
#define ENEMY_TYPE_TROLL 6

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

typedef struct Jogador
{
    int PosX;
    int PosY;
    int Vidas;
} PLAYER;

typedef struct enemy
{
    int pos_x;
    int pos_y;
    int direction;
    int enemyType;
    int steps;
} ENEMY;

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
    printf("\t(N) Novo jogo\n");
    printf("\t(C) Carregar jogo salvo (se houver)\n");
    printf("\t(S) Salvar jogo\n");
    printf("\t(Q) Sair do jogo\n");
    printf("\t(V) Voltar\n");
    do
        {
        scanf(" %c", &Resposta);
        Resposta=toupper(Resposta);
        }
    while(Resposta=='N' || Resposta=='C' || Resposta=='S' || Resposta=='Q' || Resposta=='V');
    *Selecaopont=Resposta;
    }

int LerMapa(char* NomedoMapa, char game_map[LINHAS][COLUNAS], int Linhas, int Colunas, int maxEnemies, int *qtdEnemies, ENEMY *enemies, PLAYER *Jogador){
    int I,J;
    FILE *Arquivo;
    int Status = 0;
    *qtdEnemies = 0;
    if((Arquivo=fopen(NomedoMapa, "r")))
    {
        for (I=0; I<Linhas; I++)
        {
            for (J=0; J<Colunas; J++)
            {
                game_map[I][J]= getc(Arquivo);
                //printf("%d,%d,%c\n",i,j,game_map[i][j]);
                //system("pause");
                switch(game_map[I][J]){
                    case 'z':
                    case 'Z': //*qtdEnemies = *qtdEnemies+1;
                              enemies[*qtdEnemies].pos_x = J;
                              enemies[*qtdEnemies].pos_y = I;
                              enemies[*qtdEnemies].direction = 0;
                              enemies[*qtdEnemies].steps = 0;
                              enemies[*qtdEnemies].enemyType = ENEMY_TYPE_ZOMBIE;
                              *qtdEnemies = *qtdEnemies+1;
                              break;

                    case 't':
                    case 'T': //*qtdEnemies = *qtdEnemies+1;
                              enemies[*qtdEnemies].pos_x = J;
                              enemies[*qtdEnemies].pos_y = I;
                              enemies[*qtdEnemies].direction = 0;
                              enemies[*qtdEnemies].steps = 0;
                              enemies[*qtdEnemies].enemyType = ENEMY_TYPE_TROLL;
                              *qtdEnemies = *qtdEnemies+1;
                              break;
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

int colision(char game_map[LINHAS][COLUNAS], PLAYER *player){

    int col = 5;

    switch(game_map[player -> pos_y][player -> pos_x]){
        case(PAREDE): col = 0; //se a posição a ser ocupada pelo jogador é uma parade ele retorna um código 0 para a função de movimentação
                           break;

        case(OBSTACLE_SYMBOL): col = 1; //se a posição a ser ocupada pelo jogador é um obstáculo ele retorna um código 1 para a função de movimentação
                               break;

        case(GOLD_SYMBOL): col = 2; //se a posição a ser ocupada pelo jogador é o tesouro ele retorna um código 2 para a função de movimentação
                           break;

        case(ZOMBIE_SYMBOL): col = 3; //se a posição a ser ocupada pelo jogador é um zombie ele retorna um código 3 para a função de movimentação
                             break;

        case(TROLL_SYMBOL): col = 4; //se a posição a ser ocupada pelo jogador é um troll ele retorna um código 4 para a função de movimentação
                            break;
        default: break;
    }

    return col;
}

int monster_select(){

    int n;
    n = rand()%4;

    return n;

}


void move_enemies(char game_map[LINHAS][COLUNAS], ENEMY enemies[MAX_ENEMIES], int qnt, PLAYER *player, int *flag){

    int i, x = 0, y = 0;
    char symbol;
    printf("\nVidas: %d", player->vidas+1);

    for(i = 0; i < qnt; i++)
    {
        switch(enemies[i].direction){
            case 0: y = -1;
                    x = 0;
                    break;
            case 1: x = -1;
                    y = 0;
                    break;
            case 2: y = 1;
                    x = 0;
                    break;
            case 3: x = 1;
                    y = 0;
                    break;
        }
        symbol = game_map[enemies[i].pos_y + y][enemies[i].pos_x + x];
        if((symbol == OBSTACLE_SYMBOL && enemies[i].enemyType == ENEMY_TYPE_TROLL) || (enemies[i].steps > 4) || symbol == PAREDE){
            while(enemies[i].direction == rand()%4){
                enemies[i].direction = rand()%4;
            }
            enemies[i].steps = 0;
        }
        else if(enemies[i].enemyType == ENEMY_TYPE_ZOMBIE && game_map[enemies[i].pos_y][enemies[i].pos_x] == OBSTACLE_SYMBOL){

            game_map[enemies[i].pos_y][enemies[i].pos_x] = OBSTACLE_SYMBOL;
            (enemies[i].pos_y) = (enemies[i].pos_y) + y;
            (enemies[i].pos_x) = (enemies[i].pos_x) + x;
            enemies[i].steps = enemies[i].steps + 1;
        }
        else if((enemies[i].enemyType == ENEMY_TYPE_ZOMBIE && game_map[enemies[i].pos_y][enemies[i].pos_x] == GOLD_SYMBOL) || (enemies[i].enemyType == ENEMY_TYPE_TROLL && game_map[enemies[i].pos_y][enemies[i].pos_x] == GOLD_SYMBOL)){
            game_map[enemies[i].pos_y][enemies[i].pos_x] = GOLD_SYMBOL;
            (enemies[i].pos_y) = (enemies[i].pos_y) + y;
            (enemies[i].pos_x) = (enemies[i].pos_x) + x;
            enemies[i].steps = enemies[i].steps + 1;

        }
        else if(game_map[enemies[i].pos_y][enemies[i].pos_x] == game_map[player -> pos_y][player -> pos_x]){
            *flag = 1;
            switch(enemies[i].enemyType){
                case ENEMY_TYPE_TROLL: game_map[player -> pos_y][player -> pos_x] = TROLL_SYMBOL;
                                       break;
                case ENEMY_TYPE_ZOMBIE: game_map[player -> pos_y][player -> pos_x] = ZOMBIE_SYMBOL;
                                        break;
            }

        }
        else{
            game_map[enemies[i].pos_y][enemies[i].pos_x] = BACKGROUND_SYMBOL;
            (enemies[i].pos_y) = (enemies[i].pos_y) + y;
            (enemies[i].pos_x) = (enemies[i].pos_x) + x;
            enemies[i].steps = enemies[i].steps + 1;
        }


        switch(enemies[i].enemyType){
            case ENEMY_TYPE_TROLL: if(game_map[enemies[i].pos_y][enemies[i].pos_x] == GOLD_SYMBOL){
                                        game_map[enemies[i].pos_y][enemies[i].pos_x] = GOLD_SYMBOL;
                                    }
                                   game_map[enemies[i].pos_y][enemies[i].pos_x] = TROLL_SYMBOL;
                                   break;
            case ENEMY_TYPE_ZOMBIE: if(game_map[enemies[i].pos_y][enemies[i].pos_x] == GOLD_SYMBOL){
                                        game_map[enemies[i].pos_y][enemies[i].pos_x] = GOLD_SYMBOL;
                                    }
                                    else if(game_map[enemies[i].pos_y][enemies[i].pos_x] != OBSTACLE_SYMBOL)
                                        game_map[enemies[i].pos_y][enemies[i].pos_x] = ZOMBIE_SYMBOL;
                                    break;
        }

void PintaMapa(char MapadoJogo[LINHAS][COLUNAS], int Linhas, int Colunas){
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

void HideCursor()
{
  CONSOLE_CURSOR_INFO cursor = {1, FALSE};
  SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor);
}

void Execucao(int Mapa, int Vidas)
    {
    int Acao, NumerodeMapas=0, I;
    char MapadoJogo[LINHAS][COLUNAS];
    int qtdEnemies;
    int flag_vidas = 0;
    int col;
    char tecla = 'u';
    int timerint = 0;
    ENEMY enemies[MAXIMODEMONSTROS1];
    PLAYER Jogador;
    clock_t timer, timerend, timebegin;
    timebegin = clock();
    timer = clock();
    srand(time(NULL));

    if(LerMapa("mapa1.txt", MapadoJogo, LINHAS, COLUNAS, MAXIMODEMONSTROS1, &qtdEnemies, enemies, &Jogador)){

                PintaMapa(MapadoJogo, LINHAS, COLUNAS);
                HideCursor();

                if(flag_vidas == 1){
                    Jogador.Vidas--;
                }
                flag_vidas = 0;
                for(I = 0; I < qtdEnemies; I++)
                {
                    enemies[I].direction = monster_select();
                }
                while((col = (colision(MapadoJogo, &Jogador))) != 1 && (col = (colision(MapadoJogo, &Jogador))) != 2 && (col = (colision(MapadoJogo, &Jogador))) != 3 && (col = (colision(MapadoJogo, &Jogador))) != 4){
                    if(kbhit()){
                        tecla = getch();

                    }
                    else{
                        tecla = 'u';
                    }
                    timerend = clock();
                    if(((float)(timerend - timer)/ CLOCKS_PER_SEC)>0.2){
                        move_enemies(MapadoJogo, enemies, qtdEnemies, &Jogador, &flag_vidas);
                        timerint++;
                        timer = clock();
                    }
                    switch(tecla = toupper(tecla)){
                        case 'W': move(MapadoJogo, &Jogador, 0, -1, &flag_vidas);
                                  break;
                        case 'A': move(MapadoJogo, &Jogador, -1, 0, &flag_vidas);
                                  break;
                        case 'S': move(MapadoJogo, &Jogador, 0, +1, &flag_vidas);
                                  break;
                        case 'D': move(MapadoJogo, &Jogador, +1, 0, &flag_vidas);
                                  break;
                    }
                    PintaMapa(MapadoJogo, LINHAS, COLUNAS);

                }
                //printMap(game_map, LINHAS, COLUNAS);
            }else{
                printf("Erro na leitura do mapa\n");
            }



    // faz execucao do jogo, os parametros vao definir se vai rodar o jogo do zero ou de um ponto

     //se a qualquer momento pressionado TAB , salva o jogo em 'temporario', e encerra. devido ao laco cai em menu

     //  tambem deve sair ao terminar os mapas ou fim das vidas

    }


void Carregamento()
{

}



void Salvamento()
{

}




int main()
    {
    int I;
    char MapadoJogo[LINHAS][COLUNAS];
    PLAYER Jogador;
    ENEMY enemies[MAXIMODEMONSTROS1];
    int qtdEnemies;
    int Mantenedora=1;  // variavel que se verdadeira mantem o programa rodando
    char Selecao;  // selecao do menu
    Introducao();  // mostra introducao
    Execucao(1, NUMERODEVIDAS);  // execução inicial, jogo zerado
    while(Mantenedora)  // laco da execucao, cai aqui apos 1 TAB e continua ate Pressionado sair
    {
     Menu(&Selecao);  // mostra menu e carrega selecao
     switch(Selecao= toupper(Selecao)){ // switch na selecao
    case 'N':  // zera o jogo e carrega execucao
        Execucao(1, NUMERODEVIDAS); break;

    case 'C':  // busca os dados de um jogo salvo e carrega execucao com esses dados

        Carregamento();  // talves precise de uma funcao para carregar, não sei  ainda
        Execucao(1, NUMERODEVIDAS);

    case 'S': Salvamento();  break;
        //torna o jogo salvo ao pressionar TAB , 'temporario' um jogo salvo

    case 'Q': Mantenedora=0;
                break;   // atribui 0 a mantenedora , encerra o laco e fecha o jogo

    case 'V':  // carrega a execucao com os dados 'temporario' salvos ao pressionar TAB para cair no menu
        Execucao(1, NUMERODEVIDAS); break;

    }
    }
    return 0;
    }
