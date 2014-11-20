#ifndef __GAME_TRANSFORM_COMPONENT_H__
#define __GAME_TRANSFORM_COMPONENT_H__

#include <bullet/vectormath/vmInclude.h>
#include <GameSystem/refobject.h>
#include <GameSystem/refobjectimpl.h>

struct TransformComponent : public RefObjectImpl<IRefObject> {

	Vectormath::Aos::Vector3 	m_translate ATTRIBUTE_ALIGNED16;
	Vectormath::Aos::Vector3	m_scale ATTRIBUTE_ALIGNED16;
	Vectormath::Aos::Quat		m_rotation ATTRIBUTE_ALIGNED16;
public:
	TransformComponent();
	
};


#endif //__GAME_TRANSFORM_COMPONENT_MANAGER_H__