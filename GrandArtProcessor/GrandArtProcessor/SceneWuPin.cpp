#include "SceneWuPin.h"
#include "GAPCameraController.h"
#include "GAPWorldManager.h"
#include "GAOgreUIManager.h"
#include "GASceneManager.h"
#include "GAPAudioManager.h"

SceneWuPin::SceneWuPin(GAPMainManager* mainMgr,std::string name):GASceneBase(mainMgr,name)
{
	transDelta=1;
	moveName="book";
	
	g_bornPlace[0]=Ogre::Vector3(-767,68.9,0);
	g_bornPlace[1]=Ogre::Vector3(-767,93.9,0);
	g_bornPlace[2]=Ogre::Vector3(-767,147.3,0);
	g_bornPlace[3]=Ogre::Vector3(-767,201.9,0);
	
	g_goalPlace[0]=Ogre::Vector3(-608.535,690.785,0);
	g_goalPlace[2]=Ogre::Vector3(-439.337,535.856,0);
	g_goalPlace[1]=(g_goalPlace[0]+g_goalPlace[2])/2;
	g_goalPlace[4]=Ogre::Vector3(-427.453,250.421,0);
	g_goalPlace[3]=(g_goalPlace[2]+g_goalPlace[4])/2;
	g_goalPlace[6]=Ogre::Vector3(-505.576,-100.528,0);
	g_goalPlace[5]=(g_goalPlace[4]+g_goalPlace[6])/2;
	g_objectPlace[0]=Ogre::Vector3(-823.64,74.75,0);
	g_objectPlace[1]=Ogre::Vector3(-807.90,113.81,0);
	g_objectPlace[2]=Ogre::Vector3(-807.07,152.31,0);
	g_objectPlace[3]=Ogre::Vector3(-810.444,204.437,0);
	
	for(UINT i=0;i<10;i++)
	{
		SceneQueue[i]=(UINT)-1;
	}
	SceneQueue[0]=0;
	SceneQueue[1]=1;
	SceneQueue[2]=2;
	SceneQueue[3]=3;
	g_output.createNewFile("../SceneWuPin/WuPinOutput.txt");
	g_timeOut=30;
	g_timeOutRoll=new GARollPoint(g_timeOut);
	newSceneRoll=new GARollPoint(7);
	g_deltaRollDegree=5;
	g_oneTurnFinishedSign=false;
	g_showArrow=true;
	g_soundType=0;
	g_soundEnable=true;
}

void SceneWuPin::addWoman(UINT count)
{
	Ogre::SceneNode* w;
	Ogre::Entity* e;
	if(count==0)
	{
		w=womanRootNode->createChildSceneNode("woman0");
		e=g_mainMgr->getCurrentSceneMgr()->createEntity("woman_towel_get.mesh");
	}
	if(count==1)
	{
		w=womanRootNode->createChildSceneNode("woman1");
		e=g_mainMgr->getCurrentSceneMgr()->createEntity("woman_shuibei_get.mesh");
	}
	if(count==2)
	{
		w=womanRootNode->createChildSceneNode("woman2");
		e=g_mainMgr->getCurrentSceneMgr()->createEntity("woman_xigua_get.mesh");
	}
	if(count==3)
	{
		w=womanRootNode->createChildSceneNode("woman3");
		e=g_mainMgr->getCurrentSceneMgr()->createEntity("woman_book_get.mesh");
	}
	if(count==4)
	{
		w=womanRootNode->createChildSceneNode("woman4");
		e=g_mainMgr->getCurrentSceneMgr()->createEntity("woman_towel_walk.mesh");
	}
	if(count==5)
	{
		w=womanRootNode->createChildSceneNode("woman5");
		e=g_mainMgr->getCurrentSceneMgr()->createEntity("woman_shuibei_walk.mesh");
	}
	if(count==6)
	{
		w=womanRootNode->createChildSceneNode("woman6");
		e=g_mainMgr->getCurrentSceneMgr()->createEntity("woman_xigua_walk.mesh");
	}
	if(count==7)
	{
		w=womanRootNode->createChildSceneNode("woman7");
		e=g_mainMgr->getCurrentSceneMgr()->createEntity("woman_book_walk.mesh");
	}
	if(count==8)
	{
		w=womanRootNode->createChildSceneNode("woman_walk");
		e=g_mainMgr->getCurrentSceneMgr()->createEntity("woman_walk.mesh");
	}
	w->attachObject(e);
	if(count!=8) w->setScale(0.1,0.1,0.1);
	w->setVisible(false);
	GACharacterSimpleObject *s=new GACharacterSimpleObject(g_mainMgr->getCurrentSceneMgr(),womanRootNode,womanRootNode,e);
	g_womanAI[count]=new GACharacterAISimpleObject(s);
	g_womanAI[count]->getCharacter()->setFacingDirection(5);
	rightHand[count]=e->getSkeleton()->getBone("Bip01 R Hand");
}

