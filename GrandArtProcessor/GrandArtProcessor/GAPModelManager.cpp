#include "GAPModelManager.h"
#include "GAPMainManager.h"
#include "GAPWorldManager.h"

GAPModelManager::GAPModelManager(GAPMainManager* mainMgr)
{
	g_mainMgr=mainMgr;
	g_hasPlane=false;
	g_showPlane=0;
	g_aniState1=0;
	g_animationPaused=true;
}

void GAPModelManager::init()
{
	g_worldMgr=g_mainMgr->getCurrentSceneMgr();
	initKeyMap();
}

void GAPModelManager::initKeyMap()
{
	k_planeXY=GAP_KEY_Z;
	k_planeYZ=GAP_KEY_X;
	k_planeXZ=GAP_KEY_Y;
}
void GAPModelManager::setKey(GAPKeyboardMap showPlaneXY,GAPKeyboardMap showPlaneXZ,GAPKeyboardMap showPlaneYZ)
{
	if(showPlaneXY!=GAP_KEY_UNDEFINED) k_planeXY=showPlaneXY;
	if(showPlaneXZ!=GAP_KEY_UNDEFINED) k_planeXZ=showPlaneXZ;
	if(showPlaneYZ!=GAP_KEY_UNDEFINED) k_planeYZ=showPlaneYZ;
}
bool GAPModelManager::setMoveOnPlane(Ogre::Vector3 origin)
{
	GAPKeyboardInput* input=g_mainMgr->g_keyboardInput;
	if(input->isKeyDown(k_planeXY))
	{
		g_showPlane=1;
	}
	else if(input->isKeyDown(k_planeYZ))
	{
		g_showPlane=2;
	}
	else if(input->isKeyDown(k_planeXZ))
	{
		g_showPlane=3;
	}
	g_hasPlane=true;
	switch(g_showPlane)
	{
	case 1:
		g_planeNode->detachAllObjects();
		g_planeNode->attachObject(g_planeXY);
		g_planeNode->attachObject(g_planeXY2);
		g_planeNode->setPosition(origin);
		break;
	case 2:
		g_planeNode->detachAllObjects();
		g_planeNode->attachObject(g_planeYZ);
		g_planeNode->attachObject(g_planeYZ2);
		g_planeNode->setPosition(origin);
		break;
	case 3:
		g_planeNode->detachAllObjects();
		g_planeNode->attachObject(g_planeXZ);
		g_planeNode->attachObject(g_planeXZ2);
		g_planeNode->setPosition(origin);
		break;
	default:
		g_hasPlane=false;
		g_showPlane=0;
	}
	return g_hasPlane;
}
void GAPModelManager::showPlaneXY(Ogre::Vector3 origin)
{
	g_showPlane=1;
	setMoveOnPlane(origin);
}
void GAPModelManager::showPlaneYZ(Ogre::Vector3 origin)
{
	g_showPlane=2;
	setMoveOnPlane(origin);
}
void GAPModelManager::showPlaneXZ(Ogre::Vector3 origin)
{
	g_showPlane=3;
	setMoveOnPlane(origin);
}
void GAPModelManager::freeMoveOnPlane()
{
	g_planeNode->detachAllObjects();
	g_hasPlane=false;
	g_showPlane=0;
}


bool GAPModelManager::setModelOrientation(Ogre::SceneNode* modelNode,float xdelta,float ydelta,Ogre::Vector3 xAxis,Ogre::Vector3 yAxis,float ratio)
{
	Ogre::Quaternion quax(Ogre::Radian(-ydelta*ratio),xAxis);
	Ogre::Quaternion quay(Ogre::Radian(xdelta*ratio),yAxis);
	modelNode->rotate(quax,Ogre::Node::TS_PARENT);
	modelNode->rotate(quay,Ogre::Node::TS_PARENT);
	return true;
}

bool GAPModelManager::setModelScale(Ogre::SceneNode* modelNode,float scalex,float scaley,float scalez)
{
	modelNode->scale(scalex,scaley,scalez);
	return true;
}

bool GAPModelManager::runModelAnimation(std::string modelName,std::string aniName,bool loop)
{
	Ogre::Entity* ent;
	g_worldMgr=g_mainMgr->getCurrentSceneMgr();
	if(g_aniState1!=0)
	{
		g_aniState1->setTimePosition(0);
		g_aniState1->setWeight(0);
	}
	g_aniState1=0;
	g_animationPaused=false;
	if(g_worldMgr->hasEntity(modelName)) ent=g_worldMgr->getEntity(modelName);
	else return false;
	if(!ent->hasVertexAnimation()&&!ent->hasSkeleton()) return false;
	g_aniState1=ent->getAnimationState(aniName);
	g_aniState1->setLoop(loop);
	g_aniState1->setEnabled(true);
	g_aniState1->setTimePosition(0.f);
	g_aniState1->setWeight(1);
	return true;
}

bool GAPModelManager::pauseModelAnimation()
{
	if(g_aniState1!=0)
	{
		g_animationPaused=true;
	}
	return true;
}

bool GAPModelManager::restartModelAnimation()
{
	if(g_aniState1!=0)
	{
		g_animationPaused=false;
	}
	return true;
}

bool GAPModelManager::stopModelAnimation()
{
	if(g_aniState1!=0)
	{
		g_aniState1->setTimePosition(0);
		g_aniState1=0;
	}
	g_animationPaused=true;
	return true;
}

UINT GAPModelManager::runningAnimation()
{
	if(g_aniState1==0)
	{
		return 0;
	}
	else if(g_animationPaused)
	{
		return 1;
	}
	else return 2;
}
bool GAPModelManager::update(float deltaTime)
{
	if(g_aniState1!=0&&g_animationPaused==false)
	{
		g_aniState1->addTime(deltaTime);
		return true;
	}
	return false;
}

void GAPModelManager::setSkeleton(std::string meshName,std::string skeletonName)
{
	if(g_worldMgr->hasEntity(meshName)==false) return;
}

void GAPModelManager::setAnimationTimePos(float time)
{
	if(g_aniState1!=0) g_aniState1->setTimePosition(time);
}
float GAPModelManager::getAnimationTimePos()
{
	if(g_aniState1!=0) return g_aniState1->getTimePosition();
	else return -1;
}

void GAPModelManager::moveModel(Ogre::SceneNode* node,float x,float y,float z)
{
	node->translate(x,y,z,Ogre::Node::TS_WORLD);
}

bool GAPModelManager::setModelMaterial(std::string name,std::string materialName)
{
	if(!g_mainMgr->getCurrentSceneMgr()->hasEntity(name)) return false;
	g_mainMgr->getCurrentSceneMgr()->getEntity(name)->setMaterialName(materialName);
	return true;
}