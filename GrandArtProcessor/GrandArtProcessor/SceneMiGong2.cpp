#include "SceneMiGong.h"
#include "GAPCameraController.h"
#include "GADataManager.h"
#include "GAPWorldManager.h"
#include "GAPAudioManager.h"

float runSpeedRatio=1;
float rotateSpeedRatio=1;

SceneMiGong::SceneMiGong(GAPMainManager* mainMgr,std::string sceneName) : GASceneBase(mainMgr,sceneName)
{
	totalTime=cellTime=0;
	material1=true;
	prePos=currPos=nextPosL=0;
	g_moving=false;
	g_showArrow=false;
	g_rotateCamera=false;
	currDegrees=0;
	g_processing=false;
	g_timeLimit[0]=g_timeLimit[1]=g_timeLimit[2]=g_timeLimit[3]=999;
	g_degreeSpeed=45;
	g_45Degrees=false;
	g_currentDegree=0;
	g_timeOut=30;
	g_timeOutRoll=new GARollPoint(g_timeOut);
	g_showArrows=true;
	g_showArrowsState=0;
	g_soundEnable=true;
	g_soundType=0;
}

bool SceneMiGong::createScene()
{
	Ogre::SceneManager* sm=Ogre::Root::getSingleton().createSceneManager("BspSceneManager","migong");
	Ogre::Camera* cam=sm->createCamera("migongCam");
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
	cam->setFOVy(Ogre::Degree(30));
	sm->setSkyBox(true,"Examples/MorningSkyBox",2500);
	sm->getSkyBoxNode()->pitch(Ogre::Degree(90));
	
	Ogre::SceneNode* ballnode=sm->getRootSceneNode()->createChildSceneNode("ballnode");
	ballnode->pitch(Ogre::Degree(90));
	Ogre::SceneNode* mannode=ballnode->createChildSceneNode("mannode");
	Ogre::Entity *ballEnt=sm->createEntity("woman12.mesh");
	ballEnt->setMaterialName("body");
	mannode->attachObject(ballEnt);
	mannode->setScale(0.1,0.1,0.1);
	Ogre::SceneNode* ballRoundNode=ballnode->createChildSceneNode("ballRoundNode",Ogre::Vector3(0,10,0));
	ballnode->setPosition(-280,240,0);
	GACharacterSimpleObject* simobj=new GACharacterSimpleObject(sm,ballnode,mannode,ballEnt);
	simobj->setFacingDirection(5);
	g_ballAI=new GACharacterAISimpleObject(simobj);
	g_ballAI->runAnimation("idle");
	g_height=0;
	arrow1Pivot=ballRoundNode->createChildSceneNode("arrow1Pivot");
	Ogre::SceneNode* arrownodeL=arrow1Pivot->createChildSceneNode("arrownodeL");
	Ogre::Entity *arrowEntL=sm->createEntity("arrowL","arrow.mesh");
	arrow2Pivot=ballRoundNode->createChildSceneNode("arrow2Pivot");
	Ogre::SceneNode* arrownodeR=arrow2Pivot->createChildSceneNode("arrownodeR");
	Ogre::Entity *arrowEntR=sm->createEntity("arrowR","arrow.mesh");
	arrowEntL->setMaterialName("Examples/Solid/Pink");
	arrowEntR->setMaterialName("Examples/Solid/Blue");
	arrownodeL->attachObject(arrowEntL);
	arrownodeR->attachObject(arrowEntR);
	arrownodeR->roll(Ogre::Degree(180),Ogre::Node::TS_WORLD);
	Ogre::Light *l=sm->createLight();
	l->setPosition(0,0,100);
	arrownodeL->setScale(0.6,0.6,0.6);
	arrownodeL->setPosition(20,0,0);
	arrownodeR->setScale(0.6,0.6,0.6);
	arrownodeR->setPosition(-20,0,0);
	Ogre::SceneNode* stayNode=mannode->createChildSceneNode();
	stayNode->setPosition(0,2000,-2000);
	stayNode->roll(Ogre::Degree(180),Ogre::Node::TS_WORLD);
	stayNode->pitch(Ogre::Degree(-40));
	g_mainMgr->g_cameraCtrl->setCameraStyle(GAPCameraController::CameraStyle_StayOnNode,stayNode);
	g_mainMgr->g_cameraCtrl->lock();
	g_mainMgr->g_cameraCtrl->lock();
	g_mainMgr->g_worldMgr->addModel("endCube","cube.mesh",Ogre::Vector3(0,0,0),Ogre::Vector3(0.12,0.16,0.12));
	g_endNode=g_mainMgr->getSceneNode("endCube");
	g_mainMgr->getCurrentSceneMgr()->getEntity("endCube")->setMaterialName("Examples/transparent/pink");
	g_worldName="migong1234_2.bsp";
	g_mainMgr->getCurrentSceneMgr()->setWorldGeometry(g_worldName);
	g_output.createNewFile("../SceneMiGong/MiGongOutput.txt");
	g_mainMgr->g_UIMgr->switchToUI("SceneUI");
	g_sceneUI=(GAExampleSceneUI*)g_mainMgr->g_UIMgr->findUI("SceneUI");
	waitressNode=g_mainMgr->getCurrentSceneMgr()->getRootSceneNode()->createChildSceneNode();
	Ogre::Entity* entWaitress=g_mainMgr->getCurrentSceneMgr()->createEntity("waitress.mesh");
	waitressNode->pitch(Ogre::Degree(90));
	waitressNode->roll(Ogre::Degree(-90),Ogre::Node::TS_WORLD);
	waitressNode->attachObject(entWaitress);
	waitressNode->scale(0.1,0.1,0.1);
	GACharacterSimpleObject* simobj2=new GACharacterSimpleObject(sm,waitressNode,waitressNode,entWaitress);
	simobj2->setFacingDirection(5);
	g_endManAI=new GACharacterAISimpleObject(simobj2);
	g_endManAI->runAnimation("open");
	g_endNode->setVisible(false);
	reCreateScene();
	return true;
}

bool SceneMiGong::reCreateScene()
{
	loadInitFile("../SceneMiGong/init.txt");
	g_currentScore=0;
	setScene1();
	Ogre::Vector3 fac=g_ballAI->getCharacter()->getFacingDirection();
	Ogre::Vector3 dd(0,1,0);
	float degree=dd.getRotationTo(fac).getRoll().valueDegrees();
	if(degree>180) degree-=360;
	else if(degree<-180) degree+=360;
	g_ballAI->rollAlongZAxis(-degree);
	setRotating();
	g_freezed=true;
	//g_sceneUI->hintString=L"请按F1键开始游戏";
	g_sceneUI->hideHint();
	g_started=false;
	g_sceneUI->countRestart(g_timeLimit[g_currentScore]);
	g_sceneUI->countPause();
	g_sceneUI->setProgressBarRatio(1);
	g_sceneUI->progressBar2Visible(false);
	g_sceneEnd=false;
	g_sceneUI->g_freezed=false;
	g_timeOutRoll->setTimeInterval(g_timeOut);
	g_trialState=0;
	if(g_sceneUI->welcome)
	{
		g_sceneUI->hintString=L"欢迎使用交大虚拟康复训练平台";
		g_sceneUI->showHint();
	}
	g_showArrows=true;
	g_showArrowsState=0;
	return true;
}

void SceneMiGong::start()
{
	g_mainMgr->g_cameraCtrl->lock();
	if(g_currentScore==0) setScene1();
	else if(g_currentScore==1) setScene2();
	else if(g_currentScore==2) setScene3();
	else setScene4();
	setRotating();
	g_freezed=false;
	g_sceneUI->g_freezed=false;
	g_started=true;
	g_sceneUI->hideHint();
	g_sceneUI->countRestart(g_timeLimit[g_currentScore]);
	g_sceneUI->countBegin();
	g_sceneUI->g_totalScore=4;
	g_sceneUI->g_currentScore=g_currentScore;
	g_sceneUI->ProgressBar1Start(g_timeLimit[g_currentScore]);
	g_sceneUI->progressBar2Visible(false);
	g_sceneUI->welcome=false;
}

bool SceneMiGong::mouseMoved(const OIS::MouseEvent &evt)
{
	return true;
}

