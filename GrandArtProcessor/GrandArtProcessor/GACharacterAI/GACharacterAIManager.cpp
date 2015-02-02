#include "GACharacterAIManager.h"
#include "../GAPMainManager.h"
#include "GAVehicleAI.h"
#include "../GAPCharacterController.h"

GACharacterAIManager::GACharacterAIManager(GAPMainManager* mainMgr)
{
	g_mainMgr=mainMgr;
}

void GACharacterAIManager::init()
{
}

void GACharacterAIManager::addAIToList(GACharacterAIBase* ai)
{
	g_AIList.push_back(ai);
}

void GACharacterAIManager::update(float deltaTime)
{
	GAVehicle* v=g_mainMgr->g_characterCtrl->g_vehicle;
	for(UINT i=0;i<g_AIList.size();i++)
	{
		g_AIList[i]->update(deltaTime);
		if(g_AIList[i]->g_classType==12345) ((GAVehicleAI*)g_AIList[i])->setPassingTranslate(deltaTime,v->getCharacterSceneNode()->_getDerivedPosition(),v->getFacingDirection());
	}
}

void GACharacterAIManager::createAIVehicle(std::string name,UINT facingDir,UINT upDir,UINT pathCell,float ratio,GARoadLine* line)
{
	Ogre::SceneNode* node=g_mainMgr->getCurrentSceneMgr()->getSceneNode(name);
	GAVehicle* vehicle=new GAVehicle(g_mainMgr->getCurrentSceneMgr(),name);
	vehicle->setDirs(facingDir,upDir);
	GAVehicleAI* ve=new GAVehicleAI(vehicle,g_mainMgr->getCurrentSceneMgr());
	g_AIList.push_back(ve);
	ve->setRoadLine(line);
	ve->setBornPosition(pathCell,ratio,false);
}

void GACharacterAIManager::checkAndDispatchCollide(Ogre::Vector3 carPos)
{
	Ogre::Vector3 pos1,pos2;
	GAVehicleAI* v1,*v2;
	for(UINT i=0;i<33;i++)
	{
		pos1=g_AIList[i]->getCharacter()->getCharacterSceneNode()->_getDerivedPosition();
		if(pos1.squaredDistance(carPos)>250000)
		{
			((GAVehicleAI*)g_AIList[i])->setVehicleSpeed(300,100);
		}
		else ((GAVehicleAI*)g_AIList[i])->setVehicleSpeed(30,10);
	}
	for(UINT i=0;i<17;i++)
	{
		for(UINT j=i+1;j<17;j++)
		{
			v1=(GAVehicleAI*)g_AIList[i];
			v2=(GAVehicleAI*)g_AIList[j];
			pos1=g_AIList[i]->getCharacter()->getCharacterSceneNode()->_getDerivedPosition();
			pos2=g_AIList[j]->getCharacter()->getCharacterSceneNode()->_getDerivedPosition();
			if(pos1.squaredDistance(pos2)<250000)
			{
				if(v1->getRoadCell()==v2->getRoadCell())
				{
					if(v1->getRoadRatio()>v2->getRoadRatio())
					{
						v2->setVehicleSpeed(20,5);
					}
					else v1->setVehicleSpeed(20,5);
				}
			}
		}
	}
	for(UINT i=17;i<33;i++)
	{
		for(UINT j=i+1;j<33;j++)
		{
			v1=(GAVehicleAI*)g_AIList[i];
			v2=(GAVehicleAI*)g_AIList[j];
			pos1=g_AIList[i]->getCharacter()->getCharacterSceneNode()->_getDerivedPosition();
			pos2=g_AIList[j]->getCharacter()->getCharacterSceneNode()->_getDerivedPosition();
			if(pos1.squaredDistance(pos2)<10000)
			{
				if(v1->getRoadCell()==v2->getRoadCell())
				{
					if(v1->getRoadRatio()>v2->getRoadRatio())
					{
						v2->setVehicleSpeed(20,5);
					}
					else v1->setVehicleSpeed(20,5);
				}
			}
		}
	}
}

GAVehicleAI* GACharacterAIManager::getAIVehicle(std::string vehicleName)
{
	std::vector<GACharacterAIBase*>::iterator iter;
	for(iter=g_AIList.begin();iter!=g_AIList.end();iter++)
	{
		if((*iter)->getName()==vehicleName) return (GAVehicleAI*)(*iter);
	}
	return 0;
}