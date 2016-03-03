class HUD
{
	public:
		HUD(int rows, int frames,int w,int h,int gap);
		SDL_Rect getClip(void);
		LTexture gSpriteSheetTexture;
		bool loadMedia();
		void generateClips(void);
		bool doesExist();
		//void create(int x, int y);
		bool destroy(void);
		void drawHealth(int hp, int mhp);
		void render();
		
		
	private:
		bool exists;
		int hX;
		int hY;
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
		SDL_Rect currentClip[1][5];	
};
	//--------Constructor
HUD::HUD(int rows, int frames,int w,int h,int gap)
{
	//static
	maxX=SCREEN_WIDTH;
	maxY=SCREEN_HEIGHT;
	exists = false;
	currentRow=0;
	currentFrame=0;
	//spriteFrameRate=1;
	numRows=rows;
	numFrames=frames;
	fWidth = w;
	fHeight = h;
	fGap = gap;
	loadMedia();
	generateClips();
	cout << "HUD has been created with " << rows << " rows and " << frames << " frames." << endl;
}
//--------Check if this projectile exists (for array purposes)
bool HUD::doesExist()
{
	return exists;
}
//--------Load the sprite texture
bool HUD::loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load sprite sheet texture
	if( !gSpriteSheetTexture.loadFromFile( "assets/HealthBar.png" ) )
	{
		printf( "Failed to load sprite sheet texture!\n" );
		success = false;
	}
	return success;
}
//--------Create clips from spritemap
void HUD::generateClips()
{
	for(int i=0;i<=numRows;i++)
	{
		for(int j=0;j<=numFrames;j++)
		{
			currentClip[i][j].x = (j*(fWidth+fGap));
			currentClip[i][j].y = (i*(fHeight+fGap));
			currentClip[i][j].w = fWidth;
			currentClip[i][j].h = fHeight;
			
			cout << "Generated Projectile clip (" << i << "," << j << ") at (" << currentClip[i][j].x << "," << currentClip[i][j].y <<")" << endl;
		}
	}
}

//--------Get currently selected clip
SDL_Rect HUD::getClip()
{
	//cout << "X: " << getX() << ", Y: " << getY() << endl;
	return currentClip[currentRow][currentFrame];
}
void HUD::drawHealth(int hp,int mhp)
{
	//cout << "Max HP = " << mhp << ", frame width = " << fWidth << ", current hp = " << hp << endl;
	hX=(maxX-(mhp*fWidth))/2;
	hY=20;
	for(int i=0;i<mhp;i++)
	{
		if(i<hp)
		{	
			if(i%2==0)
			{
				currentFrame=0;
			}else{
				currentFrame=1;
			}
		}else{
			if(i%2==0)
			{
				currentFrame=2;
			}else{
				currentFrame=3;
			}
		}
		hX=hX+fWidth;
		render();
	}
	
}

bool HUD::destroy()
{
	exists=false;
}

void HUD::render()
{
	SDL_Rect hudtemp = getClip();
	gSpriteSheetTexture.render(hX,hY,&hudtemp);
}
