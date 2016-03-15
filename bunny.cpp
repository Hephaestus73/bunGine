//Using SDL, SDL_image, standard in/out, and strings
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_net.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <math.h>
#include <time.h>

using namespace std;
//Settings will later be config LFF
const int SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 540;
const bool full=false;

//Array maximums
const int MAX_PROJECTILES = 50;
const int MAX_ENEMIES = 50;
const int MAX_PLAYERS = 2;


#include "libs/LTexture.h"
#include "libs/playerSprite.h"
#include "libs/enemySprite.h"
#include "libs/projectileSprite.h"
#include "libs/HUDSprite.h"
#include "libs/fireworkSprite.h"
#include "libs/sceneSprite.h"

//The music that will be played
//Mix_Music *gMusic = NULL;

//enumerate possible key presses
enum KeyPressSurfaces
{
	KEY_PRESS_SURFACE_DEFAULT,
	KEY_PRESS_SURFACE_UP,
	KEY_PRESS_SURFACE_DOWN,
	KEY_PRESS_SURFACE_LEFT,
	KEY_PRESS_SURFACE_RIGHT,
	KEY_PRESS_SURFACE_TOTAL
};



bool init();			//Starts up SDL and creates window
void close();			//Frees media and shuts down SDL
bool mainMenu();		//Runs the menu loop rather than main game


bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}
		//Create window
		gWindow = SDL_CreateWindow( "Bunny Game", 0,0/*SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED*/, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		
		if(full==true)
			SDL_SetWindowFullscreen(gWindow,SDL_WINDOW_FULLSCREEN);
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				
				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init(imgFlags) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error1: %s\n", IMG_GetError() );
					success = false;
				}
				 //Initialize SDL_mixer
                if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
                {
                    printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
                    success = false;
                }
			}
		}
	}
	return success;
}

void deathScene()
{
	for(int death=0; death <80;death++)
	{
		SDL_SetRenderDrawColor( gRenderer, 240 - 3*death, 80-death, 80-death, 0xFF );
		SDL_RenderClear( gRenderer );
		SDL_RenderPresent( gRenderer );
		SDL_Delay(32);
	}
	cout << "GAME OVER" << endl;
	SDL_Delay(1000);
}

void winScene()
{
	
	//size of fireworks array
	int numFireworks=10;
	fireworkSprite firework[numFireworks] = fireworkSprite();
	
	for(int j=0; j<400; j++)
	{
		SDL_SetRenderDrawColor( gRenderer, 60, 60, 150, 0xFF );
		SDL_RenderClear( gRenderer );
		
		for(int i=0;i<numFireworks;i++)
		{
			if(firework[i].doesExist()==true)
			{	
				firework[i].advance();
				firework[i].render();
			}else
			{
				if((rand() % 50) > 48)
				{
					firework[i].create();
				}
			}
		}
		SDL_RenderPresent( gRenderer );
	}
	cout << "YOU WIN" << endl;
	SDL_Delay(1000);
}


void close()
{
	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}


