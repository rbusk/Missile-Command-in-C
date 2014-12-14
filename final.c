//By Ryan Busk and Mary Connolly

#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include "gfx3.h"

struct Bomb
{
	double xstart; // 0 through 800
	double ystart; // 0
	double deltax; 
	double deltay;
	double end; //0 through 8
	double xend;
	double yend;
	double x;
	double y;
	int status; // dead, alive, unused
	int timeTilLaunch; // countdown until bomb drops
} typedef Bomb;

struct Missile
{
	double xstart; // one of the bases
	double ystart; 
	double x;
	double y;
	double xend;
	double yend;
	double deltax;
	double deltay;
	int status; // dead, alive, unused
	int baseNumber; // 0 to 2
} typedef Missile;

struct City
{
	double xleft;
	double yleft;
	int width;
	int height;
	int status; // dead, alive
} typedef City;

struct Base
{
	double xleft;
	double yleft;
	int width;
	int height;
	int missileNumber; // number of missiles 0 - 10
	int status; // dead, alive
} typedef Base;

struct Explosion
{
	double x;
	double y;
	double radius;
	int status; // dead or alive
} typedef Explosion;

enum STATUS
{
	dead,
	alive,
	unused
};

enum STRUCTURE
{
	base,
	city
};

int numberOfBombs(int currentLevel);
int checkNumberOfBombs(Bomb bombArray[30], int bombs);
int checkNumberOfMissiles(Missile missileArray[30]);
int checkNumberOfCities(City cityArray[6]);
int win(Bomb bombArray[30],Missile missileArray[30], City cityArray[6], int nBombs);
int lose(Bomb bombArray[30], Missile missileArray[30], City cityArray[6], int nBombs);
void initializeBomb(Bomb bombArray[30], City cityArray[6], Base baseArray[3],int currentLevel);
void randomizeBomb(Bomb bombArray[30], City cityArray[6], Base baseArray[3], int nBombs);
void bombDestination(City cityArray[6], Base baseArray[3], Bomb bombArray[30]);
void bombSpeed(Bomb bombArray[30], int currentLevel);
void initializeStructures(City cityArray[6], Base baseArray[3]);
void initializeMissiles(Missile missileArray[30], Base baseArray[3]);
void drawCities(City cityArray[6], int currentLevel);
void drawBases(Base baseArray[3], Missile missileArray[30], int currentLevel);
void drawMissiles(Missile missileArray[30]);
void drawBombs(Bomb bombArray[30], int nBombs);
int checkIfBombInsideExplosion(Bomb bombArray[30],Explosion explosionArray[30], int nBombs, int score);
void startExplosion(Explosion explosionArray[30], Missile missileArray[30]);
void initializeExplosion(Explosion explosionArray[29]);
void drawExplosion(Explosion explosionArray[30]);
void incrementExplosionRadius(Explosion explosionArray[30]);
void incrementBomb(Bomb bombArray[30], int nBombs);
void incrementMissile(Missile missileArray[30]);
void deployBomb(Bomb bombArray[30], int nBombs);
void removeMissiles(int n, Missile missileArray[30]);
void setOffMissile(Missile missileArray[30], char c, double x, double y);
void missilePath(Missile *missile);
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
void instructionScreen(void);