bool SceneMiGong::keyReleased(const OIS::KeyEvent& evt)
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
	if(g_freezed) return true;
	else if(evt.key==OIS::KC_RIGHT)
	{
		rotateRight();
	}
	else if(evt.key==OIS::KC_LEFT)
	{
		rotateLeft();
	}
	return true;
}
GAFloat3Struct SceneMiGong::getRunningTime(bool leftRotating)
{
	GAFloat3Struct ret;
	ret.x=ret.y=ret.z=0;
	GAFloat3Struct* s1=g_cornerLeaf->getDataMap()->getStructFloat3Data(currPos);
	GAFloat3Struct* s2;
	if(leftRotating) s2=g_cornerLeaf->getDataMap()->getStructFloat3Data(nextPosL);
	else s2=g_cornerLeaf->getDataMap()->getStructFloat3Data(nextPosR);
	if(g_45Degrees) ret.x=45.f/(rotateSpeedRatio*g_degreeSpeed);
	else ret.x=90.f/(rotateSpeedRatio*g_degreeSpeed);
	Ogre::Vector3 dir(s2->x-s1->x,s2->y-s1->y,s2->z-s1->z);
	float distance=dir.length();
	ret.y=distance/(runSpeedRatio*50.f);
	dir.normalise();
	if(leftRotating) ret.z=getRotateDegreeOfNextPoint(currPos,nextPosL,dir);
	else ret.z=getRotateDegreeOfNextPoint(currPos,nextPosR,dir);
	ret.z=abs(ret.z)/(rotateSpeedRatio*g_degreeSpeed);
	return ret;
}
bool SceneMiGong::rotateLeft()
{
	GAFloat3Struct* s=g_cornerLeaf->getDataMap()->getStructFloat3Data(nextPosL);
	if(s&&!g_processing&&!g_rotateCamera)
	{
		g_leftRotating=true;
		if(g_45Degrees) setWomanRotate(45);
		else setWomanRotate(90);
		arrow1Pivot->setVisible(false);
		arrow2Pivot->setVisible(false);
		g_showArrowsState=0;
		g_processing=true;
		g_ballAI->runAnimation("idle");
		g_timeOutRoll->setTimeInterval(g_timeOut);
		return true;
	}
	return false;
}
void SceneMiGong::goRoadLeft()
{
	GAFloat3Struct* s=g_cornerLeaf->getDataMap()->getStructFloat3Data(nextPosL);
	std::string line=misc.ConvertToString(totalTime)+":left";
	if(s)
	{
		prePos=currPos;
		currPos=nextPosL;
		g_ballAI->moveToNewPlaceBySpeed(s->x,s->y,g_height,50*runSpeedRatio);
		arrow1Pivot->setVisible(false);
		arrow2Pivot->setVisible(false);
		g_showArrowsState=0;
		g_ballAI->runAnimation("run");
	}
	else if(s) line="#"+line;
	else line="@"+line;
	g_output.writeLine(line);
}

bool SceneMiGong::rotateRight()
{
	GAFloat3Struct* s=g_cornerLeaf->getDataMap()->getStructFloat3Data(nextPosR);
	if(s&&!g_processing&&!g_rotateCamera)
	{
		g_leftRotating=false;
		if(g_45Degrees) setWomanRotate(-45);
		else setWomanRotate(-90);
		arrow1Pivot->setVisible(false);
		arrow2Pivot->setVisible(false);
		g_showArrowsState=0;
		g_processing=true;
		g_ballAI->runAnimation("idle");
		g_timeOutRoll->setTimeInterval(g_timeOut);
		return true;
	}
	return false;
}

void SceneMiGong::goRoadRight()
{
	GAFloat3Struct* s=g_cornerLeaf->getDataMap()->getStructFloat3Data(nextPosR);
	std::string line=misc.ConvertToString(totalTime)+":right";
	if(s)
	{
		prePos=currPos;
		currPos=nextPosR;
		g_ballAI->moveToNewPlaceBySpeed(s->x,s->y,g_height,50*runSpeedRatio);
		arrow1Pivot->setVisible(false);
		arrow2Pivot->setVisible(false);
		g_showArrowsState=0;
		g_ballAI->runAnimation("run");
	}
	else if(s) line="#"+line;
	else line="@"+line;
	g_output.writeLine(line);
}

void SceneMiGong::rotateCamera(float degrees,float speed)
{
	if(degrees<0&&speed>0) speed=-speed;
	if(degrees>0&&speed<0) speed=-speed;
	if(!g_rotateCamera) g_mainMgr->g_cameraCtrl->setAutoYaw(degrees,speed);
	g_rotateCamera=true;                                                                  
}

bool SceneMiGong::frameStarted(float deltaTime)
{
	arrow1Pivot->setVisible(false);
	arrow2Pivot->setVisible(false);
	if(g_freezed) return true;
	g_trialState=1;
	g_ballAI->update(deltaTime);
	totalTime+=deltaTime;
	if(g_timeOutRoll->getRollPoint2(deltaTime,1)) oneSceneEnd(false);
	if(!g_processing) if(g_sceneUI->isCountOver()) oneSceneEnd(false);
	if(g_rotateCamera)
	{
		float deltaDegree=deltaTime*g_degreeSpeed*rotateSpeedRatio;
		if(g_rotateDgrees<0)
		{
			deltaDegree=-deltaDegree;
			g_currentDegree+=deltaDegree;
			if(g_currentDegree<=g_rotateDgrees)
			{
				deltaDegree=g_rotateDgrees-(g_currentDegree-deltaDegree); 
				g_rotateCamera=false;
			}
		}
		else if(g_rotateDgrees>0)
		{
			g_currentDegree+=deltaDegree;
			if(g_currentDegree>=g_rotateDgrees)
			{
				deltaDegree=g_rotateDgrees-(g_currentDegree-deltaDegree); 
				g_rotateCamera=false;
			}
		}
		g_ballAI->rollAlongZAxis(deltaDegree);
		if(!g_rotateCamera&&g_processing)
		{
			if(g_leftRotating) goRoadLeft();
			else goRoadRight();
		}
	}
	if(g_processing&&g_rotateCamera==false)
	{
		doMoving(deltaTime);
	}
	g_endManAI->update(deltaTime);
	arrow1Pivot->setVisible(false);
	arrow2Pivot->setVisible(false);
	if(g_showArrows)
	{
		if(g_showArrowsState==1) arrow1Pivot->setVisible(true);
		else if(g_showArrowsState==2) arrow2Pivot->setVisible(true);
		else if(g_showArrowsState==3)
		{
			arrow1Pivot->setVisible(true);
			arrow2Pivot->setVisible(true);
		}
	}
	return true;
}

