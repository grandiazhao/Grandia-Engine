#include "GACharacterBase.h"

GACharacterBase::GACharacterBase(Ogre::SceneManager* sgr,Ogre::SceneNode* node,Ogre::Entity* ent)
{
	g_mainNode=node;
	g_mainEntity=ent;
	g_name=g_mainNode->getName();
	init(sgr);
	setDirs(1,5);
}

GACharacterBase::GACharacterBase(Ogre::SceneManager* sceneMgr,std::string name)
{
	g_name=name;
	g_mainNode=sceneMgr->getSceneNode(name);
	g_mainEntity=sceneMgr->getEntity(name);
	init(sceneMgr);
	setDirs(1,5);
}

void GACharacterBase::init(Ogre::SceneManager* sceneMgr)
{
	g_raySceneQuery=sceneMgr->createRayQuery(Ogre::Ray());
	g_raySceneQuery->setWorldFragmentType(Ogre::SceneQuery::WFT_SINGLE_INTERSECTION);
	g_tempHeight=2;
}
void GACharacterBase::setDirs(UINT facingDir,UINT upDir)
{
	g_facingDir=facingDir;
	g_upDir=upDir;
}

Ogre::Vector3 GACharacterBase::getFacingDirection()
{
	switch(g_facingDir)
	{
	case 1:
		return g_mainNode->_getDerivedOrientation().xAxis();
	case 2:
		return Ogre::Vector3(0,0,0)-g_mainNode->_getDerivedOrientation().xAxis();
	case 3:
		return g_mainNode->_getDerivedOrientation().yAxis();
	case 4:
		return Ogre::Vector3(0,0,0)-g_mainNode->_getDerivedOrientation().yAxis();
	case 5:
		return g_mainNode->_getDerivedOrientation().zAxis();
	case 6:
		return Ogre::Vector3(0,0,0)-g_mainNode->_getDerivedOrientation().zAxis();
	}
	return g_mainNode->_getDerivedOrientation().xAxis();
}

Ogre::Vector3 GACharacterBase::getUpDirection()
{
	switch(g_upDir)
	{
	case 1:
		return g_mainNode->_getDerivedOrientation().xAxis();
	case 2:
		return Ogre::Vector3(0,0,0)-g_mainNode->_getDerivedOrientation().xAxis();
	case 3:
		return g_mainNode->_getDerivedOrientation().yAxis();
	case 4:
		return Ogre::Vector3(0,0,0)-g_mainNode->_getDerivedOrientation().yAxis();
	case 5:
		return g_mainNode->_getDerivedOrientation().zAxis();
	case 6:
		return Ogre::Vector3(0,0,0)-g_mainNode->_getDerivedOrientation().zAxis();
	}
	return g_mainNode->_getDerivedOrientation().zAxis();
}

void GACharacterBase::setFacingDirection(UINT facingDir)
{
	g_facingDir=facingDir;
}

void GACharacterBase::setUpDirection(UINT upDir)
{
	g_upDir=upDir;
}

void GACharacterBase::setMaterialName(std::string name)
{
	g_mainEntity->setMaterialName(name);
}

Ogre::SceneNode* GACharacterBase::getCharacterSceneNode()
{
	return g_mainNode;
}

Ogre::Entity* GACharacterBase::getCharacterEntity()
{
	return g_mainEntity;
}

std::string GACharacterBase::getName()
{
	return g_name;
}

float GACharacterBase::getTurnDegreesXY(Ogre::Vector3 newDir)
{
	Ogre::Vector3 fac=getFacingDirection();
	fac.z=0;
	newDir.z=0;
	return fac.getRotationTo(newDir).getRoll().valueDegrees();
}

Ogre::Quaternion GACharacterBase::getTurnDegreesRollUpDirection(Ogre::Vector3 newDir)
{
	Ogre::Vector3 up=getUpDirection();
	float x1=newDir.dotProduct(up);
	newDir=newDir-up*x1;
	return getFacingDirection().getRotationTo(newDir);
}

bool GACharacterBase::getHeightFromNode(float& height,int num)
{
	Ogre::SceneNode* rayCastNode=g_mainNode;
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