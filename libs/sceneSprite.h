class sceneSprite
{
	public:
		sceneSprite(int,int);
		SDL_Rect getClip(void);
		LTexture gSpriteSheetTexture;
		
		void render(int offsetX, int offsetY);
	private:
		int sceneWidth;
		int sceneHeight;
		bool loadMedia();
		void generateClips(void);
		SDL_Rect renderedFrame;
};

//----Constructor----//
sceneSprite::sceneSprite(int w, int h)
{
	sceneWidth = w;
	sceneHeight = h;
	if(!loadMedia())
		{
			cout << "Failed to Load Media!" << endl;
		}else{
			generateClips();
		}	
}

//----Image Loader----//
bool sceneSprite::loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load sprite sheet texture
	if( !gSpriteSheetTexture.loadFromFile( "assets/sceneSprite.png" ) )
	{
		printf( "Failed to load sprite sheet texture!\n" );
		success = false;
	}
	return success;
}

void sceneSprite::generateClips()
{
	renderedFrame.x = 0;
	renderedFrame.y = 0;
	renderedFrame.w = sceneWidth;
	renderedFrame.h = sceneHeight;
}

//----Renderer----//
void sceneSprite::render(int offsetX, int offsetY)
{
	gSpriteSheetTexture.render(0 - offsetX, 0 - offsetY,&renderedFrame);
}