void SceneMiGong::doMoving(float deltaTime)
{
	g_moving=g_ballAI->g_move;
	if(g_moving==false) setRotating();
}
float SceneMiGong::getRotateDegreeOfNextPoint(UINT pre,UINT pos,Ogre::Vector3 roadDir)
{
	float ret=0;
	bool Degrees45=false;
	float degree=0;
	bool beLeft=false;
	UINT pos1,dir1,pos2,dir2;
	pos1=pos2=(UINT)-1;
	dir1=dir2=0;
	GAFloat4Struct* s4=g_leftRightLeaf->getDataMap()->getStructFloat4Data(pos);
	if(s4)
	{
		UINT count=0;
		if(s4->x>=0&&(UINT(s4->x))!=pre) count++;
		if(s4->y>=0&&(UINT(s4->y))!=pre) count++;
		if(s4->z>=0&&(UINT(s4->z))!=pre) count++;
		if(s4->w>=0&&(UINT(s4->w))!=pre) count++;
		if(count==0)
		{
			if(pos==finalPos)
			{
				ret=0;
			}
			else ret=90;
		}
		else if(count==1)
		{
			if(s4->x>=0&&(UINT(s4->x))!=pre)
			{
				pos1=s4->x;
				dir1=1;
			}
			else if(s4->y>=0&&(UINT(s4->y))!=pre)
			{
				pos1=s4->y;
				dir1=2;
			}
			else if(s4->z>=0&&(UINT(s4->z))!=pre)
			{
				pos1=s4->z;
				dir1=3;
			}
			else 
			{
				pos1=s4->w;
				dir1=4;
			}
			Ogre::Vector3 fac=roadDir;
			UINT facPos=0;
			if(fac.dotProduct(Ogre::Vector3(0,1,0))>0.9) facPos=1;
			else if(fac.dotProduct(Ogre::Vector3(1,0,0))>0.9) facPos=2;
			else if(fac.dotProduct(Ogre::Vector3(0,-1,0))>0.9) facPos=3;
			else facPos=4;
			int d1=facPos-dir1;
			if(d1==2||d1==-2)
			{
				ret=90;
			}
			else if(d1==0)
			{
				ret=90;
			}
			if(d1==-1||d1==3)
			{
				ret=0;
			}
		}
		else if(count==2)
		{
			if(s4->x>=0&&(UINT(s4->x))!=pre)
			{
				pos1=s4->x;dir1=1;
			}
			if(s4->y>=0&&(UINT(s4->y))!=pre)
			{
				if(pos1==(UINT)-1)
				{
					pos1=s4->y;dir1=2;
				}
				else
				{
					pos2=s4->y;dir2=2;
				}
			}
			if(s4->z>=0&&(UINT(s4->z))!=pre)
			{
				if(pos1==(UINT)-1)
				{
					pos1=s4->z;dir1=3;
				}
				else
				{
					pos2=s4->z;dir2=3;
				}
			}
			if(s4->w>=0&&(UINT(s4->w))!=pre)
			{
				pos2=s4->w;dir2=4;
			}
			Ogre::Vector3 fac=roadDir;
			UINT facPos=0;
			if(fac.dotProduct(Ogre::Vector3(0,1,0))>0.9) facPos=1;
			else if(fac.dotProduct(Ogre::Vector3(1,0,0))>0.9) facPos=2;
			else if(fac.dotProduct(Ogre::Vector3(0,-1,0))>0.9) facPos=3;
			else facPos=4;
			int d1=dir2-dir1;
			if(d1==1||d1==3||d1==-1||d1==-3)
			{
				Degrees45=true;
				UINT d2=dir1+dir2;
				Ogre::Vector3 dd(1,1,0);
				if(d2==5)
				{
					if(dir1==1||dir2==1) dd.x=-1;
					else if(dir1==2||dir2==2) dd.y=-1;
				}
				else if(d2==7)
				{
					dd.x=-1;dd.y=-1;
				}
				dd.normalise();
				float degree=dd.getRotationTo(fac).getRoll().valueDegrees();
				if(degree>180) degree-=360;
				else if(degree<-180) degree+=360;
				ret=degree;
			}
		}
	}
	return ret;
}
void SceneMiGong::setRotating()
{
	g_ballAI->runAnimation("idle");
	g_45Degrees=false;
	float degree=0;
	nextPosL=(UINT)-1;
	nextPosR=(UINT)-1;
	bool beLeft=false;
	UINT pos1,dir1,pos2,dir2;
	pos1=pos2=(UINT)-1;
	dir1=dir2=0;
	GAFloat4Struct* s4=g_leftRightLeaf->getDataMap()->getStructFloat4Data(currPos);
	if(s4)
	{
		UINT count=0;
		if(s4->x>=0&&(UINT(s4->x))!=prePos) count++;
		if(s4->y>=0&&(UINT(s4->y))!=prePos) count++;
		if(s4->z>=0&&(UINT(s4->z))!=prePos) count++;
		if(s4->w>=0&&(UINT(s4->w))!=prePos) count++;
		if(count==0)
		{
			if(currPos==finalPos)
			{
				oneSceneEnd(true);
				return;
			}
			if(s4->x>=0)
			{
				pos1=s4->x;
				dir1=1;
			}
			else if(s4->y>=0)
			{
				pos1=s4->y;
				dir1=2;
			}
			else if(s4->z>=0)
			{
				pos1=s4->z;
				dir1=3;
			}
			else 
			{
				pos1=s4->w;
				dir1=4;
			}
			if(beLeft) setWomanRotate(-90);
			else setWomanRotate(90);
		}
		else if(count==1)
		{
			if(s4->x>=0&&(UINT(s4->x))!=prePos)
			{
				pos1=s4->x;
				dir1=1;
			}
			else if(s4->y>=0&&(UINT(s4->y))!=prePos)
			{
				pos1=s4->y;
				dir1=2;
			}
			else if(s4->z>=0&&(UINT(s4->z))!=prePos)
			{
				pos1=s4->z;
				dir1=3;
			}
			else 
			{
				pos1=s4->w;
				dir1=4;
			}
			Ogre::Vector3 fac=g_ballAI->getCharacter()->getFacingDirection();
			UINT facPos=0;
			if(fac.dotProduct(Ogre::Vector3(0,1,0))>0.9) facPos=1;
			else if(fac.dotProduct(Ogre::Vector3(1,0,0))>0.9) facPos=2;
			else if(fac.dotProduct(Ogre::Vector3(0,-1,0))>0.9) facPos=3;
			else facPos=4;
			int d1=facPos-dir1;
			if(d1==2||d1==-2)
			{
				if(beLeft) setWomanRotate(-90);
				else setWomanRotate(90);
			}
			else if(d1==0)
			{
				if(beLeft) setWomanRotate(90);
				else setWomanRotate(-90);
			}
			if(d1==-1||d1==3)
			{
				dir2=dir1;
				pos2=pos1;
				dir1=0;pos1=(UINT)-1;
			}
		}
		else if(count==2)
		{
			if(s4->x>=0&&(UINT(s4->x))!=prePos)
			{
				pos1=s4->x;dir1=1;
			}
			if(s4->y>=0&&(UINT(s4->y))!=prePos)
			{
				if(pos1==(UINT)-1)
				{
					pos1=s4->y;dir1=2;
				}
				else
				{
					pos2=s4->y;dir2=2;
				}
			}
			if(s4->z>=0&&(UINT(s4->z))!=prePos)
			{
				if(pos1==(UINT)-1)
				{
					pos1=s4->z;dir1=3;
				}
				else
				{
					pos2=s4->z;dir2=3;
				}
			}
			if(s4->w>=0&&(UINT(s4->w))!=prePos)
			{
				pos2=s4->w;dir2=4;
			}
			Ogre::Vector3 fac=g_ballAI->getCharacter()->getFacingDirection();
			UINT facPos=0;
			if(fac.dotProduct(Ogre::Vector3(0,1,0))>0.9) facPos=1;
			else if(fac.dotProduct(Ogre::Vector3(1,0,0))>0.9) facPos=2;
			else if(fac.dotProduct(Ogre::Vector3(0,-1,0))>0.9) facPos=3;
			else facPos=4;
			int d1=dir2-dir1;
			if(d1==1||d1==3||d1==-1||d1==-3)
			{
				g_45Degrees=true;
				UINT d2=dir1+dir2;
				Ogre::Vector3 dd(1,1,0);
				if(d2==5)
				{
					if(dir1==1||dir2==1) dd.x=-1;
					else if(dir1==2||dir2==2) dd.y=-1;
				}
				else if(d2==7)
				{
					dd.x=-1;dd.y=-1;
				}
				dd.normalise();
				float degree=dd.getRotationTo(fac).getRoll().valueDegrees();
				if(degree>180) degree-=360;
				else if(degree<-180) degree+=360;
				setWomanRotate(-degree);
				if(d1==3)
				{
					UINT temp1;
					temp1=pos1;pos1=pos2;pos2=temp1;
					temp1=dir1;dir1=dir2;dir2=temp1;
				}
			}
			else if(d1==2)
			{
				if(facPos!=((dir1+dir2)/2))
				{
					UINT temp1;
					temp1=pos1;pos1=pos2;pos2=temp1;
					temp1=dir1;dir1=dir2;dir2=temp1;
				}
			}
		}
	}
	else return;
	arrowLDir=dir1;
	nextPosL=pos1;
	arrowRDir=dir2;
	nextPosR=pos2;
	setArrows();
	g_processing=false;   
}

