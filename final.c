#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include "gfx3.h"

struct Bomb
{
	int xstart;
	int ystart;
	int deltax;
	int deltay;
	int end; //0 through 8
	int xend;
	int yend;
	int status;
	int timeTilLaunch;
} typedef Bomb;

struct Missile
{
	int xstart;
	int ystart;
	int endx;
	int endy;
	int deltax;
	int deltay;
	int status;
	int baseNumber;
} typedef Missile;

struct City
{
	int xleft;
	int yleft;
	int width;
	int height;
	int status;
} typedef City;

struct Base
{
	int xleft;
	int yleft;
	int width;
	int height;
	int missileNumber;
	int status;
} typedef Base;

struct Explosion
{
	int x;
	int y;
	int radius;
	int deltaR;
} typedef Explosion;

enum STATUS
{
	dead,
	alive
};

int checkNumberOfBombs(Bomb bombArray[30]);
int checkNumberOfMissiles(Missile missileArray[30]);
int checkNumberOfCities(City cityArray[6]);
int win(Bomb bombArray[30],Missile missileArray[30], City cityArray[6]);
int lose(Bomb bombArray[30], Missile missileArray[30], City cityArray[6]);
int speed(int currentLevel);
int numberOfBombs(int currentLevel);
int checkNumberOfBombs(Bomb bombArray[30]); //done
int checkNumberOfMissiles(Missile missileArray[30]); //done
int checkNumberOfCities(City cityArray[6]); //done
int win(Bomb bombArray[30],Missile missileArray[30], City cityArray[6]); //done
int lose(Bomb bombArray[30], Missile missileArray[30], City cityArray[6]); //done
int speed(int currentLevel); //done
int numberOfBombs(int currentLevel); //done
void initializeBomb(Bomb bombArray[30], City cityArray[6], Base baseArray[3],int currentLevel);
void randomizeBomb(Bomb bombArray[30]); //done
void bombDestination(City cityArray[6], Base baseArray[3], Bomb bombArray[30]); //done
void initializeStructures(City cityArray[6], Base baseArray[3]); //done
void initializeMissiles(Missile missileArray[30], Base baseArray[3]);
void drawCities(City cityArray[6]);
void drawBases(Base baseArray[3]);
void drawMissiles(Missile missileArray[30]);
void drawBombs(Bomb bombArray[30]);
int checkIfBombInsideExplosion(Bomb bombArray[30],Explosion explosionArray[30]);


int main()
{
	time_t t;

	srand(time(&t));

	Base baseArray[3];
	City cityArray[6];
	Missile missileArray[30];
	Bomb bombArray[30];
	Explosion explosionArray[30];

	
	int currentLevel,maxLevel = 5;
	int width = 700;
	int height = 700;

	gfx_open(width,height,"MISSILE COMMAND");
	drawBases(baseArray);
	drawCities(cityArray);
	gfx_wait();
	
	for (currentLevel = 1; currentLevel <= maxLevel; currentLevel++)
	{
		while (!win(bombArray, missileArray, cityArray) && !lose(bombArray, missileArray, cityArray))
		{

		}

		if(lose(bombArray, missileArray, cityArray))
		{
			break;
		}
		
	}


}

//randomizes the time each bomb is detonated and the destination of each bomb
void randomizeBomb(Bomb bombArray[30])
{
	int i;

	for (i=0; i<30; i++)
	{
		if (bombArray[i].status==alive)
		{
			bombArray[i].end= rand() % 9; //generate a number 0 through 8
			bombArray[i].timeTilLaunch= rand() % 200; // generate a number 0 through 199
			bombArray[i].xstart = rand() % 700 +1; // generate a number 1 to 700
		}
	}
}

