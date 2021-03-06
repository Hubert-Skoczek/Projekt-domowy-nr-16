#include "math.h"
#include "stdio.h"
#include "winbgi2.h"
#include "stdlib.h"
#include "rk4.h"
//gestosc powietrza, stały współczynnik C oraz przyciąganie ziemskie są stałe
double q=1.29;
double C=0.47;
double g=9.81;
//masa, wymiary, wysokosc, oraz współczynnik prędkości wiatru są zadawane z klawiatury
double m;
double r;
double S;
double H;
double wsp_v_wiatru;
void fun(double t,double *X, double *Fx);
void main()
{
	//
	int a;
	//warunki początkowe
	double vy0=0;
	double vx0;
	double x0=0;
	double t=0;
	double y0;
	//stały krok czasowy
	double h=0.001;	
	//tablice potrzebne do dzialania metody numerycznej
	double Fx[4];
	double X1[4];
	//
	FILE *f;
	FILE *p;
	//wczytywanie z klawiatury masy, promienia, współczynnika prędkości wiatru i warunków początkowych
	//oraz pętle zabezpieczające przed podaniem błędnej wartości
	printf("podaj mase kuli m [w kilogramach]\n");
	scanf("%lf",&m);
	while(m<=0)
	{
		printf("podaj mase kuli m [w kilogramach]\n");
		scanf("%lf",&m);
	}
	printf("podaj promien kuli r [w centymetrach]\n");
	scanf("%lf",&r);
	while(r<=0)
	{
		printf("podaj promien kuli r [w centymetrach]\n");
		scanf("%lf",&r);
	}
	printf("podaj wspolczynnik predkosci wiatru wsp_v_wiatru [w kg na sekunde kwadrat]\n(sile wiatru przelicza sie w sposob wsp_v_wiatru*(y0-y))\n");
	scanf("%lf",&wsp_v_wiatru);
	printf("podaj wysokosc z jakiej wysterzelono poziomo kule y0 [w metrach]\n");
	scanf("%lf",&y0);
	while(y0<=0)
	{
	printf("podaj wysokosc z jakiej wystrzelono poziomo kule y0 [w metrach]\n");
		scanf("%lf",&y0);
	}
	H=y0;
	printf("podaj predkosc poczatkowa z jaka wystrzelono kule v0 [w metrach na sekunde]\n");
	scanf("%lf",&vx0);
	//program moze narysowac wykresy y(x), x(t), y(t), vx(t), vy(t)
	printf("Jaki wykres ma wyswietlic program?\n");
	printf("wykres y(x)   -->   1\n");
	printf("wykres x(t)   -->   2\n");
	printf("wykres y(t)   -->   3\n");
	printf("wykres vx(t)  -->   4\n");
	printf("wykres vy(t)  -->   5\n");
	scanf("%d",&a);
	double X[4]={x0,vx0,y0,vy0};
	S=3.14159265359*pow(0.01*r,2.);//wyliczanie pola przekroju
	//
	graphics(1000,1000);
	double vymax=-sqrt((2*m*g)/(S*C*q));//terminalna prędkość kulki
	double vxmax=-sqrt(2*wsp_v_wiatru*(H)/(S*C*q));//predkosc potrzebna do skalowania
	//program w zależności od wyboru wykresu odpowiednio rysuje skale w oknie graficznym
	switch(a)
	{
	case 1:
		title("wykres y(x),ziemia - czerwona linia,","","");
		scale(x0-16*wsp_v_wiatru*y0,-10,x0+16*wsp_v_wiatru*y0,y0+10);
		setcolor(1.5);//kolor czerwony
		line(x0-16*wsp_v_wiatru*y0,0,x0+16*wsp_v_wiatru*y0,0);
		break;
	case 2:
		title("wykres x(t),","","");
		scale(0,x0-16*wsp_v_wiatru*y0,5*sqrt(2*y0/g),x0+16*wsp_v_wiatru*y0);
		break;
	case 3:
		title("wykres y(t),","","");
		scale(0,-10,5*sqrt(2*y0/g),y0+10);
		break;
	case 4:
		title("wykres vx(t),","","");
		scale(0,vxmax,5*sqrt(2*y0/g),vx0+50);
		break;
	case 5:
		title("wykres vy(t), predkosc terminalna - czerwona linia","","");
		scale(0,vymax-10,5*sqrt(2*y0/g),vy0+50);
		setcolor(1.5);//kolor czerwony
		line(0,vymax,5*sqrt(2*y0/g),vymax);
		break;
	}
	//otwarcie plików do których zapisywane są wyniki
	f=fopen("wyniki_y.txt","w");
	p=fopen("wyniki_x.txt","w");
	fprintf(f,"\t\tvy\t\ty\n");
	fprintf(p,"\t\tvx\t\tx\n");
	//
	while(t>=0)
	{
		fprintf(f,"%.3lf\t",t);
		fprintf(p,"%.3lf\t",t);
		//stosowanie procedury vrk4 do rozwiązania układu równań
		vrk4(t,X,h,4,fun,X1);
		//pętla sprawdzająca kiedy kulka uderzy w ziemie i zatrzymująca lot kulki
		if(X1[2]<=0)
		{
			fprintf(f," 0.000000\t\t\t%lf\n",X1[2]);
			fprintf(p," 0.000000\t\t%lf\n",X1[0]);
			break;
		}
		//zapiswanie do odpowiednich plików
		fprintf(f,"%lf\t\t",X1[3]);
		fprintf(f,"%lf\n",X1[2]);
		fprintf(p,"%lf\t\t",X1[1]);
		fprintf(p,"%lf\n",X1[0]);
		setcolor(0.6);//kolor zielony
		//program w zależności od wyboru wykresu rysuje w oknie graficznym odpowiedni wykres
		switch(a)
		{
		case 1:
			circle(X1[0],X1[2],2);//y(x)
			break;
		case 2:
			circle(t,X1[0],2);//x(t)

			break;
		case 3:
			circle(t,X1[2],2);//y(t)
			break;
		case 4:
			circle(t,X1[1],2);//vx(t)
			break;
		case 5:
			circle(t,X1[3],2);//vy(t)
			break;
		}
		X[0]=X1[0];
		X[1]=X1[1];
		X[2]=X1[2];
		X[3]=X1[3];
		t+=h;
	}
	//zamknięcie plików
	fclose(f);
	fclose(p);
	wait();
}
//funkcja potrzebna do procedury vrk4 obliczająca prawe strony równań
void fun(double t,double *X, double *Fx)
{
	//
	Fx[3]=-g+S*C*pow(X[3],2)*q/(2*m);
	Fx[2]=X[3];
	//
	if(X[1]>=0)
	{		
		Fx[1]=-S*C*pow(X[1],2)*q/(2*m)-wsp_v_wiatru*(H-X[2])/m;
	}else if(X[1]<0){
		Fx[1]=S*C*pow(X[1],2)*q/(2*m)-wsp_v_wiatru*(H-X[2])/m;
	}
	Fx[0]=X[1];
	//
}
