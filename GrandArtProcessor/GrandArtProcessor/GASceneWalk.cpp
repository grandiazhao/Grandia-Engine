#include "GASceneWalk.h"
#include "GAPCameraController.h"
#include "GAPWorldManager.h"
#include "GAOgreUIManager.h"

GASceneWalk::GASceneWalk(GAPMainManager* mainMgr,std::string sceneName) : GASceneBase(mainMgr,sceneName)
{
	keyUpDown=keyDownDown=keyLeftDown=keyRightDown=false;
	g_gamePos[0]=Ogre::Vector3(-463,-383,0);
	g_gamePos[1]=Ogre::Vector3(-320,-383,0);
	g_gamePos[2]=Ogre::Vector3(-173,-383,0);
	g_gamePos[3]=Ogre::Vector3(-340,77,0);
	g_gamePos[4]=Ogre::Vector3(-300,170,0);
	g_gamePos[5]=Ogre::Vector3(-300,653,0);
	g_walking=false;
	g_rotateDirection=Ogre::Vector3(1,0,0);
	g_sceneUI=(GAExampleSceneUI*)g_mainMgr->g_UIMgr->findUI("SceneUI");
}

bool GASceneWalk::createScene()
{
	Ogre::SceneManager* sm=Ogre::Root::getSingleton().createSceneManager("BspSceneManager","walk");
	Ogre::Camera* cam=sm->createCamera("walk");
	cam->setPosition(0,0,100);
    cam->lookAt(0,0,-300);
    cam->setNearClipDistance(15);
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
	g_mainMgr->g_UIMgr->switchToUI("SceneUI");

	g_mainMgr->g_worldMgr->addModel("yizi1","yizi2_fix.mesh",Ogre::Vector3(-618,-328,28),Ogre::Vector3(3.99094,3.99094,3.99094),Ogre::Quaternion(0.5,0.5,0.5,0.5));
	g_mainMgr->g_worldMgr->addModel("yizi2","yizi2_fix.mesh",Ogre::Vector3(-618,-458,28),Ogre::Vector3(3.99094,3.99094,3.99094),Ogre::Quaternion(0.5,0.5,0.5,0.5));
	g_mainMgr->g_worldMgr->addModel("zhuozi1","zhuozi_fix.mesh",Ogre::Vector3(-613.607,-390.19,48),Ogre::Vector3(1,1,1),Ogre::Quaternion(0.707107,0.707107,0,0));
	g_mainMgr->g_worldMgr->addModel("beizi1","coca1.mesh",Ogre::Vector3(-616.8,-382.0,100.7),Ogre::Vector3(1,1,1));
	g_mainMgr->g_worldMgr->addModel("beizi2","coca1.mesh",Ogre::Vector3(-618.3,-411.0,100.7),Ogre::Vector3(0.9,0.9,0.9));
	g_mainMgr->getCurrentSceneMgr()->getSceneNode("yizi2")->roll(Ogre::Degree(180),Ogre::Node::TS_WORLD);

	g_mainMgr->g_worldMgr->addModel("shafa1","shafa3_fix.mesh",Ogre::Vector3(0,-318.824,29.0582),Ogre::Vector3(3.14328,3.14328,3.14328),Ogre::Quaternion(0.5,0.5,0.5,0.5));
	g_mainMgr->g_worldMgr->addModel("chaji1","chaji1_fix.mesh",Ogre::Vector3(0,-455.585,29.3137),Ogre::Vector3(2.66635,2.66635,2.66635),Ogre::Quaternion(0.5,0.5,0.5,0.5));
	Ogre::SceneNode *pinghengNode=g_mainMgr->getCurrentSceneMgr()->getRootSceneNode()->createChildSceneNode("pingheng",Ogre::Vector3(0,-455,57));
	pinghengNode->pitch(Ogre::Degree(90));
	pinghengNode->roll(Ogre::Degree(90),Ogre::Node::TS_WORLD);
	Ogre::SceneNode* pinghengmuNode=pinghengNode->createChildSceneNode("pinghengmu",Ogre::Vector3(0,4.3,0));
	Ogre::Entity* ent1=g_mainMgr->getCurrentSceneMgr()->createEntity("pinghengmu1.mesh");
	Ogre::Entity* ent2=g_mainMgr->getCurrentSceneMgr()->createEntity("pinghengmu2.mesh");
	pinghengNode->attachObject(ent2);
	pinghengNode->setScale(1,2,2);
	pinghengmuNode->attachObject(ent1);
	pinghengmuNode->setScale(1,0.5,0.5);
	Ogre::SceneNode* pinghengqiuNodeBase=pinghengmuNode->createChildSceneNode("pinghengqiubase",Ogre::Vector3(0,3.5,0));
	Ogre::SceneNode* ballNode=pinghengqiuNodeBase->createChildSceneNode();
	Ogre::Entity* ent3=g_mainMgr->getCurrentSceneMgr()->createEntity("pinghengqiu.mesh");
	ballNode->attachObject(ent3);
	Ogre::SceneNode* huapen1=g_mainMgr->getCurrentSceneMgr()->getRootSceneNode()->createChildSceneNode("huapen1",Ogre::Vector3(-120,-700,15));
	Ogre::SceneNode* huapen2=g_mainMgr->getCurrentSceneMgr()->getRootSceneNode()->createChildSceneNode("huapen2",Ogre::Vector3(120,-700,15));
	huapen1->pitch(Ogre::Degree(90));
	huapen2->pitch(Ogre::Degree(90));
	huapen1->attachObject(g_mainMgr->getCurrentSceneMgr()->createEntity("huapen1.mesh"));
	huapen2->attachObject(g_mainMgr->getCurrentSceneMgr()->createEntity("huapen1.mesh"));
	Ogre::SceneNode* plant1=huapen1->createChildSceneNode("plant1");
	Ogre::SceneNode* plant2=huapen2->createChildSceneNode("plant2");
	plant1->attachObject(g_mainMgr->getCurrentSceneMgr()->createEntity("zhiwu2.mesh"));
	plant2->attachObject(g_mainMgr->getCurrentSceneMgr()->createEntity("zhiwu2.mesh"));

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

	g_mainMgr->g_worldMgr->addModel("yaling","yaling.mesh",Ogre::Vector3(160,200,20),Ogre::Vector3(1,1,1));
	g_mainMgr->g_worldMgr->addModel("yaling2","yaling.mesh",Ogre::Vector3(140,220,20),Ogre::Vector3(1,1,1));
	g_mainMgr->getSceneNode("yaling")->roll(Ogre::Degree(30),Ogre::Node::TS_WORLD);
	g_mainMgr->getSceneNode("yaling2")->roll(Ogre::Degree(100),Ogre::Node::TS_WORLD);

	g_personNode=g_mainMgr->getCurrentSceneMgr()->getRootSceneNode()->createChildSceneNode("personNode");
	g_personNode->pitch(Ogre::Degree(90));
	Ogre::Entity* personEnt=g_mainMgr->getCurrentSceneMgr()->createEntity("man_walk.mesh");
	Ogre::Entity* personEnt2=g_mainMgr->getCurrentSceneMgr()->createEntity("woman_walk.mesh");
	g_manNode=g_personNode->createChildSceneNode();
	g_manNode->attachObject(personEnt);
	g_womanNode=g_personNode->createChildSceneNode();
	g_womanNode->attachObject(personEnt2);
	GACharacterSimpleObject *simObj=new GACharacterSimpleObject(g_mainMgr->getCurrentSceneMgr(),g_personNode,g_personNode,personEnt);
	GACharacterSimpleObject *simObj2=new GACharacterSimpleObject(g_mainMgr->getCurrentSceneMgr(),g_personNode,g_personNode,personEnt2);
	simObj->setFacingDirection(5);
	simObj2->setFacingDirection(5);
	g_walkMan=new GACharacterAISimpleObject(simObj);
	g_walkMan->runAnimation("open");
	g_walkWoman=new GACharacterAISimpleObject(simObj2);
	g_walkWoman->runAnimation("open");
	g_currentPerson=g_walkMan;
	g_manNode->setVisible(true);
	g_womanNode->setVisible(false);
	Ogre::SceneNode* aroundNode=g_personNode->createChildSceneNode("personRotateNode");
	aroundNode->translate(0,150,0);
	newCamc->setCameraStyle(GAPCameraController::CameraStyle_AroundNode,aroundNode);
	g_mainMgr->g_cameraCtrl->g_cameraPivot->setOrientation(Ogre::Quaternion::IDENTITY);
	g_mainMgr->g_cameraCtrl->g_cameraPivot->pitch(Ogre::Degree(-30));
	g_mainMgr->g_cameraCtrl->g_cameraPivot->roll(Ogre::Degree(-180),Ogre::Node::TS_WORLD);
	g_mainMgr->g_cameraCtrl->g_cameraGoal->translate(0,0,-18);
	reCreateScene();
	return true;
}