bool SceneWuPin::createScene()
{
	Ogre::SceneManager* sm=Ogre::Root::getSingleton().createSceneManager("BspSceneManager","wupin");
	Ogre::Camera* cam=sm->createCamera("wupin");
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
	cam->setFOVy(Ogre::Degree(60));
	g_worldName="house1_6.bsp";
	sm->setSkyBox(true,"Examples/MorningSkyBox",2500);
	sm->getSkyBoxNode()->pitch(Ogre::Degree(90));
	sm->setWorldGeometry(g_worldName);
	Ogre::Light* l=sm->createLight();
	l->setPosition(-600,-500,200);
	Ogre::Light* l2=sm->createLight();
	l2->setPosition(-200,-500,200);

	g_mainMgr->g_worldMgr->addModel("watermelon","watermelon.mesh",Ogre::Vector3(-807.07,152.31,113.64),Ogre::Vector3(0.11,0.1,0.1),Ogre::Quaternion(0.5,0.5,0.5,0.5));
	g_mainMgr->g_worldMgr->addModel("maotan","towel1.mesh",Ogre::Vector3(-823.64,74.75,111.55),Ogre::Vector3(0.11,0.1,0.1),Ogre::Quaternion(0.5,0.5,0.5,0.5));
	g_mainMgr->g_worldMgr->addModel("book","booksingle.mesh",Ogre::Vector3(-810.444,204.437,101.17),Ogre::Vector3(0.11,0.1,0.1),Ogre::Quaternion(-0.5,-0.5,0.5,0.5));
	g_mainMgr->g_worldMgr->addModel("shuibei","shuibei1.mesh",Ogre::Vector3(-807.90,113.81,110.96),Ogre::Vector3(0.1,0.1,0.1),Ogre::Quaternion(0.0739,0.0739,0.703,0.703));
	g_mainMgr->g_worldMgr->addModel("huapen","huapen1.mesh",Ogre::Vector3(-436.636,536.073,21.0763),Ogre::Vector3(1,1,1),Ogre::Quaternion(0.707107,0.707107,0,0));
	g_mainMgr->g_worldMgr->addModel("zhiwu","zhiwu1.mesh",Ogre::Vector3(-439.337,535.856,29.4195),Ogre::Vector3(1,1,1),Ogre::Quaternion(-0.5,-0.5,-0.5,-0.5));
	g_mainMgr->g_worldMgr->addModel("xiyiji","xiyiji1.mesh",Ogre::Vector3(-427.453,250.421,6.71402),Ogre::Vector3(0.731262,0.731262,0.731262),Ogre::Quaternion(0.5,0.5,-0.5,-0.5));
	g_mainMgr->g_worldMgr->addModel("shugui","shugui2_fix.mesh",Ogre::Vector3(-505.576,-100.528,97.6922),Ogre::Vector3(1.49282,1.49282,1.49282),Ogre::Quaternion(-1.01751e-007,-1.01751e-007,0.707107,0.707107));
	g_mainMgr->g_worldMgr->addModel("bangongzhuo","bangongzhuo1.mesh",Ogre::Vector3(-851.77,143.93,51.99),Ogre::Vector3(0.37,0.37,0.37),Ogre::Quaternion(0.7071,0.7071,0,0));
	g_mainMgr->g_worldMgr->addModel("bingxiang","bingxiang1.mesh",Ogre::Vector3(-608.535,690.785,101.348),Ogre::Vector3(2.80556,2.80556,2.80556),Ogre::Quaternion(0.7071,0.7071,0,0));
	g_mainMgr->getSceneNode("zhiwu")->roll(Ogre::Degree(30),Ogre::Node::TS_WORLD);
	//shuibei-767,93.9,0
	womanRootNode=g_mainMgr->getCurrentSceneMgr()->getRootSceneNode()->createChildSceneNode("womanRootNode",Ogre::Vector3(-600,100,0));
	womanRootNode->pitch(Ogre::Degree(90));
	womanRootNode->roll(Ogre::Degree(-90),Ogre::Node::TS_WORLD);
	addWoman(0);addWoman(1);addWoman(2);addWoman(3);addWoman(4);addWoman(5);addWoman(6);addWoman(7);addWoman(8);
	
	//Ogre::SceneNode* handRNode=g_mainMgr->getCurrentSceneMgr()->getRootSceneNode()->createChildSceneNode("handRNode");
	Ogre::SceneNode* handRNode=womanRootNode->createChildSceneNode("handRNode");
	Ogre::SceneNode* shuibeiNode=handRNode->createChildSceneNode("shuibeiNode",Ogre::Vector3(8.27,1.10,-4.42),Ogre::Quaternion(0.99697,0.03442,-0.0024,0.0697));
	Ogre::Entity* entshuibei=g_mainMgr->getCurrentSceneMgr()->createEntity("shuibei1.mesh");
	shuibeiNode->attachObject(entshuibei);
	shuibeiNode->setScale(0.1,0.1,0.1);
	Ogre::SceneNode* xiguaNode=handRNode->createChildSceneNode("xiguaNode",Ogre::Vector3(8.843,-0.131,-17.927),Ogre::Quaternion(0.598,-0.006,-0.551,0.581));
	Ogre::Entity* entxigua=g_mainMgr->getCurrentSceneMgr()->createEntity("watermelon.mesh");
	xiguaNode->attachObject(entxigua);
	xiguaNode->setScale(0.11,0.1,0.1);
	xiguaNode->setVisible(false);
	Ogre::SceneNode* shuNode=handRNode->createChildSceneNode("shuNode",Ogre::Vector3(19.187,6.467,-10.822),Ogre::Quaternion(0.640,-0.581,0.489,0.116));
	Ogre::Entity* entshu=g_mainMgr->getCurrentSceneMgr()->createEntity("booksingle.mesh");
	shuNode->attachObject(entshu);
	shuNode->setScale(0.11,0.1,0.1);
	shuNode->setVisible(false);
	Ogre::SceneNode* maotanNode=handRNode->createChildSceneNode("maotanNode",Ogre::Vector3(20.04,16.67,-17.76),Ogre::Quaternion(0.540,0.156,-0.648,0.514));
	Ogre::Entity* entmaotan=g_mainMgr->getCurrentSceneMgr()->createEntity("towel1.mesh");
	maotanNode->attachObject(entmaotan);
	maotanNode->setScale(0.11,0.1,0.1);
	maotanNode->setVisible(false);
	g_mainMgr->getSceneNode("woman1")->setVisible(true);
	currentAni=1;
	g_womanAI[currentAni]->runAnimation("open",false);
	g_womanAI[currentAni]->pauseAnimation();

	Ogre::SceneNode* aroundNode=womanRootNode->createChildSceneNode("womanRotateNode");
	aroundNode->translate(0,150,0);
	newCamc->setCameraStyle(GAPCameraController::CameraStyle_AroundNode,aroundNode);

	Ogre::SceneNode* arrowBaseNode=womanRootNode->createChildSceneNode("arrowBaseNode");
	arrowBaseNode->translate(0,0,180,Ogre::Node::TS_WORLD);
	arrowLeftNode=arrowBaseNode->createChildSceneNode("arrowLeftNode");
	arrowRightNode=arrowBaseNode->createChildSceneNode("arrowRightNode");
	arrowGoNode=arrowBaseNode->createChildSceneNode("arrowGoNode");
	arrowRightNode->roll(Ogre::Degree(90),Ogre::Node::TS_WORLD);
	arrowLeftNode->roll(Ogre::Degree(-90),Ogre::Node::TS_WORLD);
	arrowGoNode->roll(Ogre::Degree(180),Ogre::Node::TS_WORLD);
	Ogre::Entity *arrowEntL=sm->createEntity("arrowL","arrow.mesh");
	Ogre::Entity *arrowEntR=sm->createEntity("arrowR","arrow.mesh");
	Ogre::Entity *arrowEntGo=sm->createEntity("arrowGo","arrow.mesh");
	arrowEntL->setMaterialName("Examples/Solid/Pink");
	arrowEntR->setMaterialName("Examples/Solid/Blue");
	arrowEntGo->setMaterialName("Examples/Solid/Green");
	arrowLeftNode->attachObject(arrowEntL);
	arrowRightNode->attachObject(arrowEntR);
	arrowGoNode->attachObject(arrowEntGo);
	arrowLeftNode->translate(0,0,20);
	arrowRightNode->translate(0,0,-20);
	arrowGoNode->translate(-20,0,0);
	arrowBaseNode->setVisible(true);
	arrowGoNode->setVisible(false);
	arrowLeftNode->setVisible(false);
	arrowRightNode->setVisible(false);
	arrowBaseNode->roll(Ogre::Degree(90),Ogre::Node::TS_WORLD);
	g_mainMgr->g_UIMgr->switchToUI("SceneUI");
	g_sceneUI=(GAExampleSceneUI*)g_mainMgr->g_UIMgr->findUI("SceneUI");
	g_mainMgr->g_cameraCtrl->g_cameraPivot->setOrientation(Ogre::Quaternion::IDENTITY);
	g_mainMgr->g_cameraCtrl->g_cameraPivot->pitch(Ogre::Degree(-30));
	g_mainMgr->g_cameraCtrl->g_cameraPivot->roll(Ogre::Degree(-180),Ogre::Node::TS_WORLD);
	g_mainMgr->g_cameraCtrl->g_cameraGoal->translate(0,0,40);
	handRNode->setPosition(0,0,-1000);
	reCreateScene();
	return true;
}

