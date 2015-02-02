#include "ScenePingHeng.h"
#include "GAPCameraController.h"
#include "GAPWorldManager.h"
#include "GAOgreUIManager.h"
#include "GARollPoint.h"
#include "GAPAudioManager.h"
#include "GASceneManager.h"

GARollPoint* pinghengRoll=new GARollPoint(5);
GARollPoint* forceLRoll=new GARollPoint(1);
GARollPoint* forceRRoll=new GARollPoint(1);
Ogre::ParticleSystem* particleSmokeRight,*particleSmokeLeft;
Ogre::SceneNode* particleNodeRight,*particleNodeLeft;
float ballAccRatio=2;
float angleRatio=1;
float windRatio=1;
float oneBlowTime=10;
float oneBlowDeltaTime=0;
ScenePingHeng::ScenePingHeng(GAPMainManager* mainMgr,std::string sceneName) : GASceneBase(mainMgr,sceneName)
{
	muHalflength=40;
	forceL=forceR=roacc=rovel=0;
	muDeltaDegree=muTotalDegree=0;
	ballWeight=0.05;//kg
	muMass=1;
	ballAcc=ballDistance=ballVel=0;
	addForceL=addForceR=false;
	plantForceDegree=0;
	plantBlowTotalTime=plantBlowDeltaTime=0;
	g_started=false;
	g_plantYaw=false;
	g_ballDrop=false;
	g_output.createNewFile("../ScenePingHeng/PingHengOutput.txt");
	g_toleranceTime=1;
	g_limitTime=200;
	totalTime=0;
	windAcc=0;
	newSceneRoll=new GARollPoint(7);
	g_toNewScene=false;
	isWindBlowing=false;
	g_timeOut=30;
	g_timeOutRoll=new GARollPoint(g_timeOut);
	arrowDirL=arrowDirR=false;
	arrowTransL=arrowTransR=0;
	g_showArrows=true;
	g_soundType=0;
	g_soundEnable=true;
}

