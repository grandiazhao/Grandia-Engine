/*
在GA引擎中，所有的场景都交由继承于基类GASceneBase的场景类来完成，由继承于GASceneManager的场景管理器统一负责切换场景的功能。

GA Engine Created By Grandia Zhao

该场景是赛车场景，描述由人操作的赛车在场景中的驾驶。
*/

#ifndef GASCENECAR1_H
#define GASCENECAR1_H

#include "GASceneBase.h"
#include "GAPCameraController.h"
#include "GAPWorldManager.h"
#include "GAPPhysicsManager.h"
#include "GAPCharacterController.h"
#include "GAPModelManager.h"
#include "GAPModel.h"
#include "GACharacterAI\GAVehicleAI.h"
#include "GACharacterAI\GACharacterAIManager.h"
#include "GARollPoint.h"
#include "GAEventBase.h"
#include "GARoadCorner.h"
#include "GARoadLine.h"

class GASceneCar1 : public GASceneBase
{
public:
	GASceneCar1(GAPMainManager *mainMgr,std::string sceneName);
	bool createScene();
	/*bool freezeScene();
	bool destroyScene();*/
	bool frameStarted(float deltaTime);
	bool frameEnded(float deltaTime);

	bool keyPressed(const OIS::KeyEvent& evt);
	bool keyReleased(const OIS::KeyEvent& evt);

	/*bool mouseMoved(const OIS::MouseEvent& evt);
	bool mousePressed(const OIS::MouseEvent& evt, OIS::MouseButtonID id);
	bool mouseReleased(const OIS::MouseEvent& evt,OIS::MouseButtonID id);

	bool buttonPressed(const OIS::JoyStickEvent &evt,int num);
	bool buttonReleased(const OIS::JoyStickEvent &evt,int num);
	bool axisMoved(const OIS::JoyStickEvent &evt,int num);
	bool povMoved(const OIS::JoyStickEvent &evt,int num);
	bool sliderMoved(const OIS::JoyStickEvent &evt,int num);*/

private:
	GAVehicleAI *v1;
	GARollPoint *rollPoint;
	GAEventBase* eventBase;
	GACharacterAIManager *g_AIMgr;
	GARoadCorner* corner;
	UINT crossedLineNo,signStep;
	float sparkingTime;
	float sparkingTotalTime;
	bool sparkingShow;
	bool showSign;
	float translateLength;
	float g_signTime;
	bool start;
	float maxSignSpeed;
	float signSpeedTotalTime;
	float signSpeedTime;
	UINT iCarToFollow;
	GARoadLine* g_aiVehicleLine;
	GARoadLine* g_aiVehicleLineR;

	void sceneNodeSparking(Ogre::SceneNode* node,float deltaTime,bool init)
	{
		if(!showSign)
		{
			node->setVisible(false);
			return;
		}
		sparkingTime+=deltaTime;
		sparkingTotalTime+=deltaTime;
		if(!init)
		{
			if(sparkingTime>0.5)
			{
				sparkingTime=0;
				sparkingShow=!sparkingShow;
				node->setVisible(sparkingShow);
			}
		}
		else
		{
			sparkingShow=true;
			node->setVisible(false);
		}
	}

	void rollPointForSign(Ogre::SceneManager* sceneMgr,float deltaTime)
	{
		if(rollPoint->getImmediateRollPoint(1))
		{
			Ogre::Entity *ent=sceneMgr->getEntity("sign");
			sceneMgr->getSceneNode("sign")->setVisible(true);
			g_signTime=0;
			switch(rollPoint->getImmediateSeparateRollPoint(7))
			{
			case 1:
				ent->setMaterialName("xiansu40");
				break;
			case 2:
				ent->setMaterialName("xiansu60");
				break;
			case 3:
				ent->setMaterialName("xiansu80");
				break;
			case 4:
				ent->setMaterialName("xiansu90");
				break;
			case 5:
				ent->setMaterialName("zuozhuan");
				break;
			case 6:
				ent->setMaterialName("youzhuan");
				break;
			case 7:
				ent->setMaterialName("stop");
			default:
				break;
			}
		}
	}

