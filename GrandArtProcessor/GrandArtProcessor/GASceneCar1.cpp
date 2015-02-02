#include "GASceneCar1.h"
#include "GARoadLine.h"

GASceneCar1::GASceneCar1(GAPMainManager *mainMgr,std::string sceneName) : GASceneBase(mainMgr,sceneName)
{
	sparkingTime=0;
	sparkingTotalTime=0;
	sparkingShow=true;
	showSign=false;
	translateLength=1;
	g_signTime=0;
	start=false;
	maxSignSpeed=0;
	signSpeedTotalTime=30;
	signSpeedTime=0;
	iCarToFollow=0;
}

bool GASceneCar1::createScene()
{
	Ogre::SceneManager* sm=Ogre::Root::getSingleton().createSceneManager("BspSceneManager","car");
	Ogre::Camera* cam=sm->createCamera("car1");
    cam->setNearClipDistance(1);
	g_mainMgr->setNewSceneManager(sm,cam);
	GAPCameraController *newCamc=new GAPCameraController(g_mainMgr);
	newCamc->init();
	g_mainMgr->g_cameraCtrl=newCamc;
	g_mainMgr->mWindow->removeAllViewports();
	Ogre::Viewport* vp=g_mainMgr->mWindow->addViewport(cam);
	cam->setAspectRatio((float)vp->getActualWidth()/(float)vp->getActualHeight());
	//cam->setFOVy(Ogre::Degree(60));
	g_worldName="city2_step6.bsp";
	sm->setSkyBox(true,"Examples/MorningSkyBox",2500);
	sm->getSkyBoxNode()->pitch(Ogre::Degree(90));
	sm->setWorldGeometry(g_worldName);

	
	GAPMisc misc;
	std::string name="car_test";
	g_mainMgr->g_worldMgr->addModel(name,"car5.mesh",Ogre::Vector3(0,0,10),Ogre::Vector3(0.34,0.34,0.34));
	GAVehicle *ve=new GAVehicle(g_mainMgr->getCurrentSceneMgr(),name);
	ve->setDirs(2,5);
	g_mainMgr->g_characterCtrl->setCharacter(ve);
	Ogre::SceneNode* stayNode=ve->getCharacterSceneNode()->createChildSceneNode("stayNode1",Ogre::Vector3(0,7,0));
	g_mainMgr->g_cameraCtrl->setCameraStyle(GAPCameraController::CameraStyle_StayOnNode,stayNode);
	g_mainMgr->g_cameraCtrl->setStayOnNodeMaxMinPitchYaw(90,-90,180,-180);
	g_mainMgr->g_cameraCtrl->rotateCameraNode(90,2);
	g_AIMgr=new GACharacterAIManager(g_mainMgr);
	g_mainMgr->g_worldMgr->loadSceneFromFile("signPos.txt");

	g_aiVehicleLine=new GARoadLine();
	g_aiVehicleLine->initRoad("1111.txt");
	g_aiVehicleLineR=new GARoadLine();
	g_aiVehicleLineR->initRoad("1111r.txt");
	createVehicle(g_mainMgr);
	createVehicleR(g_mainMgr);
	//createSingleVehicle(g_mainMgr,"car5","car23.mesh",Ogre::Vector3(0.17,0.17,0.17),0,0.5);
	//g_AIMgr->getAIVehicle("car5")->setVehicleSpeed(100,30);
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
	return true;
}

bool GASceneCar1::frameStarted(float deltaTime)
{
	g_AIMgr->checkAndDispatchCollide(g_mainMgr->g_characterCtrl->g_vehicle->getCharacterSceneNode()->_getDerivedPosition());
	g_AIMgr->update(deltaTime);
	//createRollSign(g_mainMgr,deltaTime);
	return true;
}

bool GASceneCar1::frameEnded(float deltaTime)
{
	return true;
}

bool GASceneCar1::keyPressed(const OIS::KeyEvent& evt)
{
	return true;
}

bool GASceneCar1::keyReleased(const OIS::KeyEvent& evt)
{
	if(evt.key==OIS::KC_T)
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
		//g_mainMgr->g_cameraCtrl->rotateCameraNode(90,2);
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
	else if(evt.key==OIS::KC_U)
	{
		GAPMisc misc;
		std::string str="car"+misc.ConvertToString(iCarToFollow);
		Ogre::Vector3 pos=g_AIMgr->getAIVehicle(str)->g_recorder->getFormerPosition(3);
		g_mainMgr->getCurrentSceneMgr()->getSceneNode("car_test")->setPosition(pos);
	}
	return true;
}