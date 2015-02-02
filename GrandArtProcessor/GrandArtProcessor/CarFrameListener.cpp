#include "CarFrameListener.h"
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


GAVehicleAI *v1;
float translateLength=1;
float g_signTime=0;
bool start=false;
GARollPoint *rollPoint;
GAEventBase* eventBase;
GACharacterAIManager *g_AIMgr;
GARoadCorner* corner;
float sparkingTime=0;
float sparkingTotalTime=0;
bool sparkingShow=true;
bool showSign=false;
UINT crossedLineNo,signStep;
float maxSignSpeed=0;
float signSpeedTotalTime=30;
float signSpeedTime=0;
UINT iCarToFollow=0;

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

void createSingleVehicle(GAPMainManager* g_mainMgr,std::string name,std::string meshName,Ogre::Vector3 scale,std::string road_txt,UINT facingDir=1,std::string materialName="")
{
	g_mainMgr->g_worldMgr->addModel(name,meshName,Ogre::Vector3(0,0,10),scale);
	if(materialName!="") g_mainMgr->g_modelMgr->setModelMaterial(name,materialName);
	g_AIMgr->createAIVehicle(name,facingDir,2,road_txt);
}

void createVehicle(GAPMainManager* g_mainMgr)
{
	createSingleVehicle(g_mainMgr,"car0","car2.mesh",Ogre::Vector3(0.34,0.34,0.34),"vehicleRoad\\road0.txt",1,"chuzu031");
	createSingleVehicle(g_mainMgr,"car1","car2.mesh",Ogre::Vector3(0.34,0.34,0.34),"vehicleRoad\\road1.txt",1,"chuzu032");
	createSingleVehicle(g_mainMgr,"car2","car2.mesh",Ogre::Vector3(0.34,0.34,0.34),"vehicleRoad\\road2.txt");
	createSingleVehicle(g_mainMgr,"car3","car2.mesh",Ogre::Vector3(0.34,0.34,0.34),"vehicleRoad\\road3.txt",1,"chuzu034");
	createSingleVehicle(g_mainMgr,"car4","car2.mesh",Ogre::Vector3(0.34,0.34,0.34),"vehicleRoad\\road4.txt",1,"chuzu035");

	createSingleVehicle(g_mainMgr,"car5","car23.mesh",Ogre::Vector3(0.17,0.17,0.17),"vehicleRoad\\road5.txt");
	createSingleVehicle(g_mainMgr,"car6","car18.mesh",Ogre::Vector3(0.17,0.17,0.17),"vehicleRoad\\road6.txt");
	createSingleVehicle(g_mainMgr,"car7","car17.mesh",Ogre::Vector3(0.17,0.17,0.17),"vehicleRoad\\road7.txt");
	createSingleVehicle(g_mainMgr,"car8","car13.mesh",Ogre::Vector3(0.34,0.34,0.34),"vehicleRoad\\road8.txt");
	createSingleVehicle(g_mainMgr,"car9","car5.mesh",Ogre::Vector3(0.34,0.34,0.34),"vehicleRoad\\road9.txt",2);

	createSingleVehicle(g_mainMgr,"car10","car23.mesh",Ogre::Vector3(0.17,0.17,0.17),"vehicleRoad\\road10.txt");
	createSingleVehicle(g_mainMgr,"car11","car22.mesh",Ogre::Vector3(0.17,0.17,0.17),"vehicleRoad\\road11.txt");
	createSingleVehicle(g_mainMgr,"car12","car20.mesh",Ogre::Vector3(0.17,0.17,0.17),"vehicleRoad\\road12.txt");
	createSingleVehicle(g_mainMgr,"car13","car5.mesh",Ogre::Vector3(0.34,0.34,0.34),"vehicleRoad\\road13.txt",2);
	createSingleVehicle(g_mainMgr,"car14","car10.mesh",Ogre::Vector3(0.34,0.34,0.34),"vehicleRoad\\road14.txt",2);

	createSingleVehicle(g_mainMgr,"car15","car6.mesh",Ogre::Vector3(0.34,0.34,0.34),"vehicleRoad\\road15.txt",2);
	createSingleVehicle(g_mainMgr,"car16","car18.mesh",Ogre::Vector3(0.17,0.17,0.17),"vehicleRoad\\road16.txt");
	createSingleVehicle(g_mainMgr,"car17","car19.mesh",Ogre::Vector3(0.17,0.17,0.17),"vehicleRoad\\road17.txt");
	createSingleVehicle(g_mainMgr,"car18","car8.mesh",Ogre::Vector3(0.34,0.34,0.34),"vehicleRoad\\road18.txt");
	createSingleVehicle(g_mainMgr,"car19","car15.mesh",Ogre::Vector3(0.17,0.17,0.17),"vehicleRoad\\road19.txt");

	createSingleVehicle(g_mainMgr,"car20","car11.mesh",Ogre::Vector3(0.34,0.34,0.34),"vehicleRoad\\road20.txt",2);
	createSingleVehicle(g_mainMgr,"car21","car2.mesh",Ogre::Vector3(0.34,0.34,0.34),"vehicleRoad\\road21.txt",1,"chuzu032");
	createSingleVehicle(g_mainMgr,"car22","car23.mesh",Ogre::Vector3(0.17,0.17,0.17),"vehicleRoad\\road22.txt");
	createSingleVehicle(g_mainMgr,"car23","car8.mesh",Ogre::Vector3(0.34,0.34,0.34),"vehicleRoad\\road23.txt");
	createSingleVehicle(g_mainMgr,"car24","car2.mesh",Ogre::Vector3(0.34,0.34,0.34),"vehicleRoad\\road24.txt",1,"chuzu035");

	createSingleVehicle(g_mainMgr,"carOuter1","car18.mesh",Ogre::Vector3(0.17,0.17,0.17),"vehicleRoad\\outer1auto.txt");
	createSingleVehicle(g_mainMgr,"carOuter2","car16.mesh",Ogre::Vector3(0.17,0.17,0.17),"vehicleRoad\\outer2auto.txt");
	createSingleVehicle(g_mainMgr,"carOuter3","car15.mesh",Ogre::Vector3(0.17,0.17,0.17),"vehicleRoad\\outer3auto.txt");
	createSingleVehicle(g_mainMgr,"carOuter4","car22.mesh",Ogre::Vector3(0.17,0.17,0.17),"vehicleRoad\\outer4auto.txt");
	createSingleVehicle(g_mainMgr,"carOuter1r","car13.mesh",Ogre::Vector3(0.34,0.34,0.34),"vehicleRoad\\outer1auto_r.txt");
	createSingleVehicle(g_mainMgr,"carOuter2r","car8.mesh",Ogre::Vector3(0.34,0.34,0.34),"vehicleRoad\\outer2auto_r.txt");
	createSingleVehicle(g_mainMgr,"carOuter3r","car11.mesh",Ogre::Vector3(0.34,0.34,0.34),"vehicleRoad\\outer3auto_r.txt",2);
	createSingleVehicle(g_mainMgr,"carOuter4r","car7.mesh",Ogre::Vector3(0.34,0.34,0.34),"vehicleRoad\\outer4auto_r.txt",2);
}

