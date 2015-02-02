#include "GASceneKangFu0.h"
#include "GAOgreUIManager.h"


Ogre::Matrix4 GASceneKangFu0::concatenate2(Ogre::Matrix4 &m,Ogre::Matrix4 &m2)
{
    Ogre::Matrix4 r;
    r.m[0][0] = m[0][0] * m2.m[0][0] + m[0][1] * m2.m[1][0] + m[0][2] * m2.m[2][0] + m[0][3] * m2.m[3][0];
    r.m[0][1] = m[0][0] * m2.m[0][1] + m[0][1] * m2.m[1][1] + m[0][2] * m2.m[2][1] + m[0][3] * m2.m[3][1];
    r.m[0][2] = m[0][0] * m2.m[0][2] + m[0][1] * m2.m[1][2] + m[0][2] * m2.m[2][2] + m[0][3] * m2.m[3][2];
    r.m[0][3] = m[0][0] * m2.m[0][3] + m[0][1] * m2.m[1][3] + m[0][2] * m2.m[2][3] + m[0][3] * m2.m[3][3];

    r.m[1][0] = m[1][0] * m2.m[0][0] + m[1][1] * m2.m[1][0] + m[1][2] * m2.m[2][0] + m[1][3] * m2.m[3][0];
    r.m[1][1] = m[1][0] * m2.m[0][1] + m[1][1] * m2.m[1][1] + m[1][2] * m2.m[2][1] + m[1][3] * m2.m[3][1];
    r.m[1][2] = m[1][0] * m2.m[0][2] + m[1][1] * m2.m[1][2] + m[1][2] * m2.m[2][2] + m[1][3] * m2.m[3][2];
    r.m[1][3] = m[1][0] * m2.m[0][3] + m[1][1] * m2.m[1][3] + m[1][2] * m2.m[2][3] + m[1][3] * m2.m[3][3];

    r.m[2][0] = m[2][0] * m2.m[0][0] + m[2][1] * m2.m[1][0] + m[2][2] * m2.m[2][0] + m[2][3] * m2.m[3][0];
    r.m[2][1] = m[2][0] * m2.m[0][1] + m[2][1] * m2.m[1][1] + m[2][2] * m2.m[2][1] + m[2][3] * m2.m[3][1];
    r.m[2][2] = m[2][0] * m2.m[0][2] + m[2][1] * m2.m[1][2] + m[2][2] * m2.m[2][2] + m[2][3] * m2.m[3][2];
    r.m[2][3] = m[2][0] * m2.m[0][3] + m[2][1] * m2.m[1][3] + m[2][2] * m2.m[2][3] + m[2][3] * m2.m[3][3];

    r.m[3][0] = m[3][0] * m2.m[0][0] + m[3][1] * m2.m[1][0] + m[3][2] * m2.m[2][0] + m[3][3] * m2.m[3][0];
    r.m[3][1] = m[3][0] * m2.m[0][1] + m[3][1] * m2.m[1][1] + m[3][2] * m2.m[2][1] + m[3][3] * m2.m[3][1];
    r.m[3][2] = m[3][0] * m2.m[0][2] + m[3][1] * m2.m[1][2] + m[3][2] * m2.m[2][2] + m[3][3] * m2.m[3][2];
    r.m[3][3] = m[3][0] * m2.m[0][3] + m[3][1] * m2.m[1][3] + m[3][2] * m2.m[2][3] + m[3][3] * m2.m[3][3];

    return r;
}

GASceneKangFu0::GASceneKangFu0(GAPMainManager* mainMgr,std::string sceneName) : GASceneBase(mainMgr,sceneName)
{
	transDelta=0.2;
	moveName="";
	b1=b2=b3=b4=b5=runBeiziAni=false;
	endTime=0;
}

bool GASceneKangFu0::createScene()
{
	Ogre::SceneManager* sm=Ogre::Root::getSingleton().createSceneManager("BspSceneManager","kangfu0");
	Ogre::Camera* cam=sm->createCamera("chabei");
	cam->setPosition(0,0,100);
    cam->lookAt(0,0,-300);
    cam->setNearClipDistance(5);
	g_mainMgr->setNewSceneManager(sm,cam);
	GAPCameraController *newCamc=new GAPCameraController(g_mainMgr);
	newCamc->init();
	g_mainMgr->g_cameraCtrl=newCamc;
	g_mainMgr->mWindow->removeAllViewports();
	Ogre::Viewport* vp=g_mainMgr->mWindow->addViewport(cam);
	cam->setAspectRatio((float)vp->getActualWidth()/(float)vp->getActualHeight());
	g_worldName="house1_4.bsp";
	sm->setWorldGeometry(g_worldName);
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
	sm->setSkyBox(true,"Examples/MorningSkyBox",2500);
	sm->getSkyBoxNode()->pitch(Ogre::Degree(90));
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
	Ogre::Skeleton* ske1=sm->getEntity("mandrink")->getSkeleton();
	ske=ske1;
	bone=ske1->getBone("m6Bip R Hand");
	Ogre::Matrix4 mat=bone->_getFullTransform();
	return true;
}

bool GASceneKangFu0::frameStarted(float deltaTime)
{
	g_mainMgr->g_physicsMgr->update(g_mainMgr->getCurrentSceneMgr()->getSceneNode("man"));
	runBeiziAniFunc(g_mainMgr);
	Ogre::Vector3 vec=bone->_getDerivedPosition();
	Ogre::Matrix4 mat=bone->_getFullTransform();
	Ogre::Matrix4 mat2(1,0,0,0,0,0,-1,0,0,1,0,0,0,0,0,1);
	Ogre::Matrix4 mat3=concatenate2(mat2,mat);
	vec=Ogre::Vector3(-541.421+vec.x,-383.508-vec.z,101.897+vec.y);
	Ogre::Vector3 vec2=mat3.getTrans()+Ogre::Vector3(-541.421,-383.508,101.897);
	Ogre::SceneNode* node=g_mainMgr->getCurrentSceneMgr()->getSceneNode("beizi3.mesh");
	node->setPosition(vec2);
	node->setOrientation(mat3.extractQuaternion());
	if(g_mainMgr->g_modelMgr->getAnimationTimePos()>3.19) 
	{
		runBeiziAni=false;
		g_mainMgr->g_modelMgr->setAnimationTimePos(0);
		//ogrehead->resetRidingOnRoad();
		b1=b2=b3=b4=b5=false;
		endTime=0;
	}
	else if(g_mainMgr->g_modelMgr->getAnimationTimePos()>1.68)
	{
		//if(runBeiziAni==false) ogrehead->resetRidingOnRoad();
		//runBeiziAni=true;
	}
	/*if(runBeiziAni)
	{
		ogrehead->updateOnCertainTime(g_mainMgr->g_modelMgr->getAnimationTimePos());
	}*/
	return true;
}

bool GASceneKangFu0::frameEnded(float deltaTime)
{

	return true;
}

bool GASceneKangFu0::keyPressed(const OIS::KeyEvent& evt)
{
	return true;
}

bool GASceneKangFu0::keyReleased(const OIS::KeyEvent& evt)
{
	if(evt.key==OIS::KC_ESCAPE)
	{
		g_mainMgr->g_UIMgr->switchToUI("EnterUI");
		g_mainMgr->g_cameraCtrl->lock();
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
		g_mainMgr->g_cameraCtrl->setStayOnNodeMaxMinPitchYaw(30,-27,90,-90);
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
	return true;
}

