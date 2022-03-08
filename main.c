#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#define largura 640
#define altura 640
#define peca_largura 80
#define peca_altura 80
//Declaração de variáveis universais
bool movimento, clique;
int jogador, posicao_x, posicao_y, i, j, ii, jj, i_f, j_f, x, y, xx, yy, quadrado_clicado_x, quadrado_clicado_y;
int quadrado_clicado_x_f, quadrado_clicado_y_f, mouse_x, mouse_y, erro, jogada, q_p, q_b;
int tabuleiro[8][8] = {};

ALLEGRO_DISPLAY *janela = NULL;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_BITMAP *peca_azul_normal = NULL;
ALLEGRO_BITMAP *peca_rosa_normal = NULL;
ALLEGRO_BITMAP *peca_azul_dama   = NULL;
ALLEGRO_BITMAP *peca_rosa_dama   = NULL;
ALLEGRO_BITMAP *menu1            = NULL;
ALLEGRO_BITMAP *menu2            = NULL;
ALLEGRO_BITMAP *menu3            = NULL;
ALLEGRO_BITMAP *mouse            = NULL;
ALLEGRO_AUDIO_STREAM *musica     = NULL;
ALLEGRO_SAMPLE *sample           = NULL;
ALLEGRO_SAMPLE *contagem         = NULL;
ALLEGRO_SAMPLE *fail             = NULL;
ALLEGRO_SAMPLE *tururu           = NULL;
ALLEGRO_AUDIO_STREAM *vitoria    = NULL;
ALLEGRO_BITMAP *destab           = NULL;
ALLEGRO_BITMAP *azulv            = NULL;
ALLEGRO_BITMAP *azulv2           = NULL;
ALLEGRO_BITMAP *azulv3           = NULL;
ALLEGRO_BITMAP *rosav            = NULL;
ALLEGRO_BITMAP *rosav2           = NULL;
ALLEGRO_BITMAP *rosav3           = NULL;
ALLEGRO_BITMAP *tela_regras      = NULL;
ALLEGRO_BITMAP *tela_regras_2    = NULL;
ALLEGRO_BITMAP *cont3            = NULL;
ALLEGRO_BITMAP *cont2            = NULL;
ALLEGRO_BITMAP *cont1            = NULL;
ALLEGRO_BITMAP *cont0            = NULL;
ALLEGRO_BITMAP *contgo           = NULL;

void error_msg(char *text)
{
   al_show_native_message_box(al_get_current_display(),"ERRO",
            "Movimento Invalido", text, NULL, ALLEGRO_MESSAGEBOX_ERROR);

}
void sair(char *text)
{
   al_show_native_message_box(al_get_current_display(),"ADEUS",
            "Entao isso e um adeus? :c", text, NULL, ALLEGRO_MESSAGEBOX_ERROR);

}
void mouse_clicado(bool draw, int quadrado_clicado_x, int quadrado_clicado_y)
{
            int x = posicao_x;
            int y = posicao_y;
            int pretox = quadrado_clicado_x*80;
            int pretoy = quadrado_clicado_y*80;

            for(i = 0; i < 8; i++){
                if ((x >= i*80) && (x <= (i)*80+78)){
                    xx = i;
            for(j = 0; j <8; j++){
                if ((y >= j*80) && (y <= (j)*80+78)){
                    yy = j;
            }}}}
            jog(quadrado_clicado_x, quadrado_clicado_y);

            if (!draw){
                    if ((((xx == 0)||(xx == 2) ||(xx == 4)||(xx == 6))&&((yy == 0)||(yy == 2) ||(yy == 4)||(yy == 6)))||
                        (((xx == 1)||(xx == 3) ||(xx == 5)||(xx == 7))&&((yy == 1)||(yy == 3) ||(yy == 5)||(yy == 7)))){
                        if (jogador == 1){
                            al_draw_filled_rectangle(pretox, pretoy, pretox+78, pretoy+78, al_map_rgb(0, 0, 0));
                            al_draw_bitmap(peca_azul_normal, x, y, 0);
                            al_draw_bitmap(mouse, x, y, 0);
                        }
                        else if (jogador == 2){
                            al_draw_filled_rectangle(pretox, pretoy, pretox+78, pretoy+78, al_map_rgb(0, 0, 0));
                            al_draw_bitmap(peca_rosa_normal, x, y, 0);
                            al_draw_bitmap(mouse, x, y, 0);
                        }
                        else if (jogador == 3){
                            al_draw_filled_rectangle(pretox, pretoy, pretox+78, pretoy+78, al_map_rgb(0, 0, 0));
                            al_draw_bitmap(peca_azul_dama, x, y, 0);
                            al_draw_bitmap(mouse, x, y, 0);
                        }
                        else if (jogador == 4){
                            al_draw_filled_rectangle(pretox, pretoy, pretox+78, pretoy+78, al_map_rgb(0, 0, 0));
                            al_draw_bitmap(peca_rosa_dama, x, y, 0);
                            al_draw_bitmap(mouse, x, y, 0);
                        }

                    }
                    else{
                        if (jogador == 1){
                            al_draw_filled_rectangle(pretox, pretoy, pretox+78, pretoy+78, al_map_rgb(0, 0, 0));
                            al_draw_bitmap(peca_azul_normal, x, y, 0);
                            al_draw_bitmap(mouse, x, y, 0);
                        }
                        else if (jogador == 2){
                            al_draw_filled_rectangle(pretox, pretoy, pretox+78, pretoy+78, al_map_rgb(0, 0, 0));
                            al_draw_bitmap(peca_rosa_normal, x, y, 0);
                            al_draw_bitmap(mouse, x, y, 0);
                        }
                        else if (jogador == 3){
                            al_draw_filled_rectangle(pretox, pretoy, pretox+78, pretoy+78, al_map_rgb(0, 0, 0));
                            al_draw_bitmap(peca_azul_dama, x, y, 0);
                            al_draw_bitmap(mouse, x, y, 0);
                        }
                        else if (jogador == 4){
                            al_draw_filled_rectangle(pretox, pretoy, pretox+78, pretoy+78, al_map_rgb(0, 0, 0));
                            al_draw_bitmap(peca_rosa_dama, x, y, 0);
                            al_draw_bitmap(mouse, x, y, 0);
                        }
                    }
            }
            else
            {
                al_draw_bitmap(mouse, x, y, 0);
            }
}

void desenho_tabuleiro()
{
    al_flip_display();
    al_draw_bitmap(destab, 0, 0, 0);
}

void desenho_pecas()
{
    i = quadrado_clicado_x;
    j = quadrado_clicado_y;

    for(i=0;i<8;i++)
    {
        for(j=0;j<8;j++)
        {
            if(tabuleiro[j][i]==1)
            {
                al_draw_bitmap(peca_azul_normal, i*80, j*80, 0);
            }
            if(tabuleiro[j][i]==2)
            {
                al_draw_bitmap(peca_rosa_normal, i*80, j*80, 0);
            }
            if(tabuleiro[j][i]==3)
            {
                al_draw_bitmap(peca_azul_dama, i*80, j*80, 0);
            }
            if(tabuleiro[j][i]==4)
            {
                al_draw_bitmap(peca_rosa_dama, i*80, j*80, 0);
            }
        }
    }

}

int jog(int quadrado_clicado_x, int quadrado_clicado_y)
{
    i = quadrado_clicado_x;
    j = quadrado_clicado_y;

            if(tabuleiro[j][i]==1)
            {
                jogador = 1;
            }
            if(tabuleiro[j][i]==2)
            {
                jogador = 2;
            }
            if(tabuleiro[j][i]==3)
            {
                jogador = 3;
            }
            if(tabuleiro[j][i]==4)
            {
                jogador = 4;
            }

return jogador;
}