int main( int argc, char* args[] )
{
	srand(time(0));
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
//----------------Set Game loop variables----------------//
		int spriteOffsetX;
		int spriteOffsetY;
		int mapWidth=1536;		//will be dynamic later
		int mapHeight=1152;		//will be dynamic later
		
		int genEns=20; //how many enemies to generate
		
//----------------Create entity arrays----------------//
	//--------Scene effects--------//
		sceneSprite sceneEffects = sceneSprite(mapWidth,mapHeight);

	//--------Players--------//
		int numPlayers = 1;
		int thisPlayer = 0;
		HUD hud(1,4,28,48,0);
		playerSprite player[MAX_PLAYERS] = playerSprite(4,4,72,172,2,1,1,mapWidth,mapHeight,8,20,100,25,6,12);//rows,frames,width,height,gap,x,y,mX,mY,hp,dmg,range,fr,spd,sspd
		for(int i=0;i<numPlayers;i++)
		{
			cout << "Player " << i << " created!" << endl;
		}
	//--------Enemies--------//
		int enX;
		int enY;
		enemySprite enemies[MAX_ENEMIES] = enemySprite(4,4,72,172,2,430,50,mapWidth,mapHeight);
		for(int ens=0; ens<genEns;ens++)
		{
			enX = (rand() % (mapWidth-(enemies[ens].getWidth())-200))+200;
			enY = (rand() % (mapHeight-(enemies[ens].getHeight())-200))+200;
			cout << "X = " << enX << ", Y = " << enY << endl;
			//enemies[ens].create(0,SCREEN_WIDTH-(100+ens*25),SCREEN_HEIGHT-(200+ens*25),1);
			enemies[ens].create(0, enX, enY,1);
		}
	//--------Projectiles--------//
		projectileSprite projectiles[MAX_PROJECTILES] = projectileSprite(false,4,4,48,48,2,mapWidth,mapHeight);
			
	/*--------------------------------Main Game Loop--------------------------------*/
			bool quit = false;			//Main loop continutation/quit flag	
			SDL_Event e;				//Event handler		
			bool pAction[5] = {false};	//Multi action Handler array
			
			
			while( !quit )
			{				
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					{
						//User requests quit
						if( e.type == SDL_QUIT )
						{
							quit = true;
						}
						//User presses a key
						else if( e.type == SDL_KEYDOWN || SDL_KEYUP)
						{
							//set actions based on keys pressed
							switch( e.key.keysym.sym )
							{
								case SDLK_DOWN:
									if(e.type == SDL_KEYDOWN)
									{
										pAction[0]=true;
									}else if(e.type == SDL_KEYUP)
									{
										pAction[0]=false;
										player[thisPlayer].resetFrame();
									}
								break;
								case SDLK_UP:
									if(e.type == SDL_KEYDOWN)
									{
										pAction[1]=true;
									}else if(e.type == SDL_KEYUP)
									{
										pAction[1]=false;
										player[thisPlayer].resetFrame();
									}
								break;
								case SDLK_LEFT:
									if(e.type == SDL_KEYDOWN)
									{
										pAction[2]=true;
									}else if(e.type == SDL_KEYUP)
									{
										pAction[2]=false;
										player[thisPlayer].resetFrame();
									}
								break;
								case SDLK_RIGHT:
									if(e.type == SDL_KEYDOWN)
									{
										pAction[3]=true;
									}else if(e.type == SDL_KEYUP)
									{
										pAction[3]=false;
										player[thisPlayer].resetFrame();
									}
								break;
								case SDLK_SPACE:
									if(e.type == SDL_KEYDOWN)
									{
										pAction[4]=true;
									}else if(e.type == SDL_KEYUP)
									{
										pAction[4]=false;
										player[thisPlayer].resetFrame();
									}
								default:
								break;
							}
						}
					}
				}				
				
				if(pAction[0]==true)	//move down
				{
					if(pAction[2]==true||pAction[3]==true)
					{
						player[thisPlayer].moveDown(0.72);
					}else{
						player[thisPlayer].moveDown(1);
					}
					player[thisPlayer].selectRow(0);
					player[thisPlayer].setMoving(true);		
				}
				if(pAction[1]==true)	//move up
				{
					if(pAction[2]==true||pAction[3]==true)
					{
						player[thisPlayer].moveUp(0.72);
					}else{
						player[thisPlayer].moveUp(1);
					}
					player[thisPlayer].selectRow(1);
					player[thisPlayer].setMoving(true);
				}
				if(pAction[2]==true)	//move left
				{
					if(pAction[0]==true||pAction[1]==true)
					{
						player[thisPlayer].moveLeft(0.72);
					}else{
						player[thisPlayer].moveLeft(1);
					}
					player[thisPlayer].selectRow(2);
					player[thisPlayer].setMoving(true);
				}
				if(pAction[3]==true)	//move right
				{
					if(pAction[0]==true||pAction[1]==true)
					{
						player[thisPlayer].moveRight(0.72);
					}else{
						player[thisPlayer].moveRight(1);
					}
					player[thisPlayer].selectRow(3);
					player[thisPlayer].setMoving(true);
				}
				if(pAction[4]==true)	//fire carrot
				{
					for(int numproj=0;numproj<MAX_PROJECTILES;numproj++)
					{
						if(player[thisPlayer].getProjDelay()<1 && !projectiles[numproj].doesExist())
						{
							projectiles[numproj].create(player[thisPlayer].getRow(),player[thisPlayer].getX(),player[thisPlayer].getY()+player[thisPlayer].getHeight()/2,player[thisPlayer].getShotSpeed(),player[thisPlayer].getRange(),player[thisPlayer].getDamage());
							player[thisPlayer].projDelayStart();
						}
					}
				}
				

//----------------PROCESS FRAME----------------//
	
	//--------Get Sprite offset (applies to all sprites)--------//			
				if(player[thisPlayer].getX() < (SCREEN_WIDTH/2) - (player[thisPlayer].getWidth()/2)) //if less than half the screen width
				{
					spriteOffsetX = 0;
				}else if(player[thisPlayer].getX() > (mapWidth-(SCREEN_WIDTH/2)) - (player[thisPlayer].getWidth()/2)) //if more than half screen width from map width
				{
					spriteOffsetX = mapWidth - SCREEN_WIDTH;
				}else{
					spriteOffsetX = player[thisPlayer].getX() - (SCREEN_WIDTH /2)+(player[thisPlayer].getWidth()/2);
				}
				
				if(player[thisPlayer].getY() < (SCREEN_HEIGHT /2) - (player[thisPlayer].getHeight()/2))//if less than half the screen width
				{
					spriteOffsetY = 0;
				}else if(player[thisPlayer].getY() > mapHeight-(SCREEN_HEIGHT/2) - (player[thisPlayer].getHeight()/2))//if more than half screen width from map width
				{
					spriteOffsetY =  mapHeight - SCREEN_HEIGHT;
				}else{
					spriteOffsetY = player[thisPlayer].getY() - (SCREEN_HEIGHT /2)+(player[thisPlayer].getHeight()/2);
				}
				
				
				
				

				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 80, 180, 80, 0xFF );
				SDL_RenderClear( gRenderer );
				