bool ScenePingHeng::createScene()
{
	Ogre::SceneManager* sm=Ogre::Root::getSingleton().createSceneManager("BspSceneManager","pingheng");
	Ogre::Camera* cam=sm->createCamera("pingheng");
    cam->setNearClipDistance(5);
	g_mainMgr->setNewSceneManager(sm,cam);
	GAPCameraController *newCamc=new GAPCameraController(g_mainMgr);
	newCamc->init();
	g_mainMgr->g_cameraCtrl=newCamc;
	g_mainMgr->mWindow->removeAllViewports();
	Ogre::Viewport* vp=g_mainMgr->mWindow->addViewport(cam);
	cam->setAspectRatio((float)vp->getActualWidth()/(float)vp->getActualHeight());
	//cam->setFOVy(Ogre::Degree(60));
	g_worldName="house1_6.bsp";
	sm->setSkyBox(true,"Examples/MorningSkyBox",2500);
	sm->getSkyBoxNode()->pitch(Ogre::Degree(90));
	sm->setWorldGeometry(g_worldName);
	Ogre::Light* l=sm->createLight();
	l->setPosition(-600,-500,200);
	Ogre::Light* l2=sm->createLight();
	l2->setPosition(-200,-500,200);
	g_mainMgr->g_UIMgr->switchToUI("SceneUI");

	g_mainMgr->g_worldMgr->addModel("shafa1","shafa3_fix.mesh",Ogre::Vector3(0,-318.824,29.0582),Ogre::Vector3(3.14328,3.14328,3.14328),Ogre::Quaternion(0.5,0.5,0.5,0.5));
	g_mainMgr->g_worldMgr->addModel("chaji1","chaji1_fix.mesh",Ogre::Vector3(0,-455.585,29.3137),Ogre::Vector3(2.66635,2.66635,2.66635),Ogre::Quaternion(0.5,0.5,0.5,0.5));
	pinghengNode=g_mainMgr->getCurrentSceneMgr()->getRootSceneNode()->createChildSceneNode("pingheng",Ogre::Vector3(0,-455,57));
	pinghengNode->pitch(Ogre::Degree(90));
	pinghengNode->roll(Ogre::Degree(90),Ogre::Node::TS_WORLD);
	pinghengmuNode=pinghengNode->createChildSceneNode("pinghengmu",Ogre::Vector3(0,4.3,0));
	Ogre::Entity* ent1=g_mainMgr->getCurrentSceneMgr()->createEntity("pinghengmu1.mesh");
	Ogre::Entity* ent2=g_mainMgr->getCurrentSceneMgr()->createEntity("pinghengmu2.mesh");
	pinghengNode->attachObject(ent2);
	pinghengNode->setScale(1,2,2);
	pinghengmuNode->attachObject(ent1);
	pinghengmuNode->setScale(1,0.5,0.5);

	Ogre::SceneNode* pinghengqiuNodeBase=pinghengmuNode->createChildSceneNode("pinghengqiubase",Ogre::Vector3(0,3.5,0));
	ballNode=pinghengqiuNodeBase->createChildSceneNode();
	Ogre::Entity* ent3=g_mainMgr->getCurrentSceneMgr()->createEntity("pinghengqiu.mesh");
	ballNode->attachObject(ent3);

	Ogre::SceneNode* huapen1=g_mainMgr->getCurrentSceneMgr()->getRootSceneNode()->createChildSceneNode("huapen1",Ogre::Vector3(-120,-700,15));
	Ogre::SceneNode* huapen2=g_mainMgr->getCurrentSceneMgr()->getRootSceneNode()->createChildSceneNode("huapen2",Ogre::Vector3(120,-700,15));
	huapen1->pitch(Ogre::Degree(90));
	huapen2->pitch(Ogre::Degree(90));
	huapen1->attachObject(g_mainMgr->getCurrentSceneMgr()->createEntity("huapen1.mesh"));
	huapen2->attachObject(g_mainMgr->getCurrentSceneMgr()->createEntity("huapen1.mesh"));
	plant1=huapen1->createChildSceneNode("plant1");
	plant2=huapen2->createChildSceneNode("plant2");
	plant1->attachObject(g_mainMgr->getCurrentSceneMgr()->createEntity("zhiwu2.mesh"));
	plant2->attachObject(g_mainMgr->getCurrentSceneMgr()->createEntity("zhiwu2.mesh"));
	arrowL=g_mainMgr->getCurrentSceneMgr()->getRootSceneNode()->createChildSceneNode("arrowL");
	arrowR=g_mainMgr->getCurrentSceneMgr()->getRootSceneNode()->createChildSceneNode("arrowR");
	arrowEntL=sm->createEntity("arrowL","arrow.mesh");
	arrowEntR=sm->createEntity("arrowR","arrow.mesh");
	arrowEntL->setMaterialName("Examples/transparent/pink");
	arrowEntR->setMaterialName("Examples/transparent/blue");
	arrowL->attachObject(arrowEntL);
	arrowL->yaw(Ogre::Degree(-90));
	arrowL->yaw(Ogre::Degree(180),Ogre::Node::TS_WORLD);
	arrowL->setScale(0.3,0.5,0.5);
	arrowR->yaw(Ogre::Degree(-90));
	arrowR->setScale(0.3,0.5,0.5);
	arrowR->yaw(Ogre::Degree(180),Ogre::Node::TS_WORLD);
	arrowR->attachObject(arrowEntR);
	arrowL->setPosition(35,-460,85);
	arrowR->setPosition(-35,-460,85);
	arrowR->setVisible(true);
	arrowL->setVisible(true);

	Ogre::SceneNode* cameraStayNode=g_mainMgr->getCurrentSceneMgr()->getRootSceneNode()->createChildSceneNode("cameraStayNode",Ogre::Vector3(0,-375,83));
	cameraStayNode->pitch(Ogre::Degree(83));
	cameraStayNode->roll(Ogre::Degree(180),Ogre::Node::TS_WORLD);
	g_mainMgr->g_cameraCtrl->setCameraStyle(GAPCameraController::CameraStyle_StayOnNode,cameraStayNode);
	g_mainMgr->g_cameraCtrl->lock();
	g_sceneUI=(GAExampleSceneUI*)g_mainMgr->g_UIMgr->findUI("SceneUI");

	Ogre::NameValuePairList params;
	params["numberOfChains"] = "2";
	params["maxElements"] = "80";
	windTrail = (Ogre::RibbonTrail*)g_mainMgr->getCurrentSceneMgr()->createMovableObject("RibbonTrail", &params);
	windTrail->setMaterialName("Examples/LightRibbonTrail");
	windTrail->setTrailLength(50);
	windTrail->setVisible(false);
	g_mainMgr->getCurrentSceneMgr()->getRootSceneNode()->attachObject(windTrail);
	for (int i = 0; i < 2; i++)
	{
		windTrail->setInitialColour(i, 1, 0.8, 0);
		windTrail->setColourChange(i, 0.75, 1.25, 1.25, 1.25);
		windTrail->setWidthChange(i, 1);
		windTrail->setInitialWidth(i, 1);
	}
	windTrail->setVisible(false);
	windTrailNode1=g_mainMgr->getCurrentSceneMgr()->getRootSceneNode()->createChildSceneNode("windTrailNode1");
	windTrailNode2=g_mainMgr->getCurrentSceneMgr()->getRootSceneNode()->createChildSceneNode("windTrailNode2");
	windTrailNode1->setPosition(100,-500,120);
	windTrailNode2->setPosition(100,-500,100);
	windTrail->addNode(windTrailNode1);
	windTrail->addNode(windTrailNode2);
	Ogre::Entity* entTemp=g_mainMgr->getCurrentSceneMgr()->createEntity("pinghengqiu.mesh");
	Ogre::Entity* entTemp2=g_mainMgr->getCurrentSceneMgr()->createEntity("pinghengqiu.mesh");
	//windTrailNode1->attachObject(entTemp);
	windTrailNode1->createChildSceneNode(Ogre::Vector3(0,0,0))->attachObject(entTemp);
	windTrailNode2->createChildSceneNode(Ogre::Vector3(0,0,0))->attachObject(entTemp2);
	entTemp->setMaterialName("Examples/transparent/yellow");
	entTemp2->setMaterialName("Examples/transparent/yellow");
	windTrailNode1->scale(0.2,0.2,0.2);
	windTrailNode2->scale(0.2,0.2,0.2);
	windTrailNode1->setVisible(false);
	windTrailNode2->setVisible(false);
	particleSmokeRight=g_mainMgr->getCurrentSceneMgr()->createParticleSystem("smoke","Examples/Snow2");
	particleSmokeLeft=g_mainMgr->getCurrentSceneMgr()->createParticleSystem("smoke2","Examples/Snow3");
	particleSmokeLeft->setEmitting(false);
	particleSmokeRight->setEmitting(false);
	particleNodeLeft=g_mainMgr->getCurrentSceneMgr()->getRootSceneNode()->createChildSceneNode();
	particleNodeRight=g_mainMgr->getCurrentSceneMgr()->getRootSceneNode()->createChildSceneNode();
	particleNodeLeft->attachObject(particleSmokeLeft);
	particleNodeRight->attachObject(particleSmokeRight);
	particleNodeLeft->scale(0.1,0.1,0.1);
	particleNodeRight->scale(0.1,0.1,0.1);
	particleNodeRight->setPosition(-100,-500,80);
	particleNodeLeft->setPosition(100,-500,80);
	reCreateScene();
	return true;
}