void SceneMiGong::setRotating2()
{
	g_ballAI->runAnimation("idle");
	g_45Degrees=false;
	float degree=0;
	nextPosL=(UINT)-1;
	nextPosR=(UINT)-1;
	bool beLeft=false;
	UINT pos1,dir1,pos2,dir2;
	pos1=pos2=(UINT)-1;
	dir1=dir2=0;
	GAFloat4Struct* s4=g_leftRightLeaf->getDataMap()->getStructFloat4Data(currPos);
	if(s4)
	{
		UINT count=0;
		if(s4->x>=0&&(UINT(s4->x))!=prePos) count++;
		if(s4->y>=0&&(UINT(s4->y))!=prePos) count++;
		if(s4->z>=0&&(UINT(s4->z))!=prePos) count++;
		if(s4->w>=0&&(UINT(s4->w))!=prePos) count++;
		if(count==0)
		{
			if(currPos==finalPos)
			{
				oneSceneEnd(true);
				return;
			}
			if(s4->x>=0)
			{
				pos1=s4->x;
				dir1=1;
			}
			else if(s4->y>=0)
			{
				pos1=s4->y;
				dir1=2;
			}
			else if(s4->z>=0)
			{
				pos1=s4->z;
				dir1=3;
			}
			else 
			{
				pos1=s4->w;
				dir1=4;
			}
			Ogre::Vector3 fac=g_ballAI->getCharacter()->getFacingDirection();
			Ogre::Vector3 dd(1,1,0);
			if(dir1==1) dd.x=0;
			else if(dir1==2) dd.y=0;
			else if(dir1==3) {dd.x=0;dd.y=-1;}
			else if(dir1==4) {dd.x=-1;dd.y=0;}
			float degree=dd.getRotationTo(fac).getRoll().valueDegrees();
			if(degree>180) degree-=360;
			else if(degree<-180) degree+=360;
			if(degree>=0) setWomanRotate(degree-90); 
			else setWomanRotate(degree+90);
		}
		else if(count==1)
		{
			if(s4->x>=0&&(UINT(s4->x))!=prePos)
			{
				pos1=s4->x;
				dir1=1;
			}
			else if(s4->y>=0&&(UINT(s4->y))!=prePos)
			{
				pos1=s4->y;
				dir1=2;
			}
			else if(s4->z>=0&&(UINT(s4->z))!=prePos)
			{
				pos1=s4->z;
				dir1=3;
			}
			else 
			{
				pos1=s4->w;
				dir1=4;
			}
			Ogre::Vector3 fac=g_ballAI->getCharacter()->getFacingDirection();
			UINT facPos=0;
			if(fac.dotProduct(Ogre::Vector3(0,1,0))>0.9) facPos=1;
			else if(fac.dotProduct(Ogre::Vector3(1,0,0))>0.9) facPos=2;
			else if(fac.dotProduct(Ogre::Vector3(0,-1,0))>0.9) facPos=3;
			else facPos=4;
			int d1=facPos-dir1;
			Ogre::Vector3 dd(1,1,0);
			if(dir1==1) dd.x=0;
			else if(dir1==2) dd.y=0;
			else if(dir1==3) {dd.x=0;dd.y=-1;}
			else if(dir1==4) {dd.x=-1;dd.y=0;}
			float degree=dd.getRotationTo(fac).getRoll().valueDegrees();
			if(degree>180) degree-=360;
			else if(degree<-180) degree+=360;
			if(degree>=0) setWomanRotate(degree-90); 
			else setWomanRotate(degree+90);
			if(d1==-1||d1==3)
			{
				dir2=dir1;
				pos2=pos1;
				dir1=0;pos1=(UINT)-1;
			}
		}
		else if(count==2)
		{
			if(s4->x>=0&&(UINT(s4->x))!=prePos)
			{
				pos1=s4->x;dir1=1;
			}
			if(s4->y>=0&&(UINT(s4->y))!=prePos)
			{
				if(pos1==(UINT)-1)
				{
					pos1=s4->y;dir1=2;
				}
				else
				{
					pos2=s4->y;dir2=2;
				}
			}
			if(s4->z>=0&&(UINT(s4->z))!=prePos)
			{
				if(pos1==(UINT)-1)
				{
					pos1=s4->z;dir1=3;
				}
				else
				{
					pos2=s4->z;dir2=3;
				}
			}
			if(s4->w>=0&&(UINT(s4->w))!=prePos)
			{
				pos2=s4->w;dir2=4;
			}
			Ogre::Vector3 fac=g_ballAI->getCharacter()->getFacingDirection();
			UINT facPos=0;
			if(fac.dotProduct(Ogre::Vector3(0,1,0))>0.9) facPos=1;
			else if(fac.dotProduct(Ogre::Vector3(1,0,0))>0.9) facPos=2;
			else if(fac.dotProduct(Ogre::Vector3(0,-1,0))>0.9) facPos=3;
			else facPos=4;
			int d1=dir2-dir1;
			if(d1==1||d1==3||d1==-1||d1==-3)
			{
				g_45Degrees=true;
				UINT d2=dir1+dir2;
				Ogre::Vector3 dd(1,1,0);
				if(d2==5)
				{
					if(dir1==1||dir2==1) dd.x=-1;
					else if(dir1==2||dir2==2) dd.y=-1;
				}
				else if(d2==7)
				{
					dd.x=-1;dd.y=-1;
				}
				dd.normalise();
				float degree=dd.getRotationTo(fac).getRoll().valueDegrees();
				if(degree>180) degree-=360;
				else if(degree<-180) degree+=360;
				setWomanRotate(-degree);
				if(d1==3)
				{
					UINT temp1;
					temp1=pos1;pos1=pos2;pos2=temp1;
					temp1=dir1;dir1=dir2;dir2=temp1;
				}
			}
			else if(d1==2)
			{
				if(facPos!=((dir1+dir2)/2))
				{
					UINT temp1;
					temp1=pos1;pos1=pos2;pos2=temp1;
					temp1=dir1;dir1=dir2;dir2=temp1;
				}
			}
		}
	}
	else return;
	arrowLDir=dir1;
	nextPosL=pos1;
	arrowRDir=dir2;
	nextPosR=pos2;
	setArrows();
	g_processing=false;   
}

void SceneMiGong::setArrows()
{
	arrow1Pivot->setOrientation(Ogre::Quaternion::IDENTITY);
	arrow2Pivot->setOrientation(Ogre::Quaternion::IDENTITY);
	g_showArrowsState=0;
	if(arrowLDir>0)
	{
		arrow1Pivot->setVisible(true);
		g_showArrowsState+=1;
		arrow1Pivot->roll(Ogre::Degree(180-arrowLDir*90),Ogre::Node::TS_WORLD);
	}
	if(arrowRDir>0)
	{
		g_showArrowsState+=2;
		arrow2Pivot->setVisible(true);
		arrow2Pivot->roll(Ogre::Degree(-90*arrowRDir),Ogre::Node::TS_WORLD);
	}
}

void SceneMiGong::toNewScene()
{
	if(g_currentScene==1)
	{
		std::string line="Scene1 ended at "+misc.ConvertToString(totalTime);
		g_output.writeLine(line);
		setScene2();
		g_sceneUI->countRestart(g_timeLimit[g_currentScene]);
		g_sceneUI->countBegin();
		g_sceneUI->ProgressBar1Start(g_timeLimit[g_currentScene]);
	}
	else if(g_currentScene==2)
	{
		std::string line="Scene2 ended at "+misc.ConvertToString(totalTime);
		g_output.writeLine(line);
		setScene3();
		g_sceneUI->countRestart(g_timeLimit[g_currentScene]);
		g_sceneUI->countBegin();
		g_sceneUI->ProgressBar1Start(g_timeLimit[g_currentScene]);
	}
	else if(g_currentScene==3)
	{
		std::string line="Scene3 ended at "+misc.ConvertToString(totalTime);
		g_output.writeLine(line);
		setScene4();
		g_sceneUI->countRestart(g_timeLimit[g_currentScene]);
		g_sceneUI->countBegin();
		g_sceneUI->ProgressBar1Start(g_timeLimit[g_currentScene]);
	}
	else
	{
		g_freezed=true;
		std::string line="Scene4 ended at "+misc.ConvertToString(totalTime);
		g_output.writeLine(line);
		end();
		return;
		//g_mainMgr->g_UIMgr->switchToUI("EnterUI");
	}
	setRotating();
	g_sceneUI->g_currentScore=g_currentScore;
}

int SceneMiGong::getLRCommandFromFile(std::string filePath)
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

void SceneMiGong::loadInitFile(std::string filePath)
{
	srand(GetTickCount());
	if(!fileIO.openFile(filePath)) return;
	std::string str=fileIO.getNextLine();
	std::string temp;
	std::string::size_type st;
	UINT scene=0;
	while(str!="")
	{
		
		st=str.find_first_of(":");
		temp=str.substr(0,st);
		str=str.substr(st+1,str.length()-1);
		if(temp=="timeLimit")
		{
			st=str.find_first_of(",");
			UINT tempCount=0;
			while(st!=str.npos)
			{
				temp=str.substr(0,st);
				str=str.substr(st+1,str.length()-1);
				g_timeLimit[tempCount]=misc.ConvertToInt(temp);
				tempCount++;
				st=str.find_first_of(",");
			}
			g_timeLimit[tempCount]=misc.ConvertToInt(str);
		}
		else if(temp=="timeOut")
		{
			g_timeOut=misc.ConvertToFloat(str);
			g_timeOutRoll->setTimeInterval(g_timeOut);
		}
		str=fileIO.getNextLine();
	}
	fileIO.endInput();
}

