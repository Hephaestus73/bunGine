//#include <iostream>
//using namespace std;

class tileSprite
{
	public:
		tileSprite(int rows, int frames, int w, int h, int gap, int x, int y);
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
tileSprite::tileSprite(int rows, int frames, int w, int h, int gap, int x, int y)
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
	maxX=mX-fWidth;
	maxY=mY-fHeight;
	if(!loadMedia())
	{
		cout << "Failed to Load Media!" << endl;
	}else{
		generateClips();
	}
	//cout << "Sprite has been created with " << rows << " rows and " << frames << " frames." << endl;
}
//--------Check if the enemy exists
bool tileSprite::doesExist()
{
	return exists;
}
//--------Create enemy
void tileSprite::create(int dir, int x, int y, int dmg)
{
	exists=true;
	health=100;
	DMG = dmg;
	currentRow=dir;
	currentFrame=0;
	pX=x;
	pY=y;
}

bool tileSprite::loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load sprite sheet texture
	if( !gSpriteSheetTexture.loadFromFile( "assets/tileSprite.png" ) )
	{
		printf( "Failed to load sprite sheet texture!\n" );
		success = false;
	}
	return success;
}
//--------Create clips from spritemap
void tileSprite::generateClips()
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
SDL_Rect tileSprite::getClip()
{
	return currentClip[currentRow][currentFrame/spriteFrameRate];
}
//--------Set the row (direction) of sprite
void tileSprite::selectRow(int row)
{
	currentRow=row;
}
//--------Advance to next frame in row
void tileSprite::pushNextFrame()
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
void tileSprite::resetFrame()
{
	currentFrame=0;
}
//--------Set if moving or not
void tileSprite::setMoving(bool setMovingTo)
{
	moving = setMovingTo;
}
//----------------Enemy GETS----------------//

//--------Get Enemy Health
int tileSprite::getHealth(){return health;}
//--------Get Enemy Damage
int tileSprite::getDamage(){return DMG;}

//--------Check if moving or not
bool tileSprite::isMoving(void){return moving;}
//--------Get x position
int tileSprite::getX(){return pX;}
//--------Get y position
int tileSprite::getY(){return pY;}
//--------Get width
int tileSprite::getWidth(){return fWidth;}
//--------Get height
int	tileSprite::getHeight(){return fHeight;}
//--------Get enemy speed
int tileSprite::getSpeed(){return SPD;}
//--------Set x position
void tileSprite::setX(int x)
{
	if(0 < x && x < maxX)
	{
		pX = x;
	}
}
//--------set y position
void tileSprite::setY(int y)
{
	if(0 < y && y < maxY)
	{
		pY = y;
	}
}
//--------Get enemy speed
void tileSprite::setSpeed(int s)
{
		SPD = s;
}
void tileSprite::damageEnemy(int dmg)
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
int tileSprite::destroy()
{
	exists=false;
	cout << "Enemy Destroyed" << endl;
}

void tileSprite::behaviourPassive()
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
void tileSprite::render()
{
	int flash = hitTimer*20;
	gSpriteSheetTexture.tint(255,255-flash,255-flash);
	renderedFrame = getClip();
	gSpriteSheetTexture.render(pX,pY,&renderedFrame);
}
