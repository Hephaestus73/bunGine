//Using SDL, SDL_image, standard in/out, and strings
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int FRAME_DELAY = 20;

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

//Texture wrapper class
class LTexture
{
	public:
		//Initializes variables
		LTexture();

		//Deallocates memory
		~LTexture();

		//Loads image at specified path
		bool loadFromFile( std::string path );

		//Deallocates texture
		void free();

		//Renders texture at given point
		void render( int x, int y, SDL_Rect* clip = NULL );

		//Gets image dimensions
		int getWidth();
		int getHeight();

	private:
		//The actual hardware texture
		SDL_Texture* mTexture;

		//Image dimensions
		int mWidth;
		int mHeight;
};

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Scene sprites
SDL_Rect gSpriteClips[4][4];
LTexture gSpriteSheetTexture;


LTexture::LTexture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture()
{
	//Deallocate
	free();
}

bool LTexture::loadFromFile( std::string path )
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Color key image
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

void LTexture::free()
{
	//Free texture if it exists
	if( mTexture != NULL )
	{
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::render( int x, int y, SDL_Rect* clip )
{
	//Set rendering space and render to screen
	SDL_Rect bunnySprite = { x, y, mWidth, mHeight };
	//SDL_RenderSetScale(gRenderer, 2,2);

	//Set clip rendering dimensions
	if( clip != NULL )
	{
		bunnySprite.w = clip->w;
		bunnySprite.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopy( gRenderer, mTexture, clip, &bunnySprite );
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
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
		gWindow = SDL_CreateWindow( "Bunny Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
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
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_mage Error: %s\n", IMG_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load sprite sheet texture
	if( !gSpriteSheetTexture.loadFromFile( "bunnySpriteMap.png" ) )
	{
		printf( "Failed to load sprite sheet texture!\n" );
		success = false;
	}
	else
	{
	//--------Set Down sprite--------//
		//Stationary F1
		gSpriteClips[0][0].x = 0;
		gSpriteClips[0][0].y = 0;
		gSpriteClips[0][0].w = 72;
		gSpriteClips[0][0].h = 172;
		//Move F1
		/*gSpriteClips[0][1].x = 74;
		gSpriteClips[0][1].y = 0;
		gSpriteClips[0][1].w = 72;
		gSpriteClips[0][1].h = 192;
		//Move F2
		gSpriteClips[0][2].x = 74;
		gSpriteClips[0][2].y = 0;
		gSpriteClips[0][2].w = 72;
		gSpriteClips[0][2].h = 192;
		//Move F3
		gSpriteClips[0][3].x = 74;
		gSpriteClips[0][3].y = 0;
		gSpriteClips[0][3].w = 72;
		gSpriteClips[0][3].h = 192;*/
	
	//--------Set Down sprite--------//
		//Stationary
		gSpriteClips[1][0].x = 0;
		gSpriteClips[1][0].y = 174;
		gSpriteClips[1][0].w = 72;
		gSpriteClips[1][0].h = 172;
	
	
	//--------Set Left sprite--------//	
		//Stationary
		gSpriteClips[2][0].x = 0;
		gSpriteClips[2][0].y = 348;
		gSpriteClips[2][0].w = 72;
		gSpriteClips[2][0].h = 172;
	
	
	//--------Set Right sprite--------//
		//Stationary
		gSpriteClips[3][0].x = 0;
		gSpriteClips[3][0].y = 522;
		gSpriteClips[3][0].w = 72;
		gSpriteClips[3][0].h = 172;
	}

	return success;
}

class Sprite
{
	public:
		Sprite();
};



void close()
{
	//Free loaded images
	gSpriteSheetTexture.free();

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
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{	
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;
			
			//Set position to 50,50 and direction to up
			bool pAction[4] = {false};
			int pX = 50;
			int pY = 50;
			int pDir = 0;
			int pAnimFrame = 0;
			
			//Render arrow for first time
			gSpriteSheetTexture.render( pX, pY, &gSpriteClips[pDir][pAnimFrame] );
			
	/*--------------------------------Main Game Loop--------------------------------*/
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
							//Select surfaces based on key press
							switch( e.key.keysym.sym )
							{
								case SDLK_DOWN:
									if(e.type == SDL_KEYDOWN)
									{
										pAction[0]=true;
									}else if(e.type == SDL_KEYUP)
									{
										pAction[0]=false;
									}
								break;
								case SDLK_UP:
									if(e.type == SDL_KEYDOWN)
									{
										pAction[1]=true;
									}else if(e.type == SDL_KEYUP)
									{
										pAction[1]=false;
									}
								break;
								case SDLK_LEFT:
									if(e.type == SDL_KEYDOWN)
									{
										pAction[2]=true;
									}else if(e.type == SDL_KEYUP)
									{
										pAction[2]=false;
									}
								break;
								case SDLK_RIGHT:
									if(e.type == SDL_KEYDOWN)
									{
										pAction[3]=true;
									}else if(e.type == SDL_KEYUP)
									{
										pAction[3]=false;
									}
								break;
								default:
								break;
							}
						}
					}
				}				
				
				if(pAction[0]==true)
				{
					if(pAction[2]==true||pAction[3]==true)
					{
						pY = pY + 3;
					}else{
						pY = pY + 4;
					}
					pDir = 0;
				}
				if(pAction[1]==true)
				{
					if(pAction[2]==true||pAction[3]==true)
					{
						pY = pY - 3;
					}else{
						pY = pY - 4;
					}
					pDir = 1;
				}
				if(pAction[2]==true)
				{
					if(pAction[0]==true||pAction[1]==true)
					{
						pX = pX - 3;
					}else{
						pX = pX - 4;
					}
					pDir = 2;
				}
				if(pAction[3]==true)
				{
					if(pAction[0]==true||pAction[1]==true)
					{
						pX = pX + 3;
					}else{
						pX = pX + 4;
					}
					pDir = 3;
				}

				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0, 255, 0, 0xFF );
				SDL_RenderClear( gRenderer );
				//Render Sprite
				gSpriteSheetTexture.render( pX, pY, &gSpriteClips[pDir][pAnimFrame] );
				//Update screen
				SDL_RenderPresent( gRenderer );
				SDL_Delay(FRAME_DELAY);
			}
			
	/*--------------------------------Main Game Loop--------------------------------*/		
			
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}