void SceneMiGong::saveCurrentState(std::string filePath)
{
	GAFileIO* file=new GAFileIO();
	if(!file->createNewFile(filePath)) return;
	std::string line="";
	line+="timeLimit:"+misc.ConvertToString(g_timeLimit[0])+","+misc.ConvertToString(g_timeLimit[1])+","+misc.ConvertToString(g_timeLimit[2])+","+misc.ConvertToString(g_timeLimit[3]);
	file->writeLine(line);
	line="timeOut:"+misc.ConvertToString(g_timeOut);
	file->writeLine(line);
}


void SceneMiGong::setWomanRotate(float degree)
{
	if(g_rotateCamera==true) return;
	g_rotateCamera=true;
	g_rotateDgrees=degree;
	g_currentDegree=0;
	//g_ballAI->rollAlongZAxis(degree); //Ogre::Degree(degree),Ogre::Node::TS_WORLD);
}


void SceneMiGong::oneSceneEnd(bool finished)
{
	g_sceneUI->countPause();
	g_sceneUI->ProgressBar1Pause();
	if(finished) //toNewScene();
	{
		g_trialState=2;
		g_freezed=true;
		g_currentScore++;
		g_sceneUI->hintString="Passed!";
		if(g_soundEnable) g_mainMgr->g_audioMgr->playShortSound("../sounds/guzhang.wav");
		g_sceneUI->showHint();
		if(g_currentScore==g_sceneUI->g_totalScore) end();
	}
	else
	{
		if(!g_timeOutRoll->getRollPoint2(0,1)) g_sceneUI->hintString="Failed...";
		else g_sceneUI->hintString="no response for long time";
		g_timeOutRoll->setTimeInterval(g_timeOut);
		g_sceneUI->showHint();
		g_sceneUI->countPause();
		g_trialState=0;
		g_freezed=true;
		if(g_soundEnable) g_mainMgr->g_audioMgr->playShortSound("../sounds/tryAgain.wav");
	}
}

void SceneMiGong::end()
{
	g_sceneUI->hintString="Success!";
	g_sceneUI->showHint();
	g_sceneUI->countPause();
	g_freezed=true;
	g_sceneEnd=true;
}

bool SceneMiGong::freezeScene()
{
	GASceneBase::freezeScene();
	g_totalSeconds=g_sceneUI->g_totalSeconds;
	g_currentSeconds=g_sceneUI->g_currentTimeSeconds;
	g_currentScore=g_sceneUI->g_currentScore;
	s_progressBar1Ratio=g_sceneUI->getProgressBarRatio();
	return true;
}

bool SceneMiGong::reloadScene()
{
	GASceneBase::reloadScene();
	if(!g_started) g_freezed=true;
	g_mainMgr->g_cameraCtrl->lock();
	g_mainMgr->g_UIMgr->switchToUI("SceneUI");
	g_sceneUI->g_totalSeconds=g_totalSeconds;
	g_sceneUI->g_currentTimeSeconds=g_currentSeconds;
	g_sceneUI->g_currentScore=g_currentScore;
	g_sceneUI->progressBar1Visible(true);
	g_sceneUI->ProgressBar1Start(g_totalSeconds);
	g_sceneUI->setProgressBarRatio(s_progressBar1Ratio);
	g_sceneUI->progressBar2Visible(false);
	g_sceneUI->caption="Score";
	if(g_freezed)
	{
		reCreateScene();
	}
	else
	{
		g_sceneUI->hideHint();
		g_sceneUI->countBegin();
	}
	return true;
}

void SceneMiGong::setScene1()
{
	g_currentScene=1;
	g_totalSeconds=g_timeLimit[0];
	//g_currentScore=0;
	Ogre::SceneNode* sn=g_mainMgr->getSceneNode("ballnode");
	prePos=currPos=0;
	setData1();
	finalPos=g_cornerLeaf->getDataMap()->getStructVector()->size()-1;
	GAFloat3Struct* s3=g_cornerLeaf->getDataMap()->getStructFloat3Data(0);
	sn->setPosition(s3->x,s3->y,g_height);
	nextPosL=g_leftRightLeaf->getDataMap()->getStructFloat4Data(currPos)->y;
	arrowRDir=0;
	arrowLDir=2;
	nextPosR=(UINT)-1;
	GAFloat3Struct* s=g_cornerLeaf->getDataMap()->getStructFloat3Data(finalPos);
	waitressNode->setPosition(s->x+8,s->y,g_height);
	waitressNode->setOrientation(Ogre::Quaternion::IDENTITY);
	waitressNode->pitch(Ogre::Degree(90));
	waitressNode->roll(Ogre::Degree(-90),Ogre::Node::TS_WORLD);
	arrow1Pivot->setVisible(true);
	g_showArrowsState=1;
	arrow2Pivot->setVisible(false);
	std::string line="Scene1 Started at "+misc.ConvertToString(totalTime);
	g_output.writeLine(line);
}

void SceneMiGong::setScene2()
{
	g_currentScene=2;
	g_totalSeconds=g_timeLimit[1];
	//g_currentScore=1;
	Ogre::SceneNode* sn=g_mainMgr->getSceneNode("ballnode");
	prePos=currPos=0;
	setData2();
	finalPos=g_cornerLeaf->getDataMap()->getStructVector()->size()-1;
	GAFloat3Struct* s3=g_cornerLeaf->getDataMap()->getStructFloat3Data(0);
	sn->setPosition(s3->x,s3->y,g_height);
	nextPosL=g_leftRightLeaf->getDataMap()->getStructFloat4Data(currPos)->y;
	arrowRDir=0;
	arrowLDir=2;
	nextPosL=(UINT)-1;
	GAFloat3Struct* s=g_cornerLeaf->getDataMap()->getStructFloat3Data(finalPos);
	waitressNode->setPosition(s->x,s->y-8,g_height);
	waitressNode->setOrientation(Ogre::Quaternion::IDENTITY);
	waitressNode->pitch(Ogre::Degree(90));
	waitressNode->roll(Ogre::Degree(180),Ogre::Node::TS_WORLD);
	arrow1Pivot->setVisible(true);
	g_showArrowsState=1;
	arrow2Pivot->setVisible(false);
	std::string line="Scene2 Started at "+misc.ConvertToString(totalTime);
	g_output.writeLine(line);
}

void SceneMiGong::setScene3()
{
	g_currentScene=3;
	g_totalSeconds=g_timeLimit[2];
	//g_currentScore=2;
	Ogre::SceneNode* sn=g_mainMgr->getSceneNode("ballnode");
	prePos=currPos=0;
	setData3();
	finalPos=g_cornerLeaf->getDataMap()->getStructVector()->size()-1;
	GAFloat3Struct* s3=g_cornerLeaf->getDataMap()->getStructFloat3Data(0);
	sn->setPosition(s3->x,s3->y,g_height);
	nextPosR=g_leftRightLeaf->getDataMap()->getStructFloat4Data(currPos)->y;
	arrowRDir=2;
	arrowLDir=0;
	nextPosL=(UINT)-1;
	GAFloat3Struct* s=g_cornerLeaf->getDataMap()->getStructFloat3Data(finalPos);
	waitressNode->setPosition(s->x+8,s->y,g_height);
	waitressNode->setOrientation(Ogre::Quaternion::IDENTITY);
	waitressNode->pitch(Ogre::Degree(90));
	waitressNode->roll(Ogre::Degree(-90),Ogre::Node::TS_WORLD);
	arrow1Pivot->setVisible(true);
	g_showArrowsState=1;
	arrow2Pivot->setVisible(false);
	std::string line="Scene3 Started at "+misc.ConvertToString(totalTime);
	g_output.writeLine(line);
}

