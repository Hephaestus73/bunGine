//#include <iostream>
//using namespace std;

class enemySprite
{
	public:
		enemySprite(int rows, int frames, int w, int h, int gap, int x, int y);
		void selectRow(int row);
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
		void setSpeed(int s);
		int getX();
		int getY();
		int getWidth();
		int getHeight();
		int getSpeed();
		int getDamage();
		int getHealth();
		void create(int dir, int x, int y, int dmg);
		int destroy(void);
		bool doesExist();
		void damageEnemy(int dmg);
		void behaviourPassive(void);
		void render();
		
	private:
	//----Enemy Stats
		int DMG;
		int health;
		int SPD;	
			
		bool allowedMove;
		int move;
		int dir;
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
		int hitTimer;
		bool exists;
		bool moving;
		SDL_Rect renderedFrame;
		SDL_Rect currentClip[4][4];	
};
//--------Constructor
enemySprite::enemySprite(int rows, int frames, int w, int h, int gap, int x, int y)
{
	exists = false;
	currentRow=0;
	currentFrame=0;
	numRows=rows;
	numFrames=frames;
	fWidth=w;
	fHeight=h;
	fGap=gap;
	pX=x;
	pY=y;
	SPD=1;
	move=0;
	dir=0;
	spriteFrameRate=6;
	maxX=SCREEN_WIDTH-fWidth;
	maxY=SCREEN_HEIGHT-fHeight;
	if(!loadMedia())
	{
		cout << "Failed to Load Media!" << endl;
	}else{
		generateClips();
	}
	//cout << "Sprite has been created with " << rows << " rows and " << frames << " frames." << endl;
}
//--------Check if the enemy exists
bool enemySprite::doesExist()
{
	return exists;
}
//--------Create enemy
void enemySprite::create(int dir, int x, int y, int dmg)
{
	exists=true;
	health=100;
	DMG = dmg;
	currentRow=dir;
	currentFrame=0;
	pX=x;
	pY=y;
}

bool enemySprite::loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load sprite sheet texture
	if( !gSpriteSheetTexture.loadFromFile( "assets/enemySpriteMap.png" ) )
	{
		printf( "Failed to load sprite sheet texture!\n" );
		success = false;
	}
	return success;
}
//--------Create clips from spritemap
void enemySprite::generateClips()
{
	for(int i=0;i<numRows;i++)
	{
		for(int j=0;j<numFrames;j++)
		{
			currentClip[i][j].x = (j*(fWidth+fGap));
			currentClip[i][j].y = (i*(fHeight+fGap));
			currentClip[i][j].w = fWidth;
			currentClip[i][j].h = fHeight;
			
			//cout << "Generated clip (" << i << "," << j << ") at (" << currentClip[i][j].x << "," << currentClip[i][j].y <<")" << endl;
		}
	}
}
//--------Get currently selected clip
SDL_Rect enemySprite::getClip()
{
	return currentClip[currentRow][currentFrame/spriteFrameRate];
}
//--------Set the row (direction) of sprite
void enemySprite::selectRow(int row)
{
	currentRow=row;
}
//--------Advance to next frame in row
void enemySprite::pushNextFrame()
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
void enemySprite::resetFrame()
{
	currentFrame=0;
}
//--------Set if moving or not
void enemySprite::setMoving(bool setMovingTo)
{
	moving = setMovingTo;
}
//----------------Enemy GETS----------------//

//--------Get Enemy Health
int enemySprite::getHealth(){return health;}
//--------Get Enemy Damage
int enemySprite::getDamage(){return DMG;}

//--------Check if moving or not
bool enemySprite::isMoving(void){return moving;}
//--------Get x position
int enemySprite::getX(){return pX;}
//--------Get y position
int enemySprite::getY(){return pY;}
//--------Get width
int enemySprite::getWidth(){return fWidth;}
//--------Get height
int	enemySprite::getHeight(){return fHeight;}
//--------Get enemy speed
int enemySprite::getSpeed(){return SPD;}
//--------Set x position
void enemySprite::setX(int x)
{
	if(0 < x && x < maxX)
	{
		pX = x;
	}
}
//--------set y position
void enemySprite::setY(int y)
{
	if(0 < y && y < maxY)
	{
		pY = y;
	}
}
//--------Get enemy speed
void enemySprite::setSpeed(int s)
{
		SPD = s;
}
void enemySprite::damageEnemy(int dmg)
{
	health = health - dmg;
	if(health <=0)
	{
		cout << "Enemy health <0" << endl;
		destroy();
	}else
	{
		hitTimer=10;
	}
}
//--------Destroy enemy
int enemySprite::destroy()
{
	exists=false;
	cout << "Enemy Destroyed" << endl;
}

void enemySprite::behaviourPassive()
{
	if(hitTimer>0)
		hitTimer--;
	allowedMove = true;
	if(allowedMove==true)
	{
		if(move==0)
		{
			
			dir = rand() % 6;
			move = rand() % 30 + 10;
		}else{
			switch(dir)
			{
				case 0:
					setY(getY()+2);
					selectRow(dir);
					pushNextFrame();
				break;
				case 1:
					setY(getY()-2);
					selectRow(dir);
					pushNextFrame();
				break;
				case 2:
					setX(getX()-2);
					selectRow(dir);
					pushNextFrame();
				break;
				case 3:
					setX(getX()+2);
					selectRow(dir);
					pushNextFrame();
				break;
				default:
					resetFrame();
				break;
			}
			
			move--;
		}
	}
}
void enemySprite::render()
{
	int flash = hitTimer*20;
	gSpriteSheetTexture.tint(255,255-flash,255-flash);
	renderedFrame = getClip();
	gSpriteSheetTexture.render(pX,pY,&renderedFrame);
}
