#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include "gfx3.h"

struct Bomb
{
	double xstart;
	double ystart;
	double deltax;
	double deltay;
	double end; //0 through 8
	double xend;
	double yend;
	double x;
	double y;
	int status;
	int timeTilLaunch;
} typedef Bomb;

struct Missile
{
	double xstart;
	double ystart;
	double x;
	double y;
	double xend;
	double yend;
	double deltax;
	double deltay;
	int status;
	int baseNumber;
} typedef Missile;

struct City
{
	double xleft;
	double yleft;
	int width;
	int height;
	int status;
} typedef City;

struct Base
{
	double xleft;
	double yleft;
	int width;
	int height;
	int missileNumber;
	int status;
} typedef Base;

struct Explosion
{
	double x;
	double y;
	double radius;
	int status;
} typedef Explosion;

enum STATUS
{
	dead,
	alive,
	unused
};

enum SRUCTURE
{
	base,
	city
};

int numberOfBombs(int currentLevel);//done
int checkNumberOfBombs(Bomb bombArray[30], int bombs); //done
int checkNumberOfMissiles(Missile missileArray[30]); //done
int checkNumberOfCities(City cityArray[6]); //done
int win(Bomb bombArray[30],Missile missileArray[30], City cityArray[6], int nBombs); //done
int lose(Bomb bombArray[30], Missile missileArray[30], City cityArray[6], int nBombs); //done
void initializeBomb(Bomb bombArray[30], City cityArray[6], Base baseArray[3],int currentLevel); //done
void randomizeBomb(Bomb bombArray[30], City cityArray[6], Base baseArray[3], int nBombs); //done
void bombDestination(City cityArray[6], Base baseArray[3], Bomb bombArray[30]); //done
void bombSpeed(Bomb bombArray[30], int currentLevel); //done
void initializeStructures(City cityArray[6], Base baseArray[3]); //done
void initializeMissiles(Missile missileArray[30], Base baseArray[3]);//done
void drawCities(City cityArray[6], int currentLevel); //done
void drawBases(Base baseArray[3], Missile missileArray[30], int currentLevel);//done
void drawMissiles(Missile missileArray[30]);//done
void drawBombs(Bomb bombArray[30], int nBombs);//done
int checkIfBombInsideExplosion(Bomb bombArray[30],Explosion explosionArray[30], int nBombs, int score); //done
void startExplosion(Explosion explosionArray[30], Missile missileArray[30]); //done
void initializeExplosion(Explosion explosionArray[29]); //done
void drawExplosion(Explosion explosionArray[30]); //done
void incrementExplosionRadius(Explosion explosionArray[30]); //done
void incrementBomb(Bomb bombArray[30], int nBombs); //done
void incrementMissile(Missile missileArray[30]); //done
void deployBomb(Bomb bombArray[30], int nBombs);
void removeMissiles(int n, Missile missileArray[30]); //done
void setOffMissile(Missile missileArray[30], char c, double x, double y); //done
void missilePath(Missile *missile); //done
void startMissileExplosion(double x, double y, Explosion explosionMissileArray[30]);
void startExplosion2(Explosion explosionArray2[30], Bomb bombArray[30], City cityArray[6], Base baseArray[3], Missile missileArray[30], int nBombs);
void drawLevel(int currentLevel, int height, int width);
void drawScore(int score, int height, int width);
int calculateScore(int score, Missile missileArray[30], City cityArray[6]);
void aliveCities(City cityArray[6]);
void findEndForBomb(Bomb bombArray[30], City cityArray[6], Base baseArray[3], int nBombs);
void changeFontSize(int fontSize);
void startScreen(int width, int height);
void setColor(int currentLevel, int);