void SceneMiGong::setScene4()
{
	g_currentScene=4;
	g_totalSeconds=g_timeLimit[3];
	//g_currentScore=3;
	Ogre::SceneNode* sn=g_mainMgr->getSceneNode("ballnode");
	prePos=currPos=0;
	setData4();
	finalPos=g_cornerLeaf->getDataMap()->getStructVector()->size()-1;
	GAFloat3Struct* s3=g_cornerLeaf->getDataMap()->getStructFloat3Data(0);
	sn->setPosition(s3->x,s3->y,g_height);
	nextPosR=g_leftRightLeaf->getDataMap()->getStructFloat4Data(currPos)->y;
	arrowRDir=2;
	arrowLDir=0;
	nextPosL=(UINT)-1;
	GAFloat3Struct* s=g_cornerLeaf->getDataMap()->getStructFloat3Data(finalPos);
	waitressNode->setPosition(s->x+8,s->y,g_height);
	waitressNode->setOrientation(Ogre::Quaternion::IDENTITY);
	waitressNode->pitch(Ogre::Degree(90));
	waitressNode->roll(Ogre::Degree(-90),Ogre::Node::TS_WORLD);
	arrow1Pivot->setVisible(true);
	g_showArrowsState=1;
	arrow2Pivot->setVisible(false);
	std::string line="Scene4 Started at "+misc.ConvertToString(totalTime);
	g_output.writeLine(line);
}

void SceneMiGong::setData1()
{
	float transx=-960,transy=0;
	GADataManager* dataMgr=g_mainMgr->g_dataMgr;
	g_dataNode=dataMgr->getRootNode()->getChildContainerNode("SceneMiGong1");
	if(g_dataNode!=0)
	{
		g_cornerLeaf=g_dataNode->getChildLeafNode("corner");
		g_leftRightLeaf=g_dataNode->getChildLeafNode("leftRight");
		if(g_cornerLeaf!=0&&g_leftRightLeaf!=0)
		{
			return;
		}
	}
	g_dataNode=dataMgr->getRootNode()->createChildContainerNode("SceneMiGong1");
	g_cornerLeaf=g_dataNode->createChildLeafNode("corner");
	g_leftRightLeaf=g_dataNode->createChildLeafNode("leftRight");
	g_cornerLeaf->initDataMap(GADataMap::DataMapType_StructFloat3);
	g_leftRightLeaf->initDataMap(GADataMap::DataMapType_StructFloat4);
	GAFloat3Struct* s3;
	GAFloat4Struct* s4;
	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
	s3->x=-1024,s3->y=56,s3->z=7.5;
	s4->x=-1;s4->y=1;s4->z=-1;s4->w=-1;
	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
	s3->x=-904,s3->y=56,s3->z=7.5;
	s4->x=-1;s4->y=-1;s4->z=2;s4->w=0;
	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
	s3->x=-904,s3->y=-40,s3->z=7.5;
	s4->x=1;s4->y=-1;s4->z=-1;s4->w=3;
	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
	s3->x=-1000,s3->y=-40,s3->z=7.5;
	s4->x=4;s4->y=2;s4->z=-1;s4->w=-1;
	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
	s3->x=-1000,s3->y=24,s3->z=7.5;
	s4->x=-1;s4->y=-1;s4->z=3;s4->w=5;
	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
	s3->x=-1032,s3->y=24,s3->z=7.5;
	s4->x=-1;s4->y=4;s4->z=6;s4->w=-1;
	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
	s3->x=-1032,s3->y=-72,s3->z=7.5;
	s4->x=5;s4->y=7;s4->z=-1;s4->w=-1;
	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
	s3->x=-912,s3->y=-72,s3->z=7.5;
	s4->x=-1;s4->y=-1;s4->z=-1;s4->w=6;
	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
}

void SceneMiGong::setData2()
{
	float transx=0,transy=832;
	GADataManager* dataMgr=g_mainMgr->g_dataMgr;
	g_dataNode=dataMgr->getRootNode()->getChildContainerNode("SceneMiGong2");
	if(g_dataNode!=0)
	{
		g_cornerLeaf=g_dataNode->getChildLeafNode("corner");
		g_leftRightLeaf=g_dataNode->getChildLeafNode("leftRight");
		if(g_cornerLeaf!=0&&g_leftRightLeaf!=0)
		{
			return;
		}
	}
	g_dataNode=dataMgr->getRootNode()->createChildContainerNode("SceneMiGong2");
	g_cornerLeaf=g_dataNode->createChildLeafNode("corner");
	g_leftRightLeaf=g_dataNode->createChildLeafNode("leftRight");
	g_cornerLeaf->initDataMap(GADataMap::DataMapType_StructFloat3);
	g_leftRightLeaf->initDataMap(GADataMap::DataMapType_StructFloat4);
	GAFloat3Struct* s3;
	GAFloat4Struct* s4;
	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
	s3->x=-112,s3->y=952,s3->z=7.5;
	s4->x=-1;s4->y=1;s4->z=-1;s4->w=-1;
	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
	s3->x=-8,s3->y=952,s3->z=7.5;
	s4->x=-1;s4->y=-1;s4->z=2;s4->w=0;
	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
	s3->x=-8,s3->y=856,s3->z=7.5;
	s4->x=1;s4->y=10;s4->z=-1;s4->w=3;
	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
	s3->x=-88,s3->y=856,s3->z=7.5;
	s4->x=-1;s4->y=2;s4->z=4;s4->w=-1;
	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
	s3->x=-88,s3->y=792,s3->z=7.5;
	s4->x=3;s4->y=5;s4->z=-1;s4->w=-1;
	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
	s3->x=-56,s3->y=792,s3->z=7.5;
	s4->x=-1;s4->y=6;s4->z=11;s4->w=4;
	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
	s3->x=24,s3->y=792,s3->z=7.5;
	s4->x=-1;s4->y=7;s4->z=12;s4->w=5;
	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
	s3->x=88,s3->y=792,s3->z=7.5;
	s4->x=8;s4->y=-1;s4->z=-1;s4->w=6;
	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
	s3->x=88,s3->y=920,s3->z=7.5;
	s4->x=-1;s4->y=-1;s4->z=7;s4->w=9;
	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
	s3->x=40,s3->y=920,s3->z=7.5;
	s4->x=-1;s4->y=8;s4->z=10;s4->w=-1;
	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
	s3->x=40,s3->y=856,s3->z=7.5;
	s4->x=9;s4->y=-1;s4->z=-1;s4->w=2;
	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
	s3->x=-56,s3->y=744,s3->z=7.5;
	s4->x=5;s4->y=12;s4->z=-1;s4->w=-1;
	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
	s3->x=24,s3->y=744,s3->z=7.5;
	//s4->x=6;s4->y=-1;s4->z=13;s4->w=11;
	s4->x=-1;s4->y=-1;s4->z=13;s4->w=-1;
	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
	s3->x=24,s3->y=720,s3->z=7.5;
	s4->x=12;s4->y=-1;s4->z=-1;s4->w=-1;
	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
}

