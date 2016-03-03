//#include <iostream>
//using namespace std;

class playerSprite
{
	
	public:
		playerSprite(int rows, int frames, int w, int h, int gap, int x, int y, int health, int damage, int range, int firerate, int speed, int shotspeed);
		void selectRow(int row);
		int getRow();
		void pushNextFrame(void);
		void resetFrame(void);
		SDL_Rect getClip(void);
		LTexture gSpriteSheetTexture;
		bool loadMedia();
		void generateClips(void);
		void setMoving(bool);
		bool isMoving(void);
		void setX(int x);
		void setY(int y);
		int getX();
		int getY();
		void moveDown(int mod);
		void moveUp(int mod);
		void moveLeft(int mod);
		void moveRight(int mod);
		int getWidth();
		int getHeight();
		int getMaxHealth();
		int getHealth();
		int getDamage();
		int getRange();
		int getFireRate();
		int getSpeed();
		int getShotSpeed();
		void setMaxHealth(int mh);
		void setHealth(int health);
		void setDamage(int damage);
		void setRange(int range);
		void setFireRate(int firerate);
		void setSpeed(int speed);
		void setShotSpeed(int shotspeed);
		bool checkCollision(int eX, int eY, int esizeX, int esizeY);
		void setProjReady(bool pr);
		bool getProjReady();
		void damagePlayer(int dmgamnt);
		
		//----Counters----
		void hitDelayStart();
		int getHitDelay();
		int setHitDelay();
		void projDelayStart();
		int getProjDelay();
		int setProjDelay();
		void stateIncrement();
		void render();
		
	private:
//--------Player Stats
		int maxHP;
		int HP;
		int DMG;
		int RNG;
		int FR;
		int SPD;
		int SSPD;
		
//--------Other Sprite info		
		int hitDelay;
		int projDelay;
		int hitCount;
		bool projReady;
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
		bool moving;
		SDL_Rect renderedFrame;
		SDL_Rect currentClip[4][4];	
};
//--------Constructor
playerSprite::playerSprite(int rows, int frames, int w, int h, int gap, int x, int y, int health, int damage, int range, int firerate, int speed, int shotspeed)
{
	maxHP=health;
	HP=health;
	DMG=damage;
	RNG=range;
	FR=firerate;
	SPD=speed;
	SSPD=shotspeed;
	currentRow=0;
	currentFrame=0;
	hitDelay=0;
	numRows=rows;
	numFrames=frames;
	fWidth=w;
	fHeight=h;
	fGap=gap;
	pX=x;
	pY=y;
	spriteFrameRate=6;
	maxX=SCREEN_WIDTH-fWidth;
	maxY=SCREEN_HEIGHT-fHeight;
	loadMedia();
	generateClips();	
	//cout << "Sprite has been created with " << rows << " rows and " << frames << " frames." << endl;
}
bool playerSprite::loadMedia()
{
	//Loading success flag
	bool success = true;
	//Load sprite sheet texture
	if( !gSpriteSheetTexture.loadFromFile( "assets/bunnySpriteMap.png" ) )
	{
		printf( "Failed to load sprite sheet texture!\n" );
		success = false;
	}
	return success;
}
//--------Create clips from spritemap
void playerSprite::generateClips()
{
	for(int i=0;i<numRows;i++)
	{
		for(int j=0;j<numFrames;j++)
		{
			currentClip[i][j].x = (j*(fWidth+fGap));
			currentClip[i][j].y = (i*(fHeight+fGap));
			currentClip[i][j].w = fWidth;
			currentClip[i][j].h = fHeight;
			
			cout << "Generated Player clip (" << i << "," << j << ") at (" << currentClip[i][j].x << "," << currentClip[i][j].y <<")" << endl;
		}
	}
}
//--------Get currently selected clip
SDL_Rect playerSprite::getClip()
{
	return currentClip[currentRow][currentFrame/spriteFrameRate];
}
//--------Set the row (direction) of sprite
void playerSprite::selectRow(int row)
{
	currentRow=row;
}
//--------Get the row (direction facing)
int playerSprite::getRow()
{
	return currentRow;
}
//--------Advance to next frame in row
void playerSprite::pushNextFrame()
{
	if(currentFrame>=spriteFrameRate*(numFrames)-1)
	{
		currentFrame=0;
	}
	else
	{
		currentFrame++;
	}
}
//--------Set frame back to start
void playerSprite::resetFrame()
{
	currentFrame=0;
}
//--------Set if moving or not
void playerSprite::setMoving(bool setMovingTo)
{
	moving = setMovingTo;
}
//--------Check if moving or not
bool playerSprite::isMoving(void)
{
	return moving;
}
//--------Get x position
int playerSprite::getX(){return pX;}
//--------Get y position
int playerSprite::getY(){return pY;}
//--------Get Width
int playerSprite::getWidth(){return fWidth;}
//--------Get height
int	playerSprite::getHeight(){return fHeight;}