bool SceneWuPin::reCreateScene()
{
	g_bodyTurning=g_walking=g_getting=false;
	g_bodyTurning2=false;
	g_aniEnd=false;
	g_started=false;
	g_startRotating=false;
	currentAni=8;
	showSingleWoman(8);
	for(UINT i=0;i<9;i++)
	{
		g_womanAI[i]->g_move=false;
	}
	g_womanAI[currentAni]->runAnimation("open");
	g_womanAI[currentAni]->pauseAnimation();
	womanWalk=womanRotate=false;
	rollDegree=0;
	g_chooseDir=false;
	g_oneTurnLastWalk=false;
	g_toNewScene=false;
	g_freezed=true;
	g_currentScore=0;
	g_sceneUI->g_currentScore=g_currentScore;
	g_currentObject=0;
	showWuPin();hideWuPinOnHand();
	g_sceneUI->progressBar2Visible(false);
	g_mainMgr->g_cameraCtrl->lock();
	g_mainMgr->g_UIMgr->switchToUI("SceneUI");
	loadInitFile("../SceneWuPin/init.txt");
	g_currentObject=SceneQueue[0];
	//g_sceneUI->hintString=L"请按F1键开始游戏";
	g_sceneUI->hideHint();
	g_sceneUI->countRestart(g_limitTime);
	g_sceneUI->countPause();
	g_sceneUI->progressBar2Visible(false);
	g_sceneUI->ProgressBar1Start(g_limitTime);
	g_sceneUI->ProgressBar1Pause();
	womanRootNode->setPosition(-600,100,0);
	womanRootNode->setOrientation(Ogre::Quaternion::IDENTITY);
	womanRootNode->pitch(Ogre::Degree(90));
	womanRootNode->roll(Ogre::Degree(-90),Ogre::Node::TS_WORLD);
	g_sceneEnd=false;
	g_sceneUI->g_freezed=false;
	g_timeOutRoll->setTimeInterval(g_timeOut);
	g_trialState=0;
	if(g_sceneUI->welcome)
	{
		g_sceneUI->hintString=L"欢迎使用交大虚拟康复训练平台";
		g_sceneUI->showHint();
	}
	g_oneTurnFinishedSign=false;
	g_showArrow=true;
	return true;
}

void SceneWuPin::start()
{
	g_freezed=false;
	g_sceneUI->g_freezed=false;
	g_started=true;
	g_sceneUI->hideHint();
	g_sceneUI->welcome=false;
}

void SceneWuPin::setRotate(UINT type)
{
	hideAllWoman();
	if(currentAni<4) currentAni+=4;
	g_mainMgr->getSceneNode("woman"+misc.ConvertToString(currentAni))->setVisible(true);
	placeWalkTo=type;
	if(type==0)
	{
		Ogre::Vector3 pos1=g_mainMgr->getSceneNode("bingxiang")->_getDerivedPosition();
		turnBody(pos1);
	}
	else if(type==1)
	{
		Ogre::Vector3 pos1=g_mainMgr->getSceneNode("huapen")->_getDerivedPosition();
		turnBody(pos1);
	}
	else if(type==2)
	{
		Ogre::Vector3 pos1=g_mainMgr->getSceneNode("xiyiji")->_getDerivedPosition();
		turnBody(pos1);
	}
	else if(type==3)
	{
		Ogre::Vector3 pos1=g_mainMgr->getSceneNode("shugui")->_getDerivedPosition();
		turnBody(pos1);
	}
	if(g_womanAI[currentAni]->getAnimationState()==1) g_womanAI[currentAni]->unpauseAnimation();
	else g_womanAI[currentAni]->runAnimation("open");
}

bool SceneWuPin::setWalkTo(UINT type)
{
	if(g_chooseDir==false) return false;
	if(currentAni==4&&currentTurn!=4) return false;
	if(currentAni==5&&currentTurn!=2) return false;
	if(currentAni==6&&currentTurn!=0) return false;
	if(currentAni==7&&currentTurn!=6) return false;
	if(currentAni<4||currentAni>7) return false;
	
	showSingleWoman(currentAni);
	walkToPlace(g_goalPlace[currentTurn],0.7);
	g_chooseDir=false;
	g_oneTurnLastWalk=true;
	g_timeOutRoll->setTimeInterval(g_timeOut);
	return true;
}