bool GASceneWalk::reCreateScene()
{
	g_mainMgr->g_UIMgr->switchToUI("SceneUI");
	g_sceneUI->progressBar2Visible(false);
	g_sceneUI->hideHint();
	return true;
}

bool GASceneWalk::freezeScene()
{
	return GASceneBase::freezeScene();
}

bool GASceneWalk::reloadScene()
{
	return GASceneBase::reloadScene();
}

bool GASceneWalk::keyPressed(const OIS::KeyEvent& evt)
{
	if(evt.key==OIS::KC_UP)
	{
		keyUpDown=true;
	}
	else if(evt.key==OIS::KC_DOWN)
	{
		keyDownDown=true;
	}
	else if(evt.key==OIS::KC_LEFT)
	{
		keyLeftDown=true;
	}
	else if(evt.key==OIS::KC_RIGHT)
	{
		keyRightDown=true;
	}
	return true;
}

bool GASceneWalk::keyReleased(const OIS::KeyEvent& evt)
{
	if(evt.key==OIS::KC_ESCAPE)
	{
		Ogre::Root::getSingleton().queueEndRendering();
	}
	else if(evt.key==OIS::KC_UP)
	{
		keyUpDown=false;
	}
	else if(evt.key==OIS::KC_DOWN)
	{
		keyDownDown=false;
	}
	else if(evt.key==OIS::KC_LEFT)
	{
		keyLeftDown=false;
	}
	else if(evt.key==OIS::KC_RIGHT)
	{
		keyRightDown=false;
	}
	return true;
}