//----------------Process Scene----------------//
				sceneEffects.render(spriteOffsetX, spriteOffsetY);

//----------------Process Player----------------//
				if(player[thisPlayer].isMoving()==true)
					player[thisPlayer].pushNextFrame();
				player[thisPlayer].setMoving(false);
				player[thisPlayer].render(spriteOffsetX,spriteOffsetY);
				player[thisPlayer].stateIncrement();
				

//----------------Process Enemies----------------//			
				int currentEnemies = 0;
				bool playerHit = false;
				int damageCounter = 0;
				for(int encount = 0;encount<MAX_ENEMIES;encount++)
				{
					if(enemies[encount].doesExist())
					{	
						currentEnemies++;
						enemies[encount].behaviourPassive();
						if(player[thisPlayer].getHitDelay()<1 && player[thisPlayer].checkCollision(enemies[encount].getX(),enemies[encount].getY(),enemies[encount].getWidth(),enemies[encount].getHeight())==true)
						{
							playerHit=true;
							damageCounter=enemies[encount].getDamage();
						}
						enemies[encount].render(spriteOffsetX, spriteOffsetY);
					}	
				}
				if(playerHit==true)
				{
					player[thisPlayer].damagePlayer(damageCounter);
					player[thisPlayer].hitDelayStart();
				}
				
//----------------Process Projectiles----------------//
				
					for(int projcount=0;projcount<MAX_PROJECTILES;projcount++)
					{	
						if(projectiles[projcount].doesExist())
						{
							for(int encount = 0;encount<MAX_ENEMIES;encount++)
							{
								if(enemies[encount].doesExist())
								{
									if(projectiles[projcount].checkCollision(enemies[encount].getX(),enemies[encount].getY(),enemies[encount].getWidth(),enemies[encount].getHeight())==true)
										enemies[encount].damageEnemy(player[thisPlayer].getDamage());
								}
							}
							projectiles[projcount].advance();
							projectiles[projcount].render(spriteOffsetX,spriteOffsetY); 
						}
					}
	//--------Process HUD--------//
	hud.drawHealth(player[thisPlayer].getHealth(), player[thisPlayer].getMaxHealth());

				//Update screen
				SDL_RenderPresent(gRenderer);
				//SDL_Delay(FRAME_DELAY);
				
	//--------Check if player is dead--------//

				if(player[thisPlayer].getHealth() == 0)
				{
					quit=true;
					deathScene();
				}
	//---------Check if all enemies are dead--------//				
				if(currentEnemies==0)
				{
					quit=true;
					winScene();
				}
			}
	/*--------------------------------End Main Game Loop--------------------------------*/		
	}
	//Free resources and close SDL
	close();
	return 0;
}