int main()
{
	time_t t;

	srand(time(&t));

	Base baseArray[3];
	City cityArray[6];
	Missile missileArray[30];
	Bomb bombArray[30];
	Explosion explosionArray[30];
	Explosion explosionMissileArray[30];
	Explosion explosionArray2[30];

	char c, c2; //use to save user's input
	
	int currentLevel,maxLevel = 5;
	int width = 800;
	int height = 700;
	int j;
	int score = 0;

	double x, y; //coordinates of mouse when an event occurs

	int nExplosions=0, nBombs;

	gfx_open(width,height,"MISSILE COMMAND");

	aliveCities(cityArray);

	startScreen(width, height);
	
	for (currentLevel = 1; currentLevel <= maxLevel; currentLevel++)
	{
		
		initializeStructures(cityArray,baseArray); //done
		initializeMissiles(missileArray, baseArray);
		initializeBomb(bombArray,cityArray, baseArray, currentLevel);
		initializeExplosion(explosionArray);
		initializeExplosion(explosionArray2);
		int i;
		nBombs= numberOfBombs(currentLevel);
	
		while (!win(bombArray, missileArray, cityArray, nBombs) && !lose(bombArray, missileArray, cityArray, nBombs))
		{
			gfx_clear();

			drawBases(baseArray,missileArray, currentLevel);
			drawCities(cityArray, currentLevel);
			drawBombs(bombArray, nBombs);
			drawMissiles(missileArray);
			drawExplosion(explosionArray);
			drawExplosion(explosionArray2);
			drawLevel(currentLevel,height, width);
			drawScore(score, height, width);
			gfx_flush();


			usleep(10000);

			if (gfx_event_waiting())
			{
				c=gfx_wait();

				if (gfx_xpos() > 5 && gfx_xpos() < width - 5)
				{
					x = gfx_xpos();
				}
				else if (gfx_xpos() < 5)
				{
					x = 5;
				}
				else
				{
					x = width - 5;
				}


				if (gfx_ypos() > 30 && gfx_ypos() < (height - 70))
				{
					y=gfx_ypos();
				}
				else if (gfx_ypos() < 30)
				{
					y = 30;
				}
				else
				{
					y = height - 70;
				}
			}

			if ((c=='a') || (c=='s') || (c=='d') || (c=='w'))
			{
				setOffMissile(missileArray, c, x, y);
				c=' ';
			}

			deployBomb(bombArray, nBombs);
			incrementBomb(bombArray, nBombs);
			incrementExplosionRadius(explosionArray);
			startExplosion2(explosionArray2, bombArray, cityArray, baseArray, missileArray, nBombs);
			score = checkIfBombInsideExplosion(bombArray, explosionArray, nBombs, score);
			incrementMissile(missileArray);
			startExplosion(explosionArray, missileArray);
			incrementExplosionRadius(explosionArray);
			incrementExplosionRadius(explosionArray2);	
			int i;
		}

		score = calculateScore(score, missileArray, cityArray);
		drawScore(score, height, width);

		if (lose(bombArray, missileArray, cityArray, nBombs))
		{
			gfx_clear();
			changeFontSize(100);
			gfx_text(170, 300, "YOU LOSE!");
			drawLevel(currentLevel,height, width);
			drawScore(score, height, width);

			if (gfx_wait())
			{
				break;
			}
		}

		if (win(bombArray, missileArray, cityArray, nBombs) && currentLevel == 5)
		{
			gfx_clear();
			changeFontSize(100);
			gfx_text(170, 300, "YOU WIN!");
			drawLevel(currentLevel,height, width);
			drawScore(score, height, width);

			if (gfx_wait())
			{
				break;
			}
		}

		gfx_clear();
		gfx_text(400, 240, "Next Level?");
		gfx_text(400, 270, "Press any key to Continue");
		gfx_text(400, 300, "Press q to quit");
		drawLevel(currentLevel,height, width);
		drawScore(score, height, width);

		c2 = gfx_wait();

		if (c2 == 'q')
		{
			break;
		}

	}


}
//sets color of bases and cities
void setColor(int currentLevel, int structure)
{
	if (currentLevel==1)
	{
		if (structure==city)
		{
			gfx_color(0, 253, 194);
		}

		if (structure==base)
		{
			gfx_color(0, 9, 255);
		}

	}

	if (currentLevel==2)
	{
		if (structure==city)
		{
			gfx_color(255, 0, 0);
		}

		if (structure==base)
		{
			gfx_color(0, 196, 255); 
		}

	}

	if (currentLevel==3)
	{
		if (structure==city)
		{
			gfx_color(0, 179, 255);
		}

		if (structure==base)
		{
			gfx_color(255, 162, 0);
		}
	}

	if (currentLevel==4)
	{
		if (structure==city)
		{
			gfx_color(1, 255, 212);
		}

		if (structure==base)
		{
			gfx_color(128, 1, 255);
		}

	}

	if (currentLevel==5)
	{
		if (structure==city)
		{
			gfx_color(212, 4, 177);
		}

		if (structure==base)
		{
			gfx_color(8, 253, 194);
		}
	}
}