bool GASceneWalk::mouseMoved(const OIS::MouseEvent& evt)
{
	float t=evt.state.Z.rel*-0.05f;
	if(t!=0)
	{
		g_mainMgr->g_cameraCtrl->g_cameraGoal->translate(0,0,t);
	}
	return true;
}

bool GASceneWalk::frameStarted(float deltaTime)
{
	g_currentPerson->update(deltaTime);
	updateWalkTo(deltaTime);
	if(keyUpDown) g_currentPerson->getCharacter()->getCharacterSceneNode()->translate(g_currentPerson->getCharacter()->getFacingDirection()*(deltaTime*100),Ogre::Node::TS_WORLD);
	if(keyDownDown) g_currentPerson->getCharacter()->getCharacterSceneNode()->translate(g_currentPerson->getCharacter()->getFacingDirection()*(-deltaTime*100),Ogre::Node::TS_WORLD);
	if(keyLeftDown) g_currentPerson->getCharacter()->getCharacterSceneNode()->roll(Ogre::Degree(45*deltaTime),Ogre::Node::TS_WORLD);
	if(keyRightDown) g_currentPerson->getCharacter()->getCharacterSceneNode()->roll(Ogre::Degree(-45*deltaTime),Ogre::Node::TS_WORLD);
	return true;
}

void GASceneWalk::setPersonOnPoint(UINT point)
{
	switch(point)
	{
	case 1:
		g_currentPerson->getCharacter()->getCharacterSceneNode()->setPosition(g_gamePos[0]);
		break;
	case 2:
		g_currentPerson->getCharacter()->getCharacterSceneNode()->setPosition(g_gamePos[4]);
		break;
	case 3:
		g_currentPerson->getCharacter()->getCharacterSceneNode()->setPosition(g_gamePos[2]);
		break;
	case 4:
		g_currentPerson->getCharacter()->getCharacterSceneNode()->setPosition(g_gamePos[3]);
		break;
	case 5:
		g_currentPerson->getCharacter()->getCharacterSceneNode()->setPosition(g_gamePos[5]);
		break;
	}
}

