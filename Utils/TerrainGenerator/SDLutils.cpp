#include "SDLutils.h"

#include <iostream>
#include "ClimateGenerator.h"

const float GROUND_CAPACITY = 2000;


SDLutils::SDLutils(int32_t MAP_WIDTH, int32_t MAP_HEIGHT, int32_t multiplier) : mMultiplier(multiplier)
{
	//Initialize SDL 
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) 
	{
		std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl; 
		return;
	} 
	
	if(SDL_CreateWindowAndRenderer(MAP_WIDTH*mMultiplier, MAP_HEIGHT*mMultiplier, SDL_WINDOW_SHOWN, &mWindow, &mRenderer) != 0)
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
	
	mScreenBeginX = 0;
	mScreenBeginY = 0;
	
	mMapWidth = MAP_WIDTH;
	mMapHeight = MAP_HEIGHT;
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
	
	int32_t screenEndX = mScreenBeginX + (mScreenSurface->w/mMultiplier);
	int32_t screenEndY = mScreenBeginY + (mScreenSurface->h/mMultiplier);
	
	screenEndX = (screenEndX > mapSizeX) ? mapSizeX : screenEndX;
	screenEndY = (screenEndY > mapSizeY) ? mapSizeY : screenEndY;
	
	for (int32_t y = mScreenBeginY; y < screenEndY; ++y)
	{
		for (int32_t x = mScreenBeginX; x < screenEndX; ++x)
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
			rect.x = (x-mScreenBeginX)*mMultiplier;
			rect.y = (y-mScreenBeginY)*mMultiplier;
			//SDL_FillRect(mScreenSurface, &rect, actual);
			SDL_SetRenderDrawColor(mRenderer, r, g, b, SDL_ALPHA_OPAQUE);
			SDL_RenderFillRect(mRenderer,&rect);
		}
	}

	//Update the surface 
	SDL_RenderPresent(mRenderer);
	//SDL_UpdateWindowSurface( mWindow ); 
	
}

void SDLutils::writeMove(const Urho3D::Vector2 &streamDir, int32_t xPos, int32_t yPos, int32_t flags)
{
	if(mMultiplier <= 2) return;
	
	float speed = 0.0f;
	
	float angle = 0.0f;
	
	speed = streamDir.Length();
	angle = streamDir.Angle(Urho3D::Vector2(0.0, -1.0));
	
	SDL_SetRenderDrawColor(mRenderer, restrict((int32_t)((speed/200.0)*255.0)), restrict((int32_t)(-(speed/200.0)*255.0) + 200), 0, SDL_ALPHA_OPAQUE);
	
	int32_t xBegin = xPos + (mMultiplier / 2), yBegin = yPos + (mMultiplier / 2);
	if(speed <= 0.0)
	{
		SDL_RenderDrawPoint(mRenderer, xBegin, yBegin);
		return;
	}
	
	int32_t weMult = (streamDir.x_ >= 0) ? 1 : -1;
	
	// end line
	int32_t xEnd, yEnd;
	if(angle < 22.5)
	{
		xEnd = xBegin;
		yEnd = yBegin - (mMultiplier / 2);
	} else if (angle < 67.5) {
		xEnd = xBegin + weMult*(mMultiplier / 2);
		yEnd = yBegin - (mMultiplier / 2);
	} else if (angle < 112.5) {
		xEnd = xBegin + weMult*(mMultiplier / 2);
		yEnd = yBegin;			
	} else if (angle < 157.5) {
		xEnd = xBegin + weMult*(mMultiplier / 2);
		yEnd = yBegin + (mMultiplier / 2);
	} else {
		xEnd = xBegin;
		yEnd = yBegin + (mMultiplier / 2);
	}
		
	SDL_RenderDrawLine(mRenderer, xBegin, yBegin, xEnd , yEnd);
}