bool ScenePingHeng::reCreateScene()
{
	loadInitFile("../ScenePingHeng/init.txt");
	g_sceneUI->countRestart((UINT)(g_limitTime));
	g_sceneUI->countPause();
	g_output.writeLine("Scene PingHeng created");
	g_sceneUI->g_totalScore=1;
	//g_sceneUI->hintString=L"请按 F1 键开始游戏";
	g_sceneUI->hideHint();
	g_currentDropTime=0;
	g_freezed=true;
	g_sceneEnd=false;
	g_toNewScene=false;
	g_sceneUI->progressBar1Visible(true);
	g_sceneUI->ProgressBar1Start(g_limitTime);
	g_sceneUI->ProgressBar1Pause();
	g_sceneUI->progressBar2Visible(false);
	g_sceneRunningTime=0;
	forceL=forceR=roacc=rovel=0;
	muDeltaDegree=muTotalDegree=0;
	ballAcc=ballDistance=ballVel=0;
	ballNode->setPosition(0,0,ballDistance);
	pinghengmuNode->setOrientation(Ogre::Quaternion::IDENTITY);
	addForceL=addForceR=false;
	g_ballDrop=false;
	g_sceneUI->countRestart((UINT)(g_limitTime));
	g_sceneUI->caption="Drops";
	g_sceneUI->g_currentScore=0;
	g_sceneUI->g_totalScore=1;
	pinghengRoll->setTimeInterval(5);
	g_sceneUI->ProgressBar1Start(g_limitTime);
	g_sceneUI->countPause();
	g_sceneUI->ProgressBar1Pause();
	g_vSpeedDown=0;
	g_totalBallDis=0;
	g_sceneUI->g_freezed=false;
	g_timeOutRoll->setTimeInterval(g_timeOut);
	g_trialState=0;
	windTrail->setVisible(false);
	particleSmokeLeft->setEmitting(false);
	particleSmokeRight->setEmitting(false);
	isWindBlowed=false;
	isWindBlowing=false;
	g_plantYaw=false;
	g_ballDrop=false;
	if(g_sceneUI->welcome)
	{
		g_sceneUI->hintString=L"欢迎使用交大虚拟康复训练平台";
		g_sceneUI->showHint();
	}
	g_showArrows=true;
	return true;
}

void ScenePingHeng::start()
{
	g_started=true;
	g_freezed=false;
	g_sceneUI->countBegin();
	g_sceneUI->ProgressBar1UnPause();
	srand(GetTickCount());
	//windBlow(rand()%2==0);
	g_sceneUI->hideHint();
	g_timeOutRoll->setTimeInterval(g_timeOut);
	g_sceneUI->g_freezed=false;
	g_sceneUI->welcome=false;
}

bool ScenePingHeng::keyPressed(const OIS::KeyEvent& evt)
{
	if(g_freezed) return false;
	if(evt.key==OIS::KC_LEFT)
	{
		addForceR=true;
		forceRRoll->setTimeInterval(1);
		g_timeOutRoll->setTimeInterval(g_timeOut);
		if(forceL<0) forceL=0;
		forceL+=1;
	}
	else if(evt.key==OIS::KC_RIGHT)
	{
		addForceL=true;
		forceLRoll->setTimeInterval(1);
		g_timeOutRoll->setTimeInterval(g_timeOut);
		if(forceL>0) forceL=0;
		forceL-=1;
	}
	else if(evt.key==OIS::KC_R)
	{
		reCreateScene();
	}
	return true;
}

