#include "gentityfactory.h"

#include <wx/scopedptr.h>
#include "../GameObjects/gsfmlgeom.h"
#include "../GameObjects/grendercomp.h"


void GameEntityFactory::addRef()
{
	wxAtomicInc(m_refCount);
}

wxInt32 GameEntityFactory::release()
{
	wxInt32 refCount = wxAtomicDec(m_refCount);
	if (refCount == 0)
	{
		delete this;
	}
	return refCount;
}

GameErrorCode GameEntityFactory::Initialize(GameResourceHolder* pResHolder, GameLogger* pLogger)
{
	m_spResHolder = pResHolder;
	m_spLogger = pLogger;
	return FWG_NO_ERROR;
}


GameErrorCode GameEntityFactory::CreateAllEntities(GameDefinitionHolder& defHolder, GameCompManager& compMgr)
{
}