void GASceneWalk::setPersonDirection(UINT dir)
{
	switch(dir)
	{
	case 1:
		g_currentPerson->getCharacter()->getCharacterSceneNode()->roll(Ogre::Degree(g_currentPerson->getCharacter()->getTurnDegreesXY(Ogre::Vector3(0,1,0))),Ogre::Node::TS_WORLD);
		break;
	case 2:
		g_currentPerson->getCharacter()->getCharacterSceneNode()->roll(Ogre::Degree(g_currentPerson->getCharacter()->getTurnDegreesXY(Ogre::Vector3(1,0,0))),Ogre::Node::TS_WORLD);
		break;
	case 3:
		g_currentPerson->getCharacter()->getCharacterSceneNode()->roll(Ogre::Degree(g_currentPerson->getCharacter()->getTurnDegreesXY(Ogre::Vector3(0,-1,0))),Ogre::Node::TS_WORLD);
		break;
	case 4:
		g_currentPerson->getCharacter()->getCharacterSceneNode()->roll(Ogre::Degree(g_currentPerson->getCharacter()->getTurnDegreesXY(Ogre::Vector3(-1,0,0))),Ogre::Node::TS_WORLD);
		break;
	default:
		break;
	}
}

void GASceneWalk::setWalkTo(UINT startGame,UINT endGame)
{
	if(g_walking) return;
	if(startGame==1)
	{
		setPersonOnPoint(1);
		g_startGame=1;
		if(endGame==2)
		{
			g_endGame=2;
			g_needRotate=true;
			g_needRotate2=true;
			g_walking=true;
			g_rotateDegrees=0;
			g_nextPos=1;
			g_finalPos=4;
			g_manNode->setVisible(true);
			g_womanNode->setVisible(false);
			g_currentPerson=g_walkMan;
			setPersonDirection(2);
			g_rotateDirection=Ogre::Vector3(1,0,0);
			g_rotatingTotalDegrees=g_currentPerson->getCharacter()->getTurnDegreesXY(g_gamePos[g_finalPos]-g_gamePos[g_nextPos]);
			g_currentPerson->moveToNewPlaceBySpeed(g_gamePos[g_nextPos].x,g_gamePos[g_nextPos].y,g_gamePos[g_nextPos].z,100);
		}
		else if(endGame==3)
		{
			g_endGame=3;
			g_needRotate=false;
			g_nextPos=1;
			g_needRotate2=false;
			g_rotateDirection=Ogre::Vector3(1,0,0);
			g_rotatingTotalDegrees=0;
			g_walking=true;
			g_rotateDegrees=0;
			g_finalPos=2;
			g_manNode->setVisible(true);
			g_womanNode->setVisible(false);
			g_currentPerson=g_walkMan;
			setPersonDirection(2);
			g_currentPerson->moveToNewPlaceBySpeed(g_gamePos[g_finalPos].x,g_gamePos[g_finalPos].y,g_gamePos[g_finalPos].z,100);
		}
		else if(endGame==4)
		{
			g_endGame=4;
			g_needRotate=true;
			g_needRotate2=true;
			g_walking=true;
			g_rotateDegrees=0;
			g_nextPos=1;
			g_finalPos=3;
			g_manNode->setVisible(false);
			g_womanNode->setVisible(true);
			g_currentPerson=g_walkWoman;
			setPersonDirection(2);
			g_rotateDirection=Ogre::Vector3(-1,0,0);
			g_rotatingTotalDegrees=g_currentPerson->getCharacter()->getTurnDegreesXY(g_gamePos[g_finalPos]-g_gamePos[g_nextPos]);
			g_currentPerson->moveToNewPlaceBySpeed(g_gamePos[g_nextPos].x,g_gamePos[g_nextPos].y,g_gamePos[g_nextPos].z,100);
		}
		else if(endGame==5)
		{
			g_endGame=5;
			g_needRotate=true;
			g_needRotate2=true;
			g_walking=true;
			g_rotateDegrees=0;
			g_nextPos=1;
			g_finalPos=5;
			g_manNode->setVisible(false);
			g_womanNode->setVisible(true);
			g_currentPerson=g_walkWoman;
			setPersonDirection(2);
			g_rotateDirection=Ogre::Vector3(1,0,0);
			g_rotatingTotalDegrees=g_currentPerson->getCharacter()->getTurnDegreesXY(g_gamePos[g_finalPos]-g_gamePos[g_nextPos]);
			g_currentPerson->moveToNewPlaceBySpeed(g_gamePos[g_nextPos].x,g_gamePos[g_nextPos].y,g_gamePos[g_nextPos].z,100);
		}
	}
	else if(startGame==2)
	{
		setPersonOnPoint(2);
		g_startGame=2;
		if(endGame==1)
		{
			g_endGame=1;
			g_needRotate=true;
			g_needRotate2=true;
			g_walking=true;
			g_rotateDegrees=0;
			g_nextPos=1;
			g_finalPos=0;
			g_manNode->setVisible(true);
			g_womanNode->setVisible(false);
			g_currentPerson=g_walkMan;
			setPersonDirection(3);
			g_rotateDirection=Ogre::Vector3(-1,0,0);
			g_rotatingTotalDegrees=g_currentPerson->getCharacter()->getTurnDegreesXY(g_gamePos[g_finalPos]-g_gamePos[g_nextPos]);
			g_currentPerson->moveToNewPlaceBySpeed(g_gamePos[g_nextPos].x,g_gamePos[g_nextPos].y,g_gamePos[g_nextPos].z,100);
		}
		else if(endGame==3)
		{
			g_endGame=3;
			g_needRotate=true;
			g_needRotate2=true;
			g_walking=true;
			g_rotateDegrees=0;
			g_nextPos=1;
			g_finalPos=2;
			g_manNode->setVisible(true);
			g_womanNode->setVisible(false);
			g_currentPerson=g_walkMan;
			setPersonDirection(3);
			g_rotateDirection=Ogre::Vector3(1,0,0);
			g_rotatingTotalDegrees=g_currentPerson->getCharacter()->getTurnDegreesXY(g_gamePos[g_finalPos]-g_gamePos[g_nextPos]);
			g_currentPerson->moveToNewPlaceBySpeed(g_gamePos[g_nextPos].x,g_gamePos[g_nextPos].y,g_gamePos[g_nextPos].z,100);
		}
		else if(endGame==4)
		{
			g_endGame=4;
			g_needRotate=false;
			g_needRotate2=true;
			g_walking=true;
			g_rotateDegrees=0;
			g_nextPos=0;
			g_finalPos=3;
			g_manNode->setVisible(false);
			g_womanNode->setVisible(true);
			g_currentPerson=g_walkWoman;
			setPersonDirection(3);
			g_rotateDirection=Ogre::Vector3(-1,0,0);
			g_rotatingTotalDegrees=g_currentPerson->getCharacter()->getTurnDegreesXY(g_rotateDirection);
			g_currentPerson->moveToNewPlaceBySpeed(g_gamePos[g_finalPos].x,g_gamePos[g_finalPos].y,g_gamePos[g_finalPos].z,100);
		}
		else if(endGame==5)
		{
			g_endGame=5;
			g_needRotate=false;
			g_needRotate2=true;
			g_walking=true;
			g_rotateDegrees=0;
			g_nextPos=1;
			g_finalPos=5;
			g_manNode->setVisible(false);
			g_womanNode->setVisible(true);
			g_currentPerson=g_walkWoman;
			setPersonDirection(1);
			g_rotateDirection=Ogre::Vector3(1,0,0);
			g_rotatingTotalDegrees=g_currentPerson->getCharacter()->getTurnDegreesXY(g_rotateDirection);
			g_currentPerson->moveToNewPlaceBySpeed(g_gamePos[g_finalPos].x,g_gamePos[g_finalPos].y,g_gamePos[g_finalPos].z,100);
		}
	}
	else if(startGame==3)
	{
		setPersonOnPoint(3);
		g_startGame=3;
		if(endGame==1)
		{
			g_endGame=1;
			g_needRotate=false;
			g_nextPos=1;
			g_needRotate2=false;
			g_rotateDirection=Ogre::Vector3(1,0,0);
			g_rotatingTotalDegrees=0;
			g_walking=true;
			g_rotateDegrees=0;
			g_finalPos=0;
			g_manNode->setVisible(true);
			g_womanNode->setVisible(false);
			g_currentPerson=g_walkMan;
			setPersonDirection(4);
			g_currentPerson->moveToNewPlaceBySpeed(g_gamePos[g_finalPos].x,g_gamePos[g_finalPos].y,g_gamePos[g_finalPos].z,100);
		}
		else if(endGame==2)
		{
			g_endGame=2;
			g_needRotate=true;
			g_needRotate2=true;
			g_walking=true;
			g_rotateDegrees=0;
			g_nextPos=1;
			g_finalPos=4;
			g_manNode->setVisible(true);
			g_womanNode->setVisible(false);
			g_currentPerson=g_walkMan;
			setPersonDirection(4);
			g_rotateDirection=Ogre::Vector3(1,0,0);
			g_rotatingTotalDegrees=g_currentPerson->getCharacter()->getTurnDegreesXY(g_gamePos[g_finalPos]-g_gamePos[g_nextPos]);
			g_currentPerson->moveToNewPlaceBySpeed(g_gamePos[g_nextPos].x,g_gamePos[g_nextPos].y,g_gamePos[g_nextPos].z,100);
		}
		else if(endGame==4)
		{
			g_endGame=4;
			g_needRotate=true;
			g_needRotate2=true;
			g_walking=true;
			g_rotateDegrees=0;
			g_nextPos=1;
			g_finalPos=3;
			g_manNode->setVisible(false);
			g_womanNode->setVisible(true);
			g_currentPerson=g_walkWoman;
			setPersonDirection(4);
			g_rotateDirection=Ogre::Vector3(-1,0,0);
			g_rotatingTotalDegrees=g_currentPerson->getCharacter()->getTurnDegreesXY(g_gamePos[g_finalPos]-g_gamePos[g_nextPos]);
			g_currentPerson->moveToNewPlaceBySpeed(g_gamePos[g_nextPos].x,g_gamePos[g_nextPos].y,g_gamePos[g_nextPos].z,100);
		}
		else if(endGame==5)
		{
			g_endGame=5;
			g_needRotate=true;
			g_needRotate2=true;
			g_walking=true;
			g_rotateDegrees=0;
			g_nextPos=1;
			g_finalPos=5;
			g_manNode->setVisible(false);
			g_womanNode->setVisible(true);
			g_currentPerson=g_walkWoman;
			setPersonDirection(4);
			g_rotateDirection=Ogre::Vector3(1,0,0);
			g_rotatingTotalDegrees=g_currentPerson->getCharacter()->getTurnDegreesXY(g_gamePos[g_finalPos]-g_gamePos[g_nextPos]);
			g_currentPerson->moveToNewPlaceBySpeed(g_gamePos[g_nextPos].x,g_gamePos[g_nextPos].y,g_gamePos[g_nextPos].z,100);
		}
	}
	else if(startGame==4)
	{
		setPersonOnPoint(4);
		g_startGame=4;
		if(endGame==1)
		{
			g_endGame=1;
			g_needRotate=true;
			g_needRotate2=true;
			g_walking=true;
			g_rotateDegrees=0;
			g_nextPos=1;
			g_finalPos=0;
			g_manNode->setVisible(true);
			g_womanNode->setVisible(false);
			g_currentPerson=g_walkMan;
			setPersonDirection(3);
			g_rotateDirection=Ogre::Vector3(-1,0,0);
			g_rotatingTotalDegrees=g_currentPerson->getCharacter()->getTurnDegreesXY(g_gamePos[g_finalPos]-g_gamePos[g_nextPos]);
			g_currentPerson->moveToNewPlaceBySpeed(g_gamePos[g_nextPos].x,g_gamePos[g_nextPos].y,g_gamePos[g_nextPos].z,100);
		}
		else if(endGame==2)
		{
			g_endGame=2;
			g_needRotate=false;
			g_needRotate2=true;
			g_walking=true;
			g_rotateDegrees=0;
			g_nextPos=0;
			g_finalPos=4;
			g_manNode->setVisible(true);
			g_womanNode->setVisible(false);
			g_currentPerson=g_walkMan;
			setPersonDirection(1);
			g_rotateDirection=Ogre::Vector3(1,0,0);
			g_rotatingTotalDegrees=g_currentPerson->getCharacter()->getTurnDegreesXY(g_rotateDirection);
			g_currentPerson->moveToNewPlaceBySpeed(g_gamePos[g_finalPos].x,g_gamePos[g_finalPos].y,g_gamePos[g_finalPos].z,100);
		}
		else if(endGame==3)
		{
			g_endGame=3;
			g_needRotate=true;
			g_needRotate2=true;
			g_walking=true;
			g_rotateDegrees=0;
			g_nextPos=1;
			g_finalPos=2;
			g_manNode->setVisible(true);
			g_womanNode->setVisible(false);
			g_currentPerson=g_walkMan;
			setPersonDirection(3);
			g_rotateDirection=Ogre::Vector3(1,0,0);
			g_rotatingTotalDegrees=g_currentPerson->getCharacter()->getTurnDegreesXY(g_gamePos[g_finalPos]-g_gamePos[g_nextPos]);
			g_currentPerson->moveToNewPlaceBySpeed(g_gamePos[g_nextPos].x,g_gamePos[g_nextPos].y,g_gamePos[g_nextPos].z,100);
		}
		else if(endGame==5)
		{
			g_endGame=5;
			g_needRotate=false;
			g_needRotate2=true;
			g_walking=true;
			g_rotateDegrees=0;
			g_nextPos=1;
			g_finalPos=5;
			g_manNode->setVisible(false);
			g_womanNode->setVisible(true);
			g_currentPerson=g_walkWoman;
			setPersonDirection(1);
			g_rotateDirection=Ogre::Vector3(1,0,0);
			g_rotatingTotalDegrees=g_currentPerson->getCharacter()->getTurnDegreesXY(g_rotateDirection);
			g_currentPerson->moveToNewPlaceBySpeed(g_gamePos[g_finalPos].x,g_gamePos[g_finalPos].y,g_gamePos[g_finalPos].z,100);
		}
	}
	else if(startGame==5)
	{
		setPersonOnPoint(5);
		g_startGame=5;
		if(endGame==1)
		{
			g_endGame=1;
			g_needRotate=true;
			g_needRotate2=true;
			g_walking=true;
			g_rotateDegrees=0;
			g_nextPos=1;
			g_finalPos=0;
			g_manNode->setVisible(true);
			g_womanNode->setVisible(false);
			g_currentPerson=g_walkMan;
			setPersonDirection(3);
			g_rotateDirection=Ogre::Vector3(-1,0,0);
			g_rotatingTotalDegrees=g_currentPerson->getCharacter()->getTurnDegreesXY(g_gamePos[g_finalPos]-g_gamePos[g_nextPos]);
			g_currentPerson->moveToNewPlaceBySpeed(g_gamePos[g_nextPos].x,g_gamePos[g_nextPos].y,g_gamePos[g_nextPos].z,100);
		}
		else if(endGame==2)
		{
			g_endGame=2;
			g_needRotate=false;
			g_needRotate2=true;
			g_walking=true;
			g_rotateDegrees=0;
			g_nextPos=0;
			g_finalPos=4;
			g_manNode->setVisible(true);
			g_womanNode->setVisible(false);
			g_currentPerson=g_walkMan;
			setPersonDirection(3);
			g_rotateDirection=Ogre::Vector3(1,0,0);
			g_rotatingTotalDegrees=g_currentPerson->getCharacter()->getTurnDegreesXY(g_rotateDirection);
			g_currentPerson->moveToNewPlaceBySpeed(g_gamePos[g_finalPos].x,g_gamePos[g_finalPos].y,g_gamePos[g_finalPos].z,100);
		}
		else if(endGame==3)
		{
			g_endGame=3;
			g_needRotate=true;
			g_needRotate2=true;
			g_walking=true;
			g_rotateDegrees=0;
			g_nextPos=1;
			g_finalPos=2;
			g_manNode->setVisible(true);
			g_womanNode->setVisible(false);
			g_currentPerson=g_walkMan;
			setPersonDirection(3);
			g_rotateDirection=Ogre::Vector3(1,0,0);
			g_rotatingTotalDegrees=g_currentPerson->getCharacter()->getTurnDegreesXY(g_gamePos[g_finalPos]-g_gamePos[g_nextPos]);
			g_currentPerson->moveToNewPlaceBySpeed(g_gamePos[g_nextPos].x,g_gamePos[g_nextPos].y,g_gamePos[g_nextPos].z,100);
		}
		else if(endGame==4)
		{
			g_endGame=4;
			g_needRotate=false;
			g_needRotate2=true;
			g_walking=true;
			g_rotateDegrees=0;
			g_nextPos=1;
			g_finalPos=3;
			g_manNode->setVisible(false);
			g_womanNode->setVisible(true);
			g_currentPerson=g_walkMan;
			setPersonDirection(3);
			g_rotateDirection=Ogre::Vector3(-1,0,0);
			g_rotatingTotalDegrees=g_currentPerson->getCharacter()->getTurnDegreesXY(g_rotateDirection);
			g_currentPerson->moveToNewPlaceBySpeed(g_gamePos[g_finalPos].x,g_gamePos[g_finalPos].y,g_gamePos[g_finalPos].z,100);
		}
	}
}