void SceneMiGong::setData3()
{
	float transx=1152,transy=0;
	GADataManager* dataMgr=g_mainMgr->g_dataMgr;
	g_dataNode=dataMgr->getRootNode()->getChildContainerNode("SceneMiGong3");
	if(g_dataNode!=0)
	{
		g_cornerLeaf=g_dataNode->getChildLeafNode("corner");
		g_leftRightLeaf=g_dataNode->getChildLeafNode("leftRight");
		if(g_cornerLeaf!=0&&g_leftRightLeaf!=0)
		{
			return;
		}
	}
	g_dataNode=dataMgr->getRootNode()->createChildContainerNode("SceneMiGong3");
	g_cornerLeaf=g_dataNode->createChildLeafNode("corner");
	g_leftRightLeaf=g_dataNode->createChildLeafNode("leftRight");
	g_cornerLeaf->initDataMap(GADataMap::DataMapType_StructFloat3);
	g_leftRightLeaf->initDataMap(GADataMap::DataMapType_StructFloat4);
	GAFloat3Struct* s3;
	GAFloat4Struct* s4;
	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
	s3->x=960,s3->y=136,s3->z=7.5;
	s4->x=-1;s4->y=1;s4->z=-1;s4->w=-1;
	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
	s3->x=1032,s3->y=136,s3->z=7.5;
	//s4->x=-1;s4->y=2;s4->z=5;s4->w=0;
	s4->x=-1;s4->y=-1;s4->z=5;s4->w=0; //封住不通的路
	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
	s3->x=1088,s3->y=136,s3->z=7.5;
	s4->x=-1;s4->y=-1;s4->z=-1;s4->w=1;
	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
	s3->x=928,s3->y=72,s3->z=7.5;
	s4->x=-1;s4->y=4;s4->z=-1;s4->w=-1;
	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
	s3->x=968,s3->y=72,s3->z=7.5;
	//s4->x=-1;s4->y=5;s4->z=8;s4->w=3;
	s4->x=-1;s4->y=5;s4->z=8;s4->w=-1;
	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
	s3->x=1032,s3->y=72,s3->z=7.5;
	s4->x=1;s4->y=6;s4->z=-1;s4->w=4;
	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
	s3->x=1080,s3->y=72,s3->z=7.5;
	s4->x=-1;s4->y=7;s4->z=10;s4->w=5;
	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
	s3->x=1144,s3->y=72,s3->z=7.5;
	s4->x=-1;s4->y=-1;s4->z=11;s4->w=6;
	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
	s3->x=968,s3->y=-8,s3->z=7.5;
	s4->x=4;s4->y=9;s4->z=-1;s4->w=-1;
	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
	s3->x=1016,s3->y=-8,s3->z=7.5;
	s4->x=-1;s4->y=10;s4->z=13;s4->w=8;
	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
	s3->x=1080,s3->y=-8,s3->z=7.5;
	s4->x=6;s4->y=11;s4->z=-1;s4->w=9;
	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
	s3->x=1144,s3->y=-8,s3->z=7.5;
	s4->x=7;s4->y=-1;s4->z=15;s4->w=10;
	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
	s3->x=968,s3->y=-72,s3->z=7.5;
	s4->x=-1;s4->y=13;s4->z=16;s4->w=-1;
	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
	s3->x=1016,s3->y=-72,s3->z=7.5;
	s4->x=9;s4->y=14;s4->z=-1;s4->w=12;
	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
	s3->x=1112,s3->y=-72,s3->z=7.5;
	s4->x=-1;s4->y=15;s4->z=20;s4->w=13;
	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
	s3->x=1144,s3->y=-72,s3->z=7.5;
	s4->x=11;s4->y=-1;s4->z=-1;s4->w=14;
	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
	s3->x=968,s3->y=-136,s3->z=7.5;
	s4->x=12;s4->y=17;s4->z=-1;s4->w=-1;
	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
	s3->x=1016,s3->y=-136,s3->z=7.5;
	s4->x=18;s4->y=20;s4->z=-1;s4->w=16;
	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
	s3->x=1016,s3->y=-104,s3->z=7.5;
	//s4->x=-1;s4->y=19;s4->z=17;s4->w=-1;
	s4->x=-1;s4->y=-1;s4->z=17;s4->w=-1;
	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
	s3->x=1056,s3->y=-104,s3->z=7.5;
	s4->x=-1;s4->y=-1;s4->z=-1;s4->w=18;
	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
	s3->x=1112,s3->y=-136,s3->z=7.5;
	//s4->x=14;s4->y=21;s4->z=-1;s4->w=17;
	s4->x=-1;s4->y=21;s4->z=-1;s4->w=-1;
	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
	s3->x=1136,s3->y=-136,s3->z=7.5;
	s4->x=-1;s4->y=-1;s4->z=-1;s4->w=20;
	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
}

void SceneMiGong::setData4()
{
	float transx=0,transy=-1024;
	GADataManager* dataMgr=g_mainMgr->g_dataMgr;
	g_dataNode=dataMgr->getRootNode()->getChildContainerNode("SceneMiGong4");
	if(g_dataNode!=0)
	{
		g_cornerLeaf=g_dataNode->getChildLeafNode("corner");
		g_leftRightLeaf=g_dataNode->getChildLeafNode("leftRight");
		if(g_cornerLeaf!=0&&g_leftRightLeaf!=0)
		{
			return;
		}
	}
	g_dataNode=dataMgr->getRootNode()->createChildContainerNode("SceneMiGong4");
	g_cornerLeaf=g_dataNode->createChildLeafNode("corner");
	g_leftRightLeaf=g_dataNode->createChildLeafNode("leftRight");
	g_cornerLeaf->initDataMap(GADataMap::DataMapType_StructFloat3);
	g_leftRightLeaf->initDataMap(GADataMap::DataMapType_StructFloat4);
	GAFloat3Struct* s3;
	GAFloat4Struct* s4;
	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
	s3->x=-120,s3->y=-904,s3->z=7.5;
	s4->x=-1;s4->y=1;s4->z=9;s4->w=-1;
	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
	s3->x=-56,s3->y=-904,s3->z=7.5;
	s4->x=-1;s4->y=-1;s4->z=8;s4->w=0;
	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
	s3->x=-8,s3->y=-904,s3->z=7.5;
	s4->x=-1;s4->y=3;s4->z=12;s4->w=-1;
	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
	s3->x=24,s3->y=-904,s3->z=7.5;
	s4->x=-1;s4->y=4;s4->z=19;s4->w=2;
	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
	s3->x=56,s3->y=-904,s3->z=7.5;
	s4->x=-1;s4->y=-1;s4->z=13;s4->w=3;
	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
	s3->x=88,s3->y=-904,s3->z=7.5;
	s4->x=-1;s4->y=6;s4->z=14;s4->w=-1;
	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
	s3->x=120,s3->y=-904,s3->z=7.5;
	s4->x=-1;s4->y=-1;s4->z=21;s4->w=5;
	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
	s3->x=-88,s3->y=-936,s3->z=7.5;
	s4->x=-1;s4->y=8;s4->z=10;s4->w=-1;
	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
	s3->x=-56,s3->y=-936,s3->z=7.5;
	s4->x=1;s4->y=-1;s4->z=11;s4->w=7;
	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
	s3->x=-120,s3->y=-952,s3->z=7.5;
	s4->x=0;s4->y=10;s4->z=-1;s4->w=-1;
	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
	s3->x=-88,s3->y=-952,s3->z=7.5;
	s4->x=7;s4->y=-1;s4->z=15;s4->w=9;
	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
	s3->x=-56,s3->y=-968,s3->z=7.5;
	s4->x=8;s4->y=12;s4->z=-1;s4->w=-1;
	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
	s3->x=-8,s3->y=-968,s3->z=7.5;
	s4->x=2;s4->y=-1;s4->z=-1;s4->w=11;
	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
	s3->x=56,s3->y=-936,s3->z=7.5;
	s4->x=4;s4->y=14;s4->z=20;s4->w=-1;
	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
	s3->x=88,s3->y=-936,s3->z=7.5;
	s4->x=5;s4->y=-1;s4->z=-1;s4->w=13;
	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
	s3->x=-88,s3->y=-1000,s3->z=7.5;
	s4->x=10;s4->y=16;s4->z=-1;s4->w=-1;
	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
	s3->x=-8,s3->y=-1000,s3->z=7.5;
	s4->x=-1;s4->y=-1;s4->z=25;s4->w=15;
	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
	s3->x=-104,s3->y=-1032,s3->z=7.5;
	s4->x=-1;s4->y=18;s4->z=26;s4->w=-1;
	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
	s3->x=-40,s3->y=-1032,s3->z=7.5;
	s4->x=-1;s4->y=-1;s4->z=23;s4->w=17;//18
	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
	s3->x=24,s3->y=-1032,s3->z=7.5;
	s4->x=3;s4->y=20;s4->z=28;s4->w=-1;//19
	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
	s3->x=56,s3->y=-1032,s3->z=7.5;
	s4->x=13;s4->y=21;s4->z=-1;s4->w=19;//20
	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
	s3->x=120,s3->y=-1032,s3->z=7.5;
	s4->x=6;s4->y=-1;s4->z=30;s4->w=20;//21
	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
	s3->x=-72,s3->y=-1064,s3->z=7.5;
	s4->x=-1;s4->y=23;s4->z=24;s4->w=-1;//22
	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
	s3->x=-40,s3->y=-1064,s3->z=7.5;
	s4->x=18;s4->y=-1;s4->z=-1;s4->w=22;
	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
	s3->x=-72,s3->y=-1096,s3->z=7.5;
	s4->x=22;s4->y=25;s4->z=27;s4->w=-1;
	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
	s3->x=-8,s3->y=-1096,s3->z=7.5;
	s4->x=16;s4->y=-1;s4->z=-1;s4->w=24;
	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
	s3->x=-104,s3->y=-1128,s3->z=7.5;
	s4->x=17;s4->y=27;s4->z=31;s4->w=-1;
	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
	s3->x=-72,s3->y=-1128,s3->z=7.5;
	s4->x=24;s4->y=28;s4->z=-1;s4->w=26;
	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
	s3->x=24,s3->y=-1128,s3->z=7.5;
	s4->x=19;s4->y=29;s4->z=-1;s4->w=27;
	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
	s3->x=72,s3->y=-1128,s3->z=7.5;
	s4->x=-1;s4->y=30;s4->z=32;s4->w=28;
	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
	s3->x=120,s3->y=-1128,s3->z=7.5;
	s4->x=21;s4->y=-1;s4->z=-1;s4->w=29;
	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
	s3->x=-104,s3->y=-1176,s3->z=7.5;
	s4->x=26;s4->y=32;s4->z=-1;s4->w=-1;
	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
	s3->x=72,s3->y=-1176,s3->z=7.5;
	//s4->x=29;s4->y=33;s4->z=-1;s4->w=31;
	s4->x=-1;s4->y=33;s4->z=-1;s4->w=-1;
	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
	s3->x=112,s3->y=-1176,s3->z=7.5;
	s4->x=-1;s4->y=-1;s4->z=-1;s4->w=32;
	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
}

