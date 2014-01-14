#include "engine.h"

#include <iostream>
#include <windows.h>

using namespace std;

static const wchar_t* TOP_LEFT_CORNER = L"\u2554";
static const wchar_t* TOP_RIGHT_CORNER = L"\u2557";
static const wchar_t* BOTTOM_LEFT_CORNER = L"\u255A";
static const wchar_t* BOTTOM_RIGHT_CORNER = L"\u255D";
static const wchar_t* HORIZONTAL_DOUBLE_LINE = L"\u2550";
static const wchar_t* VERTICAL_DOUBLE_LINE = L"\u2551";

bool ConsoleGameEngine::WriteMargin(ConsoleWindowWrapper &console, const ConsoleCoord &leftTopPos, const ConsoleCoord &insizeSize)
{
	short unsigned int right = 0, bottom = 0;
	
	right = leftTopPos.x + insizeSize.x + 1;
	bottom = leftTopPos.y + insizeSize.y + 1;
	
	if(!console.WriteChar(*TOP_LEFT_CORNER, leftTopPos)) return false;
	if(!console.WriteChar(*BOTTOM_LEFT_CORNER, leftTopPos.x, bottom)) return false;
	if(!console.WriteChar(*TOP_RIGHT_CORNER, right, leftTopPos.y)) return false;
	if(!console.WriteChar(*BOTTOM_RIGHT_CORNER, right, bottom)) return false;
	
	short unsigned int colors = CONSOLE_FOREGROUND_GREEN | CONSOLE_FOREGROUND_INTENSITY;
	
	for(short unsigned int i = (leftTopPos.x + 1); i < right; i++)
	{
		if(!console.WriteChar(*HORIZONTAL_DOUBLE_LINE, ConsoleCoordX(i, leftTopPos.y), colors)) return false;
		if(!console.WriteChar(*HORIZONTAL_DOUBLE_LINE, i, bottom)) return false;
	}
	
	for(short unsigned int i = (leftTopPos.y + 1); i < bottom; i++)
	{
		if(!console.WriteChar(*VERTICAL_DOUBLE_LINE, leftTopPos.x, i)) return false;
		if(!console.WriteChar(*VERTICAL_DOUBLE_LINE, right, i)) return false;
	}
	
	return true;	
}


bool ConsoleGameEngine::Play(ConsoleWindowWrapper &console)
{
	bool ends = false;
	short int moveX, moveY;
		
	unsigned int keystate = 0;
	
	ConsoleCoord marginOrigin = {0,0}, playGroundSize = {30, 30};
	
	m_viewFieldMin.x = marginOrigin.x + 1;
	m_viewFieldMin.y = marginOrigin.y + 1;
	
	m_viewFieldMax.x = marginOrigin.x + playGroundSize.x;
	m_viewFieldMax.y = marginOrigin.y + playGroundSize.y;
	
	m_avatar.pos.x = m_viewFieldMin.x;
	m_avatar.pos.y = m_viewFieldMin.y;
	
	if(!console.EnableCursor(false))
		return false;
	if(!console.EnableManualProcessEvent(true))
		return true;
	
	if(!WriteMargin(console, marginOrigin, playGroundSize))
	{
		return false;
	}
	
	if(!console.WriteString( (wchar_t*)(L"Hello Game"), ConsoleCoordX(34, 0)))
	{
		return false;
	}
		
	while (!ends)
	{
		ConsoleEvent event;
		
		while(console.ReadInputEvent(event))
		{
			switch (event.m_type)
			{
				case CONSOLE_EVENT_KEY:
					switch(event.Event.m_keyEvent.m_virtualScanCode)
					{
						case CONSOLE_VK_LEFT:
							SetUpFlag(KEY_STATE_LEFT, event.Event.m_keyEvent.m_keyDown, keystate);
							break;
						case CONSOLE_VK_UP:
							SetUpFlag(KEY_STATE_UP, event.Event.m_keyEvent.m_keyDown, keystate);
							break;
						case CONSOLE_VK_RIGHT:
							SetUpFlag(KEY_STATE_RIGHT, event.Event.m_keyEvent.m_keyDown, keystate);
							break;
						case CONSOLE_VK_DOWN:
							SetUpFlag(KEY_STATE_DOWN, event.Event.m_keyEvent.m_keyDown, keystate);
							break;
						case CONSOLE_VK_ESCAPE:
							return true;
						default:
							break;
					}
					break;
				default:
					break;
			}
		}

		moveX = 0;
		moveY = 0;
		
		if(keystate & KEY_STATE_LEFT)
		{
			moveX--;
		}
		
		if(keystate & KEY_STATE_RIGHT)
		{
			moveX++;
		}
			
		if(keystate & KEY_STATE_UP)
		{
			moveY--;
		}
		
		if(keystate & KEY_STATE_DOWN)
		{
			moveY++;
		}
		
		if((moveX != 0)||(moveY != 0))
		{
			if(!console.WriteChar(' ', m_avatar.pos.x, m_avatar.pos.y))
			{
				return false;
			}
		}
		
		m_avatar.pos.x = (short unsigned)(((int)m_avatar.pos.x) + moveX);
		m_avatar.pos.y = (short unsigned)(((int)m_avatar.pos.y) + moveY);
		
		if (m_avatar.pos.x < m_viewFieldMin.x)
		{
			m_avatar.pos.x = m_viewFieldMin.x;
		}
		
		if (m_avatar.pos.x > m_viewFieldMax.x)
		{
			m_avatar.pos.x = m_viewFieldMax.x;
		}

		if (m_avatar.pos.y < m_viewFieldMin.y)
		{
			m_avatar.pos.y = m_viewFieldMin.y;
		}		
		
		if (m_avatar.pos.y > m_viewFieldMax.y)
		{
			m_avatar.pos.y = m_viewFieldMax.y;
		}
		
		if((moveX != 0)||(moveY != 0))
		{
			if(!console.WriteChar('X', m_avatar.pos.x, m_avatar.pos.y))
			{
				return false;
			}
		}		
		
		Sleep(100);
		
	}
	
	
	
	
	return true;
	
}




