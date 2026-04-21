#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include <time.h>
#include <string.h>

#define ALTU_MAPA 12
#define LARG_MAPA 240
#define SUB_MAPA 30
#define LARGURA 1400
#define ALTURA 800

typedef struct fase { //struct da fase
  float velo;
  char nome[100];
} fase_t ;

typedef struct { //struct para verifcar colisao
  float cordSE[2];
  float cordID[2];
} area_t;

float deseMapa( fase_t fase ); // roda o jogo tendo a struct fase como parametro
int leMapa(char *nome,char matr[ALTU_MAPA][LARG_MAPA]);  //converte um arquivo txt em matriz
int coliMapa(area_t pers, area_t obje);   // verifica se ouve colisao entre os blocos
void aleMatr(char matr1[ALTU_MAPA][LARG_MAPA],char matr2[ALTU_MAPA][LARG_MAPA], int lado); // deixa a matriz aleatoria
void iniMatr(char matr1[ALTU_MAPA][LARG_MAPA],char matr2[ALTU_MAPA][LARG_MAPA]); //  deixa o inicio da matriz sem ostaculos
int menu(void);
int menujogo(void);
void leaderboard(void);

int main() {
    //Declaração de váriaveis da 'int'
    int funcao = 0, fasejogo;
    float pontuacao, pega;
    fase_t fase1 = {2, "mapa1.txt"}, fase2 = {2.5, "mapa1.txt"}, fase3 = {3, "mapa1.txt"};
    char liberafase;

    //Cria janela
    InitWindow(LARGURA, ALTURA, "titulo");
    SetTargetFPS(60);

    //loop principal
    while (!WindowShouldClose() && funcao != 3) {
        FILE *arqlead = fopen("leaderboard.txt", "r+");
        if(arqlead == NULL) {
            printf("Erro ao abrir programa!!");
            return 0;
        }
        funcao = menu();
        //Após sair função menu, ele seleciona qual a prox função
        //Função que chma o leaderboard
        if (funcao == 2) {
            leaderboard();
            printf("leaderboard executado");
        }
        //Funcao que chama 'jogo'
        if (funcao == 1) {
            fasejogo = menujogo();
            if(fasejogo == 1) {
                pega = deseMapa(fase1);
                if(pega > 30000) {
                    fseek(arqlead, -1, SEEK_END);
                    fprintf(arqlead, "B");
                }
            }else if(fasejogo == 2) {
                pega = deseMapa(fase2);
                if(pega > 30000) {
                    fseek(arqlead, -1, SEEK_END);
                    fprintf(arqlead, "C");
                }
                fclose(arqlead);
            }else {
                pega = deseMapa(fase3);
                fclose(arqlead);
            }
            printf("Pontucao: %f", pega);
            printf ("A fase eh: %d", fasejogo);
            funcao = 0;
        }
    }
    CloseWindow();
    return 0;
}
//A função meu tem como objetivo selecionar a próxima função a ser executada
int menu(void) {
    int sair = 0;
    int x = 430, y = 530, z = 630, aux = x;

    while (sair == 0 && !WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(GRAY);

        DrawRectangle(0, 100, 1400, 150, RED);
        DrawText("Jetpack dos guri", 370, 125, 70, BLACK);
        DrawText("Jogar", 200, 420, 40, BLACK);
        DrawText("Leaderboard", 200, 520, 40, BLACK);
        DrawText("Sair", 200, 620, 40, BLACK);
        DrawCircle(130, aux, 12, BLACK);

        //Loop que modifica a posição do 'circulo'que seleciona a próxima função a ser executada
        if (IsKeyPressed(KEY_ENTER)) {
            sair = 1;
        }
        if (IsKeyPressed(KEY_UP) && aux != x) {
            if (aux == y) {
                aux = x;
            } else {
                aux = y;
            }
        }
        if (IsKeyPressed(KEY_DOWN) && aux != z) {
            if (aux == x) {
                aux = y;
            } else {
                aux = z;
            }
        }

        EndDrawing();
    }
    //Retorna a próxima função a ser executada
    if(aux == z) {
        aux = 3;
    }else if(aux == y){
        aux = 2;
    }else{
        aux = 1;
    }
    printf("%d", aux);
    return aux;
}