void startExplosion2(Explosion explosionArray2[30], Bomb bombArray[30], City cityArray[6], Base baseArray[3], Missile missileArray[30], int nBombs)
{
	int i, j, n;

	for (i=0; i<nBombs; i++)
	{
		j=0;

		n=bombArray[i].end;


		if ((bombArray[i].status==alive) && (bombArray[i].y>bombArray[i].yend))
		{
			while (explosionArray2[j].status!=unused)
			{
				j++;
			}

			bombArray[i].status=dead;
			explosionArray2[j].status=alive;

			explosionArray2[j].radius=0;

			if ((n>=1) && (n<=3))
			{
				cityArray[n-1].status=dead;
				explosionArray2[j].x=cityArray[n-1].xleft+25;
				explosionArray2[j].y=cityArray[n-1].yleft;
			}

			if ((n>=5) && (n<=7))
			{
				cityArray[n-2].status=dead;
				explosionArray2[j].x=cityArray[n-2].xleft+25;
				explosionArray2[j].y=cityArray[n-2].yleft;
			}

			if ((n==0) || (n==4) || (n==8))
			{
				baseArray[n/4].status=dead;
				removeMissiles(n/4, missileArray);
				explosionArray2[j].x=baseArray[n/4].xleft+25;
				explosionArray2[j].y=baseArray[n/4].yleft;
			}
		}
	}	
}

//calculates deltax and deltay for missile that has been set off
void missilePath(Missile *missile)
{
	double speed=5;

	double x, y, theta;

	x=missile->xstart-missile->xend;

	y=missile->ystart-missile->yend;

	theta=atan(y/abs(x));

	missile->deltax=cos(theta)*speed;

	missile->deltay=sin(theta)*speed;
}


//sets off a missile cooresponding to user's input
void setOffMissile(Missile missileArray[30], char c, double x, double y)
{
	int i;

	if (c=='a')
	{
		i=0;

		while ((missileArray[i].status!=unused) && (i<=9))
		{
			i++;
		}

		if (i>=10)
		{
			return;
		}

		missileArray[i].status=alive;

		missileArray[i].xend=x;

		missileArray[i].yend=y;

		missilePath(&missileArray[i]); //calculate deltax and deltay
	}

	else if ((c=='w') || (c=='s'))
	{
		i=10;

		while((missileArray[i].status!=unused) && (i<=19))
		{
			i++;
		}

		if (i>=20)
		{
			return;
		}

		missileArray[i].status=alive;

		missileArray[i].xend=x;

		missileArray[i].yend=y;

		missilePath(&missileArray[i]); //calculate deltax and deltay
		
	}

	else if (c=='d')
	{
		i=20;

		while((missileArray[i].status!=unused) && (i<=29))
		{
			i++;
		}

		if (i>=30)
		{
			return;
		}

		missileArray[i].status=alive;

		missileArray[i].xend=x;

		missileArray[i].yend=y;

		missilePath(&missileArray[i]); //calculate deltax and deltay
	}
}

//sets the status of all explosions to unused
void initializeExplosion(Explosion explosionArray[30])
{
	int i;

	for(i=0; i<30; i++)
	{
		explosionArray[i].status=unused;
	}
}

