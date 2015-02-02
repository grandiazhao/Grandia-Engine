#ifndef GACHARACTERSIMPLEOBJECT_H
#define GACHARACTERSIMPLEOBJECT_H

#include "GACharacterBase.h"

class GAExport GACharacterSimpleObject : public GACharacterBase
{
protected:
	Ogre::SceneNode* g_rotateNode;
public:
	GACharacterSimpleObject(Ogre::SceneManager* sgr,Ogre::SceneNode* g_mainNode,Ogre::SceneNode* g_rotateNode,Ogre::Entity* g_mainEntity);
	Ogre::SceneNode* getRotateNode(){return g_rotateNode;}
	Ogre::Vector3 getFacingDirection();
	float getTurnDegreesXY(Ogre::Vector3 newDir);
};

#endif