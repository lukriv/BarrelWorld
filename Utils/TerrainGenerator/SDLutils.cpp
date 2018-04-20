#include "SDLutils.h"

#include <iostream>
#include "ClimateGenerator.h"

SDLutils::SDLutils(int32_t SCREEN_WIDTH, int32_t SCREEN_HEIGHT, int32_t multiplier) : mMultiplier(multiplier)
{
	//Initialize SDL 
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) 
	{
		std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl; 
		return;
	} 
	
	if(SDL_CreateWindowAndRenderer(SCREEN_WIDTH*mMultiplier, SCREEN_HEIGHT*mMultiplier, SDL_WINDOW_SHOWN, &mWindow, &mRenderer) != 0)
	{
		std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl; 
		return;
	}
	
	SDL_SetRenderDrawBlendMode(mRenderer, SDL_BLENDMODE_BLEND);
	
	
	//mWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH*mMultiplier, SCREEN_HEIGHT*mMultiplier, SDL_WINDOW_SHOWN );
	//if( mWindow == nullptr ) 
	//{
	//	std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl; 
	//	return;
	//}
	
	mScreenSurface = SDL_GetWindowSurface( mWindow );
}

SDLutils::~SDLutils()
{
	if(mRenderer)
	{
		SDL_DestroyRenderer(mRenderer);
		mRenderer = nullptr;
	}
	
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

void SDLutils::writeMap(const uint8_t* map, int32_t mapSizeX, int32_t mapSizeY, int32_t waterLevel)
{
	
	const int32_t groundMul = 2;
	int32_t r = 0,g = 0,b = 0;
	int32_t index = 0;
	SDL_Rect rect = {0,0,mMultiplier,mMultiplier};
	for (int32_t y = 0; y < mapSizeY; ++y)
	{
		for (int32_t x = 0; x < mapSizeX; ++x)
		{
			auto height = map[y*mapSizeX + x];
			if(height > waterLevel)
			{
				index = height - waterLevel - 1;
				
				{
				// set color
				//r =  ((0x80 - index*3) > 0) ? (0x80 - index*3) : 0;
					r = parabola(index * groundMul, -10, 60, 225);
					r = restrict(r);
					g = parabola(index * groundMul, -10, 15, 200); 
					g = restrict(g);
					b = - index + 0x22;
					b = restrict(b);
				}
				
				
			} else {
				index = waterLevel - height;
				{
					r = 0x87 - index*10;
					r = restrict(r);
					g = 0xCE - index*8;
					g = restrict(g);
					b = 256 - index*6;
					b = restrict(b);
				}
			}
			rect.x = x*mMultiplier;
			rect.y = y*mMultiplier;
			//SDL_FillRect(mScreenSurface, &rect, actual);
			SDL_SetRenderDrawColor(mRenderer, r, g, b, SDL_ALPHA_OPAQUE);
			SDL_RenderFillRect(mRenderer,&rect);
		}
	}

	//Update the surface 
	SDL_RenderPresent(mRenderer);
	//SDL_UpdateWindowSurface( mWindow ); 
	
}

void SDLutils::writeMove(Urho3D::Vector2 &streamDir, int32_t xPos, int32_t yPos, int32_t flags)
{
	if(mMultiplier <= 2) return;
	
	float speed = 0.0f;
	
	float angle = 0.0f;
	
	speed = streamDir.Length();
	angle = streamDir.Angle(Urho3D::Vector2(0.0, 1.0));
	
	SDL_SetRenderDrawColor(mRenderer, restrict((int32_t)((speed/200.0)*255.0)), restrict((int32_t)(-(speed/200.0)*255.0) + 200), 0, SDL_ALPHA_OPAQUE);
	
	int32_t xBegin = xPos + (mMultiplier / 2), yBegin = yPos + (mMultiplier / 2);
	if(speed <= 0.0)
	{
		SDL_RenderDrawPoint(mRenderer, xBegin, yBegin);
		return;
	}
	
	// end line
	int32_t xEnd, yEnd;
	if(angle > 0)
	{
		if(angle < 22.5)
		{
			xEnd = xBegin;
			yEnd = yBegin - (mMultiplier / 2);
		} else if (angle < 67.5) {
			xEnd = xBegin + (mMultiplier / 2);
			yEnd = yBegin - (mMultiplier / 2);
		} else if (angle < 112.5) {
			xEnd = xBegin + (mMultiplier / 2);
			yEnd = yBegin;			
		} else if (angle < 157.5) {
			xEnd = xBegin + (mMultiplier / 2);
			yEnd = yBegin + (mMultiplier / 2);
		} else {
			xEnd = xBegin;
			yEnd = yBegin + (mMultiplier / 2);
		}
	} else {
		if(angle > -22.5)
		{
			xEnd = xBegin;
			yEnd = yBegin - (mMultiplier / 2);
		} else if (angle > -67.5) {
			xEnd = xBegin - (mMultiplier / 2);
			yEnd = yBegin - (mMultiplier / 2);
		} else if (angle > 112.5) {
			xEnd = xBegin - (mMultiplier / 2);
			yEnd = yBegin;			
		} else if (angle > 157.5) {
			xEnd = xBegin - (mMultiplier / 2);
			yEnd = yBegin + (mMultiplier / 2);
		} else {
			xEnd = xBegin;
			yEnd = yBegin + (mMultiplier / 2);
		}
	}
	
	SDL_RenderDrawLine(mRenderer, xBegin, yBegin, xEnd , yEnd);
}

void SDLutils::writeMove(CellContent* cont, int32_t xPos, int32_t yPos, int32_t flags)
{
	if(cont == nullptr) return;
	switch(cont->GetContentType())
	{
		case CellContent::AIR:
			{
				AirContent *pAir = reinterpret_cast<AirContent*>(cont);
				if((flags & HIGH_AIR) != 0)				{
					writeMove(pAir->m_highDir, xPos, yPos, flags);
				} else {
					writeMove(pAir->m_lowDir, xPos, yPos, flags);
				}
			}
			break;
		case CellContent::WATER:
			{
				WaterContent *pWater = reinterpret_cast<WaterContent*>(cont);
				writeMove(pWater->m_streamDir, xPos, yPos, flags);
			}
			break;
		default:
			return;
	}
}

void SDLutils::writeType(CellContent* cont, int32_t xPos, int32_t yPos, int32_t flags)
{
	int32_t r = 0,g = 0,b = 0;
	SDL_Rect rect = {xPos,yPos,mMultiplier,mMultiplier};
	if(cont == nullptr)
	{
		r = 0,g = 0,b = 0;
	} else {
		switch(cont->GetContentType())
		{
			case CellContent::WATER:
			{
				WaterContent* pWater = reinterpret_cast<WaterContent*>(cont);
				float ice = pWater->m_iceMass / (pWater->m_iceMass + pWater->m_waterMass);
				b = 200;
				g = r = (int32_t)(200.0f * ice);
			}
			break;
			case CellContent::GROUND:
			{
				GroundContent* pGround = reinterpret_cast<GroundContent*>(cont);
				
				b = (int32_t)(pGround->m_waterMass / pGround->m_maxWaterCapacity * 255.0f);
				g = r = 150;
			}
			break;
			case CellContent::AIR:
			{
				//AirContent* pAir = reinterpret_cast<AirContent*>(cont);
				r = 0;
				g = 141;
				b = 205;
			}
			break;
			default:
				r = 0,g = 0,b = 0;
			break;
		}
	}

	SDL_SetRenderDrawColor(mRenderer, r, g, b, SDL_ALPHA_OPAQUE);
	SDL_RenderFillRect(mRenderer,&rect);
}

void SDLutils::writeTemperature(CellContent* cont, int32_t xPos, int32_t yPos, int32_t flags)
{
	int32_t r = 0,g = 0,b = 0;
	SDL_Rect rect = {xPos,yPos,mMultiplier,mMultiplier};
	
	bool undef = false;
	float temp = CLIMATE_MIN_TEMPERATURE;
	
	if(cont != nullptr)
	{
		switch(cont->GetContentType())
		{
			case CellContent::WATER:
			{
				WaterContent* pWater = reinterpret_cast<WaterContent*>(cont);
				temp = pWater->m_temperature - 273;
			}
			break;
			case CellContent::GROUND:
			{
				GroundContent* pGround = reinterpret_cast<GroundContent*>(cont);
				temp = pGround->m_temperature - 273;
			}
			break;
			case CellContent::AIR:
			{
				AirContent* pAir = reinterpret_cast<AirContent*>(cont);
				temp = pAir->m_temperature - 273;
			}
			break;
			default:
				undef = true;
			break;
		}
	} else {
		undef = true;
	}
	
	if(undef)
	{
		SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	} else {
		b = restrict((int32_t)(-(255.0/50.0) * temp));
		g = restrict(parabola((int32_t)temp, -10, 0, 150));
		r = restrict((int32_t)((255.0/50.0) * temp));
	}

	SDL_SetRenderDrawColor(mRenderer, r, g, b, SDL_ALPHA_OPAQUE);
	SDL_RenderFillRect(mRenderer,&rect);
}

void SDLutils::writeClouds(AirContent* cont, int32_t xPos, int32_t yPos, int32_t flags)
{
	int32_t r = 240,g = 240,b = 240, alpha = 0;
	SDL_Rect rect = {xPos,yPos,mMultiplier,mMultiplier};
	if(cont != nullptr)
	{
		
		float height = ClimateGenerator::CompCloudsHeight(*cont);
		
		alpha = static_cast<int32_t>((-0.0001*height + 1.1)*255.0);
		
		if(alpha <= 0) return; // no clouds
		
		alpha = restrict(alpha);
				
		SDL_SetRenderDrawColor(mRenderer, r, g, b, alpha);
		SDL_RenderFillRect(mRenderer,&rect);
		
	}

}

void SDLutils::writeClimateMap(const ClimateCell* map, int32_t mapSizeX, int32_t mapSizeY, int32_t level, int32_t flags)
{
	
	int32_t climateLevel;
	int32_t moveFlags = 0;
	switch(level)
	{
		case 0:
			climateLevel = 0;
			moveFlags = HIGH_AIR;
			break;
		case 1:
		case 2:
		case 3:
			climateLevel = level - 1;
			break;
		default:
			return;
	}
	
	
	
	for (int32_t y = 0; y < mapSizeY; ++y)
	{
		for (int32_t x = 0; x < mapSizeX; ++x)
		{
			const ClimateCell& climate = map[y*mapSizeX + x];
			
			CellContent* cont = climate.GetContent(climateLevel);

			
			if((flags & TEMPERATURE) != 0)
			{
				writeTemperature(cont, x*mMultiplier, y*mMultiplier);
			} else {
				writeType(cont, x*mMultiplier, y*mMultiplier);
			}
			
			// write clouds
			if((flags & CLOUDS)&&(cont != nullptr)&&(cont->GetContentType() == CellContent::AIR))
			{
				writeClouds(reinterpret_cast<AirContent*>(cont), x*mMultiplier, y*mMultiplier);
			}
			
			if((flags & MOVE) != 0)
			{
				writeMove(cont, x*mMultiplier, y*mMultiplier, moveFlags);
			}
		}
	}
	
	//Update the surface 
	SDL_RenderPresent(mRenderer);

	//SDL_UpdateWindowSurface( mWindow ); 
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
					case SDLK_0:
						return ACTION::HEIGHT_MAP;
					case SDLK_1:
						return ACTION::CLIMATE_LEVEL_1;
					case SDLK_2:
						return ACTION::CLIMATE_LEVEL_2;
					case SDLK_3:
						return ACTION::CLIMATE_LEVEL_3;
					case SDLK_4:
						return ACTION::CLIMATE_LEVEL_4;
					case SDLK_t:
						return ACTION::CLIMATE_TEMPERATURE;
					case SDLK_m:
						return ACTION::CLIMATE_MOVE;
					case SDLK_c:
						return ACTION::CLIMATE_CLOUDS;
					case SDLK_s:
						return ACTION::CLIMATE_STEP;
					case SDLK_f:
						return ACTION::CLIMATE_FAST_STEP;
					default:
						break;
				}
			}
		}
	}
}