void SDLutils::writeMove(const CellContent* cont, int32_t xPos, int32_t yPos, int32_t flags)
{
	if(cont == nullptr) return;
	switch(cont->GetContentType())
	{
		case CellContent::AIR:
			{
				const AirContent *pAir = reinterpret_cast<const AirContent*>(cont);
				
				if((flags & MOVE_HIGH) != 0)
				{
				//	if((flags & MOVE_FORCE) != 0)				{
				//		writeMove(pAir->m_highForce, xPos, yPos, flags);
				//	} else {
						writeMove(pAir->m_highWind, xPos, yPos, flags);
				//	}
				//	
				} else {
					//if((flags & MOVE_FORCE) != 0)				{
						//writeMove(pAir->m_lowForce, xPos, yPos, flags);
					//} else {
						writeMove(pAir->m_lowWind, xPos, yPos, flags);
					//}
				}
			}
			break;
		case CellContent::WATER:
			{
				//const WaterContent *pWater = reinterpret_cast<const WaterContent*>(cont);
				//writeMove(pWater->m_streamDir, xPos, yPos, flags);
			}
			break;
		default:
			return;
	}
}

void SDLutils::writeType(const CellContent* cont, int32_t xPos, int32_t yPos, int32_t flags)
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
				const WaterContent* pWater = reinterpret_cast<const WaterContent*>(cont);
				float ice = pWater->m_iceMass / (1*0.2*2000);
				ice = (ice > 1) ? 1 : ice;
				b = 200;
				g = r = restrict((int32_t)(200.0f * ice));
			}
			break;
			case CellContent::GROUND:
			{
				//const GroundContent* pGround = reinterpret_cast<const GroundContent*>(cont);
				
				//b = restrict((int32_t)((pGround->m_waterMass / pGround->m_maxWaterCapacity) * 255.0f));
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

void SDLutils::writeTemperature(const CellContent* cont, int32_t xPos, int32_t yPos, int32_t flags)
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
				const WaterContent* pWater = reinterpret_cast<const WaterContent*>(cont);
				temp = pWater->m_temperature - 273;
			}
			break;
			case CellContent::GROUND:
			{
				const GroundContent* pGround = reinterpret_cast<const GroundContent*>(cont);
				temp = pGround->m_temperature - 273;
			}
			break;
			case CellContent::AIR:
			{
				const AirContent* pAir = reinterpret_cast<const AirContent*>(cont);
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

void SDLutils::writeClouds(const AirContent* cont, int32_t xPos, int32_t yPos, int32_t flags)
{
	int32_t r = 240,g = 240,b = 240, alpha = 0;
	SDL_Rect rect = {xPos,yPos,mMultiplier,mMultiplier};
	if(cont != nullptr)
	{
		
		float height = cont->m_cloudsHeight;
		
		// clouds will be visible from 5500 m
		alpha = static_cast<int32_t>(-0.046*height + 255.0);
		
		if(alpha <= 0) return; // no clouds
		
		alpha = restrict(alpha);
				
		SDL_SetRenderDrawColor(mRenderer, r, g, b, alpha);
		SDL_RenderFillRect(mRenderer,&rect);
		
	}

}

void SDLutils::writeMoisture(const ClimateCell& cont, int32_t xPos, int32_t yPos, int32_t flags)
{
	int32_t r = 0,g = 0,b = 240, alpha = 0;
	
	if(cont.GetAirContent().m_temperature < 273)
	{
		r = 200;
		g = 200;
	}
	
	const GroundContent &ground = cont.GetGroundContent();
	
	SDL_Rect rect = {xPos,yPos,mMultiplier,mMultiplier};
	
	// clouds will be visible from 5500 m
	alpha = (ground.m_waterMass/GROUND_CAPACITY) * 255.0;
		
	if(alpha <= 0) return; // no clouds
		
	alpha = restrict(alpha);
				
	SDL_SetRenderDrawColor(mRenderer, r, g, b, alpha);
	SDL_RenderFillRect(mRenderer,&rect);
		
}

void SDLutils::writeClimateMap(const ClimateCell* map, int32_t mapSizeX, int32_t mapSizeY, int32_t level, int32_t flags)
{
	
	//int32_t moveFlags = 0;
	
	int32_t screenEndX = mScreenBeginX + (mScreenSurface->w/mMultiplier);
	int32_t screenEndY = mScreenBeginY + (mScreenSurface->h/mMultiplier);
	
	screenEndX = (screenEndX > mapSizeX) ? mapSizeX : screenEndX;
	screenEndY = (screenEndY > mapSizeY) ? mapSizeY : screenEndY;
	
	int32_t scrPosX, scrPosY;
	
	for (int32_t y = mScreenBeginY; y < screenEndY; ++y)
	{
		for (int32_t x = mScreenBeginX; x < screenEndX; ++x)
		{
			const ClimateCell& climate = map[y*mapSizeX + x];
			
			const CellContent* cont = climate.GetContent(level);
			
			scrPosX = (x - mScreenBeginX)*mMultiplier;
			scrPosY = (y - mScreenBeginY)*mMultiplier;
			
			if((flags & TEMPERATURE) != 0)
			{
				writeTemperature(cont, scrPosX, scrPosY);
			} else if((flags & PRESSURE) != 0) {
				writePressure(cont, scrPosX, scrPosY);
			} else if((flags & VOLUME) != 0) {
				writeVolume(cont, scrPosX, scrPosY);
			} else {
				writeType(cont, scrPosX, scrPosY);
			}
			
			// write clouds
			//if((flags & CLOUDS)&&(cont != nullptr)&&(cont->GetContentType() == CellContent::AIR))
			//{
			//	writeClouds(reinterpret_cast<AirContent*>(cont), scrPosX, scrPosY);
			//}
			
			//if((flags & MOVE) != 0)
			//{
			//	writeMove(cont, scrPosX, scrPosY, moveFlags);
			//}
		}
	}
	
	//Update the surface 
	SDL_RenderPresent(mRenderer);

	//SDL_UpdateWindowSurface( mWindow ); 
}

void SDLutils::writeMove(const ClimateCell* map, int32_t mapSizeX, int32_t mapSizeY, int32_t flags)
{
	int32_t screenEndX = mScreenBeginX + (mScreenSurface->w/mMultiplier);
	int32_t screenEndY = mScreenBeginY + (mScreenSurface->h/mMultiplier);
	
	screenEndX = (screenEndX > mapSizeX) ? mapSizeX : screenEndX;
	screenEndY = (screenEndY > mapSizeY) ? mapSizeY : screenEndY;
	
	int32_t scrPosX, scrPosY;
	
	int32_t climateLevel = 0;
	
	if((flags & MOVE_WATER) != 0)
	{
		climateLevel = 1;
	}
	
	for (int32_t y = mScreenBeginY; y < screenEndY; ++y)
	{
		for (int32_t x = mScreenBeginX; x < screenEndX; ++x)
		{
			const ClimateCell& climate = map[y*mapSizeX + x];
			
			const CellContent* cont = climate.GetContent(climateLevel);
			
			scrPosX = (x - mScreenBeginX)*mMultiplier;
			scrPosY = (y - mScreenBeginY)*mMultiplier;
			
			
			
			writeMove(cont, scrPosX, scrPosY, flags);
		}
	}
	
	//Update the surface 
	SDL_RenderPresent(mRenderer);

	//SDL_UpdateWindowSurface( mWindow ); 
}

void SDLutils::writeClouds(const ClimateCell* map, int32_t mapSizeX, int32_t mapSizeY)
{
	int32_t screenEndX = mScreenBeginX + (mScreenSurface->w/mMultiplier);
	int32_t screenEndY = mScreenBeginY + (mScreenSurface->h/mMultiplier);
	
	screenEndX = (screenEndX > mapSizeX) ? mapSizeX : screenEndX;
	screenEndY = (screenEndY > mapSizeY) ? mapSizeY : screenEndY;
	
	int32_t scrPosX, scrPosY;
	
	for (int32_t y = mScreenBeginY; y < screenEndY; ++y)
	{
		for (int32_t x = mScreenBeginX; x < screenEndX; ++x)
		{
			const ClimateCell& climate = map[y*mapSizeX + x];
			
			const AirContent* cont = &climate.GetAirContent();
			
			scrPosX = (x - mScreenBeginX)*mMultiplier;
			scrPosY = (y - mScreenBeginY)*mMultiplier;
			
			// write clouds
			writeClouds(cont, scrPosX, scrPosY);
		}
	}
	
	//Update the surface 
	SDL_RenderPresent(mRenderer);
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
					case SDLK_e: 
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
					case SDLK_o: 
						return ACTION::CLIMATE_MOISTURE;
					case SDLK_t:
						return ACTION::CLIMATE_TEMPERATURE;
					case SDLK_i:
						return ACTION::CLIMATE_MOVE_HIGH;
					case SDLK_m:
						return ACTION::CLIMATE_MOVE;
					case SDLK_c:
						return ACTION::CLIMATE_CLOUDS;
					case SDLK_p:
						return ACTION::CLIMATE_PRESSURE;
					case SDLK_v:
						return ACTION::CLIMATE_VOLUME;
					case SDLK_u:
						return ACTION::CLIMATE_SUN_POSITION;
					case SDLK_s:
						return ACTION::CLIMATE_STEP;
					case SDLK_f:
						return ACTION::CLIMATE_FAST_STEP;
					case SDLK_UP:
						return ACTION::UP;
					case SDLK_DOWN:
						return ACTION::DOWN;
					case SDLK_LEFT:
						return ACTION::LEFT;
					case SDLK_RIGHT:
						return ACTION::RIGHT;
					case SDLK_F5:
						return ACTION::STORE_MAPS;
					case SDLK_F6:
						return ACTION::LOAD_MAPS;
					default:
						break;
				}
			}
			
			if ( e.type == SDL_MOUSEBUTTONDOWN )
			{
				mLastX = e.button.x;
				mLastY = e.button.y;
				return ACTION::MOUSE_CLICK;
			}
			
			if ( e.type == SDL_MOUSEWHEEL )
			{
				if(e.wheel.y > 0)
				{
					return ACTION::WHEEL_UP;
				} else {
					return ACTION::WHEEL_DOWN;
				}
			}
		}
	}
}