//initializes cityArray and baseArray
void initializeStructures(City cityArray[6], Base baseArray[3])
{
	int i, y=650;

	for (i=0; i<9;i++)
	{
		if (i==0)
		{
			baseArray[0].xleft=35+85*i;
			baseArray[0].yleft=y;
		}

		if (i==4)
		{
			baseArray[1].xleft=35+85*i;
			baseArray[1].yleft=y;
		}

		if (i==8)
		{
			baseArray[2].xleft=35+85*i;
			baseArray[2].yleft=y;
		}

		if ((i>=1) && (i<=3))
		{
			cityArray[i-1].xleft=35+85*i;
			cityArray[i-1].yleft=35+85*i;
		}

		if ((i>=5) && (i<=7))
		{
			cityArray[i-2].xleft=35+85*i;
			cityArray[i-2].yleft=y;
		}


	}
}

//given the number n from 0-8, calculates the 
void bombDestination(City cityArray[6], Base baseArray[3], Bomb bombArray[30])
{
	int i, n;

	for (i=0; i<30; i++)
	{
		n=bombArray[i].end;

		if ((n==0) || (n==4) || (n==8))
		{
			bombArray[i].xend=baseArray[n/4].xleft+25;
			bombArray[i].yend=baseArray[n/4].yleft;
		}

		if ((n>=1) && (n<=3))
		{
			bombArray[i].xend=cityArray[n-1].xleft+25;
			bombArray[i].yend=cityArray[n-1].yleft;
		}

		if ((n>=5) && (n<=7))
		{
			bombArray[i].xend=cityArray[n-2].xleft+25;
			bombArray[i].yend=cityArray[n-2].yleft;
		}
	}
}

int numberOfBombs(int currentLevel)
{
	int i, nBombs=10;

	for (i=1; i<currentLevel; i++)
	{
		nBombs+=2;
	}

	return nBombs;
}

//returns number of iterations through which the while loop must loop before the length of the bomb's line increases
int speed(int currentLevel)
{
	int speed;

	if (currentLevel==1)
	{
		speed=5;
	}

	else if (currentLevel==2)
	{
		speed=4;
	}

	else if (currentLevel==3)
	{
		speed=3;
	}

	else if (currentLevel==4)
	{
		speed=2;
	}

	else if (currentLevel==5)
	{
		speed=1;
	}

	return speed;
}

int lose(Bomb bombArray[30], Missile missileArray[30], City cityArray[6])
{
	int totalBombs, totalCities;

	totalBombs = checkNumberOfBombs(bombArray);

	totalCities = checkNumberOfCities(cityArray);

	if (totalCities == 0 && totalBombs == 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

//returns 1 if win and 0 otherwise
int win(Bomb bombArray[30], Missile missileArray[30], City cityArray[6])
{
	int nBombs, nCities; //number of bombs, number of cities

	nBombs=checkNumberOfBombs(bombArray);

	nCities=checkNumberOfCities(cityArray);

	if ((nBombs==0) && (nCities>0)) 
	{
		return 1;
	}

	else 
	{
		return 0;
	}
}

int checkNumberOfMissiles(Missile missileArray[30])
{
	int max = 30, i, totalAlive;

	for (i = 0; i < max; i++)
	{
		if (missileArray[i].status == alive)
		{
			totalAlive++;
		}
	}

	return totalAlive;
}

int checkNumberOfCities(City cityArray[6])
{
	int max = 6, i, totalAlive;

	for (i = 0; i < max; i++)
	{
		if (cityArray[i].status == alive)
		{
			totalAlive++;
		}
	}

	return totalAlive;
}
//returns number of alive bombs
int checkNumberOfBombs(Bomb bombArray[30])
{
	int i, nBombs; //counter, number of bombs

	for (i=0; i<30; i++)
	{
		if (bombArray[i].status==alive)
		{
			nBombs++;
		}
	}

	return nBombs;
}

void drawCities(City cityArray[6])
{
	int i, x, y;
	for (i = 0; i < 6; i++)
	{
		x = cityArray[i].xleft;
		y = cityArray[i].yleft;

		gfx_rectangle(x,y,50,50);
	}
}


void drawBases(Base baseArray[3])
{
	int i, x, y;
	for (i = 0; i < 6; i++)
	{
		x = baseArray[i].xleft;
		y = baseArray[i].yleft;

		gfx_rectangle(x,y,50,50);
	}
}


