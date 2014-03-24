#ifndef __GAME_NEW_INL__XX11__
#define __GAME_NEW_INL__XX11__

#define NEWFNC new(std::nothrow)

#include <new>

template<typename Type>
inline void GameNew(Type *&value)
{
	value = NEWFNC Type;
}


template<typename Type, typename P1>
inline void GameNew(Type *&value, P1 p1)
{
	value = NEWFNC Type(p1);
}

template<typename Type, typename P1, typename P2>
inline void GameNew(Type *&value,P1 p1, P2 p2)
{
	value= NEWFNC Type(p1, p2);
}

template<typename Type, typename P1, typename P2, typename P3>
inline void GameNew(Type *&value,P1 p1, P2 p2, P3 p3)
{
	value= NEWFNC Type(p1, p2, p3);
}

template<typename Type, typename P1, typename P2, typename P3, typename P4>
inline void GameNew(Type *&value, P1 p1, P2 p2, P3 p3, P4 p4)
{
	value= NEWFNC Type(p1, p2, p3, p4);
}

template<typename Type, typename P1, typename P2, typename P3, typename P4, typename P5>
inline void GameNew(Type *&value, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5)
{
	value= NEWFNC Type(p1, p2, p3, p4, p5);
}

template<typename Type, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
inline void GameNew(Type *&value, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6)
{
	value= NEWFNC Type(p1, p2, p3, p4, p5, p6);
}

template<typename Type, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
inline void GameNew(Type *&value, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7)
{
	value= NEWFNC Type(p1, p2, p3, p4, p5, p6, p7);
}

template<typename Type, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>
inline void GameNew(Type *&value, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8)
{
	value= NEWFNC Type(p1, p2, p3, p4, p5, p6, p7, p8);
}

template<typename Type>
inline void GameNewArray(Type *&value, size_t size)
{
	value = NEWFNC Type[size];
}

template<typename Type>
inline void GameDelete(Type *p)
{
	delete p;
}

template<typename Type>
inline void GameDeleteArray(Type *p)
{
	delete[] p;
}

template<typename Type>
GameErrorCode GameNewChecked(Type *&value)
{
	GameNew<Type>(value);
	return (value) ? FWG_NO_ERROR : FWG_E_MEMORY_ALLOCATION_ERROR;
}

template<typename Type, typename P1>
GameErrorCode GameNewChecked(Type *&value, P1 p1)
{
	GameNew<Type, P1>(value, p1);
	return (value) ? FWG_NO_ERROR : FWG_E_MEMORY_ALLOCATION_ERROR;
}

template<typename Type, typename P1, typename P2>
GameErrorCode GameNewChecked(Type *&value, P1 p1, P2 p2)
{
	GameNew<Type, P1, P2>(value, p1, p2);
	return (value) ? FWG_NO_ERROR : FWG_E_MEMORY_ALLOCATION_ERROR;
}

template<typename Type, typename P1, typename P2, typename P3>
GameErrorCode GameNewChecked(Type *&value, P1 p1, P2 p2, P3 p3)
{
	GameNew<Type, P1, P2, P3>(value, p1, p2, p3);
	return (value) ? FWG_NO_ERROR : FWG_E_MEMORY_ALLOCATION_ERROR;
}

template<typename Type, typename P1, typename P2, typename P3, typename P4>
GameErrorCode GameNewChecked(Type *&value, P1 p1, P2 p2, P3 p3, P4 p4)
{
	GameNew<Type, P1, P2, P3, P4>(value, p1, p2, p3, p4);
	return (value) ? FWG_NO_ERROR : FWG_E_MEMORY_ALLOCATION_ERROR;
}

template<typename Type, typename P1, typename P2, typename P3, typename P4, typename P5>
GameErrorCode GameNewChecked(Type *&value, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5)
{
	GameNew<Type, P1, P2, P3, P4, P5>(value, p1, p2, p3, p4, p5);
	return (value) ? FWG_NO_ERROR : FWG_E_MEMORY_ALLOCATION_ERROR;
}

template<typename Type, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
GameErrorCode GameNewChecked(Type *&value, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6)
{
	GameNew<Type, P1, P2, P3, P4, P5, P6>(value, p1, p2, p3, p4, p5, p6);
	return (value) ? FWG_NO_ERROR : FWG_E_MEMORY_ALLOCATION_ERROR;
}

template<typename Type, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
GameErrorCode GameNewChecked(Type *&value, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7)
{
	GameNew<Type, P1, P2, P3, P4, P5, P6, P7>(value, p1, p2, p3, p4, p5, p6, p7);
	return (value) ? FWG_NO_ERROR : FWG_E_MEMORY_ALLOCATION_ERROR;
}

#undef NEWFNC

#endif //__GAME_NEW_INL__XX11__