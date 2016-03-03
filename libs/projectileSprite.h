class projectileSprite
{
	public:
		projectileSprite(bool ex = false, int rows = 0, int frames = 0, int w = 0, int h = 0, int gap = 0);
		SDL_Rect getClip(void);
		LTexture gSpriteSheetTexture;
		bool loadMedia();
		void generateClips(void);
		bool isMoving(void);
		void setX(int x);
		void setY(int y);
		int getX();
		int getY();
		int getSpeed();
		bool checkCollision(int eX, int eY, int esizeX, int esizeY);
		void selectRow(int row);
		int getRow();
		bool doesExist();
		void create(int dir, int x, int y,int speed,int range,int dmg);
		void pushNextFrame(void);
		void resetFrame(void);
		bool destroy(void);
		void advance(void);
		
		
	private:
		bool exists;
		int splatState;
		int TTL;
		int SPD;
		int DMG;
		int pX;
		int pY;
		int maxX;
		int maxY;
		int spriteFrameRate;
		int numRows;
		int numFrames;
		int currentRow;
		int currentFrame;
		int fWidth;
		int fHeight;
		int fGap;
		SDL_Rect currentClip[4][4];	
};
	//--------Constructor
projectileSprite::projectileSprite(bool ex, int rows, int frames, int w, int h, int gap)
{
	//static
	exists = false;
	currentFrame=0;
	spriteFrameRate=2;
	//assigned
	splatState=0;
	numRows=rows;
	numFrames=frames;
	fWidth=w;
	fHeight=h;
	fGap=gap;

	maxX=SCREEN_WIDTH-fWidth;
	maxY=SCREEN_HEIGHT-fHeight;
	loadMedia();
	generateClips();
	cout << "Sprite has been created with " << rows << " rows and " << frames << " frames." << endl;
}
//--------Check if this projectile exists (for array purposes)
bool projectileSprite::doesExist()
{
	return exists;
}
//--------Set the projectile to exist
void projectileSprite::create(int dir, int x, int y,int speed,int range,int damage)
{
	exists=true;
	splatState=0;
	currentRow=dir;
	currentFrame=0;
	pX=x;
	pY=y;
	SPD=speed;
	TTL=range;
	DMG=damage;
}

bool projectileSprite::loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load sprite sheet texture
	if( !gSpriteSheetTexture.loadFromFile( "assets/carrot.png" ) )
	{
		printf( "Failed to load sprite sheet texture!\n" );
		success = false;
	}
	return success;
}
//--------Create clips from spritemap
void projectileSprite::generateClips()
{
	for(int i=0;i<numRows;i++)
	{
		for(int j=0;j<numFrames;j++)
		{
			currentClip[i][j].x = (j*(fWidth+fGap));
			currentClip[i][j].y = (i*(fHeight+fGap));
			currentClip[i][j].w = fWidth;
			currentClip[i][j].h = fHeight;
			
			//cout << "Generated Projectile clip (" << i << "," << j << ") at (" << currentClip[i][j].x << "," << currentClip[i][j].y <<")" << endl;
		}
	}
}
//--------Get currently selected clip
SDL_Rect projectileSprite::getClip()
{
	//cout << "X: " << getX() << ", Y: " << getY() << endl;
	return currentClip[currentRow][currentFrame/spriteFrameRate];
}
//--------Set the row (direction) of sprite
void projectileSprite::selectRow(int row)
{
	cout << "Row Changed" << endl;
	currentRow=row;
}
//--------Get the row (direction facing)
int projectileSprite::getRow()
{
	return currentRow;
}
//--------Advance to next frame in row
void projectileSprite::pushNextFrame()
{
	if(currentFrame>=numFrames*spriteFrameRate-1)
	{
		splatState=2;
	}
	else
	{
		//cout << "Current Frame is: " <<currentFrame << endl;
		currentFrame++;
	}
}
//--------Set frame back to start
void projectileSprite::resetFrame()
{
	currentFrame=0;
}

//--------Get x position
int projectileSprite::getX()
{
	//cout << px;
	return pX;
}
//--------Get y position
int projectileSprite::getY()
{
	//cout << py;
	return pY;
}
//--------Get player speed
int projectileSprite::getSpeed()
{
	return SPD;
}
//--------Set x position
void projectileSprite::setX(int x)
{
	if(x <= 0)
	{
		destroy();
	}else if(x >= maxX)
	{
		destroy();
	}else{
		pX = x;
	}
}
//--------set y position
void projectileSprite::setY(int y)
{
	if(y <= 0)
	{
		destroy();
	}else if(y >= maxY)
	{
		destroy();
	}else{
		pY = y;
	}
}
//--------Get player speed

void projectileSprite::advance()
{
	if(TTL>0 && splatState==0)
	{
		switch(currentRow)
		{
			case 0:
				setY(getY()+SPD);
			break;
			case 1:
				setY(getY()-SPD);
			break;
			case 2:
				setX(getX()-SPD);
			break;
			case 3:
				setX(getX()+SPD);
			break;
		}
		TTL--;
	}else if(splatState==1){
		pushNextFrame();
	}else{
		destroy();
		
	}
}
bool projectileSprite::destroy()
{
	exists=false;
}

//--------Check for collision with enemy
bool projectileSprite::checkCollision(int eX, int eY, int esizeX, int esizeY)
{
	if(splatState==1){
		
	}else if(splatState==0)
	{	
		//cout << "checking projectile: " << pX << " - " << pX + fWidth << " in enemy " << eX << " - " << eX + esizeX << endl;
		if((((pX+fWidth) > eX) && (pX < (eX+esizeX))) &&  (((pY+fHeight)>eY) &&(pY<(eY+esizeY))))
		{	
			cout << "Carrot Hit!" << endl;
			splatState=1;
			return true;
		}
	}else if(splatState==2)
	{
		destroy();
	}
	return false;
}