bool ScenePingHeng::keyReleased(const OIS::KeyEvent& evt)
{
	if(evt.key==OIS::KC_ESCAPE)
	{
		g_mainMgr->g_UIMgr->switchToUI("EnterUI");
		g_mainMgr->g_cameraCtrl->lock();
	}
	else if(evt.key==OIS::KC_F1)
	{
		if(g_sceneEnd) reCreateScene();
		if(g_freezed) start();
	}
	if(g_freezed) return false;
	return true;
}

bool ScenePingHeng::frameStarted(float deltaTime)
{
	if(g_ballDrop) ballDrops(deltaTime);
	if(g_toNewScene)
	{
		if(newSceneRoll->getRollPoint(deltaTime,1))
		{
			toNewScene();
		}
		else
		{
			UINT time=newSceneRoll->getTimeLeft();
			time+=1;
			if(time<6)
			{
				std::string str="into scene Daily in "+misc.ConvertToString(time)+" seconds";
				g_sceneUI->hintString=str;
				g_sceneUI->showHint();
			}
		}
	}
	arrowR->setVisible(false);
	arrowL->setVisible(false);
	if(g_showArrows)
	{
		arrowR->setVisible(true);
		arrowL->setVisible(true);
	}
	if(g_freezed) return false;
	g_trialState=1;
	//if(g_timeOutRoll->getRollPoint2(deltaTime,1)) end(false);
	arrowMove(deltaTime);
	totalTime+=deltaTime;
	if(g_started)
	{
		calcPingHeng(deltaTime);
		if(addForceL==false&&addForceR==false) forceL=0;
		if(addForceL&&forceLRoll->getRollPoint(deltaTime,1))
		{
			addForceL=false;
		}
		if(addForceR&&forceRRoll->getRollPoint(deltaTime,1))
		{
			addForceR=false;
		}
		if(plantBlowTotalTime>0)
		{
			plantBlowDeltaTime+=deltaTime;
			if(plantBlowDeltaTime>plantBlowTotalTime)
			{
				g_plantYaw=false;
				plantForceDegree=0;
				windAcc=0;
				if(plantRotate(deltaTime,plantForceDegree)==false)
				{
					plantBlowTotalTime=plantBlowDeltaTime=0;
				}
			}
			else plantRotate(deltaTime,plantForceDegree);
		}
		if(pinghengRoll->getRollPoint(deltaTime,5))
		{
			if(((abs(ballVel)<1)&&(abs(muTotalDegree)<2.1))||pinghengRoll->getImmediateRollPoint(0.1))
			{
				/*if(ballDistance>10) windBlow(false);
				else if(ballDistance<-10) windBlow(true);
				else windBlow(pinghengRoll->getImmediateRollPoint(0.5));*/
			}
		}
	}
	if(isWindBlowed) blowWindTrailFrame(deltaTime);
	if(g_sceneUI->isCountOver()) end(true);
	if(isWindBlowing)
	{
		oneBlowDeltaTime+=deltaTime;
		if(oneBlowDeltaTime>oneBlowTime)
		{
			isWindBlowing=false;
			oneBlowDeltaTime=0;
		}
	}
	return true;
}

