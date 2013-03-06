#include "GameMessageImpl.h"

GameErrorCode GameMessageBase::LoadHeader(wxInputStream& istream)
{
	wxDataInputStream iStream(istream);
	iStream.BigEndianOrdered(true);
	
	
}

GameErrorCode GameMessageBase::StoreHeader(wxOutputStream& ostream)
{
	wxDataOutputStream oStream(ostream);
	oStream.BigEndianOrdered(true);
}