void SDLutils::writePressure(const CellContent* cont, int32_t xPos, int32_t yPos, int32_t flags)
{
	int32_t r = 0,g = 0,b = 0;
	SDL_Rect rect = {xPos,yPos,mMultiplier,mMultiplier};
	
	bool undef = false;
	float pressure = 0;
	
	if(cont != nullptr)
	{
		switch(cont->GetContentType())
		{

			case CellContent::AIR:
			{
				const AirContent* pAir = reinterpret_cast<const AirContent*>(cont);
				pressure = pAir->m_airPressure;
			}
			break;
			case CellContent::WATER:
			case CellContent::GROUND:
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
		b = restrict((int32_t)(-(49.0/2900.0) * pressure) + 1725.0);
		g = restrict(parabola((int32_t)pressure, -1000, 101500, 130));
		r = restrict((int32_t)((49.0/1500.0) * pressure) - 3305.0);
	}

	SDL_SetRenderDrawColor(mRenderer, r, g, b, SDL_ALPHA_OPAQUE);
	SDL_RenderFillRect(mRenderer,&rect);
}

void SDLutils::updateMultiplier(int32_t update)
{
	mMultiplier += update*2;
	//min
	mMultiplier = (mMultiplier < 1) ? 1 : mMultiplier;
	
	//max
	mMultiplier = (mMultiplier > 17) ? 17 : mMultiplier;
}

void SDLutils::updateScreenBegin(int32_t xDiff, int32_t yDiff)
{
	int32_t sizeX = mScreenSurface->w / mMultiplier;
	int32_t sizeY = mScreenSurface->h / mMultiplier;
	
	mScreenBeginX += xDiff;
	mScreenBeginY += yDiff;
	
	//min
	mScreenBeginX = (mScreenBeginX < 0) ? 0 : mScreenBeginX;	
	mScreenBeginY = (mScreenBeginY < 0) ? 0 : mScreenBeginY;	
	
	//max
	mScreenBeginX = (mScreenBeginX > (mMapWidth - sizeX)) ? (mMapWidth - sizeX) : mScreenBeginX;	
	mScreenBeginY = (mScreenBeginY > (mMapHeight - sizeY)) ? (mMapHeight - sizeY) : mScreenBeginY;	
}

void SDLutils::writeSun(int32_t x, int32_t y)
{
	int32_t sizeX = mScreenSurface->w / mMultiplier;
	int32_t sizeY = mScreenSurface->h / mMultiplier;
	
	if((x >= mScreenBeginX) && (x < (mScreenBeginX + sizeX)) 
		&& (y >= mScreenBeginY) && (y < (mScreenBeginY + sizeY)))
	{
		x = (x - mScreenBeginX)*mMultiplier;
		y = (y - mScreenBeginY)*mMultiplier;
		
		int32_t r = 240,g = 240,b = 0, alpha = 122;

		SDL_Rect rect = {x,y,mMultiplier,mMultiplier};
			
		SDL_SetRenderDrawColor(mRenderer, r, g, b, alpha);
		SDL_RenderFillRect(mRenderer,&rect);
		
		SDL_RenderPresent(mRenderer);
			
	}
}

void SDLutils::writeVolume(const CellContent* cont, int32_t xPos, int32_t yPos, int32_t flags)
{
	int32_t r = 0,g = 0,b = 0;
	SDL_Rect rect = {xPos,yPos,mMultiplier,mMultiplier};
	
	bool undef = false;
	float volume = 0;
	
	if(cont != nullptr)
	{
		switch(cont->GetContentType())
		{

			case CellContent::AIR:
			{
				const AirContent* pAir = reinterpret_cast<const AirContent*>(cont);
				volume = pAir->m_volumeLevel;
			}
			break;
			case CellContent::WATER:
			//{
			//	const WaterContent* pWater = reinterpret_cast<const WaterContent*>(cont);
			//	volume = pWater->m_surfaceLevel;
			//}
			//break;
			case CellContent::GROUND:
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
		b = restrict((int32_t)(-(255.0) * volume) + 510.0);
		g = restrict(parabola((int32_t)volume, -1000, 0, 130));
		r = restrict((int32_t)((255.0) * volume) - 510.0);
	}

	SDL_SetRenderDrawColor(mRenderer, r, g, b, SDL_ALPHA_OPAQUE);
	SDL_RenderFillRect(mRenderer,&rect);
}

void SDLutils::writeMoisture(const ClimateCell* map, int32_t mapSizeX, int32_t mapSizeY)
{
	int32_t screenEndX = mScreenBeginX + (mScreenSurface->w/mMultiplier);
	int32_t screenEndY = mScreenBeginY + (mScreenSurface->h/mMultiplier);
	
	screenEndX = (screenEndX > mapSizeX) ? mapSizeX : screenEndX;
	screenEndY = (screenEndY > mapSizeY) ? mapSizeY : screenEndY;
	
	int32_t scrPosX, scrPosY;
	
	for (int32_t y = mScreenBeginY; y < screenEndY; ++y)
	{
		for (int32_t x = mScreenBeginX; x < screenEndX; ++x)
		{
			const ClimateCell& climate = map[y*mapSizeX + x];
			
			
			if(climate.IsCheckContent(CellContent::GROUND))
			{
				scrPosX = (x - mScreenBeginX)*mMultiplier;
				scrPosY = (y - mScreenBeginY)*mMultiplier;
				
				// write clouds
				writeMoisture(climate, scrPosX, scrPosY, 0);
			
			}
		}
	}
	
	//Update the surface 
	SDL_RenderPresent(mRenderer);
	
	
}





