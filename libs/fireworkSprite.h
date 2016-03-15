class fireworkSprite
{
	public:
		fireworkSprite();	
		LTexture gSpriteSheetTexture;
		SDL_Rect getClip(void);
		void generateClips(void);
		bool loadMedia();
		bool doesExist();
		void create();
		void advance();
		void destroy();
		void render();
		
	private:
		bool exists;
		int TTL;
		int fX;
		int fY;
		int tint;
		int maxX;
		int maxY;
		int numRows;
		int numFrames;
		int fWidth;
		int fHeight;
		int fGap;
		int currentRow;
		int currentFrame;
		int spriteFrameRate;
		SDL_Rect currentClip[1][6];	
		Mix_Chunk *fireworkSound = NULL;
};
	//--------Constructor
fireworkSprite::fireworkSprite()
{
	//static
	TTL=0;
	maxX=SCREEN_WIDTH;
	maxY=SCREEN_HEIGHT;
	exists = false;
	numRows=1;
	numFrames=6;
	currentRow=0;
	currentFrame=0;
	spriteFrameRate=4;
	fWidth = 128;
	fHeight = 128;
	fGap = 2;
	loadMedia();
	generateClips();
	
	//cout << "HUD has been created with " << numRows << " rows and " << numFrames << " frames." << endl;
}

bool fireworkSprite::doesExist()
{
	return exists;
}
//--------Load the sprite texture
bool fireworkSprite::loadMedia()
{
	//Loading success flag
	bool success = true;
	//Load sprite sheet texture
	if( !gSpriteSheetTexture.loadFromFile( "assets/firework.png" ) )
	{
		printf( "Failed to load sprite sheet texture!\n" );
		success = false;
	}
	fireworkSound = Mix_LoadWAV( "sounds/hit.wav" );
	if(fireworkSound == NULL)
	{
		printf( "Failed to load sounds\n" );
		success = false;
	}
	return success;
}
//--------Create clips from spritemap
void fireworkSprite::generateClips()
{
	for(int i=0;i<numRows;i++)
	{
		for(int j=0;j<numFrames;j++)
		{
			currentClip[i][j].x = (j*(fWidth+fGap));
			currentClip[i][j].y = (i*(fHeight+fGap));
			currentClip[i][j].w = fWidth;
			currentClip[i][j].h = fHeight;
			//cout << "Generated Firework clip (" << i << "," << j << ") at (" << currentClip[i][j].x << "," << currentClip[i][j].y <<")" << endl;
		}
	}
}

//--------Get currently selected clip
SDL_Rect fireworkSprite::getClip()
{
	//cout << "X: " << getX() << ", Y: " << getY() << endl;
	return currentClip[currentRow][currentFrame];
}

void fireworkSprite::create()
{
	exists = true;
	Mix_PlayChannel( -1, fireworkSound, 0 );
	TTL = numFrames*spriteFrameRate;
	fX = rand() % maxX + 1; 
	fY = rand() % maxY + 1;
	tint = rand() % 6 ;
	switch(tint)
	{
		case 0: gSpriteSheetTexture.tint(255,50,50);
		break;
		case 1: gSpriteSheetTexture.tint(50,255,50);
		break;
		case 2: gSpriteSheetTexture.tint(50,50,255);
		break;
		case 3: gSpriteSheetTexture.tint(255,255,50);
		break;
		case 4: gSpriteSheetTexture.tint(50,255,255);
		break;
		case 5: gSpriteSheetTexture.tint(255,50,255);
		break;
	}
	//cout << "Created firework at (X = " << fX << ", Y = " << fY << ")" << endl;
}

void fireworkSprite::advance()
{
	if(TTL<1)
		destroy();
	else
	{
		currentFrame=int(6-(TTL/spriteFrameRate));
		TTL--;
	}
}

void fireworkSprite::destroy()
{
	exists = false;
}

void fireworkSprite::render()
{
	
	SDL_Rect temp = getClip();
	gSpriteSheetTexture.render(fX,fY,&temp);
}