void SceneWuPin::turnBody(Ogre::Vector3 goal)
{
	Ogre::Vector3 v1=womanRootNode->_getDerivedPosition();
	Ogre::Vector3 dir=goal-v1;
	dir.z=0;
	dir.normalise();
	rollDegree=g_womanAI[currentAni]->getCharacter()->getTurnDegreesXY(dir);
	deltaRollDegree=0;
	g_womanAI[currentAni]->runAnimation("open",true);
}

void SceneWuPin::turnBody2(float degree)
{
	Ogre::Vector3 v1=womanRootNode->_getDerivedPosition();
	Ogre::Vector3 goal;
	rollDegree=degree;
	for(UINT i=0;i<4;i++)
	{
		if(i==0) goal=g_goalPlace[0];
		else if(i==1) goal=g_goalPlace[2];
		else if(i==2) goal=g_goalPlace[4];
		else if(i==3) goal=g_goalPlace[6];
		Ogre::Vector3 dir=goal-v1;
		dir.z=0;
		dir.normalise();
		float roll=g_womanAI[currentAni]->getCharacter()->getTurnDegreesXY(dir);
		if(roll>0)
		{
			if(rollDegree>0&&roll<=rollDegree)
			{
				rollDegree=roll;
				break;
			}
		}
		else
		{
			if(rollDegree<0&&roll>=rollDegree)
			{ 
				rollDegree=roll;
				break;
			}
		}
	}
	deltaRollDegree=0;
	g_womanAI[currentAni]->runAnimation("open",true);
}

void SceneWuPin::walkToPlace(Ogre::Vector3 goal,float walkRatio)
{
	Ogre::Vector3 v2=womanRootNode->_getDerivedPosition();
	Ogre::Vector3 v=v2+((goal-v2)*walkRatio);
	g_womanAI[currentAni]->moveToNewPlaceBySpeed(v.x,v.y,0,80);
	g_womanAI[currentAni]->runAnimation("open",true);
}
void SceneWuPin::getObject(UINT No,float deltaTime)
{
	if(!g_started) return;
	if(g_startRotating) return;
	g_oneTurnFinishedSign=false;
	if(No==0)
	{
		if(g_bodyTurning==false&&g_walking==false&&g_getting==false&&g_aniEnd==false&&g_bodyTurning2==false)
		{
			showWuPin();
			hideWuPinOnHand();
			g_bodyTurning=true;
			g_bodyTurningFinished=false;
			g_bodyTurning2=true;
			currentAni=8;
			showSingleWoman(8);
			turnBody(g_bornPlace[g_currentObject]);
		}
		else if(g_bodyTurning&&g_bodyTurningFinished==false&&g_aniEnd==false&&g_bodyTurning2) slowlyRotation(deltaTime);
		else if(g_bodyTurningFinished&&g_aniEnd==false&&g_walking==false&&g_bodyTurning)
		{
			g_bodyTurningFinished=false;
			g_bodyTurning=false;
			g_bodyTurning2=false;
			g_walking=true;
			walkToPlace(g_bornPlace[g_currentObject]);
		}
		else if(g_womanAI[currentAni]->g_move==false&&g_bodyTurning2==false&&g_getting==false&&g_walking==true)
		{
			g_bodyTurning2=true;
			g_walking=false;
			turnBody(g_objectPlace[g_currentObject]);
		}
		else if(g_bodyTurning2==true&&g_bodyTurningFinished==false&&g_getting==false&&g_aniEnd==false) slowlyRotation(deltaTime);
		else if(g_bodyTurning2==true&&g_bodyTurningFinished==true&&g_getting==false&&g_aniEnd==false)
		{
			g_getting=true;
			g_bodyTurning2=false;
			g_bodyTurningFinished=false;
			g_temp1=false;
		}
		else if(g_bodyTurning2==false&&g_aniEnd==false&&g_walking==false&&g_getting&&g_temp1==false)
		{
			runGetWuPin(g_currentObject);
			g_temp1=true;
		}
		else if(g_womanAI[currentAni]->getAnimationState()==3&&g_aniEnd==false&&g_getting==true)
		{
			currentAni+=4;
			showSingleWoman(currentAni);
			if(currentAni!=7) currentTurn=3;
			else currentTurn=5;
			turnBody(g_goalPlace[currentTurn]);
			g_aniEnd=true;
			g_getting=false;
			g_bodyTurningFinished=false;
			
		}
		else if(g_aniEnd==true&&g_bodyTurningFinished==false) slowlyRotation(deltaTime);
		else if(g_aniEnd==true&&g_bodyTurningFinished==true)
		{
			g_startRotating=true;
			g_bodyTurningFinished=false;
			g_bodyTurning=false;
			g_aniEnd=false;
			g_bodyTurning2=false;
			g_getting=false;
			g_walking=false;
			g_chooseDir=true;
			g_sceneUI->countBegin();
			g_sceneUI->ProgressBar1UnPause();
			g_timeOutRoll->setTimeInterval(g_timeOut);
			if(g_soundEnable)
			{
				int k1=getLeftRight();
				playStartSound(k1);
			}
			
		}
	}
}

bool SceneWuPin::keyReleased(const OIS::KeyEvent& evt)
{
	if(evt.key==OIS::KC_F1)
	{
		if(g_sceneEnd) reCreateScene();
		if(g_freezed)
		{
			start();
		}
	}
	if(evt.key==OIS::KC_ESCAPE)
	{
		g_mainMgr->g_UIMgr->switchToUI("EnterUI");
		g_mainMgr->g_cameraCtrl->lock();
	}
	if(g_freezed) return false;
	if(evt.key==OIS::KC_L||evt.key==OIS::KC_LEFT)
	{
		turnLeft();
	}
	else if(evt.key==OIS::KC_R||evt.key==OIS::KC_RIGHT)
	{
		turnRight();
	}
	else if(evt.key==OIS::KC_G)
	{
		setWalkTo(0);
	}
	return true;
}

