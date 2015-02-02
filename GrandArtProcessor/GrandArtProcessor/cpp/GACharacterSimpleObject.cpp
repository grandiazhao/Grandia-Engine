#include "GACharacterSimpleObject.h"


GACharacterSimpleObject::GACharacterSimpleObject(Ogre::SceneManager* sgr,Ogre::SceneNode* g_mainNode,Ogre::Entity* g_mainEntity):GACharacterBase(sgr,g_mainNode,g_mainEntity)
{

}
GACharacterSimpleObject::GACharacterSimpleObject(Ogre::SceneManager* sceneMgr,std::string name):GACharacterBase(sceneMgr,name)
{

}