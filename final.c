#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include "gfx3.h"

struct Bomb
{
	int xstart;
	int ystart;
	double deltax;
	double deltay;
	int end; //0 through 8
	int xend;
	int yend;
	double x;
	double y;
	int status;
	int timeTilLaunch;
} typedef Bomb;

struct Missile
{
	int xstart;
	int ystart;
	double x;
	double y;
	int endx;
	int endy;
	double deltax;
	double deltay;
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
	double radius;
	double deltaR;
} typedef Explosion;

enum STATUS
{
	dead,
	alive
};

int numberOfBombs(int currentLevel);//done
int checkNumberOfBombs(Bomb bombArray[30]); //done
int checkNumberOfMissiles(Missile missileArray[30]); //done
int checkNumberOfCities(City cityArray[6]); //done
int win(Bomb bombArray[30],Missile missileArray[30], City cityArray[6]); //done
int lose(Bomb bombArray[30], Missile missileArray[30], City cityArray[6]); //done
int numberOfBombs(int currentLevel); //done
void initializeBomb(Bomb bombArray[30], City cityArray[6], Base baseArray[3],int currentLevel); //done
void randomizeBomb(Bomb bombArray[30]); //done
void bombDestination(City cityArray[6], Base baseArray[3], Bomb bombArray[30]); //done
void bombSpeed(Bomb bombArray[30], int currentLevel); //done
void initializeStructures(City cityArray[6], Base baseArray[3]); //done
void initializeMissiles(Missile missileArray[30], Base baseArray[3]);//done
void drawCities(City cityArray[6]); //done
void drawBases(Base baseArray[3], Missile missileArray[30]);//done
void drawMissiles(Missile missileArray[30]);//done
void drawBombs(Bomb bombArray[30]);//done
void checkIfBombInsideExplosion(Bomb bombArray[30],Explosion explosionArray[30]);
void initializeExplosion(Explosion explosionArray[30], Missile missileArray[30]);
void drawExplosion(Explosion explosionArray[30]);
void checkIfBombIsInCity(Bomb bombArray[30], City cityArray[6], int nBombs);
void checkIfBombIsInBase(Bomb bombArray[30], Base baseArray[3], int nBombs);

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
	int width = 800;
	int height = 700;

	gfx_open(width,height,"MISSILE COMMAND");
	
	for (currentLevel = 1; currentLevel <= maxLevel; currentLevel++)
	{
		
		initializeStructures(cityArray,baseArray); //done
		initializeMissiles(missileArray, baseArray);
		initializeBomb(bombArray,cityArray, baseArray, currentLevel);

		while (!win(bombArray, missileArray, cityArray) && !lose(bombArray, missileArray, cityArray))
		{
			drawBases(baseArray,missileArray);
			drawCities(cityArray);
			drawBombs(bombArray);
			drawMissiles(missileArray);

		}

		if(lose(bombArray, missileArray, cityArray))
		{
			break;
		}
		
	}


}

//checks if Bomb has hit a city
void checkIfBombIsInCity(Bomb bombArray[30], City cityArray[6], int nBombs) 
{
	int i, n;

	for (i=0; i<nBombs; i++)
	{
		n=bombArray[i].end;

		if (bombArray[i].status==alive)
		{
			if ((n>=1) && (n<=3))
			{
				if ((bombArray[i].x>bombArray[i].xend) && (bombArray[i].y>bombArray[i].yend))
				{
					//make explosion
					cityArray[n].status=dead;
					bombArray[i].status=dead;	
				}
			}

			if ((n>=5) && (n<=7))
			{	
				if ((bombArray[i].x>bombArray[i].xend) && (bombArray[i].y>bombArray[i].yend))
				{
					//make explosion
					cityArray[n].status=dead;
					bombArray[i].status=dead;
				}
			}
		}
	}
				
}

//initializes variables for bombs
void initializeBomb(Bomb bombArray[30], City cityArray[6], Base baseArray[3],int currentLevel)
{
	int nBombs, i; //number of bombs

	nBombs=numberOfBombs(currentLevel);

	randomizeBomb(bombArray);

	for (i=0; i<nBombs; i++)
	{
		bombArray[i].ystart=0; //sets ystart

		bombArray[i].status=dead; //sets status

		bombArray[i].x=bombArray[i].xstart;

		bombArray[i].y=bombArray[i].ystart;
	}

	bombDestination(cityArray, baseArray, bombArray);

	bombSpeed(bombArray, currentLevel);
}

