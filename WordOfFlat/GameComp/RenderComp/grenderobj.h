#ifndef __GAME_RENDER_OBJECT_H__
#define __GAME_RENDER_OBJECT_H__


#include <OGRE/OgreMovableObject.h>
#include <GameSystem/refobject.h>
#include <GameSystem/refobjectimpl.h>

class RenderObject : public RefObjectImpl<IRefObject> 
{
private:
	Ogre::MovableObject* m_pObject;
public:
	RenderObject(Ogre::MovableObject* pObject) : m_pObject(pObject) {}
	~RenderObject();
	
	inline Ogre::MovableObject* GetMovableObject() { return m_pObject; }
	
	// camera cast
	inline bool IsCameraType() 
	{
		return (m_pObject->getMovableType().compare("Camera") == 0);
	}
	
	inline Ogre::Camera* GetCamera() 
	{ 
		return (IsCameraType()?reinterpret_cast<Ogre::Camera*>(m_pObject):nullptr); 
	}
	
	// manual object cast
	inline bool IsManualObject()
	{
		return (m_pObject->getMovableType().compare("ManualObject") == 0);
	}
	
	inline Ogre::ManualObject* GetManualObject()
	{
		return (IsManualObject()?reinterpret_cast<Ogre::ManualObject*>(m_pObject):nullptr);
	}
	 
};


#endif //__GAME_RENDER_OBJECT_H__