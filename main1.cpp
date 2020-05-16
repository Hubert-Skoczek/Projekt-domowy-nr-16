
#include "math.h"
#include "stdio.h"
#include "winbgi2.h"
#include "stdlib.h"
#include "rk4.h"
//gestosc powietrza, stały współczynnik C oraz przyciąganie ziemskie są stałe
double q=1.29;
double C=0.47;
double g=9.81;
//masa, wymiary, oraz współczynnik siły wiatru są zadawane z klawiatury
double m;
double r;
double S;
double wspF;
void funvy(double t,double *y, double *Fy);
void funvx(double t,double *X, double *Fy);
void main()
{
	//
	int a;
	//warunki początkowe
	double vy0=0.;
	double y0;
	double vx0;
	double x0=0;
	double t=0;
	//stały krok czasowy
	double h=0.001;	
	int n=2;
	//tablice potrzebne do dzialania metody numerycznej
	double Fy[2];
	double Fx[2];
	double Y1[2];
	double X1[2];
	//
	FILE *f;
	FILE *p;
	//wczytywanie z klawiatury masy, promienia, współczynnika siły wiatru i warunków początkowych
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
	printf("podaj wspolczynnik sily wiatru wspF [w Newtonach na sekunde] (sila wiatru\nprzelicza sie w sposob wspF*t)\n");
	scanf("%lf",&wspF);
	printf("podaj wysokosc z jakiej wystrzelono poziomo kule y0 [w metrach]\n");
	scanf("%lf",&y0);
	while(y0<=0)
	{
		printf("podaj wysokosc z jakiej wystrzelono poziomo kule y0 [w metrach]\n");
		scanf("%lf",&y0);
	}
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
	double Y[2]={y0,vy0};
	double X[2]={x0,vx0};
	S=3.14159265359*pow(r/100.,2.);//wyliczanie pola przekroju
	//
	graphics(1000,1000);
	double vxmax=-sqrt((2*m*wspF*2000)/(C*S*q));//predkosc wyliczona dla t=2000 potrzebna do skalowania wykresu
	double vymax=-sqrt((2*m*g)/(S*C*q));//terminalna prędkość kulki
	//program w zależności od wyboru wykresu odpowiednio rysuje skale w oknie graficznym
	switch(a)
	{
	case 1:
		title("wykres y(x),ziemia - czerwona linia,","","");
		scale(x0-2*wspF*y0,-10,x0+2*wspF*y0,y0+10);
		setcolor(1.5);//kolor czerwony
		line(x0-4*wspF*y0,0,x0+4*wspF*y0,0);
		break;
	case 2:
		title("wykres x(t),","","");
		scale(0,x0-4*wspF*y0,5*sqrt(2*y0/g),x0+4*wspF*y0);
		break;
	case 3:
		title("wykres y(t),","","");
		scale(0,-10,5*sqrt(2*y0/g),y0+10);
		break;
	case 4:
		title("wykres vx(t),","","");
		scale(0,vxmax/5,5*sqrt(2*y0/g),vx0+50);
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
		fprintf(f,"%.2lf\t",t);
		fprintf(p,"%.2lf\t",t);
		//stosowanie procedury vrk4 do rozwiązywania układów równań
		vrk4(t,Y,h,n,funvy,Y1);//dla składowych sił po y 
		vrk4(t,X,h,n,funvx,X1);//dla składowych sił po x
		//pętla sprawdzająca kiedy kulka uderzy w ziemie i zatrzymująca lot kulki
		if(Y1[0]<=0)
		{
			fprintf(f," 0.000000\t\t\t0.000000\n");
			fprintf(p," 0.000000\t\t\t0.000000\n");
			break;
		}
		//zapiswanie do odpowiednich plików
		fprintf(f,"%lf\t\t",Y1[1]);
		fprintf(f,"%lf\n",Y1[0]);
		fprintf(p,"%lf\t\t",X1[1]);
		fprintf(p,"%lf\n",X1[0]);
		setcolor(0.6);//kolor zielony
		//program w zależności od wyboru wykresu rysuje w oknie graficznym odpowiedni wykres
		switch(a)
		{
		case 1:
			circle(X1[0],Y1[0],2);//y(x)
			break;
		case 2:
			circle(t,X1[0],2);//x(t)
			break;
		case 3:
			circle(t,Y1[0],2);//y(t)
			break;
		case 4:
			circle(t,X1[1],2);//vx(t)
			break;
		case 5:
			circle(t,Y1[1],2);//vy(t)
			break;
		}
		Y[0]=Y1[0];
		Y[1]=Y1[1];
		X[0]=X1[0];
		X[1]=X1[1];
		t+=h;
	}
	//zamknięcie plików
	fclose(f);
	fclose(p);
	wait();
}
//funkcje potrzebne do procedury vrk4 obliczające prawe strony równań
void funvy(double t,double *Y, double *Fy)
{
	Fy[1]=-g+S*C*pow(Y[1],2)*q/(2*m);
	Fy[0]=Y[1];
}
void funvx(double t,double *X, double *Fx)
{
	if(X[1]>=0)
	{
		Fx[1]=-S*C*pow(X[1],2)*q/(2*m)-wspF*t/m;
		Fx[0]=X[1];
	}else if(X[1]<0){
		Fx[1]=S*C*pow(X[1],2)*q/(2*m)-wspF*t/m;
		Fx[0]=X[1];
	}
}