int posicao_inicial_azul()
{
    i = quadrado_clicado_x;
    j = quadrado_clicado_y;

//-------------------------------------condição para peça selecionada ser uma peça branca-------------------------------------
            if((tabuleiro[j][i]==2)||(tabuleiro[j][i]==4))
            {
                error_msg("A coordenada que voce selecionou esta ocupada por uma peca rosa");
                movimento = false;
                return -1;
            }
//-------------------------------------condição para selecionar outra peça-------------------------------------
            if((i==i_f)&&(j==j_f))
            {
                movimento = true;
                return -1;
            }
//-------------------------------------condição quando não tiver peça na posição digitada-------------------------------------
            if((tabuleiro[j][i]!=1)&&(tabuleiro[j][i]!=3))
            {
                error_msg("A coordenada que voce selecionou nao possui peca");
                movimento = false;
                return -1;
            }
//-------------------------------------condição para quando não tem como se mover mesmo com peça rosa-------------------------------------
            if(((tabuleiro[j+1][i-1]==2)||(tabuleiro[j+1][i-1]==4))&&((tabuleiro[j-1][i-1]==2)||(tabuleiro[j-1][i-1]==4)))
            {
                if((tabuleiro[j+2][i-2]!=0)&&(tabuleiro[j-2][i-2]!=0))
                {
                    error_msg("A peca selecionada nao pode se mover para nenhuma direcao");
                    movimento = false;
                    return -1;
                }
            }
            if (tabuleiro[j][i]==3)
            {
                if(((tabuleiro[j+1][i+1]==2)||(tabuleiro[j+1][i+1]==4))&&((tabuleiro[j-1][i+1]==2)||(tabuleiro[j-1][i+1]==4)))
                {
                    if((tabuleiro[j+2][i+2]!=0)&&(tabuleiro[j-2][i+2]!=0))
                    {
                        error_msg("A peca selecionada nao pode se mover para nenhuma direcao");
                        movimento = false;
                        return -1;
                    }
                }
            }
//-------------------------------------condição para selecionar peça sem peças azuis na frente-------------------------------------
            if(((tabuleiro[j-1][i-1]==1)||(tabuleiro[j-1][i-1]==3))&&
                ((tabuleiro[j+1][i-1]==1)||(tabuleiro[j+1][i-1]==3))&&
                ((tabuleiro[j-1][i+1]==1)||(tabuleiro[j-1][i+1]==3))&&
                ((tabuleiro[j+1][i+1]==1)||(tabuleiro[j+1][i+1]==3)))
            {
                error_msg("A peca selecionada nao pode se mover para nenhuma direcao");
                movimento = false;
                return -1;
            }
//-------------------------------------condição para limitar o tabuleiro-------------------------------------
            if(((i+1)>=8)&&((j-1)<0))
            {
                if ((tabuleiro[1][6]==1)||(tabuleiro[1][6]==3))
                {
                    error_msg("A coordenada que voce selecionou nao pode se mover");
                    movimento = false;
                    return -1;
                }
                else if ((tabuleiro[1][6]==2)||(tabuleiro[1][6]==4))
                {
                    if (tabuleiro[2][5]!=0)
                    {
                        error_msg("A coordenada que voce selecionou nao pode se mover");
                        movimento = false;
                        return -1;
                    }
                }
            }
            if(((i-1)<0)&&((j+1>=8)))
            {
                if ((tabuleiro[6][1]==1)||(tabuleiro[6][1]==3))
                {
                    error_msg("A coordenada que voce selecionou nao pode se mover");
                    movimento = false;
                    return -1;
                }
                else if ((tabuleiro[6][1]==2)||(tabuleiro[6][1]==4))
                {
                    if (tabuleiro[5][2]!=0)
                    {
                        error_msg("A coordenada que voce selecionou nao pode se mover");
                        movimento = false;
                        return -1;
                    }
                }
            }


    movimento = true;
}

int posicao_final_azul()
{
    j_f = quadrado_clicado_x_f;
    i_f = quadrado_clicado_y_f;
    i = quadrado_clicado_y;
    j = quadrado_clicado_x;
//-------------------------------------condição para peça não ultrapassar os limites do tabuleiro-------------------------------------
        if((j_f>=8)||(i_f>=8)||(j_f<0)||(i_f<0))
        {
            error_msg("Movimento invalido, a posicao selecionada ultrapassa os limites do tabuleiro");
            movimento = true;
            return -1;
        }
//-------------------------------------condição para peça não comer peças com outra atrás-------------------------------------
        if((tabuleiro[i_f][j_f]==2)||(tabuleiro[i_f][j_f]==4))
        {
            if(((i_f-1)==i)&&((j_f+1)==j))
            {
                if (tabuleiro[i_f+1][j_f-1]!=0)
                {
                    error_msg("Movimento invalido, a posicao selecionada possui outra peca igual atras");
                    movimento = true;
                    return -1;
                }
            }
            if(((i_f-1)==i)&&((j_f-1)==j))
            {
                if (tabuleiro[i_f+1][j_f+1]!=0)
                {
                    error_msg("Movimento invalido, a posicao selecionada possui outra peca igual atras");
                    movimento = true;
                    return -1;
                }
            }
            if(tabuleiro[i][j]==3)
            {
                if(((i_f+1) == i)&&((j_f-1) == j))
                {

                    if (tabuleiro[i_f-1][j_f+1]!=0)
                    {
                        error_msg("Movimento invalido, a posicao selecionada possui outra peca igual atras");
                        movimento = true;
                        return -1;
                    }
                }
                if(((i_f+1) == i)&&((j_f+1) == j))
                {
                    if (tabuleiro[i_f-1][j_f-1]!=0)
                    {
                        error_msg("Movimento invalido, a posicao selecionada possui outra peca igual atras");
                        movimento = true;
                        return -1;
                    }
                }
            }
        }
//-------------------------------------condição para peça normal só ir para frente e não para trás-------------------------------------
        if(tabuleiro[i][j]==1)
        {
            if(i_f<=i)
            {
                error_msg("Movimento invalido, a peca so pode se mover para frente");
                movimento = true;
                return -1;
            }
        }
        else if(tabuleiro[i][j]==3)
        {
            if((i_f<=i)&&(i_f>=i))
            {
                error_msg("Movimento invalido, a peca so pode se mover na diagonal");
                movimento = true;
                return -1;
            }
        }
//-------------------------------------condição para peça não andar mais de uma linha-------------------------------------
        if(i_f>i+1)
        {
            error_msg("Movimento invalido, a peca so pode andar uma linha");
            movimento = true;
            return -1;
        }
//-------------------------------------condição para peça só andar na diagonal-------------------------------------
        if((i_f==(i-1))||(i_f==(i+1)))
        {
            if((j_f!=j+1)&&(j_f!=j-1))
            {
                error_msg("Movimento invalido, a peca so pode se mover na diagonal");
                movimento = true;
                return -1;
            }
        }
//-------------------------------------condição para peça não andar mais de uma coluna-------------------------------------
        if((j_f>j+1)||(j_f<j-1))
        {
            error_msg("Movimento invalido, a peca so pode andar uma coluna");
            movimento = true;
            return -1;
        }
//-------------------------------------condição para peça nao comer uma peça do mesmo tipo-------------------------------------
        if((tabuleiro[i_f][j_f]==1)||(tabuleiro[i_f][j_f]==3))
        {
            error_msg("Movimento invalido, a peca selecionada nao pode comer uma peca do mesmo tipo");
            movimento = true;
            return -1;
        }

        movimento = false;
        jogada = 2;
}

