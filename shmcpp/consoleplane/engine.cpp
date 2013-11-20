#include "engine.h"

#include <iostream>

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
	
	
	
	for(short unsigned int i = (leftTopPos.x + 1); i < right; i++)
	{
		if(!console.WriteChar(*HORIZONTAL_DOUBLE_LINE, i, leftTopPos.y)) return false;
		if(!console.WriteChar(*HORIZONTAL_DOUBLE_LINE, i, bottom)) return false;
	}
	
	for(short unsigned int i = (leftTopPos.y + 1); i < bottom; i++)
	{
		if(!console.WriteChar(*VERTICAL_DOUBLE_LINE, leftTopPos.x, i)) return false;
		if(!console.WriteChar(*VERTICAL_DOUBLE_LINE, right, i)) return false;
	}
	
	return true;	
}