//Essa função lê o arquivo onde o leaderboard se encontra e mostra na tela
void leaderboard(void) {
    FILE *arq = fopen("leaderboard.txt", "r");

    if (arq == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    char linha[256];
    int y, y2, y3;
    int sair = 0;
    int linha_count = 0;

    while (sair == 0 && !WindowShouldClose()) {
        //Tela principal
        BeginDrawing();
        ClearBackground(GRAY);
        DrawRectangle(0, 40, 1400, 120, RED);
        DrawText("Leaderboard", 50, 50, 70, BLACK);
        DrawText("Clique enter para sair", 50, 720, 25, BLACK);
        DrawText("Nivel 1:", 200, 250, 30, BLACK);
        DrawText("Nivel 2:", 600, 250, 30, BLACK);
        DrawText("Nivel 3:", 1000, 250, 30, BLACK);
        y = 400;
        y2 = 400;
        y3 = 400;
        linha_count = 1;

        rewind(arq);
        //Escreve os nomes do TOP em relação a cada fase
        while (linha_count <= 15) {
            if (linha_count < 6) {
                fgets(linha, sizeof(linha), arq);
                linha[strcspn(linha, "\n")] = '\0';
                DrawText(linha, 200, y, 20, BLACK);
                y += 30;
            }
            if (linha_count < 11 && linha_count > 5) {
                fgets(linha, sizeof(linha), arq);
                linha[strcspn(linha, "\n")] = '\0';
                DrawText(linha, 600, y2, 20, BLACK);
                y2 += 30;
            }
            if (linha_count > 10) {
                fgets(linha, sizeof(linha), arq);
                linha[strcspn(linha, "\n")] = '\0';
                DrawText(linha, 1000, y3, 20, BLACK);
                y3 += 30;
            }
            linha_count++;
        }

        //Verifica o ENTER para sair do leaderboard
        if (IsKeyPressed(KEY_ENTER)) {
            sair = 1;
        }

        EndDrawing();
    }

    fclose(arq);
    return;
}
//Essa função serve para selecionar a fase que será jogada
int menujogo(void) {

    int x = 130, y = 530, z = 930;
    int sair = 0, aux = x, retorno, flagfase = 0;
    FILE *arq = fopen("leaderboard.txt", "r");
    char teste;
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return 0;
    }
    //Pega a última letra do leaderboard.txt e verifica qual fase está liberada
    fseek(arq, -1, SEEK_END);
    teste = fgetc(arq);
    printf("[%c]", teste);

    while (sair == 0 && !WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(GRAY);
        //Verifica se a fase está bloqueada
        if(flagfase == 1) {
            DrawText("**Fase bloqueada**", 580, 750, 20, BLACK);
        }
        //Tela principal
        DrawRectangle(0, 40, 1400, 120, RED);
        DrawText("Fases:", 50, 50, 70, BLACK);
        DrawRectangleGradientV(150, 220, 230, 480, RED, WHITE);
        DrawRectangleGradientV(550, 220, 230, 480, RED, WHITE);
        DrawRectangleGradientV(950, 220, 230, 480, RED, WHITE);
        DrawText("1", 250, 410, 150, BLACK);
        DrawText("2", 630, 410, 150, BLACK);
        DrawText("3", 1030, 410, 150, BLACK);
        DrawRectangleLinesEx((Rectangle){aux, 200, 270, 520}, 5, RED);

        //Assim que ENTER for pressionado, a fase é selecionada e passa pelas verificações
        if (IsKeyPressed(KEY_ENTER)) {
            if (aux == x) {
                retorno = 1;
                sair = 1;
            }else if (aux == y) {
                if(teste != 'A') {
                    retorno = 2;
                    sair = 1;
                }else {
                    flagfase = 1;
                }

            }else {
                if(teste == 'C'){
                    retorno = 3;
                    sair = 1;
                }else {
                    flagfase = 1;
                }
            }
        }
        //Loop que altera a "caixa" que seliciona a fase
        if (IsKeyPressed(KEY_LEFT) && aux != x) {
            if (aux == y) {
                aux = x;
            } else {
                aux = y;
            }
        }
        if (IsKeyPressed(KEY_RIGHT) && aux != z) {
            if (aux == x) {
                aux = y;
            } else {
                aux = z;
            }
        }

        fclose(arq);
        EndDrawing();
    }

    return retorno;
}