//starts Explosion if missile has reached its destination
void startExplosion(Explosion explosionArray[30], Missile missileArray[30])
{
	int i, j;

	for (i=0; i<30; i++)
	{
		j=0;

		if ((missileArray[i].status==alive) && (missileArray[i].yend>=missileArray[i].y))
		{
			while (explosionArray[j].status!=unused)
			{
				j++;
			}

			missileArray[i].status=dead;

			explosionArray[j].status=alive;

			explosionArray[j].x=missileArray[i].xend;

			explosionArray[j].y=missileArray[i].yend;

			explosionArray[j].radius=0;

		}
	}

	

}

//increments radius of alive explosions
void incrementExplosionRadius(Explosion explosionArray[30])
{
	double initialr=10, deltar=.5, maxr=35;
	int i;

	for (i=0; i<30; i++)
	{
		if (explosionArray[i].status == alive)
		{
			if (explosionArray[i].radius==0) // if explosion has just been set to alive, start with initial radius
			{
				explosionArray[i].radius=initialr;
			}

			else if (explosionArray[i].radius>=maxr) //if explosion has reached max radius, set status to dead
			{
				explosionArray[i].status=dead;
			}

			else //else increment the radius by deltar
			{
				explosionArray[i].radius+=deltar;
			}
		}
	}
}
//draws explosion if explosion is alive
void drawExplosion(Explosion explosionArray[30])
{
	int i, j;

	for (i=0; i<30; i++)
	{
		if (explosionArray[i].status==alive)
		{
			for (j=0; j<=explosionArray[i].radius; j++)
			{
				gfx_color(255, j*10, 1);

				gfx_circle(explosionArray[i].x, explosionArray[i].y, j);
			}
		}
	}
}

//checks if Bomb is inside an explosion and, if so, changes its status to dead
int checkIfBombInsideExplosion(Bomb bombArray[30],Explosion explosionArray[30], int nBombs, int score) 
{
	int i, j;
	
	double xbomb, ybomb, xexp, yexp, x, y;

	for(i=0; i<nBombs; i++)
	{
		xbomb=bombArray[i].x;
		ybomb=bombArray[i].y;

		for (j=0; j<30; j++)
		{
			xexp=explosionArray[j].x;
			yexp=explosionArray[j].y;

			if((bombArray[i].status==alive) && (explosionArray[j].status==alive))
			{
				x=xbomb-xexp;
				y=ybomb-yexp;

				if ((explosionArray[j].radius)>(sqrt(pow(x, 2) + pow(y, 2))))
				{
					bombArray[i].status=dead;
					score += 25;
				}

			}
		}
	}

	return score;
}

//removes missiles from a base that has been destroyed
void removeMissiles(int n, Missile missileArray[30]) 
{
	//n is the index of the base

	int i;
	
	if (n==0)
	{
		for (i=0; i<10; i++)
		{
			missileArray[i].status=dead;
		}
	}

	if (n==1)
	{
		for (i=10; i<20; i++)
		{
			missileArray[i].status=dead;
		}
	}

	if (n==2)
	{
		for (i=20; i<30; i++)
		{
			missileArray[i].status=dead;
		}
	}
}