void SceneMiGong::playStartSound(UINT leftright)
{
	if(!g_soundEnable) return;
	if(leftright==1) g_mainMgr->g_audioMgr->playShortSound("../sounds/thinkLeft.wav");
	else if(leftright==2) g_mainMgr->g_audioMgr->playShortSound("../sounds/thinkRight.wav");
	else g_mainMgr->g_audioMgr->playShortSound("../sounds/fight.wav");
}


bool SceneMiGong::processServerData(std::string recvString,GAServerSimple* server,UINT socketNum)
{
	bool processed=true;
	if(recvString=="left")
	{
		if(g_freezed||!g_started) return true;
		if(rotateLeft())
		{
			GAFloat3Struct s=getRunningTime(true);
			std::string str="rotate1:"+misc.ConvertToString(s.x)+"\trunning:"+misc.ConvertToString(s.y)+"\trotate2:"+misc.ConvertToString(s.z)+"\ttotal:"+misc.ConvertToString(s.x+s.y+s.z);
			server->sendString(str,socketNum);
		}
	}
	else if(recvString=="right")
	{
		if(g_freezed||!g_started) return true;
		if(rotateRight())
		{
			GAFloat3Struct s=getRunningTime(false);
			std::string str="rotate1:"+misc.ConvertToString(s.x)+"\trunning:"+misc.ConvertToString(s.y)+"\trotate2:"+misc.ConvertToString(s.z)+"\ttotal:"+misc.ConvertToString(s.x+s.y+s.z);
			server->sendString(str,socketNum);
		}
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
	else if(recvString=="esc")
	{
		g_mainMgr->g_UIMgr->switchToUI("EnterUI");
		g_mainMgr->g_cameraCtrl->lock();
	}
	else if(recvString=="getTime")
	{
		server->sendString(misc.ConvertToString(g_sceneRunningTime),socketNum);
	}
	else if(recvString=="getLeftRight")
	{
		UINT i=0;
		GAFloat3Struct* s=g_cornerLeaf->getDataMap()->getStructFloat3Data(nextPosL);
		if(s!=0) i++;
		s=g_cornerLeaf->getDataMap()->getStructFloat3Data(nextPosR);
		if(s!=0) i+=2;
		if(i==1) server->sendString("left",socketNum);
		else if(i==2) server->sendString("right",socketNum);
		else if(i==3) server->sendString("both",socketNum);
		else server->sendString("unknown",socketNum);
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
	else if(recvString=="autoPlaySound")
	{
		if(g_soundEnable)
		{
			UINT i=0;
			GAFloat3Struct* s=g_cornerLeaf->getDataMap()->getStructFloat3Data(nextPosL);
			if(s!=0) i++;
			s=g_cornerLeaf->getDataMap()->getStructFloat3Data(nextPosR);
			if(s!=0) i+=2;
			if(i==1) g_mainMgr->g_audioMgr->playShortSound("../sounds/tryLeft.wav");
			else if(i==2) g_mainMgr->g_audioMgr->playShortSound("../sounds/tryRight.wav");
			else g_mainMgr->g_audioMgr->playShortSound("../sounds/fight.wav");
		}
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
	else if(recvString=="getRunningTime")
	{
		float distance1=0;
		float distance2=0;
		//distance=g_
	}
	else if(recvString=="getTrialState")
	{
		/*if(g_freezed) server->sendString("Not Started",socketNum);
		else server->sendString("scene "+misc.ConvertToString(g_currentScene)+" started",socketNum);*/
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
		str+="time limit:"+misc.ConvertToString(g_timeLimit[g_currentScene-1])+";runned:"+misc.ConvertToString(g_sceneUI->g_currentTimeSeconds)+"\n";
		str+="time out:"+misc.ConvertToString(g_timeOut)+";runned:"+misc.ConvertToString(g_timeOutRoll->getCurrentTimeCount())+"\n";
		str+="rotate speed ratio:"+misc.ConvertToString(rotateSpeedRatio)+"\n";
		str+="run speed ratio:"+misc.ConvertToString(runSpeedRatio);
		server->sendString(str,socketNum);
	}
	else if(recvString=="getScore")
	{
		server->sendString(misc.ConvertToString(g_currentScore),socketNum);
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
	else if(recvString=="save")
	{
		saveCurrentState("../SceneMiGong/init.txt");
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
			if(str1=="rotateSpeedRatio")
			{
				rotateSpeedRatio=misc.ConvertToFloat(str2);
				server->sendString("new rotate speed ratio:"+misc.ConvertToString(rotateSpeedRatio),socketNum);
			}
			else if(str1=="runSpeedRatio")
			{
				runSpeedRatio=misc.ConvertToFloat(str2);
				server->sendString("new run speed ratio:"+misc.ConvertToString(runSpeedRatio),socketNum);
			}
			else if(str1=="setThisTrial")
			{
				if(g_freezed&&!g_started) start();
				g_freezed=false;
				g_sceneUI->welcome=false;
				g_sceneUI->hideHint();
				processServerData("switchScene:"+str2,server,socketNum);
			}
			else if(str1=="switchScene")
			{
				g_currentScene=misc.ConvertToInt(str2);
				Ogre::Vector3 fac=g_ballAI->getCharacter()->getFacingDirection();
				Ogre::Vector3 dd(0,1,0);
				float degree=dd.getRotationTo(fac).getRoll().valueDegrees();
				if(degree>180) degree-=360;
				else if(degree<-180) degree+=360;
				g_ballAI->rollAlongZAxis(-degree);
				if(g_currentScene==1) setScene1();
				else if(g_currentScene==2) setScene2();
				else if(g_currentScene==3) setScene3();
				else if(g_currentScene==4) setScene4();
				else
				{
					server->sendString("error input value",socketNum);
					return true;
				}
				if(g_currentScene==1) playStartSound(2);
				else if(g_currentScene==2) playStartSound(2);
				else if(g_currentScene==3) playStartSound(2);
				else if(g_currentScene==4) playStartSound(0);
				g_sceneUI->countRestart(g_timeLimit[g_currentScene-1]);
				g_sceneUI->countBegin();
				g_sceneUI->ProgressBar1Start(g_timeLimit[g_currentScene-1]);
				setRotating();
				g_sceneUI->g_currentScore=g_currentScore;
				server->sendString("done",socketNum);
			}
			else if(str1=="setTimeOut")
			{
				float time=misc.ConvertToFloat(str2);
				g_timeOut=time;
				g_timeOutRoll->setTimeInterval(g_timeOut);
				server->sendString("new time out:"+misc.ConvertToString(g_timeOut),socketNum);
			}
			else if(str1=="setTimeLimit")
			{
				if(g_freezed&&!g_started)
				{
					st=str2.find_first_of(",");
					str1=str2.substr(0,st);
					str2=str2.substr(st+1);
					UINT u1=misc.ConvertToInt(str1);
					UINT u2=misc.ConvertToInt(str2);
					u1--;
					if(u1<4&&u2<1000)
					{
						g_timeLimit[u1]=u2;
						server->sendString("new time limit:"+misc.ConvertToString(u2),socketNum);
					}
					else server->sendString("error input value",socketNum);
				}
				else server->sendString("error input value",socketNum);
			}
			else processed=false;
		}
		else processed=false;
	}
	return processed;
}