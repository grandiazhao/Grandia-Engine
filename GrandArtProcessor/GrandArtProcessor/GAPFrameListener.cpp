#include "GAPFrameListener.h"
#include "GAPCameraController.h"
#include "GAPWorldManager.h"
#include "GAPPhysicsManager.h"
#include "GAPCharacterController.h"
#include "GAPModelManager.h"
#include "GAPModel.h"

GAPModel *ogrehead;
float transDelta=0.2;
std::string moveName="";
bool b1=false,b2=false,b3=false,b4=false,b5=false;
bool runBeiziAni=false;
float endTime=0;
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


GAPFrameListener::GAPFrameListener(Ogre::RenderWindow *win,Ogre::Camera* cam,GAPMainManager* mainMgr)
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
	g_exit=false;
	g_totalTime=g_lastTime=0;
	wtf=new MyWriteToFile("frameListenerOutput.txt");
	//g_mainMgr->g_worldMgr->addModel("ogrehead","ogrehead.mesh",Ogre::Vector3(0,0,100));
	g_mainMgr->g_worldMgr->addModel("man","cylinderMan.mesh",Ogre::Vector3(-100,-500,90));
	g_mainMgr->g_characterCtrl->setCharacter(g_mainMgr->getCurrentSceneMgr()->getSceneNode("man"));
	Ogre::SceneNode* stayNode=g_mainMgr->getCurrentSceneMgr()->getSceneNode("man")->createChildSceneNode("manEye",Ogre::Vector3(0,80,0));
	g_mainMgr->g_worldMgr->addModel("mandrink","man5.mesh",Ogre::Vector3(-541.421,-383.508,101.897));
	Ogre::SceneNode* stayNode2=g_mainMgr->getCurrentSceneMgr()->getSceneNode("mandrink")->createChildSceneNode("mandrinkEye",Ogre::Vector3(0,33,2.4));
	stayNode2->yaw(Ogre::Degree(-90),Ogre::Node::TS_PARENT);
	g_mainMgr->g_cameraCtrl->setCameraStyle(GAPCameraController::CameraStyle_StayOnNode,stayNode2);
	g_mainMgr->g_worldMgr->addModel("zhuozi1.mesh","zhuozi1.mesh",Ogre::Vector3(-576.986,-446.033,50.3207));
	g_mainMgr->g_worldMgr->addModel("beizi3.mesh","beizi3.mesh",Ogre::Vector3(-562.127,-435.184,102.1));
	g_mainMgr->getCurrentSceneMgr()->getSceneNode("beizi3.mesh")->setScale(1.2,1.7,1.2);
	g_mainMgr->g_worldMgr->addModel("yizi1.mesh","yizi1.mesh",Ogre::Vector3(-543.286,-381.524,50));
	g_mainMgr->g_worldMgr->addModel("ogrehead","ogrehead.mesh",Ogre::Vector3(0,0,100));
	ogrehead=new GAPModel(g_mainMgr->getCurrentSceneMgr()->getSceneNode("beizi3.mesh"));
	ogrehead->addToRoadVec("timepos.txt");
	Ogre::Light* l=g_mainMgr->getCurrentSceneMgr()->createLight();
	l->setPosition(-600,-500,200);
	Ogre::Light* l2=g_mainMgr->getCurrentSceneMgr()->createLight();
	l2->setPosition(-200,-500,200);
	g_mainMgr->getCurrentSceneMgr()->setAmbientLight(Ogre::ColourValue(0.9,0.9,0.9));
	g_mainMgr->g_modelMgr->runModelAnimation("mandrink","right",true);
	g_mainMgr->g_modelMgr->pauseModelAnimation();
	moveName="beizi3.mesh";
	g_mainMgr->g_characterCtrl->setKey(1,GAP_KEY_Q);
	g_mainMgr->g_characterCtrl->setKey(2,GAP_KEY_Z);
	g_mainMgr->g_characterCtrl->setKey(3,GAP_KEY_UNDEFINED);
	g_mainMgr->g_characterCtrl->setKey(4,GAP_KEY_UNDEFINED);
	g_mainMgr->g_cameraCtrl->rotateCameraNode(180,2);
 	g_mainMgr->g_cameraCtrl->updateRotate(-20.f,-44.f);
	createOcean(g_mainMgr->getCurrentSceneMgr());
}