//checks if Bomb has hit a city and change statuses of bomb and city if necessary
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
				if ((bombArray[i].y>bombArray[i].yend))
				{
					//make explosion
					cityArray[n-1].status=dead;
					bombArray[i].status=dead;	
				}
			}

			if ((n>=5) && (n<=7))
			{	
				if ((bombArray[i].y>bombArray[i].yend))
				{
					//make explosion
					cityArray[n-2].status=dead;
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

	randomizeBomb(bombArray, cityArray, baseArray, nBombs);

	for (i=0; i<nBombs; i++)
	{
		bombArray[i].ystart=30; //sets ystart

		bombArray[i].status=unused; //sets status

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

	double speed=.5+currentLevel*.25;

	for(i=0; i<30; i++)
	{
		x=bombArray[i].xend-bombArray[i].xstart;

		y=bombArray[i].yend-bombArray[i].ystart;

		theta=atan(y/abs(x));

		bombArray[i].deltax=cos(theta)*speed;

		bombArray[i].deltay=sin(theta)*speed;
	}
}

//randomizes the time each bomb is detonated and the destination of each bomb
void randomizeBomb(Bomb bombArray[30], City cityArray[6], Base baseArray[3], int nBombs)
{
	int i;

	for (i=0; i<nBombs; i++)
	{
		findEndForBomb(bombArray, cityArray, baseArray, nBombs);
		bombArray[i].timeTilLaunch= rand() % 700; // generate a number 0 through 199
		bombArray[i].xstart = rand() % 700 +1; // generate a number 1 to 700
	}

	bombArray[0].timeTilLaunch=0;
}

void findEndForBomb(Bomb bombArray[30], City cityArray[6], Base baseArray[3], int nBombs)
{
	int numberArray[9], numberOfAlive = 0, i, destination;

	for (i = 0; i < 6; i++)
	{
		if (cityArray[i].status == alive)
		{
			if (i >= 0 && i < 3)
			{
				numberArray[numberOfAlive] = i + 1;
				numberOfAlive++;
			}
			if (i > 2 && i <= 5)
			{
				numberArray[numberOfAlive] = i + 2;
				numberOfAlive++;
			}
		}
	}

	for (i = 0; i < 3; i++)
	{
		if (baseArray[i].status == alive)
		{
			numberArray[numberOfAlive] = i * 4;
			numberOfAlive++;
		}
	}

	for (i = 0; i < nBombs; i++)
	{
		destination = rand() % numberOfAlive;

		bombArray[i].end = numberArray[destination];
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
		}

		if ((i>=5) && (i<=7))
		{
			cityArray[i-2].xleft=35+85*i;
			cityArray[i-2].yleft=y;
		}


	}
}

//given the number n from 0-8, calculates the cooresponding coordinate 
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

int lose(Bomb bombArray[30], Missile missileArray[30], City cityArray[6], int bombs)
{
	int totalBombs, totalCities;

	totalBombs = checkNumberOfBombs(bombArray, bombs);

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
int win(Bomb bombArray[30], Missile missileArray[30], City cityArray[6], int bombs)
{
	int nBombs, nCities; //number of bombs, number of cities

	nBombs=checkNumberOfBombs(bombArray, bombs);

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
int checkNumberOfBombs(Bomb bombArray[30], int bombs)
{
	int i, nBombs = 0; //counter, number of bombs

	for (i=0; i< bombs; i++)
	{
		if (bombArray[i].status != dead)
		{
			nBombs++;
		}
	}

	return nBombs;
}

void drawCities(City cityArray[6], int currentLevel)
{
	setColor(currentLevel, city);

	int i, x, y;
	for (i = 0; i < 6; i++)
	{
		x = cityArray[i].xleft;
		y = cityArray[i].yleft;

		if (cityArray[i].status == alive)
		{
			gfx_fill_rectangle(x,y,50,50);
		}
	}
}


void drawBases(Base baseArray[3], Missile missileArray[30], int currentLevel)
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
			if ((missileArray[j].baseNumber == i) && (missileArray[j].status == unused))
			{
				totalMissile++;
			}
		}

		setColor(currentLevel, base);
		gfx_fill_rectangle(x,y,50,50);
		gfx_color(255, 255, 255);
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
			missileArray[i].ystart = baseArray[0].yleft;
			missileArray[i].status = unused;
			missileArray[i].baseNumber = 0;
			missileArray[i].xend = 0;
			missileArray[i].yend = 0;
			missileArray[i].x = missileArray[i].xstart;
			missileArray[i].y = missileArray[i].ystart;
		}
		if (i >= 10 && i < 20)
		{
			missileArray[i].xstart = baseArray[1].xleft + 25;
			missileArray[i].ystart = baseArray[1].yleft;
			missileArray[i].status = unused;
			missileArray[i].baseNumber = 1;
			missileArray[i].xend = 0;
			missileArray[i].yend = 0;
			missileArray[i].x = missileArray[i].xstart;
			missileArray[i].y = missileArray[i].ystart;
		}
		if (i >=20 && i < 30)
		{
			missileArray[i].xstart = baseArray[2].xleft + 25;
			missileArray[i].ystart = baseArray[2].yleft;
			missileArray[i].status = unused;
			missileArray[i].baseNumber = 2;
			missileArray[i].xend = 0;
			missileArray[i].yend = 0;
			missileArray[i].x = missileArray[i].xstart;
			missileArray[i].y = missileArray[i].ystart;
		}
	}
}

