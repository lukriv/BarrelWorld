#include <UnitTest++/UnitTest++.h>

#include "../../WordOfFlat/GameSystem/memalloccont.h"

struct Testcontainter{
	wxDword m_data;
};

SUITE(MemoryAllocatorContainer)
{
   TEST(Basic_test)
   {
		Testcontainter test[10];
		for (wxDword i = 0; i < 10; i++)
		{
			test[i] = i+1;
		}
		
		
		
   }

}