bool GAPFrameListener::frameStarted(const FrameEvent &evt)
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
	g_mainMgr->g_physicsMgr->update(g_mainMgr->getCurrentSceneMgr()->getSceneNode("man"));
	if(g_mainMgr->g_keyboardInput->isKeyDown(GAP_KEY_A))
	{
		g_mainMgr->g_cameraCtrl->updateRotate(-10*(-0.1f),0);
	}
	else if(g_mainMgr->g_keyboardInput->isKeyDown(GAP_KEY_D))
	{
		g_mainMgr->g_cameraCtrl->updateRotate(10*(-0.1f),0);
	}
	if(g_mainMgr->g_keyboardInput->isKeyDown(GAP_KEY_W))
	{
		g_mainMgr->g_cameraCtrl->updateRotate(0,-10*(-0.1f));
	}
	else if(g_mainMgr->g_keyboardInput->isKeyDown(GAP_KEY_S))
	{
		g_mainMgr->g_cameraCtrl->updateRotate(0,10*(-0.1f));
	}

	runBeiziAniFunc(g_mainMgr);
	if(g_mainMgr->g_modelMgr->getAnimationTimePos()>3.19) 
	{
		runBeiziAni=false;
		g_mainMgr->g_modelMgr->setAnimationTimePos(0);
		ogrehead->resetRidingOnRoad();
		b1=b2=b3=b4=b5=false;
		endTime=0;
	}
	else if(g_mainMgr->g_modelMgr->getAnimationTimePos()>1.68)
	{
		if(runBeiziAni==false) ogrehead->resetRidingOnRoad();
		runBeiziAni=true;
	}
	if(runBeiziAni)
	{
		ogrehead->updateOnCertainTime(g_mainMgr->g_modelMgr->getAnimationTimePos());
	}

	return ExampleFrameListener::frameStarted(evt);
}

bool GAPFrameListener::frameEnded(const FrameEvent& evt)
{
	if(g_exit) return false;
	g_totalTime+=evt.timeSinceLastFrame;
	g_mainMgr->afterUpdate(evt.timeSinceLastFrame);
	return ExampleFrameListener::frameEnded(evt);
}

bool GAPFrameListener::keyPressed(const OIS::KeyEvent& evt)
{
	g_mainMgr->injectKeyDown(evt.key);
	return true;
}

