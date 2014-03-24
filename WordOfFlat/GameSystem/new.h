#ifndef __GAME_NEW_H__XX11__
#define __GAME_NEW_H__XX11__

#include "gerror.h"

template<typename Type>
void GameNew(Type *&value);

template<typename Type, typename P1>
void GameNew(Type *&value, P1 p1);

template<typename Type, typename P1, typename P2>
void GameNew(Type *&value, P1 p1, P2 p2);

template<typename Type, typename P1, typename P2, typename P3>
void GameNew(Type *&value, P1 p1, P2 p2, P3 p3);

template<typename Type, typename P1, typename P2, typename P3, typename P4>
void GameNew(Type *&value, P1 p1, P2 p2, P3 p3, P4 p4);

template<typename Type, typename P1, typename P2, typename P3, typename P4, typename P5>
void GameNew(Type *&value, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5);

template<typename Type, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
void GameNew(Type *&value, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6);

template<typename Type, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
void GameNew(Type *&value, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7);

template<typename Type>
void GameNewArray(Type *&value, size_t size);

template<typename Type>
void GameDelete(Type *p);

template<typename Type>
void GameDeleteArray(Type *p);

template<typename Type>
GameErrorCode GameNewChecked(Type *&value);

template<typename Type, typename P1>
GameErrorCode GameNewChecked(Type *&value, P1 p1);

template<typename Type, typename P1, typename P2>
GameErrorCode GameNewChecked(Type *&value, P1 p1, P2 p2);

template<typename Type, typename P1, typename P2, typename P3>
GameErrorCode GameNewChecked(Type *&value, P1 p1, P2 p2, P3 p3);

template<typename Type, typename P1, typename P2, typename P3, typename P4>
GameErrorCode GameNewChecked(Type *&value, P1 p1, P2 p2, P3 p3, P4 p4);

template<typename Type, typename P1, typename P2, typename P3, typename P4, typename P5>
GameErrorCode GameNewChecked(Type *&value, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5);

template<typename Type, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
GameErrorCode GameNewChecked(Type *&value, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6);

template<typename Type, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
GameErrorCode GameNewChecked(Type *&value, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7);

#include "new.inl"

#endif //__GAME_NEW_H__XX11__