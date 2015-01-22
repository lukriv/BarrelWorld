#ifndef __GAME_OGRE_BULLET_UTILS_MANAGER_H__
#define __GAME_OGRE_BULLET_UTILS_MANAGER_H__

#include <OGRE/OgreVector3.h>
#include <OGRE/OgreQuaternion.h>


inline btVector3 cvt(const Ogre::Vector3 &V){
    return btVector3(V.x, V.y, V.z);
}
 
inline Ogre::Vector3 cvt(const btVector3&V){
    return Ogre::Vector3(V.x(), V.y(), V.z());
}
 
inline btQuaternion cvt(const Ogre::Quaternion &Q)
{
    return btQuaternion(Q.x, Q.y, Q.z, Q.w);
};
 
inline Ogre::Quaternion cvt(const btQuaternion &Q)
{
    return Ogre::Quaternion(Q.w(), Q.x(), Q.y(), Q.z());
};


#endif // __GAME_OGRE_BULLET_UTILS_MANAGER_H__