void ScenePingHeng::arrowMove(float deltaTime)
{
	arrowEntL->setMaterialName("Examples/transparent/pink");
	arrowEntR->setMaterialName("Examples/transparent/blue");
	if(muTotalDegree>3)
	{
		arrowEntR->setMaterialName("Examples/Solid/Blue");
		arrowTransDeltaR=deltaTime*3.f;
		if(arrowDirR) arrowTransR+=arrowTransDeltaR;
		else
		{
			arrowTransR-=arrowTransDeltaR;
			arrowTransDeltaR=-arrowTransDeltaR;
		}
		if(arrowTransR>1)
		{
			arrowTransR-=arrowTransDeltaR;
			arrowTransR-=arrowTransDeltaR;
			arrowTransDeltaR=-arrowTransDeltaR;
			arrowDirR=false;
		}
		else if(arrowTransR<-1)
		{
			arrowTransDeltaR=deltaTime*3.f;
			arrowTransR+=arrowTransDeltaR;
			arrowTransR+=arrowTransDeltaR;
			arrowDirR=true;
		}
		arrowEntR->getParentNode()->translate(0,0,arrowTransDeltaR,Ogre::Node::TS_WORLD);
	}
	else if(muTotalDegree<-3)
	{
		arrowEntL->setMaterialName("Examples/Solid/Pink");
		arrowTransDeltaL=deltaTime*4.f;
		if(arrowDirL) arrowTransL+=arrowTransDeltaL;
		else
		{
			arrowTransL-=arrowTransDeltaL;
			arrowTransDeltaL=-arrowTransDeltaL;
		}
		if(arrowTransL>1)
		{
			arrowTransL-=arrowTransDeltaL;
			arrowTransL-=arrowTransDeltaL;
			arrowTransDeltaL=-arrowTransDeltaL;
			arrowDirL=false;
		}
		else if(arrowTransL<-1)
		{
			arrowTransDeltaL=deltaTime*4.f;
			arrowTransL+=arrowTransDeltaL;
			arrowTransL+=arrowTransDeltaL;
			arrowDirL=true;
		}
		arrowEntL->getParentNode()->translate(0,0,arrowTransDeltaL,Ogre::Node::TS_WORLD);
	}
	else
	{
		arrowEntR->getParentNode()->translate(0,0,-arrowTransR,Ogre::Node::TS_WORLD);
		arrowTransR=0;
		arrowEntL->getParentNode()->translate(0,0,-arrowTransL,Ogre::Node::TS_WORLD);
		arrowTransL=0;
	}
}
void ScenePingHeng::calcPingHeng(float deltaTime)
{
	float ratio=0.1;
	static bool collide=false;
	float waste=0;
	muDeltaDegree=deltaTime*forceL;
	muTotalDegree+=muDeltaDegree*angleRatio;
	float maxDegree=20;
	if(muTotalDegree<maxDegree&&muTotalDegree>-maxDegree)
	{
		pinghengmuNode->setOrientation(Ogre::Quaternion::IDENTITY);
		pinghengmuNode->pitch(Ogre::Degree(muTotalDegree));
	}
	else
	{
		if(muTotalDegree>=maxDegree) muTotalDegree=maxDegree;
		else if(muTotalDegree<=-maxDegree) muTotalDegree=-maxDegree;
		pinghengmuNode->setOrientation(Ogre::Quaternion::IDENTITY);
		pinghengmuNode->pitch(Ogre::Degree(muTotalDegree));
		//rovel=0;
	}


	//ball place
	float ballwaste=0.0;
	if(muTotalDegree<0.5&&muTotalDegree>-0.5)
	{
		ballVel=0;
	}
	ballAcc=20*sin(muTotalDegree/180);//+windAcc*windRatio;
	ballAcc*=ballAccRatio;
	if(ballAcc*ballVel<0) ballAcc*=10;
	if(ballVel>0) ballAcc-=ballwaste;
	else if(ballVel<0) ballAcc+=ballwaste;
	float deltaVel=ballAcc*deltaTime;
	if(ballVel*deltaVel<0)
	{
		deltaVel*=5;
		if(abs(ballVel)<abs(deltaVel)) ballVel=0;
		else ballVel+=deltaVel;
	}
	else ballVel+=deltaVel;
	float deltaDis=ballVel*deltaTime;
	ballDistance+=deltaDis;
	if(ballDistance>-37&&ballDistance<37)
	{
		ballNode->setPosition(0,0,ballDistance);
		ballNode->pitch(Ogre::Degree(deltaDis/3.f/3.1415926*180));
		collide=false;
	}
	else
	{
		if(ballDistance>=37) ballDistance=37;
		else if(ballDistance<=-37) ballDistance=-37;
		ballNode->setPosition(0,0,ballDistance);
		if(collide==false)
		{
			collide=true;
			g_ballDrop=true;
			g_currentDropTime++;
			end(false);
		}
	}
}

void ScenePingHeng::ballDrops(float deltaTime)
{
	float deltaDis=ballVel*deltaTime;
	float z=ballNode->_getDerivedPosition().z;
	float deltaDown=0;
	g_totalBallDis+=abs(deltaDis);
	if(z>57&&g_totalBallDis>3)
	{
		g_vSpeedDown+=9.8*deltaTime;
	}
	else
	{
		g_vSpeedDown=0;
	}
	deltaDown=-g_vSpeedDown*deltaTime;
	ballNode->translate(deltaDis,0,deltaDown,Ogre::Node::TS_WORLD);
	ballNode->pitch(Ogre::Degree(deltaDis/3.f/3.1415926*180));
}

bool ScenePingHeng::plantRotate(float deltaTime,float forceDegree)
{
	static float plantDegree=0;
	static float yawDegree=2;
	static bool rotateDir=false;
	float degree=deltaTime*10.f;
	
	if(rotateDir)
	{
		if(!g_plantYaw)
		{
			if(plantDegree<=0&&plantDegree+degree>=0) plantDegree=0;
			if(plantDegree==0)
			{
				plant1->setOrientation(Ogre::Quaternion::IDENTITY);
				plant2->setOrientation(Ogre::Quaternion::IDENTITY);
				return false;
			}
		}
		plantDegree+=degree;
		if(plantDegree>yawDegree+forceDegree)
		{
			rotateDir=false;
		}
	}
	else
	{
		if(!g_plantYaw)
		{
			if(plantDegree>=0&&plantDegree-degree<=0) plantDegree=0;
			if(plantDegree==0)
			{
				plant1->setOrientation(Ogre::Quaternion::IDENTITY);
				plant2->setOrientation(Ogre::Quaternion::IDENTITY);
				return false;
			}
		}
		plantDegree-=degree;
		if(plantDegree<-yawDegree+forceDegree)
		{
			rotateDir=true;
		}
	}
	plant1->setOrientation(Ogre::Quaternion::IDENTITY);
	plant2->setOrientation(Ogre::Quaternion::IDENTITY);
	plant1->roll(Ogre::Degree(plantDegree));
	plant2->roll(Ogre::Degree(plantDegree));
	return true;
}

