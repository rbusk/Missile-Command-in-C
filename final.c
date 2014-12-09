#include <stdio.h>
#include <math.h>
#include <time.h>
#include "gfx3.h"

struct Bomb
{
	int xstart;
	int ystart;
	int deltax;
	int deltay;
	int end; //0 through 8
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
void randomizeBomb(Bomb bombArray[30]);
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
			bombArray[i].timeTilLaunch % 200; // generate a number 0 through 199
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

int numberOfBombs(int currentLevel)
{
	int i, nBombs=10;

	for (i=1; i<currentLevel; i++)
	{
		nBombs+=2;
	}

	return nBombs;
}

int speed(int currentLevel)
{
	int speed;

	speed=currentLevel*2;

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


