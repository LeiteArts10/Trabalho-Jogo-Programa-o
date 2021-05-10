#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main(){

    int timeAtual, timeUltimoUso = clock();
    char Tecla;
    int sair =0;


    while(sair==0){

        Tecla = getch();
        switch(Tecla)
        {
            case 'w':
            case 'W': //Move(MapadoJogo, &Jogador, 0, -1, &Controle);
            break;
            case 'a':
            case 'A': //Move(MapadoJogo, &Jogador, -1, 0, &Controle);
            break;
            case 's':
            case 'S': //Move(MapadoJogo, &Jogador, 0, +1, &Controle);
            break;
            case 'd':
            case 'D': //Move(MapadoJogo, &Jogador, +1, 0, &Controle);
            break;
            case 'f':
            case 'F':   timeAtual = clock();
                        if (timeAtual > timeUltimoUso+3000){
                            printf("\nAuraUtilizada\n");
                            timeUltimoUso = clock();
                        }
                        else {
                            printf("Aura Sendo Carregada");
                        }
                    break;
            case 9 :clrscr();
            break;

        }
    }

return 0;
}