void bombSpeed(Bomb bombArray[30], int currentLevel)
{
	int i;

	double x, y, theta;

	double speed=currentLevel*.2;

	for(i=0; i<30; i++)
	{
		x=bombArray[i].xend-bombArray[i].xstart;

		y=bombArray[i].yend-bombArray[i].ystart;

		theta=atan(y/x);

		bombArray[i].deltax=cos(theta)*speed;

		bombArray[i].deltay=cos(theta)*speed;
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

	for (i=0; i<=8;i++)
	{
		if (i==0)
		{
			baseArray[0].xleft=35+85*i;
			baseArray[0].yleft=y;
			baseArray[0].status = alive;
		}

		if (i==4)
		{
			baseArray[1].xleft=35+85*i;
			baseArray[1].yleft=y;
			baseArray[1].status = alive;
		}

		if (i==8)
		{
			baseArray[2].xleft=35+85*i;
			baseArray[2].yleft=y;
			baseArray[2].status = alive;
		}

		if ((i>=1) && (i<=3))
		{
			cityArray[i-1].xleft=35+85*i;
			cityArray[i-1].yleft=y;
			cityArray[i-1].status = alive;
		}

		if ((i>=5) && (i<=7))
		{
			cityArray[i-2].xleft=35+85*i;
			cityArray[i-2].yleft=y;
			cityArray[i-2].status = alive;
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

		if (cityArray[i].status = alive)
		{
			gfx_rectangle(x,y,50,50);
		}
	}
}


void drawBases(Base baseArray[3], Missile missileArray[30])
{
	int i, j, x, y, totalMissile = 0;
	char missileTotal[3];
	for (i = 0; i < 3; i++)
	{
		totalMissile = 0;
		x = baseArray[i].xleft;
		y = baseArray[i].yleft;

		for (j = 0; j < 30; j++)
		{
			if (missileArray[j].baseNumber == i && missileArray[j].status == alive)
			{
				totalMissile++;
			}
		}

		
		gfx_rectangle(x,y,50,50);
		sprintf(missileTotal,"%d",totalMissile);
		gfx_text(x + 20, y + 20, missileTotal);


	}
}

void initializeMissiles(Missile missileArray[30], Base baseArray[3])
{
	int i;

	for (i = 0; i < 30; i++)
	{
		if (i < 10)
		{
			missileArray[i].xstart = baseArray[0].xleft + 25;
			missileArray[i].ystart = baseArray[0].yleft + 25;
			missileArray[i].status = alive;
			missileArray[i].baseNumber = 0;
			missileArray[i].endx = 0;
			missileArray[i].endy = 0;
			missileArray[i].x = 0;
			missileArray[i].y = 0;
		}
		if (i >= 10 && i < 20)
		{
			missileArray[i].xstart = baseArray[1].xleft + 25;
			missileArray[i].ystart = baseArray[1].yleft + 25;
			missileArray[i].status = alive;
			missileArray[i].baseNumber = 1;
			missileArray[i].endx = 0;
			missileArray[i].endy = 0;
			missileArray[i].x = 0;
			missileArray[i].y = 0;
		}
		if (i >=20 && i < 30)
		{
			missileArray[i].xstart = baseArray[2].xleft + 25;
			missileArray[i].ystart = baseArray[2].yleft + 25;
			missileArray[i].status = alive;
			missileArray[i].baseNumber = 2;
			missileArray[i].endx = 0;
			missileArray[i].endy = 0;
			missileArray[i].x = 0;
			missileArray[i].y = 0;
		}
	}
}

void drawMissiles(Missile missileArray[30])
{
	int i;

	for (i = 0; i < 30; i++)
	{
		if (missileArray[i].status == alive && missileArray[i].x != 0 && missileArray[i].y != 0)
		{
			gfx_line(missileArray[i].xstart,missileArray[i].ystart,missileArray[i].x,missileArray[i].y);

		}
	}
}

void drawBombs(Bomb bombArray[30])
{
	int i;

	for (i = 0; i < 30; i++)
	{
		if (bombArray[i].status == alive && bombArray[i].x != 0 && bombArray[i].y != 0)
		{
			gfx_line(bombArray[i].xstart,bombArray[i].ystart,bombArray[i].x,bombArray[i].y);

		}
	}

}