int posicao_inicial_rosa()
{
        j = quadrado_clicado_x;
        i = quadrado_clicado_y;

//-------------------------------------condição para peça selecionada ser uma peça preta-------------------------------------
            if((tabuleiro[i][j]==1)||(tabuleiro[i][j]==3))
            {
                error_msg("A coordenada que voce selecionou esta ocupada por uma peca azul");
                movimento = false;
                return -1;
            }
//-------------------------------------condição quando não tiver peça na posição digitada-------------------------------------
            if((tabuleiro[i][j]!=2)&&(tabuleiro[i][j]!=4))
            {
                error_msg("A coordenada que voce selecionou nao possui peca");
                movimento = false;
                return -1;
            }
//-------------------------------------condição para quando não tem como se mover mesmo com peça azul-------------------------------------
            if(((tabuleiro[i+1][j+1]==1)||(tabuleiro[i+1][j+1]==3))&&((tabuleiro[i-1][j+1]==1)||(tabuleiro[i-1][j+1]==3)))
            {
                if ((tabuleiro[i+2][j+2]!=0)&&(tabuleiro[i-2][j+2]!=0))
                {
                    error_msg("A peca selecionada nao pode se mover para nenhuma direcao");
                    movimento = false;
                    return -1;
                }
            }
            if (tabuleiro[i][j]==4)
            {
                if(((tabuleiro[i+1][j-1]==1)||(tabuleiro[i+1][j-1]==3))&&((tabuleiro[i-1][j-1]==1)||(tabuleiro[i-1][j-1]==3)))
                {
                    if ((tabuleiro[i+2][j-2]!=0)&&(tabuleiro[i-2][j-2]!=0))
                    {
                        error_msg("A peca selecionada nao pode se mover para nenhuma direcao");
                        movimento = false;
                        return -1;
                    }
                }
            }
//-------------------------------------condição para selecionar peça sem peças brancas na frente-------------------------------------
            if(((tabuleiro[i-1][j-1]==2)||(tabuleiro[i-1][j-1]==4))&&
                ((tabuleiro[i+1][j-1]==2)||(tabuleiro[i+1][j-1]==4))&&
                ((tabuleiro[i-1][j+1]==2)||(tabuleiro[i-1][j+1]==4))&&
                ((tabuleiro[i+1][j+1]==2)||(tabuleiro[i+1][j+1]==4)))
            {
                error_msg("A peca selecionada nao pode se mover para nenhuma direcao");
                movimento = false;
                return -1;
            }
//-------------------------------------condição para limitar o tabuleiro-------------------------------------
            if(((j+1)>=8)&&((i-1)<0))
            {
                if ((tabuleiro[1][6]==2)||(tabuleiro[1][6]==4))
                {
                    error_msg("A coordenada que voce selecionou nao pode se mover");
                    movimento = false;
                    return -1;
                }
                else if ((tabuleiro[1][6]==1)||(tabuleiro[1][6]==3))
                {
                    if (tabuleiro[2][5]!=0)
                    {
                        error_msg("A coordenada que voce selecionou nao pode se mover");
                        movimento = false;
                        return -1;
                    }
                }
            }
            if(((j-1)<0)&&((i+1>=8)))
            {
                if ((tabuleiro[6][1]==2)||(tabuleiro[6][1]==4))
                {
                    error_msg("A coordenada que voce selecionou nao pode se mover");
                    movimento = false;
                    return -1;
                }
                else if((tabuleiro[6][1]==1)||(tabuleiro[6][1]==3))
                {
                    if (tabuleiro[5][2]!=0)
                    {
                        error_msg("A coordenada que voce selecionou nao pode se mover");
                        movimento = false;
                        return -1;
                    }
                }
            }

    movimento = true;

}

int posicao_final_rosa()
{
    j_f = quadrado_clicado_x_f;
    i_f = quadrado_clicado_y_f;
    i = quadrado_clicado_y;
    j = quadrado_clicado_x;

//-------------------------------------condição para peça não ultrapassar os limites do tabuleiro-------------------------------------
    if((j_f>=8)||(i_f>=8)||(j_f<0)||(i_f<0))
    {
            error_msg("Movimento invalido, a posicao selecionada ultrapassa os limites do tabuleiro");
            movimento = true;
            return -1;
        }
//-------------------------------------condição para peça não comer peças com outra atrás-------------------------------------
    if((tabuleiro[i_f][j_f]==1)||(tabuleiro[i_f][j_f]==3))
    {
        if(((i_f+1)==i)&&((j_f-1)==j))
        {
            if (tabuleiro[i_f-1][j_f+1]!=0)
            {
                error_msg("Movimento invalido, a posicao selecionada possui outra peca igual atras");
                movimento = true;
                return -1;
            }
        }
        if(((i_f+1)==i)&&((j_f+1)==j))
        {
            if (tabuleiro[i_f-1][j_f-1]!=0)
            {
                error_msg("Movimento invalido, a posicao selecionada possui outra peca igual atras");
                movimento = true;
                return -1;
            }
        }
        if(tabuleiro[i][j]==4)
        {
            if(((i_f-1) == i)&&((j_f+1) == j))
            {
                if (tabuleiro[i_f+1][j_f-1]!=0)
                {
                    error_msg("Movimento invalido, a posicao selecionada possui outra peca igual atras");
                    movimento = true;
                    return -1;
                }
            }
            if(((i_f-1) == i)&&((j_f-1) == j))
            {
                if (tabuleiro[i_f+1][j_f+1]!=0)
                {
                    error_msg("Movimento invalido, a posicao selecionada possui outra peca igual atras");
                    movimento = true;
                    return -1;
                }
            }
        }
    }
//-------------------------------------condição para peça só ir para frente e não para trás-------------------------------------
    if(tabuleiro[i][j]==2)
    {
        if(i_f>=i)
        {
            error_msg("Movimento invalido, a peca so pode se mover para frente");
            movimento = true;
            return -1;
        }
    }
    else if(tabuleiro[i][j]==4)
        {
            if((i_f<=i)&&(i_f>=i))
            {
                error_msg("Movimento invalido, a peca so pode se mover na diagonal");
                movimento = true;
                return -1;
            }
        }
//-------------------------------------condição para peça não andar mais de uma linha-------------------------------------
    if(i_f<i-1)
    {
        error_msg("Movimento invalido, a peca so pode andar uma linha");
        movimento = true;
        return -1;
    }
//-------------------------------------condição para peça não andar mais de uma coluna-------------------------------------
    if((j_f>j+1)||(j_f<j-1))
    {
        error_msg("Movimento invalido, a peca so pode andar uma coluna");
        movimento = true;
        return -1;
    }
//-------------------------------------condição para peça só andar na diagonal-------------------------------------
    if((i_f==(i-1))||(i_f==(i+1)))
    {
        if((j_f!=j+1)&&(j_f!=j-1))
        {
            error_msg("Movimento invalido, a peca so pode se mover na diagonal");
            movimento = true;
            return -1;
        }
    }
//-------------------------------------condição para peça nao comer uma peça do mesmo tipo-------------------------------------
    if((tabuleiro[i_f][j_f]==2)||(tabuleiro[i_f][j_f]==4))
    {
        error_msg("Movimento invalido, a peca selecionada nao pode comer uma peca do mesmo tipo");
        movimento = true;
        return -1;
    }

    movimento = false;
    jogada = 1;
}

