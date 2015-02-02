#include "GAVehicle.h"

GAVehicle::GAVehicle(Ogre::SceneManager* sceneMgr,Ogre::SceneNode* g_mainNode,Ogre::Entity* g_mainEntity)
	:GACharacterBase(sceneMgr,g_mainNode,g_mainEntity)
{
	init();
	createRayCastNode(sceneMgr,0.65);
}
GAVehicle::GAVehicle(Ogre::SceneManager* sceneMgr,std::string name)
	:GACharacterBase(sceneMgr,name)
{
	init();
	createRayCastNode(sceneMgr,0.65);
}
void GAVehicle::init()
{
}
void GAVehicle::createRayCastNode(Ogre::SceneManager* sceneMgr,float ratio,bool createSphere)
{
	Ogre::Vector3 size=g_mainEntity->getWorldBoundingBox(true).getHalfSize();
	Ogre::Vector3 pos=size*getFacingDirection();
	g_halfLength=pos.length()*ratio;
	g_frontNode=g_mainNode->createChildSceneNode();
	g_frontNode->translate(ratio*pos,Ogre::Node::TS_WORLD);
	g_backNode=g_mainNode->createChildSceneNode();
	g_backNode->translate(-ratio*pos,Ogre::Node::TS_WORLD);
	if(createSphere)
	{
		Ogre::Entity* ent1=sceneMgr->createEntity("sphere.mesh");
		g_frontNode->attachObject(ent1);
		g_frontNode->setScale(0.01,0.01,0.01);
		Ogre::Entity* ent2=sceneMgr->createEntity("sphere.mesh");
		g_backNode->attachObject(ent2);
		g_backNode->setScale(0.01,0.01,0.01);
	}
}
bool GAVehicle::getHeightFromNode(float& height,int num)
{
	Ogre::SceneNode* rayCastNode;
	if(num==1)
	{
		rayCastNode=g_frontNode;
	}
	else if(num==2) rayCastNode=g_backNode;
	else return false;

	Ogre::Vector3 ori=rayCastNode->_getDerivedPosition();
	ori.z+=g_tempHeight;
	g_castRay.setOrigin(ori);
	g_castRay.setDirection(Ogre::Vector3(0,0,-1));
	g_raySceneQuery->setRay(g_castRay);
	g_raySceneQuery->setSortByDistance(true);

	Ogre::RaySceneQueryResult& qryResult=g_raySceneQuery->execute();
    Ogre::RaySceneQueryResult::iterator i=qryResult.begin();
	if(i==qryResult.end()) return false;
	Ogre::RaySceneQueryResult::iterator pi=i;
	bool otherCollidePlanes=false; 
	while(i!=qryResult.end())
	{
		if (i->worldFragment)
		{
			if(!otherCollidePlanes)
			{
				pi=i;
				otherCollidePlanes=true; 
			}
			else if(otherCollidePlanes)
			{
				if(pi->distance>i->distance) pi=i;
			}
		}
		i++;
	}
	if(!otherCollidePlanes) return false;
	else
	{
		height=pi->distance-g_tempHeight;
		return true;
	}
}