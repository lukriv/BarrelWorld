#include "gtranscomp.h"


TransformComponent::TransformComponent() : m_translate( 0.0f, 0.0f, 0.0f)
										, m_scale( 1.0f, 1.0f, 1.0f)
{
	m_rotation.identity();
}