int comer_azul ()
{
    j_f = quadrado_clicado_x_f;
    i_f = quadrado_clicado_y_f;
    i = quadrado_clicado_y;
    j = quadrado_clicado_x;
//=====================================condições para comer a peça=====================================
    do
    {
        if(((i_f-1) == i)&&((j_f-1) == j))
        {
        if((tabuleiro[i+1][j+1]==2)||(tabuleiro[i+1][j+1]==4))
        {
            if (((i_f+1)>=8)||((j_f+1)>=8))//peça no limite do tabuleiro
            {
                error_msg("Movimento invalido, a posicao selecionada ultrapassa os limites do tabuleiro");
                movimento = true;
                return -1;
            }
            if(tabuleiro[i][j]==1)
            {
                if((i_f+2)>=8)
                {//condição se a peça vai virar dama
                    tabuleiro[i][j] = 0;//determinando a posição inicial como vazia
                    tabuleiro[i_f][j_f] = 0;//determinando a posição inicial como vazia
                    tabuleiro[i_f+1][j_f+1]=3;
                    break;
                }
                tabuleiro[i_f+1][j_f+1]=1;//determinando a posição nova como ocupada por peça normal
            }
            if(tabuleiro[i][j]==3)
            {
                tabuleiro[i_f+1][j_f+1]=3;//determinando a posição nova como ocupada por dama branca
            }
            if((((i_f+3)>=8)&&(tabuleiro[i_f+2][j_f+2]==2))||//comando quando a segunda peça ta no fim do tabuleiro
               (((j_f+3)>=8)&&(tabuleiro[i_f+2][j_f+2]==2)))
            {
                tabuleiro[i_f+1][j_f+1]=1;//determinando a posição nova como ocupada por peça normal
                if(tabuleiro[i][j]==3)
                {
                    tabuleiro[i_f+1][j_f+1]=3;//determinando a posição nova como ocupada por dama branca
                }
            }
            else if(((tabuleiro[i_f+2][j_f+2]==2)&&(tabuleiro[i_f+3][j_f+3]==0))
            ||((tabuleiro[i_f+2][j_f+2]==4)&&(tabuleiro[i_f+3][j_f+3]==0)))//condição pra comer mais uma peça
            {
                if (tabuleiro[i][j]==1)
                {
                    if((i_f+4)>=8)
                    {//condição se a peça vai virar dama
                        tabuleiro[i][j]=0;
                        tabuleiro[i_f][j_f]=0;
                        tabuleiro[i_f+1][j_f+1]=0;
                        tabuleiro[i_f+2][j_f+2]=0;
                        tabuleiro[i_f+3][j_f+3]=3;
                        break;
                    }
                    tabuleiro[i][j]=0;
                    tabuleiro[i_f][j_f]=0;
                    tabuleiro[i_f+1][j_f+1]=0;
                    tabuleiro[i_f+2][j_f+2]=0;
                    tabuleiro[i_f+3][j_f+3]=1;//determinando a posição nova como ocupada
                }

                if(tabuleiro[i][j]==3)
                {
                    tabuleiro[i][j]=0;
                    tabuleiro[i_f][j_f]=0;
                    tabuleiro[i_f+1][j_f+1]=0;
                    tabuleiro[i_f+2][j_f+2]=0;
                    tabuleiro[i_f+3][j_f+3]=3;//determinando a posição nova como ocupada por dama branca
                }
            }
            tabuleiro[i_f][j_f]=0;//determinando a posição final como vazia
            tabuleiro[i][j]=0;//determinando a posição inicial como vazia
            break;
        }
        }
        if(((i_f-1) == i)&&((j_f+1) == j))
        {
        if((tabuleiro[i+1][j-1]==2)||(tabuleiro[i+1][j-1]==4))
        {
            if (((i_f+1)>=8)||((j_f-1)<0))//peça no limite do tabuleiro
            {
                error_msg("Movimento invalido, a posicao selecionada ultrapassa os limites do tabuleiro");
                movimento = true;
                return -1;
            }
            if(tabuleiro[i][j]==1)
            {
                if((i_f+2)>=8)
                {//condição se a peça vai virar dama
                    tabuleiro[i][j] = 0;//determinando a posição inicial como vazia
                    tabuleiro[i_f][j_f] = 0;//determinando a posição inicial como vazia
                    tabuleiro[i_f+1][j_f-1]=3;
                    break;
                }
                tabuleiro[i_f+1][j_f-1]=1;//determinando a posição nova como ocupada por peça normal
            }
            if(tabuleiro[i][j]==3)
            {
                tabuleiro[i_f+1][j_f-1]=3;//determinando a posição nova como ocupada por dama branca
            }
            if ((((i_f+3)>=8)&&(tabuleiro[i_f+2][j_f-2]==2))||
               (((j_f-3)<0)&&(tabuleiro[i_f+2][j_f-2]==2)))
            {
                tabuleiro[i_f+1][j_f-1]=1;//determinando a posição nova como ocupada por peça normal
                if(tabuleiro[i][j]==3)
                {
                    tabuleiro[i_f+1][j_f-1]=3;//determinando a posição nova como ocupada por dama branca
                }
            }
            else if(((tabuleiro[i_f+2][j_f-2]==2)&&(tabuleiro[i_f+3][j_f-3]==0))
            ||((tabuleiro[i_f+2][j_f-2]==4)&&(tabuleiro[i_f+3][j_f-3]==0)))//condição pra comer mais uma peça
            {
                if (tabuleiro[i][j]==1)
                {
                    if((i_f+4)>=8)
                    {//condição se a peça vai virar dama
                        tabuleiro[i][j]=0;
                        tabuleiro[i_f][j_f]=0;
                        tabuleiro[i_f+1][j_f-1]=0;
                        tabuleiro[i_f+2][j_f-2]=0;
                        tabuleiro[i_f+3][j_f-3]=3;
                        break;
                    }
                    tabuleiro[i][j]=0;
                    tabuleiro[i_f][j_f]=0;
                    tabuleiro[i_f+1][j_f-1]=0;
                    tabuleiro[i_f+2][j_f-2]=0;
                    tabuleiro[i_f+3][j_f-3]=1;//determinando a posição nova como ocupada
                }

                if(tabuleiro[i][j]==3)
                {
                    tabuleiro[i][j]=0;
                    tabuleiro[i_f][j_f]=0;
                    tabuleiro[i_f+1][j_f-1]=0;
                    tabuleiro[i_f+2][j_f-2]=0;
                    tabuleiro[i_f+3][j_f-3]=3;//determinando a posição nova como ocupada por dama branca
                }
            }
            tabuleiro[i_f][j_f]=0;//determinando a posição final como vazia
            tabuleiro[i][j]=0;//determinando a posição inicial como vazia
            break;
        }
        }
        if(((i_f+1) == i)&&((j_f+1) == j))
        {
        if((tabuleiro[i-1][j-1]==2)||(tabuleiro[i-1][j-1]==4))
        {
            if (((i_f-1)<0)||((j_f-1)<0))//peça no limite do tabuleiro
            {
                error_msg("Movimento invalido, a posicao selecionada ultrapassa os limites do tabuleiro");
                movimento = true;
                return -1;
            }
            tabuleiro[i_f-1][j_f-1]=3;//determinando a posição nova como ocupada por dama branca

            if ((((i_f-3)<0)&&(tabuleiro[i_f-2][j_f-2]==2))||
              (((j_f-3)<0)&&(tabuleiro[i_f-2][j_f-2]==2)))
            {
                tabuleiro[i_f-1][j_f-1]=3;//determinando a posição nova como ocupada por dama branca
            }
            else if(((tabuleiro[i_f-2][j_f-2]==2)&&(tabuleiro[i_f-3][j_f-3]==0))
            ||((tabuleiro[i_f-2][j_f-2]==4)&&(tabuleiro[i_f-3][j_f-3]==0)))//condição pra comer mais uma peça
            {
                tabuleiro[i][j]=0;
                tabuleiro[i_f][j_f]=0;
                tabuleiro[i_f-1][j_f-1]=0;
                tabuleiro[i_f-2][j_f-2]=0;
                tabuleiro[i_f-3][j_f-3]=3;
                break;
            }
            tabuleiro[i_f][j_f]=0;//determinando a posição final como vazia
            tabuleiro[i][j]=0;//determinando a posição inicial como vazia
            break;
        }
        }
        if(((i_f+1) == i)&&((j_f-1) == j))
        {
        if((tabuleiro[i-1][j+1]==2)||(tabuleiro[i-1][j+1]==4))
        {
            if (((i_f-1)<0)||((j_f+1)>=8))//peça no limite do tabuleiro
            {
                error_msg("Movimento invalido, a posicao selecionada ultrapassa os limites do tabuleiro");
                movimento = true;
                return -1;
            }
            tabuleiro[i_f-1][j_f+1]=3;//determinando a posição nova como ocupada por dama branca

            if((((i_f-3)<0)&&(tabuleiro[i_f-2][j_f+2]==2))||
               (((j_f+3)>=8)&&(tabuleiro[i_f-2][j_f+2]==2)))
            {
                tabuleiro[i_f-1][j_f+1]=3;//determinando a posição nova como ocupada por dama branca
            }
            else if(((tabuleiro[i_f-2][j_f+2]==2)&&(tabuleiro[i_f-3][j_f+3]==0))
            ||((tabuleiro[i_f-2][j_f+2]==4)&&(tabuleiro[i_f-3][j_f+3]==0)))//condição pra comer mais uma peça
            {
                tabuleiro[i][j]=0;
                tabuleiro[i_f][j_f]=0;
                tabuleiro[i_f-1][j_f+1]=0;
                tabuleiro[i_f-2][j_f+2]=0;
                tabuleiro[i_f-3][j_f+3]=3;
                break;
            }
            tabuleiro[i_f][j_f]=0;//determinando a posição final como vazia
            tabuleiro[i][j]=0;//determinando a posição inicial como vazia
            break;
        }
        }
        if(tabuleiro[i_f][j_f]==0)
        {
            if((i_f+1)>=8)
            {//condição se a peça vai virar dama
                tabuleiro[i_f][j_f]=3;
            }

            else if(tabuleiro[i][j]==3)
            {
                tabuleiro[i_f][j_f]=3;//determinando a posição nova como ocupada por dama branca
            }
            else if (tabuleiro[i][j]==1)
            {
            tabuleiro[i_f][j_f]=1;//determinando a posição final como ocupada
            }
            tabuleiro[i][j] = 0;//determinando a posição inicial como vazia
        }

    }while(tabuleiro[i][j]!=0);
}

