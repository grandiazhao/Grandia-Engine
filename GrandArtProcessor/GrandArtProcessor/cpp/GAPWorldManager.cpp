#include "GAPWorldManager.h"
#include "GAPMainManager.h"
#include "GAPPhysicsManager.h"

GAPWorldManager::GAPWorldManager(GAPMainManager* mainMgr)
{
	g_mainMgr=mainMgr;
	
}
void GAPWorldManager::init()
{
	g_ogreSceneMgr=g_mainMgr->getCurrentSceneMgr();
	g_zAxisUp=g_mainMgr->g_zAxisUp;
}
Ogre::MovableObject* GAPWorldManager::getTouchedObject(float x,float y)
{
	return 0;
}

bool GAPWorldManager::getTouchedDefiniteObject(float x,float y,Ogre::SceneNode* objectToBeTouched,Ogre::Vector3& pos)
{
	return true;
}
bool GAPWorldManager::addModel(std::string name,std::string meshName,Ogre::Vector3 pos,Ogre::Vector3 scale,Ogre::Quaternion qua)
{
	g_ogreSceneMgr=g_mainMgr->getCurrentSceneMgr();
	try
	{
		if(g_ogreSceneMgr->hasSceneNode(name)) return false;
		Ogre::Entity* ent=g_ogreSceneMgr->createEntity(name,meshName);
		Ogre::SceneNode *node=g_ogreSceneMgr->getRootSceneNode()->createChildSceneNode(name,pos,qua);
		if(g_zAxisUp&&qua==Ogre::Quaternion::IDENTITY) node->pitch(Ogre::Degree(90));
		node->attachObject(ent);
		node->setScale(scale);
		g_mainMgr->g_physicsMgr->createPhysicsObject(ent);
		g_mainMgr->g_physicsMgr->showPhysicsObject(name,false);
		objectNodeSet.push_back(node);
	}
	catch(...)
	{
		return false;
	}
	return true;
}

bool GAPWorldManager::deleteModel(std::string modelName)
{
	Ogre::SceneNode* node=g_ogreSceneMgr->getSceneNode(modelName);
	return deleteSceneNode(node);
}
bool GAPWorldManager::deleteModel(Ogre::Entity* entity)
{
	if(entity==0) return false;
	Ogre::SceneNode* node=entity->getParentSceneNode();
	return deleteSceneNode(node);
}
bool GAPWorldManager::deleteSceneNode(Ogre::SceneNode* nodeToDelete)
{
	if(nodeToDelete==0) return false;
	std::string name=nodeToDelete->getName();
	nodeToDelete->detachAllObjects();
	nodeToDelete->getParentSceneNode()->removeAndDestroyChild(name);
	g_mainMgr->g_physicsMgr->deletePhysicsObject(name);
	for(std::vector<Ogre::SceneNode*>::iterator iter=objectNodeSet.begin();iter!=objectNodeSet.end();iter++)
	{
		if((*iter)==nodeToDelete)
		{
			objectNodeSet.erase(iter);
			return true;
		}
	}
	return false;
}

void GAPWorldManager::resetAllObject()
{
	for(UINT i=0;i<objectNodeSet.size();i++)
	{
		objectNodeSet.at(i)->translate(0,0,1);
	}
	for(UINT i=0;i<objectNodeSet.size();i++)
	{
		objectNodeSet.at(i)->translate(0,0,-1);
	}
}