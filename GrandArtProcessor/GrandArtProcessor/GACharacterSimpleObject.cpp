#include "GACharacterSimpleObject.h"


GACharacterSimpleObject::GACharacterSimpleObject(Ogre::SceneManager* sgr,Ogre::SceneNode* mainNode,Ogre::SceneNode* rotateNode,Ogre::Entity* mainEntity)
	:GACharacterBase(sgr,mainNode,mainEntity)
{
	g_rotateNode=rotateNode;
}

Ogre::Vector3 GACharacterSimpleObject::getFacingDirection()
{
	switch(g_facingDir)
	{
	case 1:
		return g_rotateNode->_getDerivedOrientation().xAxis();
	case 2:
		return Ogre::Vector3(0,0,0)-g_rotateNode->_getDerivedOrientation().xAxis();
	case 3:
		return g_rotateNode->_getDerivedOrientation().yAxis();
	case 4:
		return Ogre::Vector3(0,0,0)-g_rotateNode->_getDerivedOrientation().yAxis();
	case 5:
		return g_rotateNode->_getDerivedOrientation().zAxis();
	case 6:
		return Ogre::Vector3(0,0,0)-g_rotateNode->_getDerivedOrientation().zAxis();
	}
	return g_rotateNode->_getDerivedOrientation().xAxis();
}

float GACharacterSimpleObject::getTurnDegreesXY(Ogre::Vector3 newDir)
{
	Ogre::Vector3 fac=getFacingDirection();
	fac.z=0;
	newDir.z=0;
	return fac.getRotationTo(newDir).getRoll().valueDegrees();
}