int comer_rosa ()
{
    j_f = quadrado_clicado_x_f;
    i_f = quadrado_clicado_y_f;
    i = quadrado_clicado_y;
    j = quadrado_clicado_x;
//=====================================condições para comer a peça=====================================
    do
    {
        if(((i_f+1) == i)&&((j_f+1) == j))
        {
            if((tabuleiro[i-1][j-1]==1)||(tabuleiro[i-1][j-1]==3))
            {
                if (((i_f-1)<0)||((j_f-1)<0))//peça no limite do tabuleiro
                {
                    error_msg("Movimento invalido, a posicao selecionada ultrapassa os limites do tabuleiro");
                    movimento = true;
                    return -1;
                }
                if(tabuleiro[i][j]==2)
                {
                    if((i_f-2)<0)
                    {//condição se a peça vai virar dama
                        tabuleiro[i][j] = 0;//determinando a posição inicial como vazia
                        tabuleiro[i_f][j_f] = 0;//determinando a posição inicial como vazia
                        tabuleiro[i_f-1][j_f-1]=4;
                        break;
                    }
                    tabuleiro[i_f-1][j_f-1]=2;//determinando a posição nova como ocupada por peça normal
                }
                if(tabuleiro[i][j]==4)
                {
                    tabuleiro[i_f-1][j_f-1]=4;//determinando a posição nova como ocupada por dama branca
                }
                if((((i_f-3)<0)&&(tabuleiro[i_f-2][j_f-2]==1))||
                   (((j_f-3)<0)&&(tabuleiro[i_f-2][j_f-2]==1)))
                {
                    tabuleiro[i_f-1][j_f-1]=2;//determinando a posição nova como ocupada por peça normal
                    if(tabuleiro[i][j]==4)
                    {
                        tabuleiro[i_f-1][j_f-1]=4;//determinando a posição nova como ocupada por dama branca
                    }
                }
                else if(((tabuleiro[i_f-2][j_f-2]==1)&&(tabuleiro[i_f-3][j_f-3]==0))
                ||((tabuleiro[i_f-2][j_f-2]==3)&&(tabuleiro[i_f-3][j_f-3]==0)))//condição pra comer mais uma peça
                {
                    if (tabuleiro[i][j]==2)
                    {
                        if((i_f-4)<0)
                        {//condição se a peça vai virar dama
                            tabuleiro[i][j]=0;
                            tabuleiro[i_f][j_f]=0;
                            tabuleiro[i_f-1][j_f-1]=0;
                            tabuleiro[i_f-2][j_f-2]=0;
                            tabuleiro[i_f-3][j_f-3]=4;
                            break;
                        }
                        tabuleiro[i][j]=0;
                        tabuleiro[i_f][j_f]=0;
                        tabuleiro[i_f-1][j_f-1]=0;
                        tabuleiro[i_f-2][j_f-2]=0;
                        tabuleiro[i_f-3][j_f-3]=2;//determinando a posição nova como ocupada
                    }

                    if(tabuleiro[i][j]==4)
                    {
                        tabuleiro[i][j]=0;
                        tabuleiro[i_f][j_f]=0;
                        tabuleiro[i_f-1][j_f-1]=0;
                        tabuleiro[i_f-2][j_f-2]=0;
                        tabuleiro[i_f-3][j_f-3]=4;//determinando a posição nova como ocupada por dama branca
                    }
                }
                tabuleiro[i_f][j_f]=0;//determinando a posição final como vazia
                tabuleiro[i][j]=0;//determinando a posição inicial como vazia
                break;
            }
        }
        if(((i_f+1) == i)&&((j_f-1) == j))
        {
            if((tabuleiro[i-1][j+1]==1)||(tabuleiro[i-1][j+1]==3))
            {
                if (((i_f-1)<0)||((j_f+1)>=8))//peça no limite do tabuleiro
                {
                    error_msg("Movimento invalido, a posicao selecionada ultrapassa os limites do tabuleiro");
                    movimento = true;
                    return -1;
                }
                if(tabuleiro[i][j]==2)
                {
                    if((i_f-2)<0)
                    {//condição se a peça vai virar dama
                        tabuleiro[i][j] = 0;//determinando a posição inicial como vazia
                        tabuleiro[i_f][j_f] = 0;//determinando a posição inicial como vazia
                        tabuleiro[i_f-1][j_f+1]=4;
                        break;
                    }
                    tabuleiro[i_f-1][j_f+1]=2;//determinando a posição nova como ocupada por peça normal
                }
                if(tabuleiro[i][j]==4)
                {
                    tabuleiro[i_f-1][j_f+1]=4;//determinando a posição nova como ocupada por dama branca
                }
                if((((i_f-3)<0)&&(tabuleiro[i_f-2][j_f+2]==1))||
                   (((j_f+3)>=8)&&(tabuleiro[i_f-2][j_f+2]==1)))
                {
                    tabuleiro[i_f-1][j_f+1]=2;//determinando a posição nova como ocupada por peça normal
                    if(tabuleiro[i][j]==4)
                    {
                        tabuleiro[i_f-1][j_f+1]=4;//determinando a posição nova como ocupada por dama branca
                    }
                }
                else if(((tabuleiro[i_f-2][j_f+2]==1)&&(tabuleiro[i_f-3][j_f+3]==0))
                ||((tabuleiro[i_f-2][j_f+2]==3)&&(tabuleiro[i_f-3][j_f+3]==0)))//condição pra comer mais uma peça
                {
                    if (tabuleiro[i][j]==2)
                    {
                        if((i_f-4)<0)
                        {//condição se a peça vai virar dama
                            tabuleiro[i][j]=0;
                            tabuleiro[i_f][j_f]=0;
                            tabuleiro[i_f-1][j_f+1]=0;
                            tabuleiro[i_f-2][j_f+2]=0;
                            tabuleiro[i_f-3][j_f+3]=4;
                            break;
                        }
                        tabuleiro[i][j]=0;
                        tabuleiro[i_f][j_f]=0;
                        tabuleiro[i_f-1][j_f+1]=0;
                        tabuleiro[i_f-2][j_f+2]=0;
                        tabuleiro[i_f-3][j_f+3]=2;//determinando a posição nova como ocupada
                    }

                    if(tabuleiro[i][j]==4)
                    {
                        tabuleiro[i][j]=0;
                        tabuleiro[i_f][j_f]=0;
                        tabuleiro[i_f-1][j_f+1]=0;
                        tabuleiro[i_f-2][j_f+2]=0;
                        tabuleiro[i_f-3][j_f+3]=4;//determinando a posição nova como ocupada por dama branca
                    }
                }
                tabuleiro[i_f][j_f]=0;//determinando a posição final como vazia
                tabuleiro[i][j]=0;//determinando a posição inicial como vazia
                break;
            }
        }
        if(((i_f-1) == i)&&((j_f-1) == j))
        {
            if((tabuleiro[i+1][j+1]==1)||(tabuleiro[i+1][j+1]==3))
            {
                if (((i_f+1)>=8)||((j_f+1)>=8))//peça no limite do tabuleiro
                {
                    error_msg("Movimento invalido, a posicao selecionada ultrapassa os limites do tabuleiro");
                    movimento = true;
                    return -1;
                }
                tabuleiro[i_f+1][j_f+1]=4;//determinando a posição nova como ocupada por dama branca

                if((((i_f+3)>=8)&&(tabuleiro[i_f+2][j_f+2]==1))||
                (((j_f+3)>=8)&&(tabuleiro[i_f+2][j_f+2]==1)))
                {
                    tabuleiro[i_f+1][j_f+1]=4;//determinando a posição nova como ocupada por dama branca
                }
                else if(((tabuleiro[i_f+2][j_f+2]==1)&&(tabuleiro[i_f+3][j_f+3]==0))
                ||((tabuleiro[i_f+2][j_f+2]==3)&&(tabuleiro[i_f+3][j_f+3]==0)))//condição pra comer mais uma peça
                {
                    tabuleiro[i][j]=0;
                    tabuleiro[i_f][j_f]=0;
                    tabuleiro[i_f+1][j_f+1]=0;
                    tabuleiro[i_f+2][j_f+2]=0;
                    tabuleiro[i_f+3][j_f+3]=4;
                    break;
                }
                tabuleiro[i_f][j_f]=0;//determinando a posição final como vazia
                tabuleiro[i][j]=0;//determinando a posição inicial como vazia
                break;
            }
        }
        if(((i_f-1) == i)&&((j_f+1) == j))
        {
            if((tabuleiro[i+1][j-1]==1)||(tabuleiro[i+1][j-1]==3))
            {
                if (((i_f+1)>=8)||((j_f-1)<0))//peça no limite do tabuleiro
                {
                    error_msg("Movimento invalido, a posicao selecionada ultrapassa os limites do tabuleiro");
                    movimento = true;
                    return -1;
                }
                tabuleiro[i_f+1][j_f-1]=4;//determinando a posição nova como ocupada por dama branca

                if((((i_f+3)>=8)&&(tabuleiro[i_f+2][j_f-2]==1))||
                (((j_f-3)<0)&&(tabuleiro[i_f+2][j_f-2]==1)))
                {
                    tabuleiro[i_f+1][j_f-1]=4;//determinando a posição nova como ocupada por dama branca
                }
                else if(((tabuleiro[i_f+2][j_f-2]==1)&&(tabuleiro[i_f+3][j_f-3]==0))
                ||((tabuleiro[i_f+2][j_f-2]==3)&&(tabuleiro[i_f+3][j_f-3]==0)))//condição pra comer mais uma peça
                {
                    tabuleiro[i][j]=0;
                    tabuleiro[i_f][j_f]=0;
                    tabuleiro[i_f+1][j_f-1]=0;
                    tabuleiro[i_f+2][j_f-2]=0;
                    tabuleiro[i_f+3][j_f-3]=4;
                    break;
                }
                tabuleiro[i_f][j_f]=0;//determinando a posição final como vazia
                tabuleiro[i][j]=0;//determinando a posição inicial como vazia
                break;
            }
        }
        if(tabuleiro[i_f][j_f]==0)
        {
                if((i_f-1)<0)
                {//condição se a peça vai virar dama
                    tabuleiro[i_f][j_f]=4;
                }

                else if(tabuleiro[i][j]==4)
                {
                    tabuleiro[i_f][j_f]=4;//determinando a posição nova como ocupada por dama branca
                }
                else if (tabuleiro[i][j]==2)
                {
                tabuleiro[i_f][j_f]=2;//determinando a posição final como ocupada
                }
                tabuleiro[i][j] = 0;//determinando a posição inicial como vazia
        }

    }while(tabuleiro[i][j]!=0);
}

