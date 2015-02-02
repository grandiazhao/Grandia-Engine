#ifndef GACHARACTERAIMANAGER_H
#define GACHARACTERAIMANAGER_H

#include "GACharacterAIBase.h"
#include "GAVehicleAI.h"

class GAPMainManager;
class GAExport GACharacterAIManager
{
private:
	std::vector<GACharacterAIBase*> g_AIList;
	GAPMainManager* g_mainMgr;
public:
	GACharacterAIManager(GAPMainManager* mainMgr);
	void init();
	void addAIToList(GACharacterAIBase* ai);
	void createAIVehicle(std::string vehicleName,UINT facingDir,UINT upDir,UINT pathCell,float ratio,GARoadLine* line=0);
	void update(float deltaTime);
	GAVehicleAI* getAIVehicle(std::string vehicleName);
	void checkAndDispatchCollide(Ogre::Vector3 carPos);
};

#endif