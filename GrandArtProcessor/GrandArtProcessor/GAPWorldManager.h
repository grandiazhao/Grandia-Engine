#ifndef GAPWorldManager_H
#define GAPWorldManager_H

#include "Common.h"
#include "Ogre.h"

class GAPMainManager;

class GAExport GAPWorldManager
{
private:
	friend class GAPLoadSaveManager;
	Ogre::SceneManager* g_ogreSceneMgr;
	GAPMainManager* g_mainMgr;
	std::vector<Ogre::SceneNode*> objectNodeSet;
	bool g_zAxisUp;
public:
	GAPWorldManager(GAPMainManager* mainMgr);
	void init();

	Ogre::MovableObject* getTouchedObject(float x,float y);
	bool getTouchedDefiniteObject(float x,float y,Ogre::SceneNode* objectToBeTouched,Ogre::Vector3 &Pos);
	bool addModel(std::string objectName,std::string meshName,Ogre::Vector3 pos=Ogre::Vector3(0,0,0),
		Ogre::Vector3 scale=Ogre::Vector3(1,1,1),Ogre::Quaternion qua=Ogre::Quaternion::IDENTITY);
	bool setModelMaterials(std::string materialName);
	bool deleteModel(std::string modelName);
	bool deleteModel(Ogre::Entity* entity);
	bool deleteSceneNode(Ogre::SceneNode* nodeToDelete);
	void resetAllObject();
	bool loadSceneFromFile(std::string fileName);
};

#endif