float deseMapa( fase_t fase){

  char mapa[ALTU_MAPA][LARG_MAPA] = {0}; //matriz base
  char mapa2[ALTU_MAPA][LARG_MAPA] = {0}; // matris da fase que é modificada
  int velo = fase.velo;

  aleMatr(mapa, mapa2, 1);
  iniMatr(mapa, mapa2);

  int pause_state = 0;
  int i =  0;
  int j = 0;
  float k = 0;

  float moeda = 0;
  float dista = 0;
  int cordx;
  int cordy;

  int most = 30;
  int tam_map = LARGURA / most;

  int tam_moe = tam_map / 2 ;
  int tam_cha = tam_map  ;
  int tam_las = tam_map  ;



  area_t cord_moe;
  area_t cord_cha;
  area_t cord_las;


  float x = most/3.5;
  float y = ALTU_MAPA/2;
  int  tam_per = tam_map/1.6;
  area_t cord_per;


  leMapa(fase.nome, mapa);
  aleMatr(mapa, mapa2, 0);
  iniMatr(mapa, mapa2);








    //inicio do loop
    for (k = 0; (!WindowShouldClose()); k += (0.1*velo)) {


      if(IsKeyPressed(KEY_P)) {
        pause_state = !pause_state;  // Alterna o estado de pausa ao pressionar P
        }

        // Verifica se o jogo está pausado
        if (!pause_state) {

        dista += (0.1 * velo);

        if (k >= LARG_MAPA) {  // reinicia k quando atinge o fim do mapa
            k = 0;

        }
        if ((k > ((LARG_MAPA / 4) * 3)   - most) && (k < ((LARG_MAPA / 4) * 3))){ // atualiza a primeira parte do mapa
          aleMatr(mapa, mapa2, 0);
        }
        if (k >= ((LARG_MAPA / 2)/2) - most && k <= ((LARG_MAPA / 2)/2)  ){ //atualiza a segunda parte do mapa
          aleMatr(mapa, mapa2, 1);
        }

        ClearBackground(RAYWHITE);
        BeginDrawing();

        // desenha duas dois trechos mapa para gerar o  loop
        for (int pass = 0; pass < 2; pass++) {
            float offset = (pass == 0) ? -k * tam_map : (LARG_MAPA - k) * tam_map;

            for (i = 0; i < LARG_MAPA; i++) {
                for (j = 0; j < ALTU_MAPA; j++) {
                    // Calcula a posição de desenho
                    int cordx = (i * tam_map) + offset;
                    int cordy = j * tam_map;

                    // Se o objeto está fora da tela, não desenha
                    if (cordx + tam_map < 0 || cordx > LARGURA) {
                        continue;
                    }

                    switch (mapa2[j][i]) {  // ao mesmo tempo que desenha verifica colisoes e aplica suas consequencias
                        case 'C':
                            DrawRectangle(cordx, cordy, tam_moe, tam_moe, YELLOW);
                            cord_moe.cordSE[0] = cordx;
                            cord_moe.cordSE[1] = cordy;
                            cord_moe.cordID[0] = cordx + tam_moe;
                            cord_moe.cordID[1] = cordy + tam_moe;
                            if (coliMapa(cord_per, cord_moe)) {
                                mapa2[j][i] = '0';
                                //printf("colidiu");
                                moeda++;
                            }
                            break;

                        case 'X':
                            DrawRectangle(cordx, cordy, tam_cha, tam_cha, GRAY);
                            cord_cha.cordSE[0] = cordx;
                            cord_cha.cordSE[1] = cordy;
                            cord_cha.cordID[0] = cordx + tam_cha;
                            cord_cha.cordID[1] = cordy + tam_cha;
                            break;

                        case 'Z':
                            DrawRectangle(cordx, cordy, tam_las, tam_las, RED);
                            cord_las.cordSE[0] = cordx;
                            cord_las.cordSE[1] = cordy;
                            cord_las.cordID[0] = cordx + tam_cha;
                            cord_las.cordID[1] = cordy + tam_cha;
                            if (coliMapa(cord_per, cord_las)){

                            printf("%f",(moeda * dista));
                            return (moeda * dista);
                            }

                            break;
                    }
                }
            }
        }


        if (IsKeyDown(KEY_SPACE) && ( cord_per.cordSE[1] > tam_map) ) {  //atualiza posição do personagem
            y -= 0.150 * velo;
        }

        if (cord_per.cordID[1]  < tam_map * (ALTU_MAPA- 1) ){
            y += 0.075 * velo;
        }

        cord_per.cordSE[0] = x * tam_map;
        cord_per.cordSE[1] = y * tam_map;
        cord_per.cordID[0] = (x * tam_map) + tam_per;
        cord_per.cordID[1] = (y * tam_map) + tam_per;
        DrawRectangle(x * tam_map, y * tam_map, tam_per, tam_per, BLACK);




        }else {
         k -= 0.1*velo; //mantem o personagem parado durante o pause
        }

        EndDrawing();
    }

    return 0;
}