void SceneWuPin::moveWuPin()
{
	switch(currentAni)
	{
	case 0:
		runAni1();
		break;
	case 1:
		runAni2();
		break;
	case 2:
		runAni3();
		break;
	case 3:
		runAni4();
		break;
	case 4:
		runAni5();
		break;
	case 5:
		runAni6();
		break;
	case 6:
		runAni7();
		break;
	case 7:
		runAni8();
		break;
	}
}

bool SceneWuPin::frameStarted(float deltaTime)
{
	if(g_toNewScene)
	{
		if(newSceneRoll->getRollPoint(deltaTime,1))
		{
			g_mainMgr->g_dataMgr->getRootNode()->getChildLeafNode("recvString")->getDataMap()->addData("recv","0-switchGameByAni:5");
			g_mainMgr->g_sceneMgr->processServerData();
		}
		else
		{
			UINT time=newSceneRoll->getTimeLeft();
			time+=1;
			if(time<6)
			{
				std::string str="into scene Maze in "+misc.ConvertToString(time)+" seconds";
				g_sceneUI->hintString=str;
				g_sceneUI->showHint();
			}
		}
	}
	if(g_freezed) return false;
	g_trialState=1;
	if(g_timeOutRoll->getRollPoint2(deltaTime,1)) end(false);
	if(g_sceneUI->isCountOver()) end(false);
	getObject(0,deltaTime);
	g_womanAI[currentAni]->update(deltaTime);
	if(g_chooseDir&&g_bodyTurningFinished==false) slowlyRotation(deltaTime);
	if(g_oneTurnLastWalk&&g_womanAI[currentAni]->g_move==false) oneTurnEnd();
	moveWuPin();
	arrowMove(deltaTime);
	arrowLeftNode->setVisible(false);
	arrowRightNode->setVisible(false);
	arrowGoNode->setVisible(false);
	if(g_startRotating&&g_showArrow)
	{
		switch(getLeftRight())
		{
		case 0:
			arrowRightNode->setVisible(true);
			break;
		case 1:
			arrowGoNode->setVisible(true);
			break;
		case 2:
			arrowLeftNode->setVisible(true);
			break;
		}
	}
	//if(isCurrentDirectionRight()) arrowLeftNode->setVisible(true);
	//else arrowLeftNode->setVisible(false);
	return true;
}

bool SceneWuPin::frameEnded(float deltaTime)
{
	return GASceneBase::frameEnded(deltaTime);
}


void SceneWuPin::slowlyRotation(float deltaTime)
{
	float rollPerSec=45;
	float delta;
	if(rollDegree>0) delta=rollPerSec*deltaTime;
	else if(rollDegree<0) delta=-rollPerSec*deltaTime;
	else
	{
		//womanRotate=false;
		g_womanAI[currentAni]->pauseAnimation();
		g_bodyTurningFinished=true;
		return;
	}
	deltaRollDegree+=delta;
	if(abs(rollDegree)<=abs(deltaRollDegree))
	{
		delta=abs(rollDegree)-abs(deltaRollDegree-delta);
		if(rollDegree>0) delta=abs(delta);
		else delta=-abs(delta);
		g_womanAI[currentAni]->rollAlongZAxis(delta);
		//womanRotate=false;
		g_bodyTurningFinished=true;
		g_womanAI[currentAni]->pauseAnimation();
	}
	else
	{
		g_womanAI[currentAni]->rollAlongZAxis(delta);
	}
}

void SceneWuPin::runAni1()
{
	float time=g_womanAI[0]->getAnimationTimePos();
	if(time<0.26)
	{
		g_womanAI[0]->setAnimationTimePos(0.26);
		time=0.26;
	}
	Ogre::Matrix4 mat;
	Ogre::Matrix4 mat2;
	g_mainMgr->getSceneNode("maotan")->setVisible(true);
	g_mainMgr->getSceneNode("maotanNode")->setVisible(false);
}

void SceneWuPin::runAni2()
{
	float time=g_womanAI[1]->getAnimationTimePos();
	Ogre::Matrix4 mat;
	Ogre::Matrix4 mat2;
	if(time>=1.4)
	{
		mat=rightHand[1]->_getFullTransform();
		Ogre::Vector3 vec2=mat.getTrans()*0.1;
		g_mainMgr->getSceneNode("handRNode")->setPosition(vec2);
		g_mainMgr->getSceneNode("handRNode")->setOrientation(mat.extractQuaternion());
		g_mainMgr->getSceneNode("shuibei")->setVisible(false);
		g_mainMgr->getSceneNode("shuibeiNode")->setVisible(true);
	}
	else
	{
		g_mainMgr->getSceneNode("shuibei")->setVisible(true);
		g_mainMgr->getSceneNode("shuibeiNode")->setVisible(false);
	}
}

void SceneWuPin::runAni3()
{
	float time=g_womanAI[2]->getAnimationTimePos();
	if(time<0.26)
	{
		g_womanAI[2]->setAnimationTimePos(0.26);
		time=0.26;
	}
	Ogre::Matrix4 mat;
	Ogre::Matrix4 mat2;
	g_mainMgr->getSceneNode("watermelon")->setVisible(true);
	g_mainMgr->getSceneNode("xiguaNode")->setVisible(false);
}

void SceneWuPin::runAni4()
{
	float time=g_womanAI[3]->getAnimationTimePos();
	if(time<0.26)
	{
		g_womanAI[3]->setAnimationTimePos(0.26);
		time=0.26;
	}
	Ogre::Matrix4 mat;
	Ogre::Matrix4 mat2;
	g_mainMgr->getSceneNode("book")->setVisible(true);
	g_mainMgr->getSceneNode("shuNode")->setVisible(false);
}