bool GAPFrameListener::keyReleased(const OIS::KeyEvent& evt)
{
	if(evt.key==OIS::KC_ESCAPE)
	{
		g_exit=true;
	}
	else if(evt.key==OIS::KC_1)
	{
		b1=!b1;
	}
	else if(evt.key==OIS::KC_2)
	{
		b2=!b2;
	}
	else if(evt.key==OIS::KC_3)
	{
		b3=!b3;
	}
	else if(evt.key==OIS::KC_4)
	{
		b4=!b4;
	}
	else if(evt.key==OIS::KC_5)
	{
		b5=!b5;
	}
	else if(evt.key==OIS::KC_6)
	{
		runBeiziAni=false;
		g_mainMgr->g_modelMgr->setAnimationTimePos(0);
		ogrehead->resetRidingOnRoad();
		b1=b2=b3=b4=b5=false;
		endTime=0;
	}
	else if(evt.key==OIS::KC_F1)
	{
		g_mainMgr->g_cameraCtrl->setCameraStyle(GAPCameraController::CameraStyle_Free);
	}
	else if(evt.key==OIS::KC_F2)
	{
		g_mainMgr->g_cameraCtrl->setCameraStyle(GAPCameraController::CameraStyle_StayOnNode,g_mainMgr->getCurrentSceneMgr()->getSceneNode("mandrinkEye"));
		g_mainMgr->g_cameraCtrl->setStayOnNodeMaxMinPitchYaw(30,-27,40,-40);
		g_mainMgr->g_cameraCtrl->rotateCameraNode(180,2);
 		g_mainMgr->g_cameraCtrl->updateRotate(-20.f,-44.f);
	}
	else if(evt.key==OIS::KC_F3)
	{
		g_mainMgr->g_cameraCtrl->setCameraStyle(GAPCameraController::CameraStyle_StayOnNode,g_mainMgr->getCurrentSceneMgr()->getSceneNode("manEye"));
		g_mainMgr->g_cameraCtrl->setStayOnNodeMaxMinPitchYaw(30,-27,-1,1);
	}
	else if(evt.key==OIS::KC_R)
	{
		g_mainMgr->g_modelMgr->setAnimationTimePos(g_mainMgr->g_modelMgr->getAnimationTimePos()+0.01);
	}
	else if(evt.key==OIS::KC_F)
	{
		g_mainMgr->g_modelMgr->setAnimationTimePos(g_mainMgr->g_modelMgr->getAnimationTimePos()-0.01);
	}
	else if(evt.key==OIS::KC_T)
	{
		GAPMisc misc;
		std::string str=misc.ConvertToString(g_mainMgr->g_modelMgr->getAnimationTimePos())+",";
		Ogre::Vector3 vec=g_mainMgr->getCurrentSceneMgr()->getSceneNode(moveName)->_getDerivedPosition();
		str+=misc.ConvertToString(vec.x)+",";
		str+=misc.ConvertToString(vec.y)+",";
		str+=misc.ConvertToString(vec.z);
		wtf->write(str);
	}
	else if(evt.key==OIS::KC_G)
	{
		if(g_mainMgr->g_modelMgr->runningAnimation()==1) g_mainMgr->g_modelMgr->restartModelAnimation();
		else if(g_mainMgr->g_modelMgr->runningAnimation()==2) g_mainMgr->g_modelMgr->pauseModelAnimation();
	}
	else if(evt.key==OIS::KC_H)
	{
		g_mainMgr->g_modelMgr->runModelAnimation("mandrink","left",true);
	}
	else if(evt.key==OIS::KC_J)
	{
		g_mainMgr->g_modelMgr->runModelAnimation("mandrink","right",true);
	}
	else if(evt.key==OIS::KC_UP)
	{
		g_mainMgr->getCurrentSceneMgr()->getSceneNode(moveName)->translate(0,0,transDelta,Ogre::Node::TS_WORLD);
	}
	else if(evt.key==OIS::KC_DOWN)
	{
		g_mainMgr->getCurrentSceneMgr()->getSceneNode(moveName)->translate(0,0,-transDelta,Ogre::Node::TS_WORLD);
	}
	else if(evt.key==OIS::KC_LEFT)
	{
		g_mainMgr->getCurrentSceneMgr()->getSceneNode(moveName)->translate(0,transDelta,0,Ogre::Node::TS_WORLD);
	}
	else if(evt.key==OIS::KC_RIGHT)
	{
		g_mainMgr->getCurrentSceneMgr()->getSceneNode(moveName)->translate(0,-transDelta,0,Ogre::Node::TS_WORLD);
	}
	else if(evt.key==OIS::KC_NUMPAD1)
	{
		g_mainMgr->getCurrentSceneMgr()->getSceneNode(moveName)->translate(transDelta,0,0,Ogre::Node::TS_WORLD);
	}
	else if(evt.key==OIS::KC_NUMPAD2)
	{
		g_mainMgr->getCurrentSceneMgr()->getSceneNode(moveName)->translate(-transDelta,0,0,Ogre::Node::TS_WORLD);
	}
	g_mainMgr->injectKeyUp(evt.key);
	return true;
}

bool GAPFrameListener::mouseMoved(const OIS::MouseEvent& evt)
{
	g_mainMgr->g_cameraCtrl->updateRotate(evt.state.X.rel*(-0.1f),evt.state.Y.rel*(-0.1f));
	return true;
}

bool GAPFrameListener::mousePressed(const OIS::MouseEvent& evt, OIS::MouseButtonID id)
{
	return true;
}
bool GAPFrameListener::mouseReleased(const OIS::MouseEvent& evt,OIS::MouseButtonID id)
{
	return true;
}
bool GAPFrameListener::buttonPressed(const OIS::JoyStickEvent &evt,int num)
{
	return true;
}
bool GAPFrameListener::buttonReleased(const OIS::JoyStickEvent &evt,int num)
{
	return true;
}
bool GAPFrameListener::axisMoved(const OIS::JoyStickEvent &evt,int num)
{
	return true;
}
bool GAPFrameListener::povMoved(const OIS::JoyStickEvent &evt,int num)
{
	return true;
}
bool GAPFrameListener::sliderMoved(const OIS::JoyStickEvent &evt,int num)
{
	return true;
}