int leMapa(char *nome , char matr[12][LARG_MAPA]){

  FILE* arq = fopen(nome, "r");
  if(arq == NULL){
    printf("erro ao abrir arquivo");
    return 1;
  }


  char part;
  int linh = 0;
  int colu = 0;

  while((part = fgetc(arq)) != EOF){

    switch(part){

      case '\n':
      linh++;
      colu = 0;
      break;

      case 'C':

      matr[linh][colu] =  part;
      break;

      case 'X':

      matr[linh][colu] =  part;
      break;

      case 'Z':

      matr[linh][colu] =  part;
      break;
    }

    colu++;

  }
  fclose(arq);
  return 0;
}

int coliMapa(area_t pers, area_t obje){

  if(((pers.cordID[0] < obje.cordSE[0]) || (pers.cordSE[0] > obje.cordID[0]) || ((pers.cordID[1] < obje.cordSE[1]) || (pers.cordSE[1] > obje.cordID[1] )))){
    return 0;
  }








  return 1;
}

void aleMatr(char matr1[ALTU_MAPA][LARG_MAPA],char matr2[ALTU_MAPA][LARG_MAPA], int lado){

  srand(time(NULL));



  char part;
  int i = 0;
  int j = 0;
  int x = 0;
  int fim =0;
  int num;
  if(lado == 1){

  x = LARG_MAPA / 2;
  fim = LARG_MAPA;
  }else{
  fim = LARG_MAPA/2;
  x = 0;
  }

  while (x < fim){

    num = (rand() % ((LARG_MAPA / SUB_MAPA )));

    i = ((num * SUB_MAPA ));

    j = 0;

    while(j < ALTU_MAPA){
      while(i < (SUB_MAPA*(num+1))){
        part = matr1[j][(i)];

          matr2[j][x] = part;
        i++;
        x++;

      }
    j++;
    i = (num * SUB_MAPA);
    x = x - SUB_MAPA;
    }
    x = x + SUB_MAPA;
    }

}

void iniMatr(char matr1[ALTU_MAPA][LARG_MAPA],char matr2[ALTU_MAPA][LARG_MAPA]){





  char part;
  int i = 0;
  int j = 0;






    j = 0;

    while(j < ALTU_MAPA){

      while(i < SUB_MAPA){
        part = matr1[j][(i)];


          matr2[j][i] = part;

        i++;


      }
    j++;
    i = 0;
    }


}