CarFrameListener::CarFrameListener(Ogre::RenderWindow *win,Ogre::Camera* cam,GAPMainManager* mainMgr)
	: ExampleFrameListener(win,cam,true,true,true)
{
	g_mainMgr=mainMgr;
	mKeyboard->setEventCallback(this);
	mMouse->setEventCallback(this);
	if(mJoy)
	{
		try{
			mJoy->setEventCallback(this);
		}catch(...)
		{
			mJoy=0;
		}
	}
	mCamera->setNearClipDistance(0.1);
	g_exit=false;
	g_totalTime=g_lastTime=0;
	wtf=new MyWriteToFile("frameListenerOutput.txt");
	g_mainMgr->g_cameraCtrl->setCameraStyle(GAPCameraController::CameraStyle_Free);
	GAPMisc misc;
	std::string name="car_test";
	g_mainMgr->g_worldMgr->addModel(name,"car5.mesh",Ogre::Vector3(0,0,10),Ogre::Vector3(0.34,0.34,0.34));
	GAVehicle *ve=new GAVehicle(g_mainMgr->getCurrentSceneMgr(),name);
	ve->setDirs(2,5);
	g_mainMgr->g_characterCtrl->setCharacter(ve);
	Ogre::SceneNode* stayNode=ve->getCharacterSceneNode()->createChildSceneNode("stayNode1",Ogre::Vector3(0,7,0));
	g_mainMgr->g_cameraCtrl->setCameraStyle(GAPCameraController::CameraStyle_StayOnNode,stayNode);
	g_mainMgr->g_cameraCtrl->rotateCameraNode(90,2);
	g_mainMgr->g_cameraCtrl->setStayOnNodeMaxMinPitchYaw(90,-90,180,-180);

	g_AIMgr=new GACharacterAIManager(mainMgr);
	createVehicle(g_mainMgr);
	
	Ogre::SceneNode* sign=ve->getCharacterSceneNode()->createChildSceneNode("sign",Ogre::Vector3(-60,20,0));
	sign->setScale(10,10,10);
	sign->yaw(Ogre::Degree(90),Ogre::Node::TS_PARENT);
	Ogre::Entity* ent=g_mainMgr->getCurrentSceneMgr()->createEntity("sign","sign1.mesh");
	ent->setMaterialName("stop");
	sign->attachObject(ent);
	sign->setVisible(false);

	rollPoint=new GARollPoint(2);
	corner=new GARoadCorner();
	corner->addCornerAutoForm1("roadCorner.txt");
	crossedLineNo=0;
	signStep=0;
	g_mainMgr->g_characterCtrl->setKey(6,GAP_KEY_C);
}

