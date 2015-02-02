#ifndef GACHARACTERBASE_H
#define GACHARACTERBASE_H

#include "Common.h"

class GAExport GACharacterBase
{
protected:
	UINT g_facingDir,g_upDir;
	Ogre::SceneNode* g_mainNode;
	Ogre::Entity* g_mainEntity;
	std::string g_name;
	float g_tempHeight;
	Ogre::Ray g_castRay;
	Ogre::RaySceneQuery* g_raySceneQuery;
public:
	GACharacterBase(Ogre::SceneManager* sgr,Ogre::SceneNode* g_mainNode,Ogre::Entity* g_mainEntity);
	GACharacterBase(Ogre::SceneManager* sceneMgr,std::string name);
	void setDirs(UINT facingDir,UINT upDir);
	void init(Ogre::SceneManager* sceneMgr);

	virtual Ogre::Vector3 getFacingDirection();
	virtual Ogre::Vector3 getUpDirection();
	virtual Ogre::SceneNode* getCharacterSceneNode();
	virtual Ogre::Entity* getCharacterEntity();
	std::string getName();
	virtual bool getHeightFromNode(float& height,int num);
	/*
	getTurnDegreesXY是getturnDegreesRollUpDirection的一种特例，在upDirection正好是世界轴的z轴时成立
	*/
	virtual float getTurnDegreesXY(Ogre::Vector3 newDir);
	virtual Ogre::Quaternion getTurnDegreesRollUpDirection(Ogre::Vector3 newDir);

	void setUpDirection(UINT upDir);
	void setFacingDirection(UINT facingDir);
	void setMaterialName(std::string name);
};

#endif