void ScenePingHeng::blowWindTrail(bool toLeft)
{
	windDirection=toLeft;
	isWindBlowed=true;
	windTime=0;
	windTrail->setVisible(false);
	if(toLeft)
	{
		windStartPoint=Ogre::Vector3(-100,-500,120);
		windEndPoint=Ogre::Vector3(100,-500,80);
		windTrailNode1->setPosition(-70,-500,85);
		windTrailNode2->setPosition(-70,-500,80);
		particleSmokeLeft->setEmitting(false);
		particleSmokeRight->setEmitting(true);
	}
	else
	{
		windStartPoint=Ogre::Vector3(100,-500,120);
		windEndPoint=Ogre::Vector3(-100,-500,80);
		windTrailNode1->setPosition(70,-500,85);
		windTrailNode2->setPosition(70,-500,80);
		particleSmokeLeft->setEmitting(true);
		particleSmokeRight->setEmitting(false);
		/*particleSmokeLeft->setVisible(true);
		particleSmokeRight->setVisible(false);*/
	}
}
void ScenePingHeng::blowWindTrailFrame(float deltaTime)
{
	static float speed=30;
	Ogre::Vector3 vec=windEndPoint-windStartPoint;
	vec.normalise();
	vec=deltaTime*speed*vec;
	windTime+=deltaTime;
	if(windTime>5)
	{
		isWindBlowed=false;
		windTrail->setVisible(false);
		particleSmokeLeft->setEmitting(false);
		particleSmokeRight->setEmitting(false);
	}
	windTrailNode1->translate(vec);
	windTrailNode2->translate(vec);
}

void ScenePingHeng::windBlow(bool toLeft,float speedAdd,float lastTime)
{
	if(isWindBlowing) return;
	if(toLeft)
	{
		plantBlowTotalTime=lastTime;
		plantBlowDeltaTime=0;
		plantForceDegree=-5;
		g_plantYaw=true;

		//windAcc=speedAdd;
		
		addForceR=true;
		forceRRoll->setTimeInterval(2);
		forceL+=2*windRatio;
	}
	else
	{
		plantBlowTotalTime=lastTime;
		plantBlowDeltaTime=0;
		plantForceDegree=5;
		g_plantYaw=true;
		//windAcc=-speedAdd;
		
		addForceL=true;
		forceLRoll->setTimeInterval(2);
		forceL-=2*windRatio;
	}
	isWindBlowing=true;
	g_mainMgr->g_audioMgr->playShortSound("../sounds/wind.wav");
	blowWindTrail(toLeft);
}

int ScenePingHeng::getLRCommandFromFile(std::string filePath)
{
	if(!fileIO2.openFile(filePath)) return 0;
	std::string str=fileIO2.getNextLine();
	fileIO2.endInput();
	fileIO2.createNewFile(filePath);
	fileIO2.endOutput();
	if(str=="Left") return 1;
	else if(str=="Right") return 2;
	return 0;
}

void ScenePingHeng::loadInitFile(std::string filePath)
{
	g_toleranceTime=2;
	g_limitTime=200;
	if(!fileIO.openFile(filePath)) return;
	std::string str=fileIO.getNextLine();
	std::string temp;
	std::string::size_type st;
	while(str!="")
	{
		st=str.find_first_of(":");
		temp=str.substr(0,st);
		str=str.substr(st+1,str.length()-1);
		if(temp=="timeLimit")
		{
			g_limitTime=misc.ConvertToInt(str);
		}
		else if(temp=="windRatio")
		{
			windRatio=misc.ConvertToFloat(str);
		}
		else if(temp=="ballAccRatio")
		{
			ballAccRatio=misc.ConvertToFloat(str);
		}
		else if(temp=="angleRatio")
		{
			angleRatio=misc.ConvertToFloat(str);
		}
		str=fileIO.getNextLine();
	}
	fileIO.endInput();
}

void ScenePingHeng::saveCurrentState(std::string filePath)
{
	GAFileIO* file=new GAFileIO();
	if(!file->createNewFile(filePath)) return;
	std::string line="";
	line="timeLimit:"+misc.ConvertToString(g_limitTime);
	file->writeLine(line);
	line="windRatio:"+misc.ConvertToString(windRatio);
	file->writeLine(line);
	line="ballAccRatio:"+misc.ConvertToString(ballAccRatio);
	file->writeLine(line);
	line="angleRatio:"+misc.ConvertToString(angleRatio);
	file->writeLine(line);
	file->endOutput();

}