void GASceneWalk::updateWalkTo(float deltaTime)
{
	if(g_walking==false) return;
	if(g_currentPerson->g_move==false)
	{
		if(g_needRotate)
		{
			float delta=0;
			if(g_rotatingTotalDegrees>0) delta=45*deltaTime;
			else if(g_rotatingTotalDegrees<0) delta=-45*deltaTime;
			g_rotateDegrees+=delta;
			if(abs(g_rotateDegrees)>=abs(g_rotatingTotalDegrees))
			{
				g_needRotate=false;
				delta=delta-g_rotateDegrees+g_rotatingTotalDegrees;
				g_rotateDegrees=0;
				g_currentPerson->getCharacter()->getCharacterSceneNode()->roll(Ogre::Degree(delta),Ogre::Node::TS_WORLD);
				g_rotatingTotalDegrees=g_currentPerson->getCharacter()->getTurnDegreesXY(g_rotateDirection);
				g_currentPerson->moveToNewPlaceBySpeed(g_gamePos[g_finalPos].x,g_gamePos[g_finalPos].y,g_gamePos[g_finalPos].z,100);
			}
			else g_currentPerson->getCharacter()->getCharacterSceneNode()->roll(Ogre::Degree(delta),Ogre::Node::TS_WORLD);
		}
		else if(g_needRotate2)
		{
			float delta=0;
			if(g_rotatingTotalDegrees>0) delta=45*deltaTime;
			else if(g_rotatingTotalDegrees<0) delta=-45*deltaTime;
			g_rotateDegrees+=delta;
			if(abs(g_rotateDegrees)>=abs(g_rotatingTotalDegrees))
			{
				g_needRotate2=false;
				delta=delta-g_rotateDegrees+g_rotatingTotalDegrees;
				g_rotateDegrees=0;
				g_rotatingTotalDegrees=0;
				g_currentPerson->getCharacter()->getCharacterSceneNode()->roll(Ogre::Degree(delta),Ogre::Node::TS_WORLD);
			}
			else g_currentPerson->getCharacter()->getCharacterSceneNode()->roll(Ogre::Degree(delta),Ogre::Node::TS_WORLD);
		}
		else
		{
			g_walking=false;
			end();
		}
	}
}

void GASceneWalk::end()
{
	g_sceneUI->g_enterScene=true;
	if(g_endGame==1) g_sceneUI->g_sceneName="chabei";
	else if(g_endGame==2) g_sceneUI->g_sceneName="yundong";
	else if(g_endGame==3) g_sceneUI->g_sceneName="pingheng";
	else if(g_endGame==4) g_sceneUI->g_sceneName="wupin";
	else if(g_endGame==5) g_sceneUI->g_sceneName="migong";
}


bool GASceneWalk::processServerData(std::string recvString,GAServerSimple* server,UINT socketNum)
{
	std::string::size_type st;
	std::string str1,str2,temp;
	st=recvString.find_first_of(":");
	if(st!=std::string::npos)
	{
		str1=recvString.substr(0,st);
		str2=recvString.substr(st+1);
		if(str1=="setWalkTo")
		{
			st=str2.find_first_of(",");
			if(st!=std::string::npos)
			{
				UINT t1=misc.ConvertToInt(str2.substr(0,st));
				UINT t2=misc.ConvertToInt(str2.substr(st+1));
				setWalkTo(t1,t2);
			}
		}
	}
	return true;
}