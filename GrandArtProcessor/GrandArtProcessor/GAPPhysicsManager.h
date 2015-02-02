#ifndef GAPPHYSICSMANAGER_H
#define GAPPHYSICSMANAGER_H

#include "Common.h"
#include "GAPhysicsBaseObject.h"

class GAPMainManager;


class GAExport GAPPhysicsManager
{
private:
	typedef std::vector<GAPhysicsBaseObject*> PhysicsObjSet;
	typedef std::vector<int> CollideSimbolSet;
	struct DetectCell
	{
		GAPhysicsBaseObject* obj;
		float length;
		float start;
		float end;
	};
	typedef std::vector<DetectCell> DetectCellSet;
	DetectCellSet g_xDistanceSet,g_yDistanceSet,g_zDistanceSet;
	DetectCellSet g_staticXDistanceSet,g_staticYDistanceSet,g_staticZDistanceSet;
	PhysicsObjSet g_physicsObjSet;
	CollideSimbolSet g_collideSimbolSet;
public:
	GAPMainManager* g_mainMgr;

public:
	GAPPhysicsManager(GAPMainManager* mainMgr);
	void init();

	bool setBspWorld(std::string filePath);

	void update();
	void update(Ogre::SceneNode* node);
	bool createPhysicsObject(Ogre::Entity* entity,GAPhysicsBaseObject::ObjectType type=GAPhysicsBaseObject::ObjectType_Cube,Ogre::Vector3 ratio=Ogre::Vector3(1,1,1));
	bool reCreatePhysicsObject(GAPhysicsBaseObject* object,Ogre::Entity* entity);
	void changeObjectVisibleType(std::string name);
	void showPhysicsObject(std::string name,bool isShow);
	void disableAllphysicsObj();
	void enableAllphysicsObj();

	void insertDetectCell(GAPhysicsBaseObject* obj);
	void updateDetectCell(int cellNo=-1);
	void useDetectCellGenerateSimbol();
	void deleteDetectCell(GAPhysicsBaseObject* obj);

	void testCollide();
	void testAndDispatchCollideWithSingleObject(std::string name);
	void dispatchCollideResult();
	void dispatchCollideResult(GAPhysicsBaseObject* obj,int type);

	void deletePhysicsObject(std::string name);
private:
	bool checkEntityName(std::string name);
	GAPhysicsBaseObject* getPhysicsObject(std::string name);
	GAPhysicsBaseObject* getPhysicsObject(std::string name,int& count);
	void rePushObject(GAPhysicsBaseObject* obj,GAVector3 collidePoint);
};
#endif