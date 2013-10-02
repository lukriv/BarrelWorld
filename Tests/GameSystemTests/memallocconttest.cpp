#include <UnitTest++/UnitTest++.h>

#include "../../WordOfFlat/GameSystem/memalloccont.h"

struct Testcontainter{
	wxDword m_data;
};

SUITE(MemoryAllocatorContainer)
{
   TEST(Basic_test)
   {
	    wxDword newIndex = 0, *item = NULL;
		Testcontainter test[10];
		MemoryAllocContainer testAlloc;
		
		for (wxDword i = 0; i < 10; i++)
		{
			test[i] = i+1;
		}
		
		for (wxDword i = 0; i < 10; i++)
		{
			CHECK(FWG_SUCCEDED(testAlloc.AllocNewItem(newIndex));
			CHECK(newIndex == i);
			item = testAlloc.GetItem(newIndex);
			CHECK(item != NULL);
			*item = i;
		}
		
		for (wxDword i = 0; i < 10; i++)
		{
			item = testAlloc.GetItem(i);
			CHECK(item != NULL);
			CHECK(*item == i);
		}
		
		for (wxDword i = 0; i < 10; i++)
		{
			testAlloc.FreeItem(i);
			CHECK(testAlloc.GetSize() == (10 - i));
		}
		CHECK(testAlloc.GetSize() == (10 - i));
		
		for (wxDword i = 0; i < 10; i++)
		{
			item = testAlloc.GetItem(i);
			CHECK(item == NULL);
		}
   }
   
   TEST(AddHoc_Test)
   {
		wxDword newIndex = 0, *item = NULL;
		Testcontainter test[10];
		MemoryAllocContainer testAlloc;
		
		for (wxDword i = 0; i < 10; i++)
		{
			test[i] = i+1;
		}
		
		for (wxDword i = 0; i < 10; i++)
		{
			CHECK(FWG_SUCCEDED(testAlloc.AllocNewItem(newIndex));
			CHECK(newIndex == i);
			item = testAlloc.GetItem(newIndex);
			CHECK(item != NULL);
			*item = i;
		}
		
		testAlloc.Free(5);
		item = testAlloc.GetItem(5);
		CHECK(item == NULL);
		
		CHECK(testAlloc.GetSize() == 9);
		
		CHECK(FWG_SUCCEDED(testAlloc.AllocNewItem(newIndex)));
		CHECK(newIndex == 5);
		
		CHECK(testAlloc.GetSize() == 10);
		
		testAlloc.Free(3);
		testAlloc.Free(7);
		item = testAlloc.GetItem(3);
		CHECK(item == NULL);
		
		item = testAlloc.GetItem(7);
		CHECK(item == NULL);
		
		CHECK(testAlloc.GetSize() == 8);
		
		CHECK(FWG_SUCCEDED(testAlloc.AllocNewItem(newIndex)));
		CHECK(newIndex == 7);
		CHECK(testAlloc.GetSize() == 9);
		
		CHECK(FWG_SUCCEDED(testAlloc.AllocNewItem(newIndex)));
		CHECK(newIndex == 3);
		CHECK(testAlloc.GetSize() == 10);
   }
   
   TEST(FreeAll_test)
   {
		wxDword newIndex = 0, *item = NULL;
		Testcontainter test[10];
		MemoryAllocContainer testAlloc;
		
		for (wxDword i = 0; i < 10; i++)
		{
			test[i] = i+1;
		}
		
		for (wxDword i = 0; i < 10; i++)
		{
			CHECK(FWG_SUCCEDED(testAlloc.AllocNewItem(newIndex));
			CHECK(newIndex == i);
			item = testAlloc.GetItem(newIndex);
			CHECK(item != NULL);
			*item = i;
		}
		
		testAlloc.FreeAll();
		
		for (wxDword i = 0; i < 10; i++)
		{
			item = testAlloc.GetItem(i);
			CHECK(item == NULL);
		}
		
   }

}