	bool rollPointForLeftRight(Ogre::SceneManager* sceneMgr,UINT type,float ratio)
	{
		Ogre::Entity *ent=sceneMgr->getEntity("sign");
		if(type==2) //left
		{
			if(rollPoint->getImmediateRollPoint(ratio))
			{
				ent->setMaterialName("zuozhuan");
				return true;
			}
		}
		else if(type==0) //right
		{
			if(rollPoint->getImmediateRollPoint(ratio))
			{
				ent->setMaterialName("youzhuan");
				return true;
			}
		}
		else if(type==1)
		{
			if(rollPoint->getImmediateRollPoint(ratio))
			{
				if(rollPoint->getImmediateRollPoint(0.5))
				{
					ent->setMaterialName("zuozhuan");
				}
				else ent->setMaterialName("youzhuan");
				return true;
			}
		}
		return false;
	}

	float rollPointForSpeed(Ogre::SceneManager* sceneMgr,float currentSpeed,float ratio)
	{
		Ogre::Entity *ent=sceneMgr->getEntity("sign");
		if(currentSpeed>40)
		{
			if(rollPoint->getImmediateRollPoint(ratio))
			{
				ent->setMaterialName("xiansu90");
				return 40;
			}
		}
		else if(currentSpeed>35)
		{
			if(rollPoint->getImmediateRollPoint(ratio))
			{
				ent->setMaterialName("xiansu80");
				return 35;
			}
		}
		else if(currentSpeed>30)
		{
			if(rollPoint->getImmediateRollPoint(ratio))
			{
				ent->setMaterialName("xiansu60");
				return 30;
			}
		}
		else if(currentSpeed>25)
		{
			if(rollPoint->getImmediateRollPoint(ratio))
			{
				ent->setMaterialName("xiansu40");
				return 25;
			}
		}
		return -1;
	}

	bool rollPointForStop(Ogre::SceneManager* sceneMgr,float ratio)
	{
		Ogre::Entity *ent=sceneMgr->getEntity("sign");
		if(rollPoint->getImmediateRollPoint(ratio))
		{
			ent->setMaterialName("stop");
			return true;
		}
		return false;
	}

	void createRollSign(GAPMainManager* g_mainMgr,float deltaTime)
	{
		if(signStep==0)
		{
			showSign=false;
			Ogre::Vector3 pos1=g_mainMgr->getCurrentSceneMgr()->getSceneNode("car_test")->_getDerivedPosition();
			Ogre::Vector3 pos2=pos1+5*g_mainMgr->g_characterCtrl->g_vehicle->getFacingDirection()*g_mainMgr->g_characterCtrl->getCurrentSpeed();
			GARoadCorner::Corner* cornerCell=corner->whetherLineCrossedWithCorner(pos1.x,pos1.y,pos2.x,pos2.y,pos2.z,crossedLineNo);
			if(cornerCell)
			{
				if(rollPointForStop(g_mainMgr->getCurrentSceneMgr(),0.1))
				{
					signStep=1;
					showSign=true;
				}
				else
				{
					signStep=2;
					if(cornerCell->lineNum==3)
					{
						showSign=rollPointForLeftRight(g_mainMgr->getCurrentSceneMgr(),crossedLineNo,0.5);
					}
					else
					{
						showSign=rollPointForLeftRight(g_mainMgr->getCurrentSceneMgr(),1,0.5);
					}
					g_mainMgr->getCurrentSceneMgr()->getSceneNode("sign")->setVisible(true);
					sparkingTotalTime=0;
				}
			}
			else
			{
				signSpeedTime+=deltaTime;
				if(signSpeedTime>signSpeedTotalTime)
				{
					signSpeedTime=0;
					maxSignSpeed=rollPointForSpeed(g_mainMgr->getCurrentSceneMgr(),g_mainMgr->g_characterCtrl->getCurrentSpeed(),0.1);
					if(maxSignSpeed>0)
					{
						signStep=3;
						showSign=true;
					}
				}
			}
		}

		if(signStep==1)
		{
			if(g_mainMgr->g_characterCtrl->getCurrentSpeed()<=0.001)
			{
				signStep=0;
				sceneNodeSparking(g_mainMgr->getCurrentSceneMgr()->getSceneNode("sign"),deltaTime,true);
			}
			sceneNodeSparking(g_mainMgr->getCurrentSceneMgr()->getSceneNode("sign"),deltaTime,false);
		}
		else if(signStep==2)
		{
			Ogre::Vector3 pos2=g_mainMgr->getCurrentSceneMgr()->getSceneNode("car_test")->_getDerivedPosition();
			Ogre::Vector3 pos1=g_mainMgr->g_characterCtrl->g_recorder->getFormerPosition(1);
			if(corner->whetherLineCrossedWithCorner(pos1.x,pos1.y,pos2.x,pos2.y,pos2.z,crossedLineNo))
			{
				signStep=0;
				sceneNodeSparking(g_mainMgr->getCurrentSceneMgr()->getSceneNode("sign"),deltaTime,true);
			}
			sceneNodeSparking(g_mainMgr->getCurrentSceneMgr()->getSceneNode("sign"),deltaTime,false);
		}
		else if(signStep==3)
		{
			if(g_mainMgr->g_characterCtrl->getCurrentSpeed()<=maxSignSpeed)
			{
				signStep=0;
				sceneNodeSparking(g_mainMgr->getCurrentSceneMgr()->getSceneNode("sign"),deltaTime,true);
			}
			sceneNodeSparking(g_mainMgr->getCurrentSceneMgr()->getSceneNode("sign"),deltaTime,false);
		}
		else if(signStep==4)
		{
		}
	}