void ScenePingHeng::end(bool finished)
{
	g_started=false;
	g_freezed=true;
	g_sceneUI->countPause();
	g_sceneEnd=true;
	windTrail->setVisible(false);
	particleSmokeLeft->setEmitting(false);
	particleSmokeRight->setEmitting(false);
	if(finished)
	{
		if(g_soundEnable) g_mainMgr->g_audioMgr->playShortSound("../sounds/guzhang.wav");
		g_sceneUI->hintString="Success!";
		g_toNewScene=true;
		newSceneRoll->setTimeInterval(7);
		g_trialState=2;
	}
	else
	{
		if(g_soundEnable) g_mainMgr->g_audioMgr->playShortSound("../sounds/tryAgain.wav");
		g_sceneUI->hintString="Failed...";
		g_trialState=0;
	}
	g_sceneUI->showHint();
}

bool ScenePingHeng::reloadScene()
{
	GASceneBase::reloadScene();
	if(!g_started) g_freezed=true;
	g_mainMgr->g_cameraCtrl->lock();
	g_mainMgr->g_UIMgr->switchToUI("SceneUI");
	g_sceneUI->g_totalScore=1;
	g_sceneUI->g_currentScore=0;
	g_sceneUI->g_currentTimeSeconds=g_currentTime;
	g_sceneUI->g_totalSeconds=g_limitTime;
	g_sceneUI->progressBar1Visible(true);
	g_sceneUI->ProgressBar1Start(g_limitTime);
	g_sceneUI->setProgressBarRatio(s_progressRatio);
	g_sceneUI->progressBar2Visible(false);
	g_sceneUI->caption="Drops";
	if(g_freezed)
	{
		reCreateScene();
	}
	else
	{
		g_sceneUI->hideHint();
		g_timeOutRoll->setTimeInterval(g_timeOut);
		g_sceneUI->countBegin();
	}
	return true;
}

bool ScenePingHeng::freezeScene()
{
	GASceneBase::freezeScene();
	g_currentTime=g_sceneUI->g_currentTimeSeconds;
	s_progressRatio=g_sceneUI->getProgressBarRatio();
	return true;
}

void ScenePingHeng::toNewScene()
{
	g_mainMgr->g_dataMgr->getRootNode()->getChildLeafNode("recvString")->getDataMap()->addData("recv","0-switchGameByAni:4");
	g_mainMgr->g_sceneMgr->processServerData();
}

void ScenePingHeng::playStartSound(UINT leftright)
{
	if(!g_soundEnable) return; 
	if(leftright==1) g_mainMgr->g_audioMgr->playShortSound("../sounds/thinkLeft.wav");
	else if(leftright==2) g_mainMgr->g_audioMgr->playShortSound("../sounds/thinkRight.wav");
	else g_mainMgr->g_audioMgr->playShortSound("../sounds/fight.wav");
}


