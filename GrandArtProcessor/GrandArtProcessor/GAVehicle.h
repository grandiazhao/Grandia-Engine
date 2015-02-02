#ifndef GAVEHICLE_H
#define GAVEHICLE_H

#include "GACharacterBase.h"

class GAExport GAVehicle : public GACharacterBase
{
private:
	Ogre::SceneNode* g_frontNode;
	Ogre::SceneNode* g_backNode;
protected:
public:
	float g_halfLength;
public:
	GAVehicle(Ogre::SceneManager* sceneMgr,std::string name);
	GAVehicle(Ogre::SceneManager* sceneMgr,Ogre::SceneNode* g_mainNode,Ogre::Entity* g_mainEntity);
	void init();
	void createRayCastNode(Ogre::SceneManager* sceneMgr,float ratio,bool createSphere=false);
public:
	bool getHeightFromNode(float& height,int num);
};

#endif