	void createSingleVehicle(GAPMainManager* g_mainMgr,std::string name,std::string meshName,Ogre::Vector3 scale,UINT pathCell,float ratio,UINT facingDir=1,std::string materialName="")
	{
		g_mainMgr->g_worldMgr->addModel(name,meshName,Ogre::Vector3(0,0,10),scale);
		if(materialName!="") g_mainMgr->g_modelMgr->setModelMaterial(name,materialName);
		g_AIMgr->createAIVehicle(name,facingDir,2,pathCell,ratio,g_aiVehicleLine);
		g_AIMgr->getAIVehicle(name)->setVehicleSpeed(50,30);
	}

	void createSingleVehicleR(GAPMainManager* g_mainMgr,std::string name,std::string meshName,Ogre::Vector3 scale,UINT pathCell,float ratio,UINT facingDir=1,std::string materialName="")
	{
		g_mainMgr->g_worldMgr->addModel(name,meshName,Ogre::Vector3(0,0,10),scale);
		if(materialName!="") g_mainMgr->g_modelMgr->setModelMaterial(name,materialName);
		g_AIMgr->createAIVehicle(name,facingDir,2,pathCell,ratio,g_aiVehicleLineR);
		g_AIMgr->getAIVehicle(name)->setVehicleSpeed(50,30);
	}
	void createVehicle(GAPMainManager* g_mainMgr)
	{
		createSingleVehicle(g_mainMgr,"car0","car2.mesh",Ogre::Vector3(0.34,0.34,0.34),0,0.5,1,"chuzu031");
		createSingleVehicle(g_mainMgr,"car1","car2.mesh",Ogre::Vector3(0.34,0.34,0.34),2,0.5,1,"chuzu032");
		createSingleVehicle(g_mainMgr,"car2","car2.mesh",Ogre::Vector3(0.34,0.34,0.34),4,0.5);
		createSingleVehicle(g_mainMgr,"car3","car2.mesh",Ogre::Vector3(0.34,0.34,0.34),6,0.5,1,"chuzu034");
		createSingleVehicle(g_mainMgr,"car4","car2.mesh",Ogre::Vector3(0.34,0.34,0.34),8,0.5,1,"chuzu035");

		createSingleVehicle(g_mainMgr,"car5","car23.mesh",Ogre::Vector3(0.17,0.17,0.17),10,0.5);
		createSingleVehicle(g_mainMgr,"car6","car18.mesh",Ogre::Vector3(0.17,0.17,0.17),12,0.5);
		createSingleVehicle(g_mainMgr,"car7","car17.mesh",Ogre::Vector3(0.17,0.17,0.17),14,0.5);
		createSingleVehicle(g_mainMgr,"car8","car13.mesh",Ogre::Vector3(0.34,0.34,0.34),16,0.5);
		createSingleVehicle(g_mainMgr,"car9","car5.mesh",Ogre::Vector3(0.34,0.34,0.34),18,0.5,2);

		createSingleVehicle(g_mainMgr,"car10","car23.mesh",Ogre::Vector3(0.17,0.17,0.17),20,0.5);
		createSingleVehicle(g_mainMgr,"car11","car22.mesh",Ogre::Vector3(0.17,0.17,0.17),22,0.5);
		createSingleVehicle(g_mainMgr,"car12","car20.mesh",Ogre::Vector3(0.17,0.17,0.17),24,0.5);
		createSingleVehicle(g_mainMgr,"car13","car5.mesh",Ogre::Vector3(0.34,0.34,0.34),26,0.5,2);
		createSingleVehicle(g_mainMgr,"car14","car10.mesh",Ogre::Vector3(0.34,0.34,0.34),28,0.5,2);

		createSingleVehicle(g_mainMgr,"car15","car6.mesh",Ogre::Vector3(0.34,0.34,0.34),30,0.5,2);
		createSingleVehicle(g_mainMgr,"car16","car18.mesh",Ogre::Vector3(0.17,0.17,0.17),31,0.5);
	}
	void createVehicleR(GAPMainManager* g_mainMgr)
	{
		createSingleVehicleR(g_mainMgr,"car17","car19.mesh",Ogre::Vector3(0.17,0.17,0.17),0,0.5);
		createSingleVehicleR(g_mainMgr,"car18","car8.mesh",Ogre::Vector3(0.34,0.34,0.34),2,0.5);
		createSingleVehicleR(g_mainMgr,"car19","car15.mesh",Ogre::Vector3(0.17,0.17,0.17),4,0.5);

		createSingleVehicleR(g_mainMgr,"car20","car11.mesh",Ogre::Vector3(0.34,0.34,0.34),8,0.5,2);
		createSingleVehicleR(g_mainMgr,"car21","car2.mesh",Ogre::Vector3(0.34,0.34,0.34),10,0.5,1,"chuzu032");
		createSingleVehicleR(g_mainMgr,"car22","car23.mesh",Ogre::Vector3(0.17,0.17,0.17),12,0.5);
		createSingleVehicleR(g_mainMgr,"car23","car8.mesh",Ogre::Vector3(0.34,0.34,0.34),14,0.5);
		createSingleVehicleR(g_mainMgr,"car24","car2.mesh",Ogre::Vector3(0.34,0.34,0.34),16,0.5,1,"chuzu035");

		createSingleVehicleR(g_mainMgr,"carOuter1","car18.mesh",Ogre::Vector3(0.17,0.17,0.17),18,0.5);
		createSingleVehicleR(g_mainMgr,"carOuter2","car16.mesh",Ogre::Vector3(0.17,0.17,0.17),20,0.5);
		createSingleVehicleR(g_mainMgr,"carOuter3","car15.mesh",Ogre::Vector3(0.17,0.17,0.17),22,0.5);
		createSingleVehicleR(g_mainMgr,"carOuter4","car22.mesh",Ogre::Vector3(0.17,0.17,0.17),24,0.5);
		createSingleVehicleR(g_mainMgr,"carOuter1r","car13.mesh",Ogre::Vector3(0.34,0.34,0.34),26,0.5);
		createSingleVehicleR(g_mainMgr,"carOuter2r","car8.mesh",Ogre::Vector3(0.34,0.34,0.34),28,0.5);
		createSingleVehicleR(g_mainMgr,"carOuter3r","car11.mesh",Ogre::Vector3(0.34,0.34,0.34),30,0.5,2);
		createSingleVehicleR(g_mainMgr,"carOuter4r","car7.mesh",Ogre::Vector3(0.34,0.34,0.34),31,0.5,2);
	}
};


#endif