void SceneWuPin::runAni5()
{
	Ogre::Matrix4 mat=rightHand[4]->_getFullTransform();
	Ogre::Vector3 vec2=mat.getTrans()*0.1;
	g_mainMgr->getSceneNode("handRNode")->setPosition(vec2);
	g_mainMgr->getSceneNode("handRNode")->setOrientation(mat.extractQuaternion());
	g_mainMgr->getSceneNode("maotanNode")->setVisible(true);
	g_mainMgr->getSceneNode("maotan")->setVisible(false);
}
void SceneWuPin::runAni6()
{
	Ogre::Matrix4 mat=rightHand[5]->_getFullTransform();
	Ogre::Vector3 vec2=mat.getTrans()*0.1;
	g_mainMgr->getSceneNode("handRNode")->setPosition(vec2);
	g_mainMgr->getSceneNode("handRNode")->setOrientation(mat.extractQuaternion());
	g_mainMgr->getSceneNode("shuibeiNode")->setVisible(true);
	g_mainMgr->getSceneNode("shuibei")->setVisible(false);
}
void SceneWuPin::runAni7()
{
	Ogre::Matrix4 mat=rightHand[6]->_getFullTransform();
	Ogre::Vector3 vec2=mat.getTrans()*0.1;
	g_mainMgr->getSceneNode("handRNode")->setPosition(vec2);
	g_mainMgr->getSceneNode("handRNode")->setOrientation(mat.extractQuaternion());
	g_mainMgr->getSceneNode("xiguaNode")->setVisible(true);
	g_mainMgr->getSceneNode("watermelon")->setVisible(false);
}
void SceneWuPin::runAni8()
{
	Ogre::Matrix4 mat=rightHand[7]->_getFullTransform();
	Ogre::Vector3 vec2=mat.getTrans()*0.1;
	g_mainMgr->getSceneNode("handRNode")->setPosition(vec2);
	g_mainMgr->getSceneNode("handRNode")->setOrientation(mat.extractQuaternion());
	g_mainMgr->getSceneNode("shuNode")->setVisible(true);
	g_mainMgr->getSceneNode("book")->setVisible(false);
}
bool SceneWuPin::freezeScene()
{
	GASceneBase::freezeScene();
	s_progressBar1Ratio=g_sceneUI->getProgressBarRatio();
	s_countTimeLeft=g_sceneUI->g_currentTimeSeconds;
	return true;
}

bool SceneWuPin::reloadScene()
{
	GASceneBase::reloadScene();
	if(!g_started) g_freezed=true;
	g_mainMgr->g_UIMgr->switchToUI("SceneUI");
	g_sceneUI->g_totalSeconds=g_limitTime;
	g_sceneUI->g_currentTimeSeconds=s_countTimeLeft;
	g_sceneUI->countPause();
	g_sceneUI->g_totalScore=g_totalScore;
	g_sceneUI->g_currentScore=g_currentScore;
	g_sceneUI->caption="Score";
	g_sceneUI->progressBar2Visible(false);
	g_sceneUI->ProgressBar1Start(g_limitTime);
	g_sceneUI->setProgressBarRatio(s_progressBar1Ratio);
	if(g_freezed)
	{
		reCreateScene();
	}
	else
	{
		if(g_startRotating)
		{
			g_sceneUI->countBegin();
		}
		else
		{
			g_sceneUI->ProgressBar1Pause();
		}
		g_sceneUI->hideHint();
	}
	return true;
}

void SceneWuPin::arrowMove(float deltaTime)
{
	static float arrowDis=0;
	static bool arrowDir=false;
	float distance=deltaTime*5.f;
	if(arrowDir)
	{
		arrowDis+=distance;
		if(arrowDis>1)
		{
			arrowDis=1;
			arrowDir=false;
		}
	}
	else
	{
		arrowDis-=distance;
		if(arrowDis<-1)
		{
			arrowDis=-1;
			arrowDir=true;
		}
	}
	arrowLeftNode->setPosition(0,0,20-arrowDis);
	arrowRightNode->setPosition(0,0,-20-arrowDis);
	arrowGoNode->setPosition(-20-arrowDis,0,0);
}

void SceneWuPin::hideAllWoman()
{
	g_mainMgr->getSceneNode("woman0")->setVisible(false);
	g_mainMgr->getSceneNode("woman1")->setVisible(false);
	g_mainMgr->getSceneNode("woman2")->setVisible(false);
	g_mainMgr->getSceneNode("woman3")->setVisible(false);
	g_mainMgr->getSceneNode("woman4")->setVisible(false);
	g_mainMgr->getSceneNode("woman5")->setVisible(false);
	g_mainMgr->getSceneNode("woman6")->setVisible(false);
	g_mainMgr->getSceneNode("woman7")->setVisible(false);
	g_mainMgr->getSceneNode("woman_walk")->setVisible(false);
}

void SceneWuPin::showWoman(UINT No)
{
	switch(No)
	{
	case 0:
		g_mainMgr->getSceneNode("woman0")->setVisible(true);
		break;
	case 1:
		g_mainMgr->getSceneNode("woman1")->setVisible(true);
		break;
	case 2:
		g_mainMgr->getSceneNode("woman2")->setVisible(true);
		break;
	case 3:
		g_mainMgr->getSceneNode("woman3")->setVisible(true);
		break;
	case 4:
		g_mainMgr->getSceneNode("woman4")->setVisible(true);
		break;
	case 5:
		g_mainMgr->getSceneNode("woman5")->setVisible(true);
		break;
	case 6:
		g_mainMgr->getSceneNode("woman6")->setVisible(true);
		break;
	case 7:
		g_mainMgr->getSceneNode("woman7")->setVisible(true);
		break;
	case 8:
		g_mainMgr->getSceneNode("woman_walk")->setVisible(true);
		break;
	}
}

void SceneWuPin::showSingleWoman(UINT No)
{
	hideAllWoman();
	showWoman(No);
}
void SceneWuPin::selectObject(UINT type)
{

}

void SceneWuPin::runGetWuPin(UINT No)
{
	currentAni=No;
	showSingleWoman(currentAni);
	g_womanAI[currentAni]->runAnimation("open",false);
}

void SceneWuPin::turnLeft()
{
	if(currentTurn==0||g_bodyTurningFinished==false) return;
	currentTurn--;
	g_bodyTurningFinished=false;
	g_bodyTurning=true;
	turnBody(g_goalPlace[currentTurn]);
	g_timeOutRoll->setTimeInterval(g_timeOut);
}

void SceneWuPin::turnRight()
{
	if(currentTurn==6||g_bodyTurningFinished==false) return;
	currentTurn++;
	g_bodyTurningFinished=false;
	g_bodyTurning=true;
	turnBody(g_goalPlace[currentTurn]);
	g_timeOutRoll->setTimeInterval(g_timeOut);
}

