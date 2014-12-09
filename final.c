#include <stdio.h>
#include <math.h>
#include "gfx3.h"

struct Bomb
{
	int xstart;
	int ystart;
	int deltax;
	int deltay;
	int end;
	int status;
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
void initializeBomb(Bomb bombArray[30], City cityArray[6], Base baseArray[3], int currentLevel);
void initializeStructures(City cityArray[6], Base baseArray[3]);
void initializeMissiles(Missile missileArray[30], Base baseArray[3]);
void drawCities(City cityArray[6]);
void drawBases(Base baseArray[3]);
void drawMissiles(Missile missileArray[30]);
void drawBombs(Bomb bombArray[30]);
int checkIfBombInsideExplosion(Bomb bombArray[30],Explosion explosionArray[30]);


int main()
{
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

	nBombs=checkNumberOfBombs(bombArray[30]);

	nCities=checkNumberOfCities(cityArray[6]);

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