int main(void)
{
  //declaração de variáveis locais
    bool done = false;
    bool draw = true;
    bool clique = false;
    bool regras = false;
    bool menu = true;
    movimento = false;
    jogada = 1;

    if (!al_init()){
        error_msg("Falha ao inicializar a Allegro");
        return -1;
    }

    if (!al_install_audio()){
        error_msg("Falha ao inicializar audio");
        return -1;
    }

    if (!al_init_acodec_addon()){
        error_msg("Falha ao inicializar formatos de audio");
        return -1;
    }

    if (!al_reserve_samples(1)){
        error_msg("Falha ao inicializar canais de audio");
        return -1;
    }


    janela = al_create_display(largura, altura);
    if (!janela){
        error_msg("Falha ao criar janela");
        return -1;
    }

    al_init_image_addon();
    al_init_primitives_addon();
    al_install_mouse();

    sample = al_load_sample("sons/som_selecionar.wav");
    if (!sample){
        error_msg("Falha ao inicializar audio");
        return -1;
    }

    contagem = al_load_sample("sons/contagem.wav");
    if (!contagem){
        error_msg("Falha ao inicializar audio");
        return -1;
    }

    fail = al_load_sample("sons/som_fail.wav");
    if (!fail){
        error_msg("Falha ao inicializar audio");
        return -1;
    }

    tururu =  al_load_sample("sons/tururu.wav");
    if (!tururu){
        error_msg("Falha ao inicializar audio");
        return -1;
    }

    vitoria = al_load_audio_stream("sons/som_campeao.wav", 4, 1024);
    if (!vitoria){
        error_msg("Falha ao inicializar audio");
        return -1;
    }

    musica = al_load_audio_stream("sons/trilha_sonora_1.wav", 4, 1024);
    if (!musica){
        error_msg("Falha ao iniciar musica");
        return -1;
    }

    peca_azul_normal = al_load_bitmap("imagens/peca_azul_normal.png");
    peca_rosa_normal = al_load_bitmap("imagens/peca_rosa_normal.png");
    peca_azul_dama   = al_load_bitmap("imagens/peca_azul_dama.png");
    peca_rosa_dama   = al_load_bitmap("imagens/peca_rosa_dama.png");
    menu1            = al_load_bitmap("imagens/menu1.png");
    menu2            = al_load_bitmap("imagens/menu2.png");
    menu3            = al_load_bitmap("imagens/menu3.png");
    destab           = al_load_bitmap("imagens/tabuleiro.png");
    mouse            = al_load_bitmap("imagens/mouse.png");
    azulv            = al_load_bitmap("imagens/azulv.png");
    azulv2           = al_load_bitmap("imagens/azulv2.png");
    azulv3           = al_load_bitmap("imagens/azulv3.png");
    rosav            = al_load_bitmap("imagens/rosav.png");
    rosav2           = al_load_bitmap("imagens/rosav2.png");
    rosav3           = al_load_bitmap("imagens/rosav3.png");
    tela_regras      = al_load_bitmap("imagens/regras1.png");
    tela_regras_2    = al_load_bitmap("imagens/regras2.png");
    cont3            = al_load_bitmap("imagens/3.png");
    cont2            = al_load_bitmap("imagens/2.png");
    cont1            = al_load_bitmap("imagens/1.png");
    cont0            = al_load_bitmap("imagens/0.png");
    contgo           = al_load_bitmap("imagens/go.png");


    if ((!peca_azul_normal)|| (!peca_rosa_normal)||(!peca_azul_dama)||(!peca_rosa_dama)){
        error_msg("Falha ao carregar pecas");
        return -1;
    }

    if ((!destab)|| (!mouse)||(!azulv)||(!rosav)||(!menu1)||(!menu2)||(!menu3)||
        (!tela_regras)||(!tela_regras_2)||(!cont3)||(!cont2)||(!cont1)||(!contgo)){
        error_msg("Falha ao carregar telas");
        return -1;
    }

    al_set_window_title(janela, "Damas Espaciais");
    event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_display_event_source(janela));
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_hide_mouse_cursor(janela);
    al_attach_audio_stream_to_mixer(musica, al_get_default_mixer());//vinculo da musica de fundo com a tela principal
    al_set_audio_stream_playing(musica, false);
    al_attach_audio_stream_to_mixer(vitoria, al_get_default_mixer());//vinculo da musica de vitoria com a tela principal
    al_set_audio_stream_playing(vitoria, false);

    while(!done)
    {
        if(menu == true)
        {
            al_set_audio_stream_playing(vitoria, false);
            al_set_audio_stream_playing(musica, false);
            //=====================================posições iniciais das peças=====================================
            for(i=0;i<8;i++)
            {
                for(j=0;j<8;j++)
                {
                    tabuleiro[i][j]=0;
                }
            }
            //peças azuis           //peças rosa
            tabuleiro[0][1]=1;      tabuleiro[7][0]=2;
            tabuleiro[0][3]=1;      tabuleiro[7][2]=2;
            tabuleiro[0][5]=1;      tabuleiro[7][4]=2;
            tabuleiro[0][7]=1;      tabuleiro[7][6]=2;
            tabuleiro[1][0]=1;      tabuleiro[6][1]=2;
            tabuleiro[1][2]=1;      tabuleiro[6][3]=2;
            tabuleiro[1][4]=1;      tabuleiro[6][5]=2;
            tabuleiro[1][6]=1;      tabuleiro[6][7]=2;
            tabuleiro[2][1]=1;      tabuleiro[5][0]=2;
            tabuleiro[2][3]=1;      tabuleiro[5][2]=2;
            tabuleiro[2][5]=1;      tabuleiro[5][4]=2;
            tabuleiro[2][7]=1;      tabuleiro[5][6]=2;
            ALLEGRO_EVENT ev;
            al_wait_for_event(event_queue, &ev);
            if(ev.type == ALLEGRO_EVENT_MOUSE_AXES)
            {
                if(regras == true)
                {
                    posicao_x = ev.mouse.x;
                    posicao_y = ev.mouse.y;
                    al_draw_bitmap(tela_regras, 0, 0, 0);
                    al_draw_bitmap(mouse, posicao_x, posicao_y, 0);
                    al_flip_display();
                    if((ev.mouse.x >= 230 )&&(ev.mouse.x <= 410)&&
                         (ev.mouse.y >= 575 )&&(ev.mouse.y <= 610))
                        {
                            al_draw_bitmap(tela_regras_2, 0, 0, 0);
                            al_draw_bitmap(mouse, posicao_x, posicao_y, 0);
                            al_flip_display();
                        }
                }
                else if(regras==false)
                    {
                        posicao_x = ev.mouse.x;
                        posicao_y = ev.mouse.y;
                        al_draw_bitmap(menu1, 0, 0, 0);
                        al_draw_bitmap(mouse, posicao_x, posicao_y, 0);
                        al_flip_display();

                        if ((ev.mouse.x >= 190 )&&(ev.mouse.x <= 450 )&&
                             (ev.mouse.y >= 320 )&&(ev.mouse.y <= 380 ))
                        {
                            al_draw_bitmap(menu2, 0, 0, 0);
                            al_draw_bitmap(mouse, posicao_x, posicao_y, 0);
                            al_flip_display();
                        }
                        else if((ev.mouse.x >= 170 )&&(ev.mouse.x <= 470 )&&
                             (ev.mouse.y >= 410 )&&(ev.mouse.y <= 470 ))
                        {
                            al_draw_bitmap(menu3, 0, 0, 0);
                            al_draw_bitmap(mouse, posicao_x, posicao_y, 0);
                            al_flip_display();
                        }
                    }
            }
            else if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
            {
                if(regras==true)
                {
                    if((ev.mouse.x >= 230 )&&(ev.mouse.x <= 410 )&&
                     (ev.mouse.y >= 575 )&&(ev.mouse.y <= 610 ))
                    {
                        regras = false;
                    }
                }
                else
                {
                    if ((ev.mouse.x >= 190 )&&(ev.mouse.x <= 450 )&&
                    (ev.mouse.y >= 320 )&&(ev.mouse.y <= 380 ))
                    {
                        menu = false;
                        al_play_sample(contagem, 2.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                        al_draw_bitmap(cont3, 0, 0, 0);
                        al_flip_display();
                        al_rest(1.0);
                        al_draw_bitmap(cont2, 0, 0, 0);
                        al_flip_display();
                        al_rest(1.0);
                        al_draw_bitmap(cont1, 0, 0, 0);
                        al_flip_display();
                        al_rest(1.0);
                        al_draw_bitmap(cont0, 0, 0, 0);
                        al_flip_display();
                        al_rest(1.0);
                        al_draw_bitmap(contgo, 0, 0, 0);
                        al_flip_display();
                        al_rest(6.0);
                    }
                    else if ((ev.mouse.x >= 170 )&&(ev.mouse.x <= 470 )&&
                            (ev.mouse.y >= 410 )&&(ev.mouse.y <= 470 ))
                    {
                        regras = true;
                    }
                }
            }
            else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            {
                al_play_sample(tururu, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                sair("Volte logo");
                done = true;
                break;
            }
        }
        else if(menu == false)
        {
            do{
                al_set_audio_stream_playing(musica, true);
                ALLEGRO_EVENT ev;
                al_wait_for_event(event_queue, &ev);
                if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                {
                    al_set_audio_stream_playing(musica, false);
                    al_play_sample(tururu, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                    sair("Volte logo");
                    done = true;
                    break;
                }

                if(ev.type == ALLEGRO_EVENT_MOUSE_AXES)
                {
                    posicao_x = ev.mouse.x;
                    posicao_y = ev.mouse.y;
                }
                else if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
                {
                    if (jogada == 1)
                    {
                        if (ev.mouse.button & 1 && movimento == false)
                        {
                            mouse_x = ev.mouse.x;
                            mouse_y = ev.mouse.y;
                            for (i=0; i<8; i++)
                            {
                                if (mouse_x >= 80*i && mouse_x <= 80*(i + 1))
                                {
                                    quadrado_clicado_x = i;
                                }
                                    if (mouse_y >= 80*i && mouse_y <= 80*(i + 1))
                                {
                                quadrado_clicado_y = i;
                                }
                            }
                            posicao_inicial_azul();
                            if (movimento==false){
                                al_play_sample(fail, 0.5, 0.0, 2.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                            }
                            printf("x:%d y:%d\n",quadrado_clicado_x, quadrado_clicado_y);

                        }
                        if (movimento == true)
                        {
                            draw = !draw;
                            al_play_sample(sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                        }
                        if (ev.mouse.button & 2 && movimento == true)
                        {
                            mouse_x = ev.mouse.x;
                            mouse_y = ev.mouse.y;
                            for (i=0; i<8; i++)
                            {
                                if (mouse_x >= 80*i && mouse_x <= 80*(i + 1))
                                {
                                    quadrado_clicado_x_f = i;
                                }
                                    if (mouse_y >= 80*i && mouse_y <= 80*(i + 1))
                                {
                                quadrado_clicado_y_f = i;
                                }
                            }
                            if((quadrado_clicado_x_f==quadrado_clicado_x)&&(quadrado_clicado_y_f==quadrado_clicado_y))
                            {
                                movimento = false;
                                jogada = 2;
                            }
                            else
                            {
                                posicao_final_azul();
                                printf("x_f:%d y_f:%d\n",quadrado_clicado_x_f, quadrado_clicado_y_f);
                                if (movimento == false)
                                {
                                    //condições para comer peça
                                    comer_azul();
                                    //contagem de peças após a jogada
                                    q_b=q_p=0;
                                    for(i=0;i<8;i++)
                                    {
                                        for(j=0;j<8;j++)
                                        {
                                            if(tabuleiro[i][j]==1)q_b++;
                                            if(tabuleiro[i][j]==2)q_p++;
                                        }
                                    }
                                    printf("Pecas Azul: %d \nPecas Rosa: %d\n\n",q_b,q_p);
                                }
                            }
                        }

                    }

                    else if (jogada == 2)
                    {
                        if (ev.mouse.button & 1 && movimento == false)
                        {
                            mouse_x = ev.mouse.x;
                            mouse_y = ev.mouse.y;
                            for (i=0; i<8; i++)
                            {
                                if (mouse_x >= 80*i && mouse_x <= 80*(i + 1))
                                {
                                    quadrado_clicado_x = i;
                                }
                                    if (mouse_y >= 80*i && mouse_y <= 80*(i + 1))
                                {
                                quadrado_clicado_y = i;
                                }
                            }
                            posicao_inicial_rosa();
                            if (movimento==false){
                                al_play_sample(fail, 0.5, 0.0, 2.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                            }
                            printf("x:%d y:%d\n",quadrado_clicado_x, quadrado_clicado_y);
                        }
                        if (movimento == true)
                        {
                            draw = !draw;
                            al_play_sample(sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                        }
                        if (ev.mouse.button & 2 && movimento == true)
                        {
                            mouse_x = ev.mouse.x;
                            mouse_y = ev.mouse.y;
                            for (i=0; i<8; i++)
                            {
                                if (mouse_x >= 80*i && mouse_x <= 80*(i + 1))
                                {
                                    quadrado_clicado_x_f = i;
                                }
                                    if (mouse_y >= 80*i && mouse_y <= 80*(i + 1))
                                {
                                quadrado_clicado_y_f = i;
                                }
                            }
                            if((quadrado_clicado_x_f==quadrado_clicado_x)&&(quadrado_clicado_y_f==quadrado_clicado_y))
                            {
                                movimento = false;
                                jogada = 1;
                            }
                            else
                            {
                                posicao_final_rosa();
                                printf("x_f:%d y_f:%d\n",quadrado_clicado_x_f, quadrado_clicado_y_f);
                                if (movimento == false)
                                {
                                    //condições para comer peça
                                    comer_rosa();
                                    //contagem de peças após a jogada
                                    q_b = q_p = 0;
                                    for(i=0;i<8;i++)
                                    {
                                        for(j=0;j<8;j++)
                                        {
                                            if(tabuleiro[i][j]==1)q_b++;
                                            if(tabuleiro[i][j]==2)q_p++;
                                        }
                                    }
                                    printf("Pecas Brancas: %d \nPecas Pretas: %d\n\n",q_b,q_p);
                                    system("cls");
                                }
                            }
                        }
                    }
                }

            mouse_clicado(draw, quadrado_clicado_x, quadrado_clicado_y);
            desenho_tabuleiro();
            desenho_pecas();
            }while((q_b>0)&&(q_p>0));

        if ((q_b>0)||(q_p>0))
        {
            if((q_b>0)&&(q_p>0))
            {
                done=true;
                break;
            }
            do{
            ALLEGRO_EVENT ev;
            al_wait_for_event(event_queue, &ev);
            al_set_audio_stream_playing(musica, false);
            al_set_audio_stream_playing(vitoria, true);

            if ((q_b>0)&&(q_p>0))
            {
                done = true;
            }
            else
            {
                if(ev.type == ALLEGRO_EVENT_MOUSE_AXES)
                {
                    if(q_b>0)
                    {
                        posicao_x = ev.mouse.x;
                        posicao_y = ev.mouse.y;
                        al_draw_bitmap(azulv, 0, 0, 0);
                        al_draw_bitmap(mouse, posicao_x, posicao_y, 0);
                        al_flip_display();

                        if((ev.mouse.x >= 120 )&&(ev.mouse.x <= 265 )&&
                        (ev.mouse.y >= 550 )&&(ev.mouse.y <= 600 ))//sim
                        {
                            al_draw_bitmap(azulv2, 0, 0, 0);
                            al_draw_bitmap(mouse, posicao_x, posicao_y, 0);
                            al_flip_display();
                        }
                        else if((ev.mouse.x >= 350 )&&(ev.mouse.x <= 490 )&&
                        (ev.mouse.y >= 550 )&&(ev.mouse.y <= 600 ))//não
                        {
                            al_draw_bitmap(azulv3, 0, 0, 0);
                            al_draw_bitmap(mouse, posicao_x, posicao_y, 0);
                            al_flip_display();
                        }
                    }
                    else if (q_p>0)
                    {
                        posicao_x = ev.mouse.x;
                        posicao_y = ev.mouse.y;
                        al_draw_bitmap(rosav, 0, 0, 0);
                        al_draw_bitmap(mouse, posicao_x, posicao_y, 0);
                        al_flip_display();
                        if((ev.mouse.x >= 120 )&&(ev.mouse.x <= 265 )&&
                        (ev.mouse.y >= 550 )&&(ev.mouse.y <= 600 ))//sim
                        {
                            al_draw_bitmap(rosav2, 0, 0, 0);
                            al_draw_bitmap(mouse, posicao_x, posicao_y, 0);
                            al_flip_display();
                        }
                        else if((ev.mouse.x >= 350 )&&(ev.mouse.x <= 490 )&&
                        (ev.mouse.y >= 550 )&&(ev.mouse.y <= 600 ))//não
                        {
                            al_draw_bitmap(rosav3, 0, 0, 0);
                            al_draw_bitmap(mouse, posicao_x, posicao_y, 0);
                            al_flip_display();
                        }
                    }
                }
                if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
                {
                    if((ev.mouse.x >= 120 )&&(ev.mouse.x <= 265 )&&
                    (ev.mouse.y >= 550 )&&(ev.mouse.y <= 600 ))//sim
                    {
                        clique = true;
                        q_b = q_p = 12;
                        menu = true;
                        regras == true;
                        jogada = 1;
                        break;
                    }
                    else if((ev.mouse.x >= 350 )&&(ev.mouse.x <= 490 )&&
                    (ev.mouse.y >= 550 )&&(ev.mouse.y <= 600 ))//não
                    {
                        clique = true;
                        done = true;
                        break;
                    }
                }
            }
            }while((q_b>0)||(q_p>0));
        }
        }
    }
    al_flip_display();
    al_destroy_display(janela);
    return 0;
}