//--------Move Down
void playerSprite::moveDown(int mod)
{
	if(pY < maxY)
	{
		pY=pY+SPD*mod;
	}
}
void playerSprite::moveUp(int mod)
{
	if(pY > 0)
	{
		pY=pY-SPD*mod;
	}
}
void playerSprite::moveLeft(int mod)
{
	if(pX > 0)
	{
		pX=pX-SPD*mod;
	}
}
//--------
void playerSprite::moveRight(int mod)
{
	if(pX < maxX)
	{
		pX=pX+SPD*mod;
	}
}
//--------Set x position
void playerSprite::setX(int x)
{
	if(0 < x && x < maxX)
	{
		pX = x;
	}
}
//--------set y position
void playerSprite::setY(int y)
{
	if(0 < y && y < maxY)
	{
		pY = y;
	}
}

//----------------PLAYER STAT GETs----------------//

//--------Get player Health
int playerSprite::getMaxHealth(){return maxHP;}
//--------Get player Health
int playerSprite::getHealth(){return HP;}
//--------Get player Damage
int playerSprite::getDamage(){return DMG;}
//--------Get player Range
int playerSprite::getRange(){return RNG;}
//--------Get player Fire Rate
int playerSprite::getFireRate(){return FR;}
//--------Get player Speed
int playerSprite::getSpeed(){return SPD;}
//--------Get player Shot Speed
int playerSprite::getShotSpeed(){return SSPD;}

//----------------PLAYER STAT SETs----------------//

//--------Set player Maximum Health
void playerSprite::setMaxHealth(int mh){maxHP=mh;}
//--------Set player Health
void playerSprite::setHealth(int health){HP=health;}
//--------Set player Damage
void playerSprite::setDamage(int damage){DMG=damage;}
//--------Set player Range
void playerSprite::setRange(int range){RNG=range;}
//--------Set player Fire Rate
void playerSprite::setFireRate(int firerate){FR=firerate;}
//--------Set player Speed
void playerSprite::setSpeed(int speed){SPD=speed;}
//--------Set player Speed
void playerSprite::setShotSpeed(int shotspeed){SSPD=shotspeed;}


bool playerSprite::checkCollision(int eX, int eY, int esizeX, int esizeY)
{
	if((((pX+fWidth) > eX) && (pX < (eX+esizeX))) &&  (((pY+fHeight)>eY) &&(pY<(eY+esizeY))))
	{
		cout << "Bunny Hit! HP = " << HP << endl;
		return true;
	}else
	{
		return false;
	}
}
void playerSprite::damagePlayer(int dmgamnt)
{
	HP = HP-dmgamnt;
}

void playerSprite::setProjReady(bool pr)
{
	projReady = pr;
}

bool playerSprite::getProjReady()
{
	return projReady;
}
//--------Initiate Projectile Delay (will later depend on shot frequency stat)
void playerSprite::projDelayStart()
{
	projDelay = FR;
}
void playerSprite::hitDelayStart()
{
	hitDelay = 45;
}
//--------Increment Projectile delay 1 closer to being able to shoot
void playerSprite::stateIncrement()
{
	if(projDelay>0){projDelay--;}
	if(hitDelay>0){hitDelay--;}
}

int playerSprite::getProjDelay()
{
	return projDelay;
}

int playerSprite::getHitDelay()
{
	return hitDelay;
}

//--------Render this Player--------//
void playerSprite::render()
{
	//--------Generate tint colour based of sine of hitDelay. This creates a flashing pattern when hit otherwise does not tint
	double flash = 100* (sin(hitDelay*3.14159265/10));
	//cout << "Flash = :" << flash << endl;
	gSpriteSheetTexture.tint(255,255-abs(flash),255-abs(flash));
	renderedFrame = getClip();
	gSpriteSheetTexture.render(pX,pY,&renderedFrame);
}