void SceneWuPin::oneTurnEnd()
{
	g_sceneUI->countPause();
	g_sceneUI->ProgressBar1Pause();
	g_oneTurnLastWalk=false;
	g_bodyTurning=false;
	g_womanAI[currentAni]->pauseAnimation();
	currentAni=8;
	showSingleWoman(8);
	showWuPin();
	hideWuPinOnHand();
	g_currentScore++;
	g_sceneUI->g_currentScore=g_currentScore;
	g_sceneUI->g_totalScore=g_totalScore;
	g_oneTurnFinishedSign=true;
	g_sceneUI->g_currentScore=g_currentScore;
	g_currentObject=SceneQueue[g_currentScore];
	g_mainMgr->getSceneNode("handRNode")->setPosition(0,0,-1000);
	g_startRotating=false;
	if(g_currentObject!=(UINT)-1)
	{
		g_sceneUI->countRestart(g_limitTime);
		g_sceneUI->countPause();
		g_sceneUI->ProgressBar1Start(g_limitTime);
		g_sceneUI->ProgressBar1Pause();
		if(g_soundEnable) g_mainMgr->g_audioMgr->playShortSound("../sounds/guzhang.wav");
	}
	else end(true);
	g_trialState=2;
	g_freezed=true;
}

void SceneWuPin::showWuPin()
{
	g_mainMgr->getSceneNode("maotan")->setVisible(true);
	g_mainMgr->getSceneNode("shuibei")->setVisible(true);
	g_mainMgr->getSceneNode("watermelon")->setVisible(true);
	g_mainMgr->getSceneNode("book")->setVisible(true);
}
void SceneWuPin::hideWuPinOnHand()
{
	g_mainMgr->getSceneNode("maotanNode")->setVisible(false);
	g_mainMgr->getSceneNode("shuibeiNode")->setVisible(false);
	g_mainMgr->getSceneNode("xiguaNode")->setVisible(false);
	g_mainMgr->getSceneNode("shuNode")->setVisible(false);
}

void SceneWuPin::loadInitFile(std::string path)
{
	if(!g_input.openFile(path)) return;
	std::string str=g_input.getNextLine();
	std::string temp;
	std::string::size_type st;
	while(str!="")
	{
		std::string s1,s2;
		temp=str;
		st=temp.find_first_of(":");
		if(st!=temp.npos)
		{
			s1=temp.substr(0,st);
			s2=temp.substr(st+1);
			if(s1=="sceneQueue")
			{
				st=s2.find_first_of(",");
				UINT count=0;
				while(st!=s2.npos)
				{
					s1=s2.substr(0,st);
					s2=s2.substr(st+1);
					SceneQueue[count]=misc.ConvertToInt(s1)-1;
					st=s2.find_first_of(",");
					count++;
				}
				SceneQueue[count]=misc.ConvertToInt(s2)-1;
				g_totalScore=count+1;
			}
			else if(s1=="limitTime")
			{
				g_limitTime=misc.ConvertToFloat(s2);
			}
			else if(s1=="timeOut")
			{
				g_timeOut=misc.ConvertToFloat(s2);
				g_timeOutRoll->setTimeInterval(g_timeOut);
			}
		}
		str=g_input.getNextLine();
	}
	g_input.endInput();
}

void SceneWuPin::saveCurrentState(std::string filePath)
{
	GAFileIO *file=new GAFileIO();
	if(!file->createNewFile(filePath)) return;
	std::string line="";
	line="sceneQueue:";
	for(UINT i=0;i<10;i++)
	{
		if(SceneQueue[i]!=(UINT)-1) line+=misc.ConvertToString(SceneQueue[i]+1)+",";
		else break;
	}
	line=line.substr(0,line.length()-1);
	file->writeLine(line);
	line="limitTime:"+misc.ConvertToString(g_limitTime);
	file->writeLine(line);
	line="timeOut:"+misc.ConvertToString(g_timeOut);
	file->writeLine(line);
}
void SceneWuPin::end(bool finished)
{
	g_freezed=true;
	g_sceneEnd=true;
	if(finished)
	{
		g_sceneUI->hintString="Success!";
		g_toNewScene=true;
		newSceneRoll->setTimeInterval(7);
		if(g_soundEnable) g_mainMgr->g_audioMgr->playShortSound("../sounds/guzhang.wav");
	}
	else
	{
		if(g_timeOutRoll->getRollPoint2(0,1)) g_sceneUI->hintString="no response for long time";
		else g_sceneUI->hintString="Failed...";
		g_trialState=0;
		if(g_soundEnable) g_mainMgr->g_audioMgr->playShortSound("../sounds/tryAgain.wav");
	}
	g_sceneUI->showHint();
}

bool SceneWuPin::isCurrentDirectionRight()
{
	if(g_chooseDir==false) {return false;}
	else if(currentAni==4&&currentTurn!=4) {return false;}
	else if(currentAni==5&&currentTurn!=2) {return false;}
	else if(currentAni==6&&currentTurn!=0) {return false;}
	else if(currentAni==7&&currentTurn!=6) {return false;}
	else if(currentAni<4||currentAni>7) {return false;}
	else return true;
}

UINT SceneWuPin::getLeftRight()
{
	if(currentAni==4)
	{
		if(currentTurn>4) return 2;
		else if(currentTurn<4) return 0;
		else return 1;
	}
	else if(currentAni==5)
	{
		if(currentTurn>2) return 2;
		else if(currentTurn<2) return 0;
		else return 1;
	}
	else if(currentAni==6)
	{
		if(currentTurn>0) return 2;
		else if(currentTurn<0) return 0;
		else return 1;
	}
	else if(currentAni==7)
	{
		if(currentTurn>6) return 2;
		else if(currentTurn<6) return 0;
		else return 1;
	}
	else return 3;
}

void SceneWuPin::playStartSound(UINT leftright)
{
	if(!g_soundEnable) return;
	if(leftright==2) g_mainMgr->g_audioMgr->playShortSound("../sounds/thinkLeft.wav");
	else if(leftright==0) g_mainMgr->g_audioMgr->playShortSound("../sounds/thinkRight.wav");
	else g_mainMgr->g_audioMgr->playShortSound("../sounds/fight.wav");
}