bool CarFrameListener::frameStarted(const FrameEvent &evt)
{
	g_lastTime=16-(evt.timeSinceLastFrame*1000-g_lastTime);
	int lastt=g_lastTime;
	if(lastt>=0)  Sleep(lastt);
	else g_lastTime=0;
	if(mJoy!=0)
	{
		mJoy->capture();
	}
	mMouse->capture();
	mKeyboard->capture();
	g_mainMgr->preUpdate(evt.timeSinceLastFrame);
	g_AIMgr->update(evt.timeSinceLastFrame);

	GAPMisc misc;
	mDebugText=misc.ConvertToString(g_mainMgr->g_characterCtrl->getCurrentSpeed());
	createRollSign(g_mainMgr,evt.timeSinceLastFrame);
	return ExampleFrameListener::frameStarted(evt);
}

bool CarFrameListener::frameEnded(const FrameEvent& evt)
{
	if(g_exit) return false;
	g_totalTime+=evt.timeSinceLastFrame;
	g_mainMgr->afterUpdate(evt.timeSinceLastFrame);
	return ExampleFrameListener::frameEnded(evt);
}

bool CarFrameListener::keyPressed(const OIS::KeyEvent& evt)
{
	g_mainMgr->injectKeyDown(evt.key);
	return true;
}

bool CarFrameListener::keyReleased(const OIS::KeyEvent& evt)
{
	if(evt.key==OIS::KC_ESCAPE)
	{
		g_exit=true;
	}
	else if(evt.key==OIS::KC_T)
	{
		g_mainMgr->g_cameraCtrl->setAutoPitch(100,15);
	}
	else if(evt.key==OIS::KC_R)
	{
		g_mainMgr->g_cameraCtrl->setAutoPitch(-100,-15);
	}
	else if(evt.key==OIS::KC_F1)
	{
		g_mainMgr->g_cameraCtrl->setCameraStyle(GAPCameraController::CameraStyle_Free);
	}
	else if(evt.key==OIS::KC_F2)
	{
		g_mainMgr->g_cameraCtrl->setCameraStyle(GAPCameraController::CameraStyle_StayOnNode,g_mainMgr->getSceneNode("stayNode1"));
	}
	else if(evt.key==OIS::KC_F3)
	{
		g_mainMgr->g_cameraCtrl->setCameraStyle(GAPCameraController::CameraStyle_AroundNode,g_mainMgr->getSceneNode("stayNode1"));
	}
	else if(evt.key==OIS::KC_F4)
	{
		start=!start;
	}
	else if(evt.key==OIS::KC_UP)
	{
		iCarToFollow++;
		if(iCarToFollow>24) iCarToFollow=0;
	}
	else if(evt.key==OIS::KC_DOWN)
	{
		iCarToFollow--;
		if(iCarToFollow<0) iCarToFollow=24;
	}
	else if(evt.key==OIS::KC_LEFT)
	{
	}
	else if(evt.key==OIS::KC_RIGHT)
	{
	}
	else if(evt.key==OIS::KC_NUMPAD1)
	{
		translateLength*=2;
	}
	else if(evt.key==OIS::KC_NUMPAD2)
	{
		translateLength/=2;
	}
	else if(evt.key==OIS::KC_U)
	{
		GAPMisc misc;
		std::string str="car"+misc.ConvertToString(iCarToFollow);
		Ogre::Vector3 pos=g_AIMgr->getAIVehicle(str)->g_recorder->getFormerPosition(3);
		g_mainMgr->getCurrentSceneMgr()->getSceneNode("car_test")->setPosition(pos);
	}
	g_mainMgr->injectKeyUp(evt.key);
	return true;
}

bool CarFrameListener::mouseMoved(const OIS::MouseEvent& evt)
{
	g_mainMgr->g_cameraCtrl->updateRotate(evt.state.X.rel*(-0.1f),evt.state.Y.rel*(-0.1f));
	return true;
}

bool CarFrameListener::mousePressed(const OIS::MouseEvent& evt, OIS::MouseButtonID id)
{
	return true;
}
bool CarFrameListener::mouseReleased(const OIS::MouseEvent& evt,OIS::MouseButtonID id)
{
	return true;
}
bool CarFrameListener::buttonPressed(const OIS::JoyStickEvent &evt,int num)
{
	return true;
}
bool CarFrameListener::buttonReleased(const OIS::JoyStickEvent &evt,int num)
{
	return true;
}
bool CarFrameListener::axisMoved(const OIS::JoyStickEvent &evt,int num)
{
	return true;
}
bool CarFrameListener::povMoved(const OIS::JoyStickEvent &evt,int num)
{
	return true;
}
bool CarFrameListener::sliderMoved(const OIS::JoyStickEvent &evt,int num)
{
	return true;
}
