//#include "SceneMiGong.h"
//#include "GAPCameraController.h"
//#include "GADataManager.h"
//#include "GAPWorldManager.h"
//
//SceneMiGong::SceneMiGong(GAPMainManager* mainMgr,std::string sceneName) : GASceneBase(mainMgr,sceneName)
//{
//	totalTime=cellTime=0;
//	material1=true;
//	prePos=currPos=nextPosL=0;
//	g_moving=false;
//	g_showArrow=false;
//	g_rotateCamera=false;
//	currDegrees=0;
//	g_processing=false;
//	g_timeLimit[0]=g_timeLimit[1]=g_timeLimit[2]=g_timeLimit[3]=999;
//}
//
//bool SceneMiGong::createScene()
//{
//	Ogre::SceneManager* sm=Ogre::Root::getSingleton().createSceneManager("BspSceneManager","migong");
//	Ogre::Camera* cam=sm->createCamera("migongCam");
//	cam->setPosition(0,0,100);
//    cam->lookAt(0,0,-300);
//    cam->setNearClipDistance(5);
//	g_mainMgr->setNewSceneManager(sm,cam);
//	GAPCameraController *newCamc=new GAPCameraController(g_mainMgr);
//	newCamc->init();
//	g_mainMgr->g_cameraCtrl=newCamc;
//	g_mainMgr->mWindow->removeAllViewports();
//	Ogre::Viewport* vp=g_mainMgr->mWindow->addViewport(cam);
//	cam->setAspectRatio((float)vp->getActualWidth()/(float)vp->getActualHeight());
//	cam->setFOVy(Ogre::Degree(30));
//	sm->setSkyBox(true,"Examples/MorningSkyBox",2500);
//	sm->getSkyBoxNode()->pitch(Ogre::Degree(90));
//	
//	Ogre::SceneNode* ballnode=sm->getRootSceneNode()->createChildSceneNode("ballnode");
//	ballnode->pitch(Ogre::Degree(90));
//	Ogre::SceneNode* mannode=ballnode->createChildSceneNode("mannode");
//	Ogre::Entity *ballEnt=sm->createEntity("woman12.mesh");
//	ballEnt->setMaterialName("body");
//	//ballEnt->setMaterialName("Examples/SphereMappedRustySteel");
//	mannode->attachObject(ballEnt);
//	mannode->setScale(0.1,0.1,0.1);
//	Ogre::SceneNode* ballRoundNode=ballnode->createChildSceneNode("ballRoundNode",Ogre::Vector3(0,10,0));
//	ballnode->setPosition(-280,240,0);
//	GACharacterSimpleObject* simobj=new GACharacterSimpleObject(sm,ballnode,mannode,ballEnt);
//	simobj->setFacingDirection(5);
//	g_ballAI=new GACharacterAISimpleObject(simobj);
//	g_ballAI->runAnimation("idle");
//	g_height=0;
//	arrow1Pivot=ballRoundNode->createChildSceneNode("arrow1Pivot");
//	Ogre::SceneNode* arrownodeL=arrow1Pivot->createChildSceneNode("arrownodeL");
//	Ogre::Entity *arrowEntL=sm->createEntity("arrowL","arrow.mesh");
//	arrow2Pivot=ballRoundNode->createChildSceneNode("arrow2Pivot");
//	Ogre::SceneNode* arrownodeR=arrow2Pivot->createChildSceneNode("arrownodeR");
//	Ogre::Entity *arrowEntR=sm->createEntity("arrowR","arrow.mesh");
//	arrowEntL->setMaterialName("Examples/transparent/pink");
//	arrowEntR->setMaterialName("Examples/transparent/blue");
//	arrownodeL->attachObject(arrowEntL);
//	arrownodeR->attachObject(arrowEntR);
//	arrownodeR->roll(Ogre::Degree(180),Ogre::Node::TS_WORLD);
//	Ogre::Light *l=sm->createLight();
//	l->setPosition(0,0,100);
//	arrownodeL->setScale(0.6,0.6,0.6);
//	arrownodeL->setPosition(20,0,0);
//	arrownodeR->setScale(0.6,0.6,0.6);
//	arrownodeR->setPosition(-20,0,0);
//	g_mainMgr->g_cameraCtrl->setCameraStyle(GAPCameraController::CameraStyle_AroundNode,ballRoundNode);
//	g_mainMgr->g_cameraCtrl->g_cameraPivot->setOrientation(Ogre::Quaternion::IDENTITY);
//	g_mainMgr->g_cameraCtrl->g_cameraPivot->pitch(Ogre::Degree(-50));
//	g_mainMgr->g_cameraCtrl->g_cameraGoal->translate(0,0,300);
//	g_mainMgr->g_cameraCtrl->lock();
//	g_mainMgr->g_worldMgr->addModel("endCube","cube.mesh",Ogre::Vector3(0,0,0),Ogre::Vector3(0.12,0.16,0.12));
//	g_endNode=g_mainMgr->getSceneNode("endCube");
//	g_mainMgr->getCurrentSceneMgr()->getEntity("endCube")->setMaterialName("Examples/transparent/pink");
//	g_worldName="migong1234_2.bsp";
//	g_mainMgr->getCurrentSceneMgr()->setWorldGeometry(g_worldName);
//	g_output.createNewFile("../SceneMiGong/MiGongOutput.txt");
//	g_mainMgr->g_UIMgr->switchToUI("SceneUI");
//	g_sceneUI=(GAExampleSceneUI*)g_mainMgr->g_UIMgr->findUI("SceneUI");
//	waitressNode=g_mainMgr->getCurrentSceneMgr()->getRootSceneNode()->createChildSceneNode();
//	Ogre::Entity* entWaitress=g_mainMgr->getCurrentSceneMgr()->createEntity("waitress.mesh");
//	waitressNode->pitch(Ogre::Degree(90));
//	waitressNode->roll(Ogre::Degree(-90),Ogre::Node::TS_WORLD);
//	waitressNode->attachObject(entWaitress);
//	waitressNode->scale(0.1,0.1,0.1);
//	GACharacterSimpleObject* simobj2=new GACharacterSimpleObject(sm,waitressNode,waitressNode,entWaitress);
//	simobj2->setFacingDirection(5);
//	g_endManAI=new GACharacterAISimpleObject(simobj2);
//	g_endManAI->runAnimation("open");
//	g_endNode->setVisible(false);
//	//g_mainMgr->g_UIMgr->getCurrentUI()->g_trayMgr->showYesNoDialog("123","question");
//	reCreateScene();
//	return true;
//}
//
//bool SceneMiGong::reCreateScene()
//{
//	loadInitFile("../SceneMiGong/initLR.txt");
//	g_currentScore=0;
//	setScene1();
//	g_freezed=true;
//	g_sceneUI->hintString=L"请按F1键开始游戏";
//	g_sceneUI->showHint();
//	g_sceneUI->countRestart(g_timeLimit[g_currentScore]);
//	g_sceneUI->countPause();
//	g_sceneUI->progressBar2Visible(false);
//	return true;
//}
//
//void SceneMiGong::start()
//{
//	//g_mainMgr->g_cameraCtrl->setCameraStyle(GAPCameraController::CameraStyle_AroundNode,g_mainMgr->getSceneNode("ballRoundNode"));
//	g_mainMgr->g_cameraCtrl->lock();
//	if(g_currentScore==0) setScene1();
//	else if(g_currentScore==1) setScene2();
//	else if(g_currentScore==2) setScene3();
//	else setScene4();
//	g_freezed=false;
//	g_sceneUI->hideHint();
//	g_sceneUI->countRestart(g_timeLimit[g_currentScore]);
//	g_sceneUI->countBegin();
//	g_sceneUI->g_totalScore=4;
//	g_sceneUI->g_currentScore=g_currentScore;
//	g_sceneUI->ProgressBar1Start(g_timeLimit[g_currentScore]);
//	g_sceneUI->progressBar2Visible(false);
//	setRotating();
//}
//
//bool SceneMiGong::mouseMoved(const OIS::MouseEvent &evt)
//{
//	float t=evt.state.Z.rel*-0.05f;
//	g_mainMgr->g_cameraCtrl->g_cameraGoal->translate(0,0,t);
//	return true;
//}
//
//bool SceneMiGong::keyReleased(const OIS::KeyEvent& evt)
//{
//	if(evt.key==OIS::KC_ESCAPE)
//	{
//		g_mainMgr->g_UIMgr->switchToUI("EnterUI");
//		g_mainMgr->g_cameraCtrl->lock();
//	}
//	else if(evt.key==OIS::KC_F1)
//	{
//		start();
//	}
//	if(g_freezed) return true;
//	else if(evt.key==OIS::KC_F2)
//	{
//		g_mainMgr->g_cameraCtrl->setCameraStyle(GAPCameraController::CameraStyle_Free);
//		g_mainMgr->g_cameraCtrl->unlock();
//	}
//	else if(evt.key==OIS::KC_RIGHT)
//	{
//		goRoadRight();
//	}
//	else if(evt.key==OIS::KC_LEFT)
//	{
//		goRoadLeft();
//	}
//	return true;
//}
//
//void SceneMiGong::goRoadLeft()
//{
//	GAFloat3Struct* s=g_cornerLeaf->getDataMap()->getStructFloat3Data(nextPosL);
//	std::string line=misc.ConvertToString(totalTime)+":left";
//	if(s&&!g_processing)
//	{
//		prePos=currPos;
//		currPos=nextPosL;
//		g_ballAI->moveToNewPlaceBySpeed(s->x,s->y,g_height,50);
//		arrow1Pivot->setVisible(false);
//		arrow2Pivot->setVisible(false);
//		g_processing=true;
//		g_ballAI->runAnimation("run");
//	}
//	else if(s) line="#"+line;
//	else line="@"+line;
//	g_output.writeLine(line);
//}
//
//void SceneMiGong::goRoadRight()
//{
//	GAFloat3Struct* s=g_cornerLeaf->getDataMap()->getStructFloat3Data(nextPosR);
//	std::string line=misc.ConvertToString(totalTime)+":right";
//	if(s&&!g_processing)
//	{
//		prePos=currPos;
//		currPos=nextPosR;
//		g_ballAI->moveToNewPlaceBySpeed(s->x,s->y,g_height,50);
//		arrow1Pivot->setVisible(false);
//		arrow2Pivot->setVisible(false);
//		g_processing=true;
//		g_ballAI->runAnimation("run");
//	}
//	else if(s) line="#"+line;
//	else line="@"+line;
//	g_output.writeLine(line);
//}
//
//void SceneMiGong::rotateCamera(float degrees,float speed)
//{
//	if(degrees<0&&speed>0) speed=-speed;
//	if(degrees>0&&speed<0) speed=-speed;
//	if(!g_rotateCamera) g_mainMgr->g_cameraCtrl->setAutoYaw(degrees,speed);
//	g_rotateCamera=true;                                                                        
//}
//
//bool SceneMiGong::frameStarted(float deltaTime)
//{
//	if(g_freezed) return true;
//	//cellTime+=deltaTime;
//	g_ballAI->update(deltaTime);
//	g_mainMgr->g_cameraCtrl->unlock();
//	totalTime+=deltaTime;
//	if(!g_processing) if(g_sceneUI->isCountOver()) oneSceneEnd(false);
//	if(g_rotateCamera)
//	{
//		g_mainMgr->g_cameraCtrl->unlock();
//		g_rotateCamera=!g_mainMgr->g_cameraCtrl->autoYaw(deltaTime);
//		g_mainMgr->g_cameraCtrl->lock();
//		if(!g_rotateCamera)
//		{
//			//setArrows();
//			g_processing=false;
//		}
//	}
//	setNextLR("../SceneMiGong/setNextLR.txt");
//	fileControllSign=getLRCommandFromFile("../SceneMiGong/inputCommand.txt");
//	if(fileControllSign==1)
//	{
//		goRoadLeft();
//	}
//	else if(fileControllSign==2)
//	{
//		goRoadRight();
//	}
//	if(g_mainMgr->g_keyboardInput->isKeyDown(GAP_KEY_T))
//	{
//		arrow1Pivot->roll(Ogre::Degree(45*deltaTime),Ogre::Node::TS_WORLD);
//	}
//	if(g_mainMgr->g_keyboardInput->isKeyDown(GAP_KEY_Y))
//	{
//		arrow1Pivot->roll(Ogre::Degree(-45*deltaTime),Ogre::Node::TS_WORLD);
//	}
//	if(g_mainMgr->g_keyboardInput->isKeyDown(GAP_KEY_O))
//	{
//		arrow1Pivot->setOrientation(Ogre::Quaternion::IDENTITY);
//	}
//	if(g_mainMgr->g_keyboardInput->isKeyDown(GAP_KEY_G))
//	{
//		arrow2Pivot->roll(Ogre::Degree(45*deltaTime),Ogre::Node::TS_WORLD);
//	}
//	if(g_mainMgr->g_keyboardInput->isKeyDown(GAP_KEY_H))
//	{
//		arrow2Pivot->roll(Ogre::Degree(-45*deltaTime),Ogre::Node::TS_WORLD);
//	}
//	if(g_processing)
//	{
//		doMoving(deltaTime);
//	}
//	g_endManAI->update(deltaTime);
//	return true;
//}
//
//void SceneMiGong::doMoving(float deltaTime)
//{
//	//g_ballAI->update(deltaTime);
//	g_moving=g_ballAI->g_move;
//	if(g_moving==false) setRotating();
//}
//
//void SceneMiGong::setRotating()
//{
//	g_ballAI->runAnimation("idle");
//	float degree=0;
//	nextPosL=(UINT)-1;
//	nextPosR=(UINT)-1;
//	bool beLeft=g_LRs[g_currentScene-1][currPos];
//	GAFloat4Struct* s4=g_leftRightLeaf->getDataMap()->getStructFloat4Data(currPos);
//	if(s4)
//	{
//		UINT count=0;
//		if(s4->x>=0&&(UINT(s4->x))!=prePos) count++;
//		if(s4->y>=0&&(UINT(s4->y))!=prePos) count++;
//		if(s4->z>=0&&(UINT(s4->z))!=prePos) count++;
//		if(s4->w>=0&&(UINT(s4->w))!=prePos) count++;
//		if(count==0)
//		{
//			if(currPos==finalPos)
//			{
//				oneSceneEnd(true);
//				return;
//			}
//
//			UINT pos1,dir1;
//			if(s4->x>=0)
//			{
//				degree+=180;
//				pos1=s4->x;
//				dir1=1;
//			}
//			else if(s4->y>=0)
//			{
//				degree+=90;
//				pos1=s4->y;
//				dir1=2;
//			}
//			else if(s4->z>=0)
//			{
//				degree+=0;
//				pos1=s4->z;
//				dir1=3;
//			}
//			else 
//			{
//				degree-=90;
//				pos1=s4->w;
//				dir1=4;
//			}
//			if(beLeft)
//			{
//				degree+=90;
//				nextPosL=pos1;
//				arrowLDir=dir1;
//				nextPosR=(UINT)-1;
//				arrowRDir=0;
//			}
//			else
//			{
//				degree-=90;
//				nextPosR=pos1;
//				arrowRDir=dir1;
//				nextPosL=(UINT)-1;
//				arrowLDir=0;
//			}
//		}
//		else if(count==1)
//		{
//			UINT pos1,dir1;
//			if(s4->x>=0&&(UINT(s4->x))!=prePos)
//			{
//				degree+=180;
//				pos1=s4->x;
//				dir1=1;
//			}
//			else if(s4->y>=0&&(UINT(s4->y))!=prePos)
//			{
//				degree+=90;
//				pos1=s4->y;
//				dir1=2;
//			}
//			else if(s4->z>=0&&(UINT(s4->z))!=prePos)
//			{
//				degree+=0;
//				pos1=s4->z;
//				dir1=3;
//			}
//			else 
//			{
//				degree-=90;
//				pos1=s4->w;
//				dir1=4;
//			}
//			if(beLeft)
//			{
//				degree+=90;
//				nextPosL=pos1;
//				arrowLDir=dir1;
//				nextPosR=(UINT)-1;
//				arrowRDir=0;
//			}
//			else
//			{
//				degree-=90;
//				nextPosR=pos1;
//				arrowRDir=dir1;
//				nextPosL=(UINT)-1;
//				arrowLDir=0;
//			}
//		}
//		else if(count==2)
//		{
//			UINT pos1,dir1,pos2,dir2;
//			pos1=0;pos2=0;
//			if(s4->x>=0&&(UINT(s4->x))!=prePos)
//			{
//				degree+=180;pos1=s4->x;dir1=1;
//			}
//			if(s4->y>=0&&(UINT(s4->y))!=prePos)
//			{
//				degree+=90;
//				if(pos1==0)
//				{
//					pos1=s4->y;dir1=2;
//				}
//				else
//				{
//					pos2=s4->y;dir2=2;
//				}
//			}
//			if(s4->z>=0&&(UINT(s4->z))!=prePos)
//			{
//				degree+=0;
//				if(pos1==0)
//				{
//					pos1=s4->z;dir1=3;
//				}
//				else
//				{
//					pos2=s4->z;dir2=3;
//				}
//			}
//			if(s4->w>=0&&(UINT(s4->w))!=prePos)
//			{
//				degree+=-90;
//				pos2=s4->w;dir2=4;
//			}
//			degree=degree/2;
//			//if(dir1==1&&dir2==2)
//			//{
//			//	//这里的beLeft实际是beInner
//			//	if(beLeft)
//			//	{
//			//		arrowLDir=dir2;
//			//		arrowRDir=dir1;
//			//		nextPosL=pos2;
//			//		nextPosR=pos1;
//			//	}
//			//	else
//			//	{
//			//		degree+=180;
//			//		arrowLDir=dir1;
//			//		arrowRDir=dir2;
//			//		nextPosL=pos1;
//			//		nextPosR=pos2;
//			//	}
//			//}
//			//else if(dir1==1&&dir2==3)
//			//{
//			//	//这里的beLeft实际是在1,3两个箭头轴的左侧
//			//	if(beLeft)
//			//	{
//			//		degree+=180;
//			//		arrowLDir=dir1;
//			//		arrowRDir=dir2;
//			//		nextPosL=pos1;
//			//		nextPosR=pos2;
//			//	}
//			//	else
//			//	{
//			//		arrowLDir=dir2;
//			//		arrowRDir=dir1;
//			//		nextPosL=pos2;
//			//		nextPosR=pos1;
//			//	}
//			//}
//			//else if(dir1==1&&dir2==4)
//			//{
//			//	//这里的beLeft实际是beInner
//			//	if(beLeft)
//			//	{
//			//		degree+=180;
//			//		arrowLDir=dir1;
//			//		arrowRDir=dir2;
//			//		nextPosL=pos1;
//			//		nextPosR=pos2;
//			//	}
//			//	else
//			//	{
//			//		arrowLDir=dir2;
//			//		arrowRDir=dir1;
//			//		nextPosL=pos2;
//			//		nextPosR=pos1;
//			//	}
//			//}
//			//else if(dir1==2&&dir2==3)
//			//{
//			//	//这里的beLeft实际是beInner
//			//	if(beLeft)
//			//	{
//			//		arrowLDir=dir2;
//			//		arrowRDir=dir1;
//			//		nextPosL=pos2;
//			//		nextPosR=pos1;
//			//	}
//			//	else
//			//	{
//			//		degree+=180;
//			//		arrowLDir=dir1;
//			//		arrowRDir=dir2;
//			//		nextPosL=pos1;
//			//		nextPosR=pos2;
//			//	}
//			//}
//			//else if(dir1==2&&dir2==4)
//			//{
//			//	//这里的beLeft实际是在1,3两个箭头轴的上侧
//			//	if(beLeft)
//			//	{
//			//		degree+=180;
//			//		arrowLDir=dir1;
//			//		arrowRDir=dir2;
//			//		nextPosL=pos1;
//			//		nextPosR=pos2;
//			//	}
//			//	else
//			//	{
//			//		arrowLDir=dir2;
//			//		arrowRDir=dir1;
//			//		nextPosL=pos2;
//			//		nextPosR=pos1;
//			//	}
//			//}
//			//else
//			//{
//			//	//这里的beLeft实际是beInner
//			//	if(beLeft)
//			//	{
//			//		arrowLDir=dir2;
//			//		arrowRDir=dir1;
//			//		nextPosL=pos2;
//			//		nextPosR=pos1;
//			//	}
//			//	else
//			//	{
//			//		degree+=180;
//			//		arrowLDir=dir1;
//			//		arrowRDir=dir2;
//			//		nextPosL=pos1;
//			//		nextPosR=pos2;
//			//	}
//			//}
//			if(beLeft)
//			{
//				arrowLDir=dir2;
//				arrowRDir=dir1;
//				nextPosL=pos2;
//				nextPosR=pos1;
//			}
//			else
//			{
//				degree+=180;
//				arrowLDir=dir1;
//				arrowRDir=dir2;
//				nextPosL=pos1;
//				nextPosR=pos2;
//			}
//		}
//
//		float yawDegree=degree-currDegrees;
//		currDegrees=degree;
//		while(yawDegree<-180) yawDegree+=360;
//		while(yawDegree>180) yawDegree-=360;
//		setArrows();
//		rotateCamera(yawDegree,90);
//	}
//	else return;
//}
//
//void SceneMiGong::setArrows()
//{
//	arrow1Pivot->setOrientation(Ogre::Quaternion::IDENTITY);
//	arrow2Pivot->setOrientation(Ogre::Quaternion::IDENTITY);
//	if(arrowLDir>0)
//	{
//		arrow1Pivot->setVisible(true);
//		arrow1Pivot->roll(Ogre::Degree(180-arrowLDir*90),Ogre::Node::TS_WORLD);
//	}
//	if(arrowRDir>0)
//	{
//		arrow2Pivot->setVisible(true);
//		arrow2Pivot->roll(Ogre::Degree(-90*arrowRDir),Ogre::Node::TS_WORLD);
//	}
//}
//
//void SceneMiGong::toNewScene()
//{
//	if(g_currentScene==1)
//	{
//		std::string line="Scene1 ended at "+misc.ConvertToString(totalTime);
//		g_output.writeLine(line);
//		setScene2();
//		g_sceneUI->countRestart(g_timeLimit[g_currentScore]);
//		g_sceneUI->countBegin();
//	}
//	else if(g_currentScene==2)
//	{
//		std::string line="Scene2 ended at "+misc.ConvertToString(totalTime);
//		g_output.writeLine(line);
//		setScene3();
//		g_sceneUI->countRestart(g_timeLimit[g_currentScore]);
//		g_sceneUI->countBegin();
//	}
//	else if(g_currentScene==3)
//	{
//		std::string line="Scene3 ended at "+misc.ConvertToString(totalTime);
//		g_output.writeLine(line);
//		setScene4();
//		g_sceneUI->countRestart(g_timeLimit[g_currentScore]);
//		g_sceneUI->countBegin();
//	}
//	else
//	{
//		g_freezed=true;
//		std::string line="Scene4 ended at "+misc.ConvertToString(totalTime);
//		g_output.writeLine(line);
//		end();
//		return;
//		//g_mainMgr->g_UIMgr->switchToUI("EnterUI");
//	}
//	setRotating();
//	g_sceneUI->g_currentScore=g_currentScore;
//}
//
//int SceneMiGong::getLRCommandFromFile(std::string filePath)
//{
//	if(!fileIO2.openFile(filePath)) return 0;
//	std::string str=fileIO2.getNextLine();
//	fileIO2.endInput();
//	fileIO2.createNewFile(filePath);
//	fileIO2.endOutput();
//	if(str=="Left") return 1;
//	else if(str=="Right") return 2;
//	return 0;
//}
//
//void SceneMiGong::setNextLR(std::string filePath)
//{
//	if(!fileIO3.openFile(filePath)) return;
//	std::string str=fileIO3.getNextLine();
//	fileIO3.endInput();
//	fileIO3.createNewFile(filePath);
//	fileIO3.endOutput();
//	if(str=="Left")
//	{
//		if(nextPosL<100) g_LRs[g_currentScene-1][nextPosL]=true;
//		if(nextPosR<100) g_LRs[g_currentScene-1][nextPosR]=true;
//	}
//	else if(str=="Right")
//	{
//		if(nextPosL<100) g_LRs[g_currentScene-1][nextPosL]=false;
//		if(nextPosR<100) g_LRs[g_currentScene-1][nextPosR]=false;
//	}
//}
//
//void SceneMiGong::loadInitFile(std::string filePath)
//{
//	srand(GetTickCount());
//	for(UINT j=0;j<4;j++)
//	{
//		for(UINT i=0;i<100;i++)
//		{
//			if(rand()%2==0) g_LRs[j][i]=true;
//			else g_LRs[j][i]=false;
//		}
//	}
//	if(!fileIO.openFile(filePath)) return;
//	std::string str=fileIO.getNextLine();
//	std::string temp;
//	std::string::size_type st;
//	UINT no;
//	UINT scene=0;
//	while(str!="")
//	{
//		
//		st=str.find_first_of(":");
//		temp=str.substr(0,st);
//		str=str.substr(st+1,str.length()-1);
//		if(temp=="timeLimit")
//		{
//			st=str.find_first_of(",");
//			UINT tempCount=0;
//			while(st!=str.npos)
//			{
//				temp=str.substr(0,st);
//				str=str.substr(st+1,str.length()-1);
//				g_timeLimit[tempCount]=misc.ConvertToInt(temp);
//				tempCount++;
//				st=str.find_first_of(",");
//			}
//			g_timeLimit[tempCount]=misc.ConvertToInt(str);
//		}
//		else if(temp=="scene")
//		{
//			scene=misc.ConvertToInt(str)-1;
//		}
//		else
//		{
//			no=misc.ConvertToInt(temp);
//			if(no<100)
//			{
//				if(str=="l") g_LRs[scene][no]=true;
//				else if(str=="r") g_LRs[scene][no]=false;
//			}
//		}
//		str=fileIO.getNextLine();
//	}
//	fileIO.endInput();
//}
//
//void SceneMiGong::oneSceneEnd(bool finished)
//{
//	if(finished) toNewScene();
//	else
//	{
//		g_sceneUI->hintString=L"未能在规定时间内抵达终点......\n请按F1键重新从本关开始";
//		g_sceneUI->showHint();
//		g_sceneUI->countPause();
//		g_freezed=true;
//	}
//}
//
//void SceneMiGong::end()
//{
//	g_sceneUI->hintString=L"恭喜您通过迷宫测试，请按ESC键退出";
//	g_sceneUI->showHint();
//	g_sceneUI->countPause();
//	g_freezed=true;
//}
//
//bool SceneMiGong::freezeScene()
//{
//	GASceneBase::freezeScene();
//	g_totalSeconds=g_sceneUI->g_totalSeconds;
//	g_currentSeconds=g_sceneUI->g_currentTimeSeconds;
//	g_currentScore=g_sceneUI->g_currentScore;
//	return true;
//}
//
//bool SceneMiGong::reloadScene()
//{
//	GASceneBase::reloadScene();
//	g_mainMgr->g_cameraCtrl->lock();
//	g_mainMgr->g_UIMgr->switchToUI("SceneUI");
//	g_sceneUI->g_totalSeconds=g_totalSeconds;
//	g_sceneUI->g_currentTimeSeconds=g_currentSeconds;
//	g_sceneUI->g_currentScore=g_currentScore;
//	return true;
//}
//
//void SceneMiGong::setScene1()
//{
//	g_currentScene=1;
//	g_totalSeconds=g_timeLimit[0];
//	g_currentScore=0;
//	Ogre::SceneNode* sn=g_mainMgr->getSceneNode("ballnode");
//	currPos=0;
//	setData1();
//	finalPos=g_cornerLeaf->getDataMap()->getStructVector()->size()-1;
//	GAFloat3Struct* s3=g_cornerLeaf->getDataMap()->getStructFloat3Data(0);
//	//sn->setPosition(s3->x,s3->y,s3->z);
//	sn->setPosition(s3->x,s3->y,g_height);
//	nextPosR=g_leftRightLeaf->getDataMap()->getStructFloat4Data(currPos)->y;
//	arrowRDir=2;
//	arrowLDir=0;
//	nextPosL=(UINT)-1;
//	GAFloat3Struct* s=g_cornerLeaf->getDataMap()->getStructFloat3Data(finalPos);
//	//g_endNode->setPosition(s->x,s->y,s->z);
//	waitressNode->setPosition(s->x,s->y,g_height);
//	waitressNode->setOrientation(Ogre::Quaternion::IDENTITY);
//	waitressNode->pitch(Ogre::Degree(90));
//	waitressNode->roll(Ogre::Degree(-90),Ogre::Node::TS_WORLD);
//	arrow1Pivot->setVisible(true);
//	arrow2Pivot->setVisible(false);
//	std::string line="Scene1 Started at "+misc.ConvertToString(totalTime);
//	g_output.writeLine(line);
//}
//
//void SceneMiGong::setScene2()
//{
//	g_currentScene=2;
//	g_totalSeconds=g_timeLimit[1];
//	g_currentScore=1;
//	Ogre::SceneNode* sn=g_mainMgr->getSceneNode("ballnode");
//	currPos=0;
//	setData2();
//	finalPos=g_cornerLeaf->getDataMap()->getStructVector()->size()-1;
//	GAFloat3Struct* s3=g_cornerLeaf->getDataMap()->getStructFloat3Data(0);
//	sn->setPosition(s3->x,s3->y,g_height);
//	nextPosR=g_leftRightLeaf->getDataMap()->getStructFloat4Data(currPos)->y;
//	arrowRDir=2;
//	arrowLDir=0;
//	nextPosL=(UINT)-1;
//	GAFloat3Struct* s=g_cornerLeaf->getDataMap()->getStructFloat3Data(finalPos);
//	waitressNode->setPosition(s->x,s->y,g_height);
//	waitressNode->setOrientation(Ogre::Quaternion::IDENTITY);
//	waitressNode->pitch(Ogre::Degree(90));
//	waitressNode->roll(Ogre::Degree(180),Ogre::Node::TS_WORLD);
//	arrow1Pivot->setVisible(true);
//	arrow2Pivot->setVisible(false);
//	std::string line="Scene2 Started at "+misc.ConvertToString(totalTime);
//	g_output.writeLine(line);
//}
//
//void SceneMiGong::setScene3()
//{
//	g_currentScene=3;
//	g_totalSeconds=g_timeLimit[2];
//	g_currentScore=2;
//	Ogre::SceneNode* sn=g_mainMgr->getSceneNode("ballnode");
//	currPos=0;
//	setData3();
//	finalPos=g_cornerLeaf->getDataMap()->getStructVector()->size()-1;
//	GAFloat3Struct* s3=g_cornerLeaf->getDataMap()->getStructFloat3Data(0);
//	sn->setPosition(s3->x,s3->y,g_height);
//	nextPosR=g_leftRightLeaf->getDataMap()->getStructFloat4Data(currPos)->y;
//	arrowRDir=2;
//	arrowLDir=0;
//	nextPosL=(UINT)-1;
//	GAFloat3Struct* s=g_cornerLeaf->getDataMap()->getStructFloat3Data(finalPos);
//	waitressNode->setPosition(s->x,s->y,g_height);
//	waitressNode->setOrientation(Ogre::Quaternion::IDENTITY);
//	waitressNode->pitch(Ogre::Degree(90));
//	waitressNode->roll(Ogre::Degree(-90),Ogre::Node::TS_WORLD);
//	arrow1Pivot->setVisible(true);
//	arrow2Pivot->setVisible(false);
//	std::string line="Scene3 Started at "+misc.ConvertToString(totalTime);
//	g_output.writeLine(line);
//}
//
//void SceneMiGong::setScene4()
//{
//	g_currentScene=4;
//	g_totalSeconds=g_timeLimit[3];
//	g_currentScore=3;
//	Ogre::SceneNode* sn=g_mainMgr->getSceneNode("ballnode");
//	currPos=0;
//	setData4();
//	finalPos=g_cornerLeaf->getDataMap()->getStructVector()->size()-1;
//	GAFloat3Struct* s3=g_cornerLeaf->getDataMap()->getStructFloat3Data(0);
//	sn->setPosition(s3->x,s3->y,g_height);
//	nextPosR=g_leftRightLeaf->getDataMap()->getStructFloat4Data(currPos)->y;
//	arrowRDir=2;
//	arrowLDir=0;
//	nextPosL=(UINT)-1;
//	GAFloat3Struct* s=g_cornerLeaf->getDataMap()->getStructFloat3Data(finalPos);
//	waitressNode->setPosition(s->x,s->y,g_height);
//	waitressNode->setOrientation(Ogre::Quaternion::IDENTITY);
//	waitressNode->pitch(Ogre::Degree(90));
//	waitressNode->roll(Ogre::Degree(-90),Ogre::Node::TS_WORLD);
//	arrow1Pivot->setVisible(true);
//	arrow2Pivot->setVisible(false);
//	std::string line="Scene4 Started at "+misc.ConvertToString(totalTime);
//	g_output.writeLine(line);
//}
//
//void SceneMiGong::setData1()
//{
//	float transx=-960,transy=0;
//	GADataManager* dataMgr=g_mainMgr->g_dataMgr;
//	g_dataNode=dataMgr->getRootNode()->getChildContainerNode("SceneMiGong1");
//	if(g_dataNode!=0)
//	{
//		g_cornerLeaf=g_dataNode->getChildLeafNode("corner");
//		g_leftRightLeaf=g_dataNode->getChildLeafNode("leftRight");
//		if(g_cornerLeaf!=0&&g_leftRightLeaf!=0)
//		{
//			return;
//		}
//	}
//	g_dataNode=dataMgr->getRootNode()->createChildContainerNode("SceneMiGong1");
//	g_cornerLeaf=g_dataNode->createChildLeafNode("corner");
//	g_leftRightLeaf=g_dataNode->createChildLeafNode("leftRight");
//	g_cornerLeaf->initDataMap(GADataMap::DataMapType_StructFloat3);
//	g_leftRightLeaf->initDataMap(GADataMap::DataMapType_StructFloat4);
//	GAFloat3Struct* s3;
//	GAFloat4Struct* s4;
//	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
//	s3->x=-1024,s3->y=56,s3->z=7.5;
//	s4->x=-1;s4->y=1;s4->z=-1;s4->w=-1;
//	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
//	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
//	s3->x=-904,s3->y=56,s3->z=7.5;
//	s4->x=-1;s4->y=-1;s4->z=2;s4->w=0;
//	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
//	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
//	s3->x=-904,s3->y=-40,s3->z=7.5;
//	s4->x=1;s4->y=-1;s4->z=-1;s4->w=3;
//	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
//	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
//	s3->x=-1000,s3->y=-40,s3->z=7.5;
//	s4->x=4;s4->y=2;s4->z=-1;s4->w=-1;
//	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
//	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
//	s3->x=-1000,s3->y=24,s3->z=7.5;
//	s4->x=-1;s4->y=-1;s4->z=3;s4->w=5;
//	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
//	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
//	s3->x=-1032,s3->y=24,s3->z=7.5;
//	s4->x=-1;s4->y=4;s4->z=6;s4->w=-1;
//	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
//	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
//	s3->x=-1032,s3->y=-72,s3->z=7.5;
//	s4->x=5;s4->y=7;s4->z=-1;s4->w=-1;
//	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
//	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
//	s3->x=-912,s3->y=-72,s3->z=7.5;
//	s4->x=-1;s4->y=-1;s4->z=-1;s4->w=6;
//	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
//}
//
//void SceneMiGong::setData2()
//{
//	float transx=0,transy=832;
//	GADataManager* dataMgr=g_mainMgr->g_dataMgr;
//	g_dataNode=dataMgr->getRootNode()->getChildContainerNode("SceneMiGong2");
//	if(g_dataNode!=0)
//	{
//		g_cornerLeaf=g_dataNode->getChildLeafNode("corner");
//		g_leftRightLeaf=g_dataNode->getChildLeafNode("leftRight");
//		if(g_cornerLeaf!=0&&g_leftRightLeaf!=0)
//		{
//			return;
//		}
//	}
//	g_dataNode=dataMgr->getRootNode()->createChildContainerNode("SceneMiGong2");
//	g_cornerLeaf=g_dataNode->createChildLeafNode("corner");
//	g_leftRightLeaf=g_dataNode->createChildLeafNode("leftRight");
//	g_cornerLeaf->initDataMap(GADataMap::DataMapType_StructFloat3);
//	g_leftRightLeaf->initDataMap(GADataMap::DataMapType_StructFloat4);
//	GAFloat3Struct* s3;
//	GAFloat4Struct* s4;
//	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
//	s3->x=-112,s3->y=952,s3->z=7.5;
//	s4->x=-1;s4->y=1;s4->z=-1;s4->w=-1;
//	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
//	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
//	s3->x=-8,s3->y=952,s3->z=7.5;
//	s4->x=-1;s4->y=-1;s4->z=2;s4->w=0;
//	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
//	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
//	s3->x=-8,s3->y=856,s3->z=7.5;
//	s4->x=1;s4->y=10;s4->z=-1;s4->w=3;
//	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
//	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
//	s3->x=-88,s3->y=856,s3->z=7.5;
//	s4->x=-1;s4->y=2;s4->z=4;s4->w=-1;
//	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
//	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
//	s3->x=-88,s3->y=792,s3->z=7.5;
//	s4->x=3;s4->y=5;s4->z=-1;s4->w=-1;
//	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
//	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
//	s3->x=-56,s3->y=792,s3->z=7.5;
//	s4->x=-1;s4->y=6;s4->z=11;s4->w=4;
//	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
//	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
//	s3->x=24,s3->y=792,s3->z=7.5;
//	s4->x=-1;s4->y=7;s4->z=12;s4->w=5;
//	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
//	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
//	s3->x=88,s3->y=792,s3->z=7.5;
//	s4->x=8;s4->y=-1;s4->z=-1;s4->w=6;
//	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
//	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
//	s3->x=88,s3->y=920,s3->z=7.5;
//	s4->x=-1;s4->y=-1;s4->z=7;s4->w=9;
//	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
//	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
//	s3->x=40,s3->y=920,s3->z=7.5;
//	s4->x=-1;s4->y=8;s4->z=10;s4->w=-1;
//	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
//	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
//	s3->x=40,s3->y=856,s3->z=7.5;
//	s4->x=9;s4->y=-1;s4->z=-1;s4->w=2;
//	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
//	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
//	s3->x=-56,s3->y=744,s3->z=7.5;
//	s4->x=5;s4->y=12;s4->z=-1;s4->w=-1;
//	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
//	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
//	s3->x=24,s3->y=744,s3->z=7.5;
//	s4->x=6;s4->y=-1;s4->z=13;s4->w=11;
//	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
//	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
//	s3->x=24,s3->y=720,s3->z=7.5;
//	s4->x=12;s4->y=-1;s4->z=-1;s4->w=-1;
//	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
//}
//
//void SceneMiGong::setData3()
//{
//	float transx=1152,transy=0;
//	GADataManager* dataMgr=g_mainMgr->g_dataMgr;
//	g_dataNode=dataMgr->getRootNode()->getChildContainerNode("SceneMiGong3");
//	if(g_dataNode!=0)
//	{
//		g_cornerLeaf=g_dataNode->getChildLeafNode("corner");
//		g_leftRightLeaf=g_dataNode->getChildLeafNode("leftRight");
//		if(g_cornerLeaf!=0&&g_leftRightLeaf!=0)
//		{
//			return;
//		}
//	}
//	g_dataNode=dataMgr->getRootNode()->createChildContainerNode("SceneMiGong3");
//	g_cornerLeaf=g_dataNode->createChildLeafNode("corner");
//	g_leftRightLeaf=g_dataNode->createChildLeafNode("leftRight");
//	g_cornerLeaf->initDataMap(GADataMap::DataMapType_StructFloat3);
//	g_leftRightLeaf->initDataMap(GADataMap::DataMapType_StructFloat4);
//	GAFloat3Struct* s3;
//	GAFloat4Struct* s4;
//	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
//	s3->x=960,s3->y=136,s3->z=7.5;
//	s4->x=-1;s4->y=1;s4->z=-1;s4->w=-1;
//	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
//	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
//	s3->x=1032,s3->y=136,s3->z=7.5;
//	s4->x=-1;s4->y=2;s4->z=5;s4->w=0;
//	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
//	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
//	s3->x=1088,s3->y=136,s3->z=7.5;
//	s4->x=-1;s4->y=-1;s4->z=-1;s4->w=1;
//	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
//	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
//	s3->x=928,s3->y=72,s3->z=7.5;
//	s4->x=-1;s4->y=4;s4->z=-1;s4->w=-1;
//	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
//	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
//	s3->x=968,s3->y=72,s3->z=7.5;
//	s4->x=-1;s4->y=5;s4->z=8;s4->w=3;
//	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
//	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
//	s3->x=1032,s3->y=72,s3->z=7.5;
//	s4->x=1;s4->y=6;s4->z=-1;s4->w=4;
//	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
//	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
//	s3->x=1080,s3->y=72,s3->z=7.5;
//	s4->x=-1;s4->y=7;s4->z=10;s4->w=5;
//	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
//	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
//	s3->x=1144,s3->y=72,s3->z=7.5;
//	s4->x=-1;s4->y=-1;s4->z=11;s4->w=6;
//	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
//	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
//	s3->x=968,s3->y=-8,s3->z=7.5;
//	s4->x=4;s4->y=9;s4->z=-1;s4->w=-1;
//	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
//	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
//	s3->x=1016,s3->y=-8,s3->z=7.5;
//	s4->x=-1;s4->y=10;s4->z=13;s4->w=8;
//	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
//	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
//	s3->x=1080,s3->y=-8,s3->z=7.5;
//	s4->x=6;s4->y=11;s4->z=-1;s4->w=9;
//	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
//	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
//	s3->x=1144,s3->y=-8,s3->z=7.5;
//	s4->x=7;s4->y=-1;s4->z=15;s4->w=10;
//	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
//	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
//	s3->x=968,s3->y=-72,s3->z=7.5;
//	s4->x=-1;s4->y=13;s4->z=16;s4->w=-1;
//	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
//	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
//	s3->x=1016,s3->y=-72,s3->z=7.5;
//	s4->x=9;s4->y=14;s4->z=-1;s4->w=12;
//	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
//	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
//	s3->x=1112,s3->y=-72,s3->z=7.5;
//	s4->x=-1;s4->y=15;s4->z=20;s4->w=13;
//	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
//	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
//	s3->x=1144,s3->y=-72,s3->z=7.5;
//	s4->x=11;s4->y=-1;s4->z=-1;s4->w=14;
//	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
//	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
//	s3->x=968,s3->y=-136,s3->z=7.5;
//	s4->x=12;s4->y=17;s4->z=-1;s4->w=-1;
//	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
//	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
//	s3->x=1016,s3->y=-136,s3->z=7.5;
//	s4->x=18;s4->y=20;s4->z=-1;s4->w=16;
//	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
//	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
//	s3->x=1016,s3->y=-104,s3->z=7.5;
//	s4->x=-1;s4->y=19;s4->z=17;s4->w=-1;
//	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
//	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
//	s3->x=1056,s3->y=-104,s3->z=7.5;
//	s4->x=-1;s4->y=-1;s4->z=-1;s4->w=18;
//	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
//	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
//	s3->x=1112,s3->y=-136,s3->z=7.5;
//	s4->x=14;s4->y=21;s4->z=-1;s4->w=17;
//	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
//	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
//	s3->x=1136,s3->y=-136,s3->z=7.5;
//	s4->x=-1;s4->y=-1;s4->z=-1;s4->w=20;
//	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
//}
//
//void SceneMiGong::setData4()
//{
//	float transx=0,transy=-1024;
//	GADataManager* dataMgr=g_mainMgr->g_dataMgr;
//	g_dataNode=dataMgr->getRootNode()->getChildContainerNode("SceneMiGong4");
//	if(g_dataNode!=0)
//	{
//		g_cornerLeaf=g_dataNode->getChildLeafNode("corner");
//		g_leftRightLeaf=g_dataNode->getChildLeafNode("leftRight");
//		if(g_cornerLeaf!=0&&g_leftRightLeaf!=0)
//		{
//			return;
//		}
//	}
//	g_dataNode=dataMgr->getRootNode()->createChildContainerNode("SceneMiGong4");
//	g_cornerLeaf=g_dataNode->createChildLeafNode("corner");
//	g_leftRightLeaf=g_dataNode->createChildLeafNode("leftRight");
//	g_cornerLeaf->initDataMap(GADataMap::DataMapType_StructFloat3);
//	g_leftRightLeaf->initDataMap(GADataMap::DataMapType_StructFloat4);
//	GAFloat3Struct* s3;
//	GAFloat4Struct* s4;
//	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
//	s3->x=-120,s3->y=-904,s3->z=7.5;
//	s4->x=-1;s4->y=1;s4->z=9;s4->w=-1;
//	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
//	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
//	s3->x=-56,s3->y=-904,s3->z=7.5;
//	s4->x=-1;s4->y=-1;s4->z=8;s4->w=0;
//	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
//	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
//	s3->x=-8,s3->y=-904,s3->z=7.5;
//	s4->x=-1;s4->y=3;s4->z=12;s4->w=-1;
//	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
//	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
//	s3->x=24,s3->y=-904,s3->z=7.5;
//	s4->x=-1;s4->y=4;s4->z=19;s4->w=2;
//	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
//	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
//	s3->x=56,s3->y=-904,s3->z=7.5;
//	s4->x=-1;s4->y=-1;s4->z=13;s4->w=3;
//	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
//	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
//	s3->x=88,s3->y=-904,s3->z=7.5;
//	s4->x=-1;s4->y=6;s4->z=14;s4->w=-1;
//	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
//	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
//	s3->x=120,s3->y=-904,s3->z=7.5;
//	s4->x=-1;s4->y=-1;s4->z=21;s4->w=5;
//	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
//	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
//	s3->x=-88,s3->y=-936,s3->z=7.5;
//	s4->x=-1;s4->y=8;s4->z=10;s4->w=-1;
//	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
//	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
//	s3->x=-56,s3->y=-936,s3->z=7.5;
//	s4->x=1;s4->y=-1;s4->z=11;s4->w=7;
//	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
//	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
//	s3->x=-120,s3->y=-952,s3->z=7.5;
//	s4->x=0;s4->y=10;s4->z=-1;s4->w=-1;
//	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
//	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
//	s3->x=-88,s3->y=-952,s3->z=7.5;
//	s4->x=7;s4->y=-1;s4->z=15;s4->w=9;
//	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
//	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
//	s3->x=-56,s3->y=-968,s3->z=7.5;
//	s4->x=8;s4->y=12;s4->z=-1;s4->w=-1;
//	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
//	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
//	s3->x=-8,s3->y=-968,s3->z=7.5;
//	s4->x=2;s4->y=-1;s4->z=-1;s4->w=11;
//	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
//	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
//	s3->x=56,s3->y=-936,s3->z=7.5;
//	s4->x=4;s4->y=14;s4->z=20;s4->w=-1;
//	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
//	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
//	s3->x=88,s3->y=-936,s3->z=7.5;
//	s4->x=5;s4->y=-1;s4->z=-1;s4->w=13;
//	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
//	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
//	s3->x=-88,s3->y=-1000,s3->z=7.5;
//	s4->x=10;s4->y=16;s4->z=-1;s4->w=-1;
//	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
//	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
//	s3->x=-8,s3->y=-1000,s3->z=7.5;
//	s4->x=-1;s4->y=-1;s4->z=25;s4->w=15;
//	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
//	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
//	s3->x=-104,s3->y=-1032,s3->z=7.5;
//	s4->x=-1;s4->y=18;s4->z=26;s4->w=-1;
//	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
//	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
//	s3->x=-40,s3->y=-1032,s3->z=7.5;
//	s4->x=-1;s4->y=-1;s4->z=23;s4->w=17;//18
//	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
//	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
//	s3->x=24,s3->y=-1032,s3->z=7.5;
//	s4->x=3;s4->y=20;s4->z=28;s4->w=-1;//19
//	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
//	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
//	s3->x=56,s3->y=-1032,s3->z=7.5;
//	s4->x=13;s4->y=21;s4->z=-1;s4->w=19;//20
//	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
//	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
//	s3->x=120,s3->y=-1032,s3->z=7.5;
//	s4->x=6;s4->y=-1;s4->z=30;s4->w=20;//21
//	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
//	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
//	s3->x=-72,s3->y=-1064,s3->z=7.5;
//	s4->x=-1;s4->y=23;s4->z=24;s4->w=-1;//22
//	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
//	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
//	s3->x=-40,s3->y=-1064,s3->z=7.5;
//	s4->x=18;s4->y=-1;s4->z=-1;s4->w=22;
//	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
//	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
//	s3->x=-72,s3->y=-1096,s3->z=7.5;
//	s4->x=22;s4->y=25;s4->z=27;s4->w=-1;
//	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
//	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
//	s3->x=-8,s3->y=-1096,s3->z=7.5;
//	s4->x=16;s4->y=-1;s4->z=-1;s4->w=24;
//	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
//	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
//	s3->x=-104,s3->y=-1128,s3->z=7.5;
//	s4->x=17;s4->y=27;s4->z=31;s4->w=-1;
//	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
//	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
//	s3->x=-72,s3->y=-1128,s3->z=7.5;
//	s4->x=24;s4->y=28;s4->z=-1;s4->w=26;
//	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
//	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
//	s3->x=24,s3->y=-1128,s3->z=7.5;
//	s4->x=19;s4->y=29;s4->z=-1;s4->w=27;
//	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
//	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
//	s3->x=72,s3->y=-1128,s3->z=7.5;
//	s4->x=-1;s4->y=30;s4->z=32;s4->w=28;
//	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
//	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
//	s3->x=120,s3->y=-1128,s3->z=7.5;
//	s4->x=21;s4->y=-1;s4->z=-1;s4->w=29;
//	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
//	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
//	s3->x=-104,s3->y=-1176,s3->z=7.5;
//	s4->x=26;s4->y=32;s4->z=-1;s4->w=-1;
//	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
//	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
//	s3->x=72,s3->y=-1176,s3->z=7.5;
//	s4->x=29;s4->y=33;s4->z=-1;s4->w=31;
//	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
//	s3=new GAFloat3Struct();s4=new GAFloat4Struct();
//	s3->x=112,s3->y=-1176,s3->z=7.5;
//	s4->x=-1;s4->y=-1;s4->z=-1;s4->w=33;
//	g_cornerLeaf->getDataMap()->addData(s3);g_leftRightLeaf->getDataMap()->addData(s4);
//}