bool ScenePingHeng::processServerData(std::string recvString,GAServerSimple* server,UINT socketNum)
{
	bool processed=true;
	if(recvString=="right")
	{
		if(g_freezed) return true;
		addForceL=true;
		forceLRoll->setTimeInterval(1);
		g_timeOutRoll->setTimeInterval(g_timeOut);
		if(forceL>0) forceL=0;
		forceL-=1;
	}
	else if(recvString=="left")
	{
		if(g_freezed) return true;
		addForceR=true;
		forceRRoll->setTimeInterval(1);
		g_timeOutRoll->setTimeInterval(g_timeOut);
		if(forceL<0) forceL=0;
		forceL+=1;
	}
	else if(recvString=="start")
	{
		if(g_freezed&&g_sceneEnd==false) start();
		else
		{
			reCreateScene();
			start();
		}
	}
	else if(recvString=="reCreate")
	{
		reCreateScene();
	}
	else if(recvString=="showSound")
	{
		g_soundEnable=true;
		server->sendString("done",socketNum);
	}
	else if(recvString=="hideSound")
	{
		g_soundEnable=false;
		server->sendString("done",socketNum);
	}
	else if(recvString=="esc")
	{
		g_mainMgr->g_UIMgr->switchToUI("EnterUI");
		g_mainMgr->g_cameraCtrl->lock();
	}
	else if(recvString=="getLeftRight")
	{
		if(muTotalDegree>0.6) server->sendString("right up",socketNum);
		else if(muTotalDegree<-0.6) server->sendString("left up",socketNum);
		else server->sendString("balanced",socketNum);
	}
	else if(recvString=="autoPlaySound")
	{
		if(g_soundEnable)
		{
			if(muTotalDegree<-0.6) g_mainMgr->g_audioMgr->playShortSound("../sounds/tryLeft.wav");
			else if(muTotalDegree>0.6) g_mainMgr->g_audioMgr->playShortSound("../sounds/tryRight.wav");
			else g_mainMgr->g_audioMgr->playShortSound("../sounds/fight.wav");
		}
	}
	else if(recvString=="getTrialState")
	{
		if(g_trialState==0) server->sendString("End_Failed",socketNum);
		else if(g_trialState==1) server->sendString("Started",socketNum);
		else if(g_trialState==2) server->sendString("End_Success",socketNum);
	}
	else if(recvString=="getFullState")
	{
		std::string str="running time:"+misc.ConvertToString(g_sceneRunningTime)+";";
		if(g_freezed) str+="Not Started";
		else str+="Started";
		str+="\n";
		str+="score:"+misc.ConvertToString(g_sceneUI->g_currentScore)+";total score:"+misc.ConvertToString(g_sceneUI->g_totalScore)+"\n";
		str+="time limit:"+misc.ConvertToString(g_limitTime)+";runned:"+misc.ConvertToString(g_sceneUI->g_currentTimeSeconds)+"\n";
		str+="angle ratio:"+misc.ConvertToString(angleRatio)+"\n";
		str+="wind ratio:"+misc.ConvertToString(windRatio)+"\n";
		str+="ball acc ratio:"+misc.ConvertToString(ballAccRatio)+"\n";
		str+="current angle:"+misc.ConvertToString(muTotalDegree)+";max angle:20\n";
		str+="ball distance:"+misc.ConvertToString(ballDistance)+";max distance:37;ball velocity:"+misc.ConvertToString(ballVel);
		server->sendString(str,socketNum);
	}
	else if(recvString=="getTime")
	{
		if(g_freezed) server->sendString("0",socketNum);
		else server->sendString(misc.ConvertToString(g_sceneRunningTime),socketNum);
	}
	else if(recvString=="getScore")
	{
		server->sendString("0",socketNum);
	}
	else if(recvString=="lockView")
	{
		g_mainMgr->g_cameraCtrl->lock();
	}
	else if(recvString=="unlockView")
	{
		g_mainMgr->g_cameraCtrl->unlock();
	}
	else if(recvString=="showArrow")
	{
		g_showArrows=true;
		server->sendString("done",socketNum);
	}
	else if(recvString=="hideArrow")
	{
		g_showArrows=false;
		server->sendString("done",socketNum);
	}
	else if(recvString=="freeze")
	{
		g_freezed=true;
		g_sceneUI->g_freezed=g_freezed;
	}
	else if(recvString=="unfreeze")
	{
		g_freezed=false;
		g_sceneUI->g_freezed=g_freezed;
	}
	else if(recvString=="save")
	{
		saveCurrentState("../ScenePingHeng/init.txt");
	}
	//复合命令
	else
	{
		std::string::size_type st;
		std::string str1,str2,temp;
		st=recvString.find_first_of(":");
		if(st!=std::string::npos)
		{
			str1=recvString.substr(0,st);
			str2=recvString.substr(st+1);
			if(str1=="setAngleRatio")
			{
				angleRatio=misc.ConvertToFloat(str2);
				server->sendString("new angle ratio:"+misc.ConvertToString(angleRatio),socketNum);
			}
			else if(str1=="setWindRatio")
			{
				windRatio=misc.ConvertToFloat(str2);
				server->sendString("new wind ratio:"+misc.ConvertToString(windRatio),socketNum);
			}
			else if(str1=="setBallAccRatio")
			{
				ballAccRatio=misc.ConvertToFloat(str2);
				server->sendString("new ball acc ratio:"+misc.ConvertToString(ballAccRatio),socketNum);
			}
			else if(str1=="setTimeOut")
			{
				g_timeOut=misc.ConvertToFloat(str2);
				g_timeOutRoll->setTimeInterval(g_timeOut);
				server->sendString("new time out:"+misc.ConvertToString(g_timeOut),socketNum);
			}
			else if(str1=="setTimeLimit")
			{
				if(g_freezed&&!g_started)
				{
					g_limitTime=misc.ConvertToInt(str2);
					g_sceneUI->countRestart(g_limitTime);
					g_sceneUI->countPause();
					g_sceneUI->ProgressBar1Start(g_limitTime);
					g_sceneUI->ProgressBar1Pause();
					server->sendString("new time limit:"+misc.ConvertToString(g_limitTime),socketNum);
				}
				else server->sendString("unable to set this value while processing!",socketNum);
			}
			else if(str1=="setThisTrial")
			{
				saveCurrentState("../ScenePingHeng/init.txt");
				reCreateScene();
				start();
				g_sceneUI->welcome=false;
				if(str2=="1")
				{
					windBlow(false);
					playStartSound(1);
				}
				else if(str2=="2")
				{
					windBlow(true);
					playStartSound(2);
				}
				else
				{
					bool kkk=(rand()%2==1);
					windBlow(kkk);
					if(kkk) playStartSound(2);
					else playStartSound(1);
				}
			}
			else if(str1=="windBlow")
			{
				if(str2=="left")
				{
					windBlow(false);
				}
				else if(str2=="right")
				{
					windBlow(true);
				}
				server->sendString("done",socketNum);
			}
			else processed=false;
		}
		else processed=false;
	}
	return processed;
}