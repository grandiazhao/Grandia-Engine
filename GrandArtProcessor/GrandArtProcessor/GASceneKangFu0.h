#ifndef GASCENEKANGFU0_H
#define GASCENEKANGFU0_H

#include "GASceneBase.h"
#include "GAPCameraController.h"
#include "GAPWorldManager.h"
#include "GAPPhysicsManager.h"
#include "GAPCharacterController.h"
#include "GAPModelManager.h"
#include "GAPModel.h"

class GASceneKangFu0 : public GASceneBase
{
public:
	GASceneKangFu0(GAPMainManager* mainMgr,std::string sceneName);
	bool createScene();

	bool frameStarted(float deltaTime);
	bool frameEnded(float deltaTime);

	bool keyPressed(const OIS::KeyEvent& evt);
	bool keyReleased(const OIS::KeyEvent& evt);
private:
	GAPModel *ogrehead;
	float transDelta;
	std::string moveName;
	bool b1,b2,b3,b4,b5;
	bool runBeiziAni;
	float endTime;
	Ogre::Skeleton* ske;
	Ogre::Bone* bone;

	void runBeiziAniFunc(GAPMainManager* mainMgr)
	{
		if(b1==true)
		{
			endTime=1.387;
		}
		if(b2==true)
		{
			endTime=1.697;
		}
		if(b3==true)
		{
			endTime=1.997;
		}
		if(b4==true)
		{
			endTime=2.717;
		}
		if(b5==true)
		{
			endTime=3.10;
		}
		if(mainMgr->g_modelMgr->getAnimationTimePos()>endTime)
		{
			mainMgr->g_modelMgr->pauseModelAnimation();
		}
		else
		{
			mainMgr->g_modelMgr->restartModelAnimation();
		}
	}
	void createOcean(Ogre::SceneManager* sm)																																																																
	{
		Ogre::Plane oceanSurface;
		oceanSurface.normal = Ogre::Vector3::UNIT_Z;
		oceanSurface.d = 5;
		Ogre::MeshManager::getSingleton().createPlane("OceanSurface",
			Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			oceanSurface,
			5000, 5000, 50, 50, true, 1, 1, 1, Ogre::Vector3::UNIT_Y);
		Ogre::Entity* mOceanSurfaceEnt = sm->createEntity( "OceanSurface", "OceanSurface" );
		Ogre::SceneNode* snode=sm->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(0,0,-5));
		snode->attachObject(mOceanSurfaceEnt);
		snode->roll(Ogre::Degree(90),Ogre::Node::TS_WORLD);
		mOceanSurfaceEnt->setMaterialName("Ocean2_Cg");
		sm->setAmbientLight(Ogre::ColourValue(0.1,0.1,0.1));
		sm->setFog(Ogre::FOG_LINEAR,Ogre::ColourValue(0.8,0.5,0.5),0.001f,1000.f,8000.f);
	}

	Ogre::Matrix4 concatenate2(Ogre::Matrix4 &m,Ogre::Matrix4 &m2);
};


#endif