bool SceneWuPin::processServerData(std::string recvString,GAServerSimple* server,UINT socketNum)
{
	bool processed=true;
	if(recvString=="left")
	{
		if(!g_freezed) turnLeft();
	}
	else if(recvString=="right")
	{
		if(!g_freezed) turnRight();
	}
	else if(recvString=="start")
	{
		if(g_freezed&&g_sceneEnd==false)
		{
			start();
		}
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
	else if(recvString=="go")
	{
		setWalkTo(0);
	}
	else if(recvString=="lockView")
	{
		g_mainMgr->g_cameraCtrl->lock();
	}
	else if(recvString=="unlockView")
	{
		g_mainMgr->g_cameraCtrl->unlock();
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
	else if(recvString=="showArrow")
	{
		g_showArrow=true;
		server->sendString("done",socketNum);
	}
	else if(recvString=="hideArrow")
	{
		g_showArrow=false;
		server->sendString("done",socketNum);
	}
	else if(recvString=="getLeftRight")
	{
		switch(getLeftRight())
		{
		case 0:
			server->sendString("right",socketNum);
			break;
		case 1:
			server->sendString("front",socketNum);
			break;
		case 2:
			server->sendString("left",socketNum);
			break;
		case 3:
			server->sendString("unknown",socketNum);
			break;
		}
	}
	else if(recvString=="autoPlaySound")
	{
		if(g_soundEnable)
		{
			int k1=getLeftRight();
			if(k1==2) g_mainMgr->g_audioMgr->playShortSound("../sounds/tryLeft.wav");
			else if(k1==0) g_mainMgr->g_audioMgr->playShortSound("../sounds/tryRight.wav");
			else g_mainMgr->g_audioMgr->playShortSound("../sounds/fight.wav");
		}
	}
	else if(recvString=="isCurrentRight")
	{
		if(isCurrentDirectionRight()) server->sendString("yes",socketNum);
		else server->sendString("no",socketNum);
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
		str+="time out:"+misc.ConvertToString(g_timeOut)+";runned:"+misc.ConvertToString(g_timeOutRoll->getCurrentTimeCount());
		server->sendString(str,socketNum);
	}
	else if(recvString=="getScore")
	{
		server->sendString(misc.ConvertToString(g_currentScore),socketNum);
	}
	else if(recvString=="getTime")
	{
		server->sendString(misc.ConvertToString(g_sceneRunningTime),socketNum);
	}
	else if(recvString=="save")
	{
		saveCurrentState("../SceneWuPin/init.txt");
	}
	//复合命令
	else
	{
		//复合命令
		std::string::size_type st;
		std::string str1,str2;
		st=recvString.find_first_of(":");
		if(st!=std::string::npos)
		{
			str1=recvString.substr(0,st);
			str2=recvString.substr(st+1);
			if(str1=="setThisObject")
			{
				if(g_freezed&&!g_started)
				{
					if(g_currentObject<8)
					{
						UINT kkk=misc.ConvertToInt(str2);
						kkk--;
						if(kkk<4)
						{
							SceneQueue[g_currentScore]=kkk;
							g_currentObject=kkk;
							server->sendString("this object:"+str2,socketNum);
						}
						else server->sendString("error input value,must be 1,2,3,4",socketNum);
					}
				}
				else server->sendString("please send this message before game begins",socketNum);
			}
			else if(str1=="setThisTrial")
			{
				//if(g_freezed&&!g_started) start();
				UINT kkk=misc.ConvertToInt(str2);
				kkk--;
				if(kkk<4)
				{
					
					if(!g_oneTurnFinishedSign||g_sceneEnd)
					{
						UINT temp=g_currentScore;
						saveCurrentState("../SceneWuPin/init.txt");
						reCreateScene();
						start();
						g_currentScore=temp;
						g_sceneUI->g_currentScore=g_currentScore;
						g_sceneUI->g_totalScore=g_totalScore;
					}
					g_oneTurnFinishedSign=false;
					SceneQueue[g_currentScore]=kkk;
					g_currentObject=kkk;
					g_freezed=false;
					server->sendString("this object:"+str2,socketNum);
				}
				else server->sendString("error input value,must be 1,2,3,4",socketNum);
			}
			else if(str1=="setNextObject")
			{
				if(g_currentObject<8)
				{
					UINT kkk=misc.ConvertToInt(str2);
					kkk--;
					if(kkk<4)
					{
						SceneQueue[g_currentScore+1]=kkk;
						server->sendString("next object:"+str2,socketNum);
					}
					else server->sendString("error input value,must be 1,2,3,4",socketNum);
				}
				else server->sendString("error input value",socketNum);
			}
			else if(str1=="setTimeLimit")
			{
				if(g_freezed&&!g_started)
				{
					g_limitTime=misc.ConvertToFloat(str2);
					g_sceneUI->countRestart(g_limitTime);
					g_sceneUI->countPause();
					g_sceneUI->ProgressBar1Start(g_limitTime);
					g_sceneUI->ProgressBar1Pause();
					g_sceneUI->welcome=false;
					server->sendString("new time limit:"+misc.ConvertToString(g_limitTime),socketNum);
				}
				else server->sendString("unable to set this value while processing",socketNum);
			}
			else if(str1=="setTimeOut")
			{
				g_timeOut=misc.ConvertToFloat(str2);
				g_timeOutRoll->setTimeInterval(g_timeOut);
				server->sendString("new time out:"+misc.ConvertToString(g_timeOut),socketNum);
			}
			else if(str1=="setTotalTrialNum")
			{
				UINT kkk=misc.ConvertToInt(str2);
				kkk--;
				if(kkk>9) 
				{
					server->sendString("max trial num is 9",socketNum);
				}
				else
				{
					for(UINT i=0;i<=kkk;i++)
					{
						if(SceneQueue[i]==UINT(-1)) SceneQueue[i]=1;
					}
					for(UINT i=kkk+1;i<9;i++)
					{
						SceneQueue[i]=UINT(-1);
					}
					g_totalScore=kkk+1;
					g_sceneUI->g_currentScore=g_currentScore;
					g_sceneUI->g_totalScore=g_totalScore;
					server->sendString("done",socketNum);
				}
			}
			else processed=false;
		}
		else processed=false;
	}
	return processed;
}