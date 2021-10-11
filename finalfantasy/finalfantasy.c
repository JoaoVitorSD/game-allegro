#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <fenv.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#define frames 90
#define NMonstro 20
const int SCREEN_W = 894;
const int SCREEN_H = 700;
ALLEGRO_FONT *FONTE;
ALLEGRO_FONT *FONTE2;
int X_OPT;
int Y_OPT;
const float FPS = 60;
void initglobais(){
	X_OPT = 4*SCREEN_W/6;
	Y_OPT = 500;
	int i = 0;
	char str[80];
	int tam_fonte = 32;
	if(SCREEN_W<300)
	tam_fonte = 15;
	FONTE =  al_load_font("arial.ttf", tam_fonte, 1);
	tam_fonte = 60;
	FONTE2 =  al_load_font("arial.ttf", tam_fonte, 1);
	if(FONTE == NULL){
		fprintf(stderr,"font file does not exist or cannot be acessed!\n");
	}
}
int trocafigura(float u){
	if(u<=frames/9&&u>=0){
		return 0;
	}
	if(u<=2*frames/9&&u>=frames/9){
		return 1;
	}
	if(u<=3*frames/9&&u>=2*frames/9){
		return 2;
	}
	if(u<=4*frames/9&&u>=3*frames/9){
		return 3;
	}
	if(u<=5*frames/9&&u>=4*frames/9){
		return 4;
	}
	if(u<=6*frames/9&&u>=5*frames/9){
		return 5;
	}
	if(u<=7*frames/9&&u>=6*frames/9){
		return 6;
	}
	if(u<=8*frames/9&&u>=7*frames/9){
		return 7;
	}if(u<=9*frames/9&&u>=8*frames/9){
		return 8;
	}
}
typedef struct Heroi
{
	float x;
	float altura_heroi;
	float largura_heroi;
	float y;
	float yrela;
	float xrela;
	float x_old,y_old;
	float vel;
	int dir, esq,cim,bai;
	int poder;
	int score;
	int especial;
	int vida,acao,naveg,navegataque,ataque;
} Heroi;
int chegouObjetivo(Heroi h, int x, int y){
	if(h.x+h.largura_heroi-5 >= SCREEN_W - x&&h.yrela<=y){
		return 1;
	}
	return 0;
}
void initHeroi(Heroi *heroi){
	heroi->x = 0;
	heroi->y = SCREEN_H-70;
	heroi->altura_heroi = 10;
	heroi->largura_heroi = 20;
	heroi->vel = 1.5;
	heroi->dir = 0;
	heroi->esq = 0;
	heroi->cim = 0;
	heroi->ataque = 0;
	heroi->especial = 0;
	heroi->score = 0;
	heroi->bai = 0;
	heroi->poder = 30;
	heroi->vida = 100;
	heroi->acao = 0;
	heroi->naveg = 0;
	heroi->navegataque = 0;
	heroi->x_old = heroi->x;
	heroi->y_old = heroi->y;
}
typedef struct Monstro{
	int xm;
	int ym;
	int vida;
	int vidaog;
	int acao;
	float altura_monstro;
	int largura;
	int elemento;
	int poder;
}Monstro;
//inicia o mostro em localizações aleatórias
void animacao(ALLEGRO_BITMAP *anim[], float tamanho,float u,int a, int b){
	float y = tamanho/frames;
	int x = round(u*(tamanho/frames));
	al_draw_bitmap(anim[(int)x],a,b,0);
	
}
void animacao_heroi(ALLEGRO_BITMAP *anim[], int tamanho,float u){
	int i=0;
	int x = 7;
	if(trocafigura(u)>=tamanho){
	al_draw_bitmap(anim[x],750,200,0);
	}else{
	al_draw_bitmap(anim[trocafigura(u)],750,200,0);}
}
float distancia(Heroi h,Monstro m){
	return sqrt(pow(h.xrela-m.xm,2)+pow(h.yrela-m.ym,2));
}
void initMonstro(Monstro monstro[],Heroi h,int a,int b,int largura[]){
	for (int i = 0; i<NMonstro; i++)
	{
	do{
	monstro[i].xm = (rand()%SCREEN_W);
	monstro[i].ym = (rand()%SCREEN_H);}while(distancia(h,monstro[i])<100||(monstro[i].xm>=a&&monstro[i].ym<=b));
	monstro[i].elemento = rand()%4;
	monstro[i].vida = 90+10*monstro[i].elemento;
	monstro[i].vidaog = monstro[i].vida;
	monstro[i].largura = largura[monstro[i].elemento];
	monstro[i].poder = 23-2*monstro[i].elemento;
	monstro[i].acao = 0;
	printf("Monstro %d: %d %d\n",i+1, monstro[i].xm, monstro[i].ym );
	}
}
int direcao(Heroi heroi,int x,int u){
	if(heroi.cim==1){
		if(trocafigura(u)==8){
			  return 8+trocafigura(u);
		}else{	
    return 9+trocafigura(u);}
	}
	else if(heroi.esq==1){
	return 17+trocafigura(u);
		}
	else if(heroi.bai==1){
	return 26+trocafigura(u);
		}
	else if(heroi.dir==1){
	return 35+trocafigura(u);
		}else{
			return x;
		}
}
void draw_monstro(Monstro monstro[]){
	for (int i = 0; i < NMonstro; i++)
	{	
	if(monstro[i].elemento==0){
	al_draw_filled_circle(monstro[i].xm, monstro[i].ym, 20, al_map_rgb(247,13,13));
	}else if(monstro[i].elemento==1){
	al_draw_filled_circle(monstro[i].xm, monstro[i].ym, 20, al_map_rgb(247,240,13));
	}else if(monstro[i].elemento==2){
	al_draw_filled_circle(monstro[i].xm, monstro[i].ym, 20, al_map_rgb(47,27,234));}
	else if(monstro[i].elemento==3){
	al_draw_filled_circle(monstro[i].xm, monstro[i].ym, 20, al_map_rgb(153,76,0));}
	}
}
// verifica se o herói está no mesmo lugar que o monstro
int verificaheroi(Heroi h,Monstro m[],int *c){
	int i;
	for(i=0;i<NMonstro;i++){
		if(distancia(h,m[i])<=21){
			*c=i;
			return 1;
		}
	}
	return 0;
}
//zera o teclado
void zera_direcao(Heroi *heroi){
	heroi->dir = 0;
	heroi->bai = 0;
	heroi->cim = 0;
	heroi->esq = 0;
}
//atualiza o heroi
void update_heroi( Heroi *heroi){
	heroi->x_old = heroi->x;
	heroi->y_old = heroi->y;
	if(heroi->dir&& heroi->x + heroi->largura_heroi <= SCREEN_W){
		heroi->x+=heroi->vel;
		heroi->xrela = heroi->x + heroi->largura_heroi/2;
	}
	if(heroi->esq && heroi->x - heroi->vel>=0){
		heroi->x-=heroi->vel;
		heroi->xrela = heroi->x + heroi->largura_heroi/2;
	}
	if(heroi->cim&& heroi->y + heroi->vel >= 0){
		heroi->y-=heroi->vel;
		heroi->yrela = heroi->y+heroi->altura_heroi;
	}
	if(heroi->bai&&heroi->y+heroi->altura_heroi<=SCREEN_H){
		heroi->y+=heroi->vel;
		heroi->yrela = heroi->y+heroi->altura_heroi;
	}
}
//navegação com o teclado
void processaTeclaNaveg(Heroi *heroi, int tecla){
	switch(tecla){
				case ALLEGRO_KEY_A:
				heroi->esq = 1;
				break;
				case ALLEGRO_KEY_LEFT:
				heroi->esq = 1; 

				break;
				case ALLEGRO_KEY_D:
				heroi->dir = 1;

				break;
				case ALLEGRO_KEY_RIGHT:
				heroi->dir = 1;

				break;
				case ALLEGRO_KEY_W:
				heroi->cim = 1;
				break;
				case ALLEGRO_KEY_UP:
				heroi->cim = 1;
				break;
				case ALLEGRO_KEY_S:
				heroi->bai = 1;
	
				break;
				case ALLEGRO_KEY_DOWN:
				heroi->bai = 1;

				break;
			}
}
void processaTecladown(Heroi *heroi,int tecla){
	switch(tecla){
				case ALLEGRO_KEY_A:
				heroi->esq = 0;
	
				break;
				case ALLEGRO_KEY_LEFT:
				heroi->esq = 0; 
	
				break;
				case ALLEGRO_KEY_D:
				heroi->dir = 0;
	
				break;
				case ALLEGRO_KEY_RIGHT:
				heroi->dir = 0;
	
				break;
				case ALLEGRO_KEY_W:
				heroi->cim = 0;

				break;
				case ALLEGRO_KEY_UP:
				heroi->cim = 0;

				break;
				case ALLEGRO_KEY_S:
				heroi->bai = 0;
		
				break;
				case ALLEGRO_KEY_DOWN:
				heroi->bai = 0;
	
				break;
			}
}
void processaTeclabatalha(Heroi *heroi,int tecla,Monstro m){
	if(heroi->acao==0){
	if(heroi->naveg==1){
	switch(tecla){
	case ALLEGRO_KEY_W:
	heroi->naveg -=1;
	break;
	case ALLEGRO_KEY_UP:
	heroi->naveg -=1;
	break;
	case ALLEGRO_KEY_S:
	heroi->naveg +=1;
	break;
	case ALLEGRO_KEY_DOWN:
	heroi->naveg +=1;
	break;
	}}else if(heroi->naveg==0){
	switch(tecla){
	case ALLEGRO_KEY_S:
	heroi->naveg +=1;
	break;
	case ALLEGRO_KEY_DOWN:
	heroi->naveg +=1;
	break;
	}}else if(heroi->naveg==2){
	switch(tecla){
	case ALLEGRO_KEY_W:
	heroi->naveg -=1;
	break;
	case ALLEGRO_KEY_UP:
	heroi->naveg -=1;
	break;
	case ALLEGRO_KEY_ENTER:
	heroi->especial =1;
	break;
	default:
		break;}
	}
	switch (tecla)
	{
	case ALLEGRO_KEY_ENTER:
		heroi->acao = 1;
		break;
	
	default:
		break;
	}}else if(heroi->naveg == 0&& heroi->acao == 1&&heroi->ataque == 0&&m.acao == 0){
		if(heroi->navegataque==1){
		switch(tecla){
	case ALLEGRO_KEY_W:
	heroi->navegataque -=1;
	break;
	case ALLEGRO_KEY_UP:
	heroi->navegataque -=1;
	break;
	case ALLEGRO_KEY_S:
	heroi->navegataque +=1;
	break;
	case ALLEGRO_KEY_DOWN:
	heroi->navegataque +=1;
	break;
	case ALLEGRO_KEY_ESCAPE:
	heroi->acao = 0;
	break;
	}}else if(heroi->navegataque==0){
	switch(tecla){
	case ALLEGRO_KEY_S:
	heroi->navegataque +=1;
	break;
	case ALLEGRO_KEY_DOWN:
	heroi->navegataque +=1;
	break;
	case ALLEGRO_KEY_ESCAPE:
	heroi->acao = 0;
	break;
	}}else if(heroi->navegataque==2){
	switch(tecla){
	case ALLEGRO_KEY_S:
	heroi->navegataque +=1;
	break;
	case ALLEGRO_KEY_DOWN:
	heroi->navegataque +=1;
	break;
	case ALLEGRO_KEY_W:
	heroi->navegataque -=1;
	break;
	case ALLEGRO_KEY_UP:
	heroi->navegataque -=1;
	break;
	case ALLEGRO_KEY_ESCAPE:
	heroi->acao = 0;
	break;
	default:
		break;}
	}
	else if(heroi->navegataque=3){
	switch(tecla){
	case ALLEGRO_KEY_W:
	heroi->navegataque -=1;
	break;
	case ALLEGRO_KEY_UP:
	heroi->navegataque -=1;
	break;
	case ALLEGRO_KEY_ESCAPE:
	heroi->acao = 0;
	break;
	default:
		break;}
	}
	switch(tecla){
		case ALLEGRO_KEY_ENTER:
		heroi->ataque = 1;
	} }
}
void processa_ataque_especial(Heroi *h,Monstro *m, int d){
	if(d==2){
		m->vida = m->vida - 2*h->poder;
	}else if(d==1){
		m->vida = m->vida - h->poder;
	}else if(d==0||d==3){
		m->vida = m->vida - 0.5*h->poder;
	}
}
void processa_ataque(Heroi *h,Monstro *m){
	if(m->elemento == h->navegataque){
		m->vida = m->vida + 15;
		m->poder =m->poder +10;
	}else if(m->elemento == 0 &&h->navegataque == 2){
		m->vida = m->vida - h->poder*1.2;
	}else if(m->elemento == 1 &&h->navegataque == 3){
		m->vida = m->vida - h->poder*1.5;
	}else if(m->elemento == 2 &&h->navegataque == 1){
		m->vida = m->vida - h->poder*1.5;
	}
	else if(m->elemento == 3 &&h->navegataque == 0){
		m->vida = m->vida - h->poder*1.5;
	}
	else{
		m->vida =m->vida - h->poder;}
}
void timer(float segundos, float *i, int *ponteiro){
	if(*i>=segundos*FPS){
		*i=0;
		*ponteiro =  1;
	}else{
		*i+=1;
		return;
	}
}
void processa_ataque_monstro(Monstro *m,Heroi *h){
		h->vida =h->vida - m->poder;
}
void ataque(Heroi *h, Monstro *m, ALLEGRO_BITMAP *hero[],ALLEGRO_BITMAP *eletric[],ALLEGRO_BITMAP *fireespirit[],ALLEGRO_BITMAP *sharkattack[],float *u,int *modo,ALLEGRO_BITMAP *fireattack[],ALLEGRO_BITMAP *aquaattack[],ALLEGRO_BITMAP *bolt[],ALLEGRO_BITMAP *eletricattack[],ALLEGRO_BITMAP *eartgattack[],ALLEGRO_BITMAP *earthattack[],ALLEGRO_BITMAP *tornadospin[],ALLEGRO_BITMAP *especialattack[],ALLEGRO_BITMAP *smoke[],float *i,float *tempo,int *ponteiro, int *playing,int *d,ALLEGRO_SAMPLE* song,float *e){
	int x;
	if(h->vida<=0){
			*modo = 3;
			*e=1;
			al_stop_samples();
		}
	if(h->naveg == 1&&h->acao == 1&&m->acao == 0){
		if(*d==0||*d==3){
		animacao(tornadospin,10,*u,100,200);
		}else if(*d == 1){
		animacao(tornadospin,10,*u,100,200);	
		}else if(*d == 2){
		animacao(especialattack,11,*u,-100,-40);
		}
		animacao_heroi(hero,7,*u);
		*u+=1.5;
		if(*u>=frames){
			processa_ataque_especial(h,m,*d);
			*d= rand()%4;
			*u = 0;
			h->acao = 0;
			h->especial = 0;
			h->ataque = 0;
			*ponteiro = 0;
			m->acao = 1;
		}
	}else if(h->ataque == 1){
		if(h->navegataque==0){
			animacao(fireespirit,20,*u,150,100);	
		}
		if(h->navegataque==1){
			animacao(bolt,13,*u,50,100);
		}
		if(h->navegataque==2){
			animacao(sharkattack,13,*u,150,150);
		}
		if(h->navegataque==3){
			animacao(earthattack,11,*u,50,50);
			*u+=1;
		}
		animacao_heroi(hero,7,*u);
		*u+=1;
	
		if(*u>=frames){
			processa_ataque(h,m);
			h->acao = 0;
			h->ataque = 0;
			*u=0;
			*ponteiro = 0;
			m->acao = 1;
		}}else{
			al_draw_bitmap(hero[1], 750, 200, 0);
		}
		if(m->vida<=0){
			if(*ponteiro == 0){
			if(*e==0){
			al_stop_samples();
			}
			al_play_sample(song, 0.7, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
			timer(4,i,ponteiro);
			animacao(smoke,27,*e,m->altura_monstro/2,m->largura/2 - 30);
			if(*e!=frames){
				*e+=0.5;
			}}
			if(*ponteiro == 1){
			*modo = 1;
			h->vida += 20;
			*ponteiro = 0;
			m->xm = -30;
			*e= 0;
			h->score +=70;
			al_stop_samples();}
		}
	if(m->acao == 1&&m->vida>0){
		timer(1.5,i,ponteiro);
		if(*ponteiro == 1){
		if(m->elemento == 0){
		animacao(fireattack,22,*i,400,150);
		*i+=1.0;}
		else if(m->elemento == 1){
		animacao(eletricattack,16,*i,550,50);
		*i+=0.5;}else if(m->elemento ==2){
		animacao(aquaattack,6,*i,300,200);
		*i+=2;}
		else if(m->elemento ==3){
		animacao(eartgattack,5,*i,550,-250);
		*i+=2;}
		if(*i>=frames){
			*ponteiro = 0;
			*i=0;
			m->acao = 0;
			processa_ataque_monstro(m,h);
		}
	}}
}
void cenario_ataque(Heroi *h){
	al_draw_filled_rounded_rectangle(X_OPT/2-5,Y_OPT-5,X_OPT-5,SCREEN_H,20,20,al_map_rgb(255, 255, 255));
	al_draw_filled_rounded_rectangle(X_OPT/2,Y_OPT,X_OPT-7,SCREEN_H-5,20,20,al_map_rgb(49, 27, 146));
	switch(h->navegataque){
	case 0:
	al_draw_filled_circle(X_OPT/2+30,Y_OPT+30,10,al_map_rgb(255,255,255));
	break;
	case 1:
	al_draw_filled_circle(X_OPT/2+30,Y_OPT+75,10,al_map_rgb(255,255,255));
	break;
	case 2:
	al_draw_filled_circle(X_OPT/2+30,Y_OPT+120,10,al_map_rgb(255,255,255));
	break;
	case 3:
	al_draw_filled_circle(X_OPT/2+30,Y_OPT+165,10,al_map_rgb(255,255,255));
	break;
	}
	al_draw_text(FONTE, al_map_rgb(255, 255, 255), X_OPT/2+45, Y_OPT+10, 0, "Ember Fire Fang");
	al_draw_text(FONTE, al_map_rgb(255, 255, 255), X_OPT/2+45, Y_OPT+55, 0, "Thunder Shock");
	al_draw_text(FONTE, al_map_rgb(255, 255, 255), X_OPT/2+45, Y_OPT+100, 0, "Shark Projectile");
	al_draw_text(FONTE, al_map_rgb(255, 255, 255), X_OPT/2+45, Y_OPT+145, 0, "Stone Edge");
}
void cenario_batalha(Heroi *h,int *modo, Monstro *m, int d){
	if(d==2){
		if(time(NULL)%2==0){
			al_draw_filled_rounded_rectangle(X_OPT-5,Y_OPT-5,SCREEN_W,SCREEN_H,20,20,al_map_rgb(241, 21, 222));
		}else{
			al_draw_filled_rounded_rectangle(X_OPT-5,Y_OPT-5,SCREEN_W,SCREEN_H,20,20,al_map_rgb(255, 255, 255));
		}
	}else{
	al_draw_filled_rounded_rectangle(X_OPT-5,Y_OPT-5,SCREEN_W,SCREEN_H,20,20,al_map_rgb(255, 255, 255));}
	al_draw_filled_rounded_rectangle(X_OPT,Y_OPT,SCREEN_W-5,SCREEN_H-5,20,20,al_map_rgb(49, 27, 146));
	//vida do heroi
	al_draw_filled_rounded_rectangle(753,158,857,170,5,5,al_map_rgb(255, 255, 255));
	if(h->vida>=0){
	if(h->vida>=67){
	al_draw_filled_rounded_rectangle(755,160,755+h->vida,168,5,5,al_map_rgb(77, 222, 41));}else if(h->vida>=33){
	al_draw_filled_rounded_rectangle(755,160,755+h->vida,168,5,5,al_map_rgb(235, 248, 95));
	}else{
	al_draw_filled_rounded_rectangle(755,160,755+h->vida,168,5,5,al_map_rgb(191, 46, 29));
	}}
	// vida do monstro
	if(m->vida<=m->vidaog&&m->vida>0){
	al_draw_filled_rounded_rectangle(m->largura/2-50,38,m->largura/2+((m->largura/2)*m->vidaog)/100+2,50,5,5,al_map_rgb(255, 255, 255));}
	else if(m->vida>0){
	al_draw_filled_rounded_rectangle(m->largura/2-50,38,m->largura/2+((m->largura/2)*m->vida)/100+2,50,5,5,al_map_rgb(255, 255, 255));	
	}
	if(m->vida>=67){
	al_draw_filled_rounded_rectangle(m->largura/2-48,40,m->largura/2+((m->largura/2)*m->vida)/100,48,5,5,al_map_rgb(77, 222, 41));}
	else if(m->vida>=33){
	al_draw_filled_rounded_rectangle(m->largura/2-48,40,m->largura/2+((m->largura/2)*m->vida)/100,48,5,5,al_map_rgb(235, 248, 95));
	}else if(m->vida>0){
	al_draw_filled_rounded_rectangle(m->largura/2-48,40,m->largura/2+((m->largura/2)*m->vida)/100/100,48,5,5,al_map_rgb(191, 46, 29));
	}
	if(h->acao == 1){
			if(h->naveg == 2){
			if(rand()%100>40){
			h->naveg = 0;
			h->xrela = h->x_old;
			h->yrela = h->y_old;
			h->especial = 0;
			h->acao = 0;
			h->naveg = 0;
			al_stop_samples();
			*modo = 1;}else{
			m->acao = 1;
			h->acao = 0;
			h->especial = 0;
			h->naveg = 0;
			}
		}else if(h->naveg <= 0){
			cenario_ataque(h);
	}else if(h->naveg == 1){
			h->especial = 1;
	}}
	switch(h->naveg){
	case 0:
	al_draw_filled_circle(X_OPT+30,Y_OPT+30,10,al_map_rgb(255,255,255));
	break;
	case 1:
	if(d==2){
	if(time(NULL)%2==0){
	al_draw_filled_circle(X_OPT+30,Y_OPT+75,10,al_map_rgb(241, 21, 222));}else{
	al_draw_filled_circle(X_OPT+30,Y_OPT+75,10,al_map_rgb(255,255,255));	
	}
	}else{
	al_draw_filled_circle(X_OPT+30,Y_OPT+75,10,al_map_rgb(255,255,255));
	}
	break;
	case 2:
	al_draw_filled_circle(X_OPT+30,Y_OPT+120,10,al_map_rgb(255,255,255));
	break;
	}
	al_draw_text(FONTE, al_map_rgb(255, 255, 255), X_OPT+45, Y_OPT+10, 0, "Atacar");
	if(d==2){
		if(time(NULL)%2==0){
			al_draw_text(FONTE, al_map_rgb(241, 21, 222), X_OPT+45, Y_OPT+55, 0, "Especial");
		}
		else{
	al_draw_text(FONTE, al_map_rgb(255, 255, 255), X_OPT+45, Y_OPT+55, 0, "Especial");}
	}else{
	al_draw_text(FONTE, al_map_rgb(255, 255, 255), X_OPT+45, Y_OPT+55, 0, "Especial");}
	al_draw_text(FONTE, al_map_rgb(255, 255, 255), X_OPT+45, Y_OPT+100, 0, "Fugir");
}
void anima_monstro(Monstro m,ALLEGRO_BITMAP *dragon[],ALLEGRO_BITMAP *eletric[],ALLEGRO_BITMAP *raios[],ALLEGRO_BITMAP *aqua[],ALLEGRO_BITMAP *earth[],float *i){
	if(m.vida>0){
	if(*i>=frames){
		*i=0;
	}
	if(m.elemento== 0){
		animacao(dragon,59,*i,-10,50);
		*i+=1;
	}else if(m.elemento == 1){
		animacao(eletric,7,*i,50,50);
		animacao(raios,6,*i,50,50);
	 *i+=1.5;
	}else if(m.elemento == 2){
		animacao(aqua,59,*i,50,100);
		*i+=1;
	}else if(m.elemento == 3){
		animacao(earth,8,*i,50,50);
		*i+=1;
	}}
}
void game_over(){
	int x = rand()%2;
	al_clear_to_color(al_map_rgb(0,0,0));
	al_draw_rectangle(SCREEN_W/2-150, SCREEN_H/2-70, SCREEN_W/2+120, SCREEN_H/2+80,al_map_rgb(218,33,33),8);
	al_draw_text(FONTE, al_map_rgb(218,33,33), SCREEN_W/2-100, SCREEN_H/2-15, 0, "Game Over");
}
void tela_vitoria(int pontuacao,int scores){
	char score[9];
	char scoreatual[9];
	sprintf(score,"%d",pontuacao);
	sprintf(scoreatual,"%d",scores);
	al_clear_to_color(al_map_rgb(0,0,0));
	if(pontuacao>scores){
	al_draw_text(FONTE, al_map_rgb(89,13, 198), SCREEN_W/2-130, SCREEN_H/2+15, 0, "Sua pontuação:");
	al_draw_text(FONTE, al_map_rgb(89,13, 198), SCREEN_W/2+100, SCREEN_H/2+15, 0, scoreatual);
	}else{
	al_draw_text(FONTE, al_map_rgb(22, 191, 129), SCREEN_W/2-130, SCREEN_H/2+15, 0, "Sua pontuação:");
	al_draw_text(FONTE, al_map_rgb(22, 191, 129), SCREEN_W/2+100, SCREEN_H/2+15, 0, scoreatual);
	}
	al_draw_rectangle(SCREEN_W/2-150, SCREEN_H/2-70, SCREEN_W/2+200, SCREEN_H/2+140,al_map_rgb(22, 191, 129),8);
	al_draw_text(FONTE, al_map_rgb(22, 191, 129), SCREEN_W/2-130, SCREEN_H/2-35, 0, "Congratulations!!");
	al_draw_text(FONTE, al_map_rgb(22, 191, 129), SCREEN_W/2-130, SCREEN_H/2+65, 0, "Record:"); 
	al_draw_text(FONTE, al_map_rgb(22, 191, 129), SCREEN_W/2-10, SCREEN_H/2+65, 0, score); 
}
int main(){
    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_TIMER *timer = NULL;
	//figura do herói
	ALLEGRO_BITMAP *castle = NULL;
	ALLEGRO_BITMAP *cenario = NULL;
	ALLEGRO_BITMAP *florest = NULL;
    bool redraw = true;
   //rotinas de inicialização
    if(!al_init()) {
		fprintf(stderr, "failed to initialize allegro!\n");
		return -1;
	}
    display = al_create_display(SCREEN_W, SCREEN_H);
	if(!display) {
		fprintf(stderr, "failed to create display!\n");
		return -1;
	}
    timer = al_create_timer(1.0 / FPS);
    if(!timer) {
		fprintf(stderr, "failed to create timer!\n");
		return -1;
	}
     if(!al_init_primitives_addon()){
		fprintf(stderr, "failed to initialize primitives!\n");
        return -1;
    }	
    event_queue = al_create_event_queue();
	if(!event_queue) { 
		fprintf(stderr, "failed to create event_queue!\n");
		al_destroy_display(display);
		return -1;
	}
    if(!al_install_keyboard()) {
		fprintf(stderr, "failed to install keyboard!\n");
		return -1;
	}
	//instala o mouse
	if(!al_install_mouse()) {
		fprintf(stderr, "failed to initialize mouse!\n");
		return -1;
	}
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));  
    al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_mouse_event_source()); 
	srand(time(NULL));
	al_init_font_addon();
	al_init_ttf_addon();
	initglobais();
	//coloca áudio no jogo
	al_install_audio();
    al_init_acodec_addon();
    al_reserve_samples(1);
	ALLEGRO_SAMPLE* song[5];
	//coloca a imagem do heroi, do castelo e do cenário
	al_init_image_addon();
	castle = al_load_bitmap("imagens/castle.png");
	cenario = al_load_bitmap("imagens/cenario.jpg");
	florest = al_load_bitmap("imagens/forest.png");
	//dimensões das imagens
	int castle_largura = al_get_bitmap_width(castle);
	int castle_altura = al_get_bitmap_height(castle);
	//cria heroi
	Heroi heroi;
	initHeroi(&heroi);
	
	//-------------------------------------------------------------
	//Cria os sprites
	ALLEGRO_BITMAP *hero[44];
	ALLEGRO_BITMAP *dragon[61];
	ALLEGRO_BITMAP *dragoneletric[9];
	ALLEGRO_BITMAP *aqua[61];
	ALLEGRO_BITMAP *earth[10];
	ALLEGRO_BITMAP *fireespirit[23];
	ALLEGRO_BITMAP *fenix[3];
	ALLEGRO_BITMAP *eletric[25];
	ALLEGRO_BITMAP *sharkattack[13];
	ALLEGRO_BITMAP *dragonfire[6];
	ALLEGRO_BITMAP *fireattack[24];
	ALLEGRO_BITMAP *eartgattack[6];
	ALLEGRO_BITMAP *earthattack[12];
	ALLEGRO_BITMAP *aquaattack[8];
	ALLEGRO_BITMAP *raios[8];
	ALLEGRO_BITMAP *eletricattack[24];
	ALLEGRO_BITMAP *tornadospin[13];
	ALLEGRO_BITMAP *bolt[15];
	ALLEGRO_BITMAP *especialattack[13];
	ALLEGRO_BITMAP *smoke[28];
	int i=0;
	float u = 0;
	char str[80];
	//define as musicas
		song[0] = al_load_sample("music/ff6atma.wav");
		song[1] = al_load_sample("music/ff6battle.wav");
		song[2] = al_load_sample("music/Won.wav");
		song[3] = al_load_sample("music/wim.ogg");
		song[4] = al_load_sample("music/GameOver.ogg");
	
	//define as animações de cada monstro, do herói e dos ataques
	//animação do eherói
	for ( i = 0; i <44; i++)
	{	
		sprintf(str,"imagens/hero%d.png",i);
		hero[i] = al_load_bitmap(str);
	}
	heroi.altura_heroi = al_get_bitmap_height(hero[8]);
	heroi.largura_heroi = al_get_bitmap_width(hero[8]);
	heroi.yrela = heroi.y + heroi.altura_heroi;
	heroi.xrela = heroi.x + heroi.largura_heroi/2;




	//ataque de água do herói
	for ( i = 0; i <13; i++)
	{	
		sprintf(str,"imagens/sharkattack (%d).png",i+1);
		sharkattack[i] = al_load_bitmap(str);
	}
	//ataque de fogo do herói
		for ( i = 0; i <21; i++)
	{	
		sprintf(str,"imagens/fireaatack (%d).png",i+1);
		fireespirit[i] = al_load_bitmap(str);
	}
	//animação ataque elétrico do herói
		for ( i = 0; i <14; i++)
	{	
		sprintf(str,"imagens/bolt (%d).png",i+1);
		bolt[i] = al_load_bitmap(str);
	}
		for ( i = 0; i <24; i++)
	{	
		sprintf(str,"imagens/eletric (%d).png",i+1);
		eletric[i] = al_load_bitmap(str);
	}
	//animação ataque de terra do herói
	for ( i = 0; i <12; i++)
	{	
		sprintf(str,"imagens/earthattack-%d.png",i);
		earthattack[i] = al_load_bitmap(str);
	}
	//animação monstro elétrico
		for ( i = 0; i <9; i++)
	{	
		sprintf(str,"imagens/eletricdragon-%d.png",i);
		dragoneletric[i] = al_load_bitmap(str);
	}
	//animação de raios no monstro elétrico
	for ( i = 0; i <8; i++)
	{	
		sprintf(str,"imagens/eletricprodg-%d.png",i);
		raios[i] = al_load_bitmap(str);
	}
	//animação ataque do monstro elétrico
	for ( i = 0; i <17; i++)
	{	
		sprintf(str,"imagens/eletricattackn-%d.png",i);
		eletricattack[i] = al_load_bitmap(str);
	}
	//animação monstro aquático
		for ( i = 0; i <60; i++)
	{	
		sprintf(str,"imagens/aqua-%d.png",i);
		aqua[i] = al_load_bitmap(str);
	}
	//animação de ataque do monstro aquático
	for ( i = 0; i <7; i++)
	{	
		sprintf(str,"imagens/aquaattack-%d.png",i);
		aquaattack[i] = al_load_bitmap(str);
	}
	//animação do monstro de fogo
		for ( i = 0; i <60; i++)
	{	
		sprintf(str,"imagens/fire-%d.png",i);
		dragon[i] = al_load_bitmap(str);
	}
	//animação de atauqe do monstro de fogo
	for ( i = 0; i <23; i++)
	{	
		sprintf(str,"imagens/firea (%d).png",i+1);
		fireattack[i] = al_load_bitmap(str);
	}
	//animação monstro de terra
	for ( i = 0; i <10; i++)
	{	
		sprintf(str,"imagens/earth-%d.png",i);
		earth[i] = al_load_bitmap(str);
	}	
	//ataque do monstro de terra
	for ( i = 0; i <6; i++)
	{	
		sprintf(str,"imagens/eartgattack-%d.png",i);
		eartgattack[i] = al_load_bitmap(str);
	}
	//especial 1
	for ( i = 0; i <6; i++)
	{	
		sprintf(str,"imagens/eartgattack-%d.png",i);
		eartgattack[i] = al_load_bitmap(str);
	}for ( i = 0; i <11; i++)
	{	
		sprintf(str,"imagens/tornado-spin-%d.png",i);
		tornadospin[i] = al_load_bitmap(str);
	}
	
	for ( i = 0; i <12; i++)
	{	
		sprintf(str,"imagens/tornado (%d).png",i+1);
		especialattack[i] = al_load_bitmap(str);
	}
	//cria os monstros e os sorteia aleatóriamente pelo mapa
	for ( i = 0; i <28; i++)
	{	
		sprintf(str,"imagens/smoke-%d.png",i);
		smoke[i] = al_load_bitmap(str);
	}
	//animação da morte dos monstros
	int largura[4];
	largura[0] = al_get_bitmap_width(dragon[1]);
	largura[1] = al_get_bitmap_width(aqua[1]);
	largura[2] = al_get_bitmap_width(dragoneletric[1]);
	largura[3]= al_get_bitmap_width(earth[1]);
	Monstro monstro[NMonstro];
	initMonstro(monstro,heroi,SCREEN_W-castle_largura,castle_altura,largura);
	//-------------------------------------------------------------
	if(!hero[0]) {
      fprintf(stderr, "failed to create bitmap hero!\n");
      al_destroy_timer(timer);
	  al_destroy_display(display);
      return -1;
   }
    if(!castle) {
      fprintf(stderr, "failed to create bitmap castle!\n");
      al_destroy_timer(timer);
	  al_destroy_display(display);
      return -1;
   }
    if(!cenario) {
      fprintf(stderr, "failed to create bitmap cenario!\n");
      al_destroy_timer(timer);
	  al_destroy_display(display);
      return -1;
   }
	FILE *arq;
	FILE *recorde;
	int record=0;
	arq = fopen("record.txt","r");
	fscanf(arq,"%d",&record);
	fclose(arq);
	int posicao = 8;
    int playing = 1;
	char score[12];
	int modo=1;
	int x=8;
	int c,d=0;
	float e=0;
	float indice = 0;
	float indice2 = 0;
	float tempo = 0;
	int ponteiro = 0;
    al_start_timer(timer);
	//comeca a música
	al_draw_bitmap(castle,SCREEN_W-castle_largura,-15,0);
	u=8;
    while(playing){
    ALLEGRO_EVENT ev;
		//espera por um evento e o armazena na variavel de evento ev
	al_wait_for_event(event_queue, &ev);
    if(ev.type == ALLEGRO_EVENT_TIMER) {
		switch (modo)
		{
		case 1:
		heroi.acao = 0;
			al_play_sample(song[0], 0.7, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
			al_draw_bitmap(cenario,0,0,0);
			al_draw_bitmap(castle,SCREEN_W-castle_largura,-15,0);
			//reseta o loop da animação
			if(u>=frames){
				u=0;
			}
			if(verificaheroi(heroi,monstro,&c)==1){
				u=0;
				zera_direcao(&heroi);
				al_stop_samples();
				al_play_sample(song[1], 0.7, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
				modo = 2;
				ponteiro = 0;
				d = rand()%4;
				printf("FIGHT!!\n");
				break;
			}
			update_heroi(&heroi);	
			x = direcao(heroi,x,u);
			al_draw_bitmap(hero[x], heroi.x, heroi.y, 0);
			u++;
			// draw_monstro(monstro);
			if(chegouObjetivo(heroi,castle_largura,castle_altura)){
				if(heroi.score>record){
					recorde = fopen("record.txt","w");
					record=heroi.score;
					fprintf(recorde,"%d",record);
					fclose(recorde);
				}
					e=1;
				al_stop_samples();
				modo = 4;
			}
			sprintf(score,"%d",heroi.score);
			al_draw_text(FONTE2, al_map_rgb(255,255,255), 0, 0, 0, score);
			al_flip_display();		
		break;
		//modo de batalha
		case 2:
		al_draw_bitmap(florest, 0, 0, 0);
		cenario_batalha(&heroi,&modo,&monstro[c],d);
		anima_monstro(monstro[c],dragon,dragoneletric,raios,aqua,earth,&indice);
		// especial(&heroi,&monstro[c],&u);
		ataque(&heroi,&monstro[c],hero,eletric,fireespirit,sharkattack,&u,&modo,fireattack,aquaattack,bolt,eletricattack,eartgattack,earthattack,tornadospin,especialattack,smoke,&indice2,&tempo,&ponteiro,&playing,&d,song[2],&e);
		al_flip_display();		
		break;
		case 3:
		if(e==1){
		al_play_sample(song[4], 0.7, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
		}
		e= 2;
		game_over();
		break;
		case 4:
		if(e==1){
		al_play_sample(song[3], 0.7, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
		}
		e= 2;
		tela_vitoria(record,heroi.score);
		break;
		default:
			break;
		
		}
			al_flip_display();
		}
			//atualiza a tela (quando houver algo para mostrar)
			//
  	    else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			playing = 0;
		}
		//se o tipo de evento for um pressionar de uma tecla
		else if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			//imprime qual tecla foi
			if(modo == 1){
				processaTeclaNaveg(&heroi,ev.keyboard.keycode);
			}else if(modo == 2){
				processaTeclabatalha(&heroi,ev.keyboard.keycode,monstro[c]);
			}
			}else if(ev.type == ALLEGRO_EVENT_KEY_UP) {
				if(modo == 1){
				processaTecladown(&heroi,ev.keyboard.keycode);
			}				
			
		}
    }
	for ( i = 0; i < 2; i++){	
	al_destroy_sample(song[i]);
	}
	al_destroy_bitmap(castle);
		for ( i = 0; i < 45; i++){	
	al_destroy_bitmap(hero[i]);
	}
		for ( i = 0; i < 62; i++){	
	al_destroy_bitmap(dragon[i]);
	}
		for ( i = 0; i < 9; i++){	
	al_destroy_bitmap(aquaattack[i]);
	}
		for ( i = 0; i < 12; i++){	
	al_destroy_bitmap(fireattack[i]);
	}
		for ( i = 0; i < 7; i++){	
	al_destroy_bitmap(dragonfire[i]);
	}
		for ( i = 0; i < 62; i++){	
	al_destroy_bitmap(aqua[i]);
	}
		for ( i = 0; i < 10; i++){	
	al_destroy_bitmap(dragoneletric[i]);
	}
	for ( i = 0; i < 14; i++){	
	al_destroy_bitmap(sharkattack[i]);
	}
	for ( i = 0; i < 4; i++){	
	al_destroy_bitmap(dragon[i]);
	}
	for ( i = 0; i < 21; i++){	
	al_destroy_bitmap(fireespirit[i]);
	}
	for ( i = 0; i < 26; i++){	
	al_destroy_bitmap(eletric[i]);
	}
	al_destroy_bitmap(cenario);
   	al_destroy_timer(timer);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue); 
    return 0;
}