#include "gentityfactory.h"

#include <wx/scopedptr.h>
#include <OGRE/OgreEntity.h>
#include <OGRE/OgreSceneNode.h>
#include <OGRE/OgreSceneManager.h>
#include <GameSystem/refobjectsmptr.h>
#include <GameComp/RenderComp/grendercompbase.h>
#include <GameComp/InputComp/ginputcomp.h>
#include <GameComp/LogicComp/glogicman.h>
#include <GameSystem/new.h>






GameErrorCode GameEntityFactory::CreateFloor(GameCompManager& compMgr)
{
	//MeshPtr meshptr = compMgr.GetRenderSystem().GetOgreRoot().getMeshManager().createPlane("floor", "") ;
	return FWG_E_NOT_IMPLEMENTED_ERROR;
}

GameErrorCode GameEntityFactory::CreateMainCamera(GameCompManager& compMgr)
{
}
