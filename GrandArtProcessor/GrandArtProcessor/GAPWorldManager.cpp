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

bool GAPWorldManager::loadSceneFromFile(std::string fileName)
{
	std::ifstream ifs;
	GAPMisc misc;
	ifs.open(fileName.c_str(),std::ios::in|std::ios::binary);
	if(ifs.fail()) return false;
	Ogre::DataStreamPtr dataPtr(OGRE_NEW Ogre::FileStreamDataStream(fileName,&ifs,false));
	std::vector<std::string> lineVec;
	while(!dataPtr->eof())
	{
		std::string line=dataPtr->getLine(true);
		if(line.length()>0&&line.at(0)!='#'&&line.at(0)!='@')
		{
			lineVec.push_back(line);
		}
	}
	ifs.close();
	std::string str;
	std::string temp,temp2,temp3;
	std::string::size_type st;
	Ogre::Vector3 pos,scale;
	std::string name,meshName;
	Ogre::Quaternion qua;
	for(UINT i=0;i<lineVec.size();i++)
	{
		str=lineVec[i];
		bool loop=true;
		while(loop)
		{
			st=str.find_first_of('\t');
			if(st==str.npos)
			{
				loop=false;
				temp=str;
			}
			else
			{
				temp=str.substr(0,st);
				str=str.substr(st+1,str.length()-1);
			}
			st=temp.find_first_of(':');
			temp2=temp.substr(0,st);
			temp=temp.substr(st+1,temp.length()-1);
			if(temp2=="name")
			{
				name=temp;
			}
			else if(temp2=="entityName")
			{
				meshName=temp;
			}
			else if(temp2=="pos")
			{
				st=temp.find_first_of(',');
				temp3=temp.substr(0,st);
				temp=temp.substr(st+1,temp.length()-1);
				pos.x=misc.ConvertToFloat(temp3);
				st=temp.find_first_of(',');
				temp3=temp.substr(0,st);
				temp=temp.substr(st+1,temp.length()-1);
				pos.y=misc.ConvertToFloat(temp3);
				pos.z=misc.ConvertToFloat(temp);
			}
			else if(temp2=="scale")
			{
				st=temp.find_first_of(',');
				temp3=temp.substr(0,st);
				temp=temp.substr(st+1,temp.length()-1);
				scale.x=misc.ConvertToFloat(temp3);
				st=temp.find_first_of(',');
				temp3=temp.substr(0,st);
				temp=temp.substr(st+1,temp.length()-1);
				scale.y=misc.ConvertToFloat(temp3);
				scale.z=misc.ConvertToFloat(temp);
			}
			else if(temp2=="qua")
			{
				st=temp.find_first_of(',');
				temp3=temp.substr(0,st);
				temp=temp.substr(st+1,temp.length()-1);
				qua.w=misc.ConvertToFloat(temp3);
				st=temp.find_first_of(',');
				temp3=temp.substr(0,st);
				temp=temp.substr(st+1,temp.length()-1);
				qua.x=misc.ConvertToFloat(temp3);
				st=temp.find_first_of(',');
				temp3=temp.substr(0,st);
				temp=temp.substr(st+1,temp.length()-1);
				qua.y=misc.ConvertToFloat(temp3);
				qua.z=misc.ConvertToFloat(temp);
			}
		}
		addModel(name,meshName,pos,scale,qua);
	}
	return true;
}