#include "SDLutils.h"

#include <iostream>

static const int32_t MULTIPLIER = 1;

SDLutils::SDLutils(int32_t SCREEN_WIDTH, int32_t SCREEN_HEIGHT)
{
	//Initialize SDL 
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) 
	{
		std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl; 
		return;
	} 
	
	mWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH*MULTIPLIER, SCREEN_HEIGHT*MULTIPLIER, SDL_WINDOW_SHOWN );
	if( mWindow == nullptr ) 
	{
		std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl; 
		return;
	}
	
	mScreenSurface = SDL_GetWindowSurface( mWindow );
}

SDLutils::~SDLutils()
{
	if(mWindow)
	{
		SDL_DestroyWindow(mWindow);
		mWindow = nullptr;
	}
	
	SDL_Quit();
}

static int32_t restrict (int32_t color)
{
	return (color > 255) ? 255 : ((color < 0) ? 0 : color);
}

static int32_t exp2(int32_t in)
{
	return in*in;
}

static int32_t parabola(int32_t x, int32_t p, int32_t vx, int32_t vy)
{
	return (exp2(x-vx) / (2*p)) + vy;
}

void SDLutils::writeMap(const int8_t* map, int32_t mapSizeX, int32_t mapSizeY, int32_t waterLevel)
{
	
	uint32_t actual = 0;
	//87CEFA
	
	uint32_t water[5], land[10];
	water [0] = SDL_MapRGB( mScreenSurface->format, 0x87,0xCE,0xFA );
	water [1] = SDL_MapRGB( mScreenSurface->format, 0x1E,0x90,0xFF );
	water [2] = SDL_MapRGB( mScreenSurface->format, 0x41,0x69,0xE1 );
	water [3] = SDL_MapRGB( mScreenSurface->format, 0x00,0x00,0xCD );
	water [4] = SDL_MapRGB( mScreenSurface->format, 0x00,0x00,0x80 );
	
	land[0] = SDL_MapRGB( mScreenSurface->format, 0x22,0x8B,0x22);
	land[1] = SDL_MapRGB( mScreenSurface->format, 0x32,0xCD,0x32);
	land[2] = SDL_MapRGB( mScreenSurface->format, 0x7C,0xFC,0x00);
	land[3] = SDL_MapRGB( mScreenSurface->format, 0xAD,0xFF,0x2F);
	land[4] = SDL_MapRGB( mScreenSurface->format, 0xFF,0xFF,0x66);
	land[5] = SDL_MapRGB( mScreenSurface->format, 0xFF,0xFF,0x00);
	land[6] = SDL_MapRGB( mScreenSurface->format, 0xDA,0xA5,0x20);
	land[7] = SDL_MapRGB( mScreenSurface->format, 0xD2,0x69,0x1E);
	land[8] = SDL_MapRGB( mScreenSurface->format, 0x8B,0x45,0x13);
	land[9] = SDL_MapRGB( mScreenSurface->format, 0x80,0x00,0x00);


	const bool predefEnabled = false;
	int32_t r = 0,g = 0,b = 0;
	int32_t index = 0;
	SDL_Rect rect = {0,0,MULTIPLIER,MULTIPLIER};
	for (int32_t y = 0; y < mapSizeY; ++y)
	{
		for (int32_t x = 0; x < mapSizeX; ++x)
		{
			auto height = map[y*mapSizeX + x];
			if(height > waterLevel)
			{
				index = height - waterLevel - 1;
				
				if ((index < 10)&&predefEnabled)
				{
					actual = land[index];
				} else {
				// set color
				//r =  ((0x80 - index*3) > 0) ? (0x80 - index*3) : 0;
					r = parabola(index * 5, -10, 60, 225);
					r = restrict(r);
					g = parabola(index * 5, -10, 15, 200); 
					g = restrict(g);
					b = - index + 0x22;
					b = restrict(b);
					actual = SDL_MapRGB(mScreenSurface->format, r, g, b);
				}
				
				
			} else {
				index = waterLevel - height;
				if((index < 5)&&predefEnabled)
				{
					actual = water[index];
				} else {
					r = 0x87 - index*10;
					r = restrict(r);
					g = 0xCE - index*8;
					g = restrict(g);
					b = 256 - index*6;
					b = restrict(b);
					actual = SDL_MapRGB(mScreenSurface->format, r, g, b);
				}
			}
			rect.x = x*MULTIPLIER;
			rect.y = y*MULTIPLIER;
			SDL_FillRect(mScreenSurface, &rect, actual);
		}
	}
	
	//Update the surface 
	SDL_UpdateWindowSurface( mWindow ); 
	
}

SDLutils::ACTION SDLutils::waitForAction()
{
	//Event handler 
	SDL_Event e;
	
	while( true ) 
	{
		while( SDL_PollEvent( &e ) != 0 ) 
		{ 
			//User requests quit 
			if( e.type == SDL_QUIT ) 
			{ 
				return ACTION::QUIT;
			} 
			
			//User presses a key else 
			if( e.type == SDL_KEYDOWN ) 
			{ 
				//Select surfaces based on key press 
				switch( e.key.keysym.sym ) 
				{ 
					case SDLK_q: 
						return ACTION::QUIT; 
					case SDLK_r: 
						return ACTION::RELOAD;
					case SDLK_n: 
						return ACTION::NEXT;
					default:
						break;
				}
			}
		}
	}
}