void drawMissiles(Missile missileArray[30])
{
	int i;

	for (i = 0; i < 30; i++)
	{
		if (missileArray[i].status == alive)
		{
			gfx_line(missileArray[i].xstart,missileArray[i].ystart,missileArray[i].x,missileArray[i].y);

		}
	}
}

void drawBombs(Bomb bombArray[30], int nBombs)
{
	int i;
	
	gfx_color(255, 255, 255);

	for (i = 0; i < nBombs; i++)
	{
		if (bombArray[i].status == alive)
		{
			gfx_line(bombArray[i].xstart,bombArray[i].ystart,bombArray[i].x,bombArray[i].y);

		}
	}

}

void incrementBomb(Bomb bombArray[30], int nBombs)
{
	int i;

	for (i = 0; i < nBombs; i++)
	{
		if (bombArray[i].status == alive)
		{
			bombArray[i].y += bombArray[i].deltay;

			if (bombArray[i].xstart >= bombArray[i].xend)
			{
				bombArray[i].x -= bombArray[i].deltax;
			}
			if (bombArray[i].xstart < bombArray[i].xend)
			{
				bombArray[i].x += bombArray[i].deltax;
			}	
		}
	}
}

void incrementMissile(Missile missileArray[30])
{
	int i;

	for (i = 0; i < 30; i++)
	{
		if (missileArray[i].status == alive)
		{
			missileArray[i].y -= missileArray[i].deltay;

			if (missileArray[i].xstart >= missileArray[i].xend)
			{
				missileArray[i].x -= missileArray[i].deltax;
			}
			if (missileArray[i].xstart < missileArray[i].xend)
			{
				missileArray[i].x += missileArray[i].deltax;
			}	
		}
	}


}

void deployBomb(Bomb bombArray[30], int nBombs)
{
	int i;

	for (i = 0; i < nBombs; i++)
	{

		if (bombArray[i].timeTilLaunch == 0 && bombArray[i].status != alive)
		{
			bombArray[i].status = alive;
		}

		bombArray[i].timeTilLaunch -= 1;
	}
}

void drawLevel(int currentLevel, int height, int width)
{
	char level[2];

	sprintf(level,"%d",currentLevel);

	changeFontSize(20);

	gfx_text(275, 20, level);

	gfx_text(200, 20, "LEVEL");
}


void drawScore(int score, int height, int width)
{
	char currentScore[10000];

	sprintf(currentScore,"%d",score);

	changeFontSize(20);

	gfx_text(600, 20, currentScore);

	gfx_text(500, 20, "SCORE");
}

int calculateScore(int score, Missile missileArray[30], City cityArray[30])
{
	int i;

	for (i = 0; i < 6; i++)
	{
		if (cityArray[i].status == alive)
		{
			score += 100;
		}
	}
	
	for (i = 0; i < 30; i++)
	{
		if (missileArray[i].status == alive)
		{
			score += 5;
		}
	}

	return score;

}
		
void aliveCities(City cityArray[6])
{
	int i;

	for (i = 0; i < 6; i++)
	{
		cityArray[i].status = alive;
	}
}

void changeFontSize(int fontSize)
{
	    /*
	     *      * Displays text with given font size
	     *          */
	    char fontStr[50];
	        sprintf(fontStr, "-*-helvetica-medium-r-normal-*-%i-120-*-*-*-*-iso8859-1", fontSize);
		    change_font(fontStr);
}

void startScreen(int width, int height)
{
	char c;

	while(1)
	{
		changeFontSize(150);
		gfx_color(222,7,7);
		gfx_text(100,200,"MISSILE");
		gfx_text(15,350,"COMMAND");
		gfx_color(238,222,4);
		changeFontSize(30);
		gfx_text(170,450,"By: Mary Connolly and Ryan Busk");

		gfx_flush();
		if (gfx_event_waiting())
		{
			c=gfx_wait();
			break;
		}
	}
}