int main()
{
	time_t t;

	srand(time(&t)); // seed rand in time

	Base baseArray[3]; // allocate bases
	City cityArray[6]; // allocate structures
	Missile missileArray[30]; // allocate missiles
	Bomb bombArray[30]; // allocate bombs
	Explosion explosionArray[30]; // allocate explosions
	Explosion explosionArray2[30];

	char c, c2; //use to save user's input
	
	int currentLevel,maxLevel = 5; // total numbe rof levels
	int width = 800;
	int height = 700;
	int j;
	int score = 0; // score of game
	int i;

	double x, y; //coordinates of mouse when an event occurs

	int nExplosions=0, nBombs;

	gfx_open(width,height,"MISSILE COMMAND"); // open window

	aliveCities(cityArray); // set cities to alive

	startScreen(width, height); // run the start screen

	instructionScreen(); // run the instructions screen
	
	for (currentLevel = 1; currentLevel <= maxLevel; currentLevel++) // run through each level
	{
		
		initializeStructures(cityArray,baseArray); // initialize the bases and missiles
		initializeMissiles(missileArray, baseArray); // initialize missiles
		initializeBomb(bombArray,cityArray, baseArray, currentLevel); // initialize bombs
		initializeExplosion(explosionArray); // initiliaze explosions
		initializeExplosion(explosionArray2);
		nBombs= numberOfBombs(currentLevel); // find number of bombs
	
		while (!win(bombArray, missileArray, cityArray, nBombs) && !lose(bombArray, missileArray, cityArray, nBombs)) // check if usr won level or lost game
		{
			gfx_clear(); // clear screen

			drawBases(baseArray,missileArray, currentLevel); // draw alive bases
			drawCities(cityArray, currentLevel); // draw alive cities
			drawBombs(bombArray, nBombs); // draw deployed bombs
			drawMissiles(missileArray); // draw shot missiles
			drawExplosion(explosionArray); // draw explosions
			drawExplosion(explosionArray2);
			drawLevel(currentLevel,height, width); // draw level on top of screen
			drawScore(score, height, width); // draw score on top of screen
			gfx_flush();


			usleep(10000);

			if (gfx_event_waiting()) // check if user is pushing button
			{
				c=gfx_wait(); 

				// check if mouse is inside score screen

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

			if ((c=='a') || (c=='s') || (c=='d') || (c=='w')) // depending on button pressed, shoot missile from different base
			{
				setOffMissile(missileArray, c, x, y);
				c=' ';
			}

			deployBomb(bombArray, nBombs); // drop bombs whose counter is zero and are set to alive
			incrementBomb(bombArray, nBombs); // decrease bomb counter by one, or move bomb by deltax and deltay
			incrementExplosionRadius(explosionArray); // make any current explosion bigger
			startExplosion2(explosionArray2, bombArray, cityArray, baseArray, missileArray, nBombs);
			score = checkIfBombInsideExplosion(bombArray, explosionArray, nBombs, score); // check if any bombs have been blown up by missiles
			incrementMissile(missileArray); // move missile by deltax and deltay
			startExplosion(explosionArray, missileArray); // if missile hits target, start explosion
			incrementExplosionRadius(explosionArray2); 
		}

		score = calculateScore(score, missileArray, cityArray); // calculate score at the end of each level
		drawScore(score, height, width);

		if (lose(bombArray, missileArray, cityArray, nBombs)) // at the end of the level check if player has lost
		{
			// display text and score if player has lost
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

		if (win(bombArray, missileArray, cityArray, nBombs) && currentLevel == 5) // at the end of five levels, check if player has won the game
		{
			// display text and score if player wins the game
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

		// at the end of the level, display text and score

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

// function that runs explsion if a bomb hits a missile or base
void startExplosion2(Explosion explosionArray2[30], Bomb bombArray[30], City cityArray[6], Base baseArray[3], Missile missileArray[30], int nBombs)
{
	int i, j, n;

	for (i=0; i<nBombs; i++) // check every bomb in level
	{
		j=0;

		n=bombArray[i].end; // checks end of bomb


		if ((bombArray[i].status==alive) && (bombArray[i].y>bombArray[i].yend)) // checks if bomb is in base
		{
			while (explosionArray2[j].status!=unused) // finds unused explosion in explosion2array
			{
				j++;
			}

			bombArray[i].status=dead; // sets bomb to dead
			explosionArray2[j].status=alive; // sets explosion array to alive

			explosionArray2[j].radius=0; // initialize radius

			// depending on hit, does different explosion
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

// calculates numbe ro fbombs used for each level
int numberOfBombs(int currentLevel)
{
	int i, nBombs=10;

	for (i=1; i<currentLevel; i++)
	{
		nBombs+=2; // each level increases numbe ro fbombs by two
	}

	return nBombs;
}

// function that checks if player has lost game
int lose(Bomb bombArray[30], Missile missileArray[30], City cityArray[6], int bombs)
{
	int totalBombs, totalCities;

	totalBombs = checkNumberOfBombs(bombArray, bombs);

	totalCities = checkNumberOfCities(cityArray);

	if (totalCities == 0 && totalBombs == 0)
	{
		return 1; // if all the bombs have run out and all the cities are dead, the player has lost
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
		return 1; // if cities are greater than zero and there are no bombs left, player wins level
	}

	else 
	{
		return 0;
	}
}

// check numbe ro f missiles
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

// checks number of alive cities at any given time
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

// draws alive cities using location stored in struct
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

// draws alive bases using position stored in struct
void drawBases(Base baseArray[3], Missile missileArray[30], int currentLevel)
{
	int i, j, x, y, totalMissile = 0;
	char missileTotal[3];
	for (i = 0; i < 3; i++)
	{
		totalMissile = 0;
		x = baseArray[i].xleft;
		y = baseArray[i].yleft;

		for (j = 0; j < 30; j++) // check how many missiles are in base
		{
			if ((missileArray[j].baseNumber == i) && (missileArray[j].status == unused))
			{
				totalMissile++;
			}
		}

		setColor(currentLevel, base);
		gfx_fill_rectangle(x,y,50,50);
		// display missile number
		gfx_color(255, 255, 255);
		sprintf(missileTotal,"%d",totalMissile);
		gfx_text(x + 20, y + 20, missileTotal);


	}
}

// initialize missiles into the bases
void initializeMissiles(Missile missileArray[30], Base baseArray[3])
{
	int i;

	for (i = 0; i < 30; i++)
	{
		// put ten missiles in each base
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

// draw missiles from starting base to current point
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

// draw bomb from starting point to current point
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

// move bomb by x and y
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

// move missile by deltax and deltay
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

// deploy bomb if counter is zero or decrement counter by 1
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

// display level on top of screen
void drawLevel(int currentLevel, int height, int width)
{
	char level[2];

	sprintf(level,"%d",currentLevel);

	changeFontSize(20);

	gfx_text(275, 20, level);

	gfx_text(200, 20, "LEVEL");
}

// display score on screen
void drawScore(int score, int height, int width)
{
	char currentScore[10000];

	sprintf(currentScore,"%d",score);

	changeFontSize(20);

	gfx_text(600, 20, currentScore);

	gfx_text(500, 20, "SCORE");
}

// calculate score a the end of each level
int calculateScore(int score, Missile missileArray[30], City cityArray[30])
{
	int i;

	for (i = 0; i < 6; i++)
	{
		if (cityArray[i].status == alive)
		{
			score += 100; // add 100 for each base left over
		}
	}
	
	for (i = 0; i < 30; i++)
	{
		if (missileArray[i].status == alive)
		{
			score += 5; // add five for each missile left over
		}
	}

	return score;

}

// set each city to alive at the beginning of each game
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

// display "MISSILE COMMAND" on start screen
void startScreen(int width, int height)
{
	char c;

	gfx_clear();

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

// display instruction on instruction screen
void instructionScreen(void)
{
	char c;

	gfx_clear();

	while (1)
	{
		changeFontSize(15);
		gfx_color(0, 0, 255);
		gfx_text(50, 100, "Welcome to Missile Command.");
		gfx_color(0, 94, 255);
		gfx_text(50, 150, "Press 'a' to fire from your left base, 'w' or 's' from your middle base, and 'd' from your right base.");
		gfx_color(41, 177, 255);
		gfx_text(50, 200, "To win, at least one city must be standing at the end of all 5 levels.");
		gfx_color(41, 249, 255);
		gfx_text(50, 250, "Press any key to start.");
		gfx_flush();

		if (gfx_event_waiting())
		{
			c=gfx_wait();
			break;
		}
	}
	
}
