#include "SceneChabei.h"
#include "GAPCameraController.h"
#include "GAPWorldManager.h"
#include "GAPModelManager.h"
#include "GAPAudioManager.h"
#include "GASceneManager.h"

float progressBar1Ratio=1;
float progressBar2Ratio=1;

SceneChabei::SceneChabei(GAPMainManager* mainMgr,std::string sceneName) : GASceneBase(mainMgr,sceneName)
{
	g_sceneUI=0;
	g_output.createNewFile("../SceneChaBei/ChabeiOutput.txt");
	newSceneRoll=new GARollPoint(7);
	g_deltaWaitRoll=new GARollPoint(2);
	s_timeLeft=0;
	g_soundEnable=true;
	g_soundType=0;
}

bool SceneChabei::createScene()
{
	Ogre::SceneManager* sm=Ogre::Root::getSingleton().createSceneManager("BspSceneManager","chabei");
	Ogre::Camera* cam=sm->createCamera("chabei");
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
	g_mainMgr->g_worldMgr->addModel("yizi1","yizi2_fix.mesh",Ogre::Vector3(-618,-328,28),Ogre::Vector3(3.99094,3.99094,3.99094),Ogre::Quaternion(0.5,0.5,0.5,0.5));
	g_mainMgr->g_worldMgr->addModel("yizi2","yizi2_fix.mesh",Ogre::Vector3(-618,-458,28),Ogre::Vector3(3.99094,3.99094,3.99094),Ogre::Quaternion(0.5,0.5,0.5,0.5));
	g_mainMgr->g_worldMgr->addModel("shafa1","shafa3_fix.mesh",Ogre::Vector3(-29.6475,-318.824,29.0582),Ogre::Vector3(3.14328,3.14328,3.14328),Ogre::Quaternion(0.5,0.5,0.5,0.5));
	g_mainMgr->g_worldMgr->addModel("chaji1","chaji1_fix.mesh",Ogre::Vector3(-86.7032,-455.585,29.3137),Ogre::Vector3(2.66635,2.66635,2.66635),Ogre::Quaternion(0.707107,0.707107,0,0));
	g_mainMgr->g_worldMgr->addModel("zhuozi1","zhuozi_fix.mesh",Ogre::Vector3(-613.607,-390.19,48),Ogre::Vector3(1,1,1),Ogre::Quaternion(0.707107,0.707107,0,0));
	g_mainMgr->g_worldMgr->addModel("mandrink","man11.mesh",Ogre::Vector3(-618,-328,-28),Ogre::Vector3(1,1,1),Ogre::Quaternion(0.707107,0.707107,0,0));
	g_mainMgr->g_worldMgr->addModel("womandrink","woman11.mesh",Ogre::Vector3(-618,-458,-28),Ogre::Vector3(1,1,1),Ogre::Quaternion(0.707107,0.707107,0,0));
	g_mainMgr->g_worldMgr->addModel("beizi1","coca1.mesh",Ogre::Vector3(-616.8,-382.0,100.7),Ogre::Vector3(1,1,1));
	g_mainMgr->g_worldMgr->addModel("beizi2","coca1.mesh",Ogre::Vector3(-618.3,-411.0,100.7),Ogre::Vector3(0.9,0.9,0.9));
	g_mainMgr->getCurrentSceneMgr()->getSceneNode("yizi2")->roll(Ogre::Degree(180),Ogre::Node::TS_WORLD);
	Ogre::Light* l=sm->createLight();
	l->setPosition(-600,-500,200);
	Ogre::Light* l2=sm->createLight();
	l2->setPosition(-200,-500,200);
	sm->getSceneNode("beizi1")->roll(Ogre::Degree(-90),Ogre::Node::TS_WORLD);
	sm->getSceneNode("beizi2")->roll(Ogre::Degree(90),Ogre::Node::TS_WORLD);
	sm->getSceneNode("womandrink")->roll(Ogre::Degree(-180),Ogre::Node::TS_WORLD);
	GACharacterBase *c1=new GACharacterBase(sm,"mandrink");
	GACharacterBase *c2=new GACharacterBase(sm,"womandrink");
	g_manAI=new GACharacterAIBase(c1);
	g_womanAI=new GACharacterAIBase(c2);
	g_manAI->runAnimation("left");
	g_womanAI->runAnimation("right",false);
	g_manAI->pauseAnimation();
	//g_womanAI->pauseAnimation();
	Ogre::Skeleton* ske1=sm->getEntity("mandrink")->getSkeleton();
	boneR=ske1->getBone("Bip01 R Hand");
	boneL=ske1->getBone("Bip01 L Hand");
	Ogre::Skeleton* ske2=sm->getEntity("womandrink")->getSkeleton();
	boneWomanR=ske2->getBone("Bip01 R Hand");
	boneWomanL=ske2->getBone("Bip01 L Hand");
	Ogre::SceneNode* node1=sm->getRootSceneNode()->createChildSceneNode("rhandNode");
	Ogre::SceneNode* node2=sm->getRootSceneNode()->createChildSceneNode("lhandNode");
	Ogre::SceneNode* capNodeR=node1->createChildSceneNode("rhandCup");
	Ogre::SceneNode* capNodeL=node2->createChildSceneNode("lhandCup");
	Ogre::SceneNode* node1w=sm->getRootSceneNode()->createChildSceneNode("rhandNodew");
	Ogre::SceneNode* node2w=sm->getRootSceneNode()->createChildSceneNode("lhandNodew");
	Ogre::SceneNode* capNodeRW=node1w->createChildSceneNode("rhandCupw");
	Ogre::SceneNode* capNodeLW=node2w->createChildSceneNode("lhandCupw");
	capNodeR->setVisible(true);
	capNodeL->setVisible(true);
	capNodeR->setPosition(10.65,1.7,-5.96);
	capNodeL->setPosition(8.83,-1.43,-4.66);
	capNodeR->setOrientation(0.986,0,-0.165,0);
	capNodeL->setOrientation(0,0.994,0,0.104);
	capNodeLW->setVisible(true);
	capNodeRW->setVisible(true);
	capNodeRW->setPosition(8.39,1.96,-4.88);
	capNodeLW->setPosition(7.71,-1.56,-5.12);
	capNodeRW->setOrientation(0.986,0,-0.165,0);
	capNodeLW->setOrientation(0,0.994,0,0.104);
	Ogre::Entity* entR=sm->createEntity("beizi_R","coca1.mesh");
	Ogre::Entity* entL=sm->createEntity("beizi_L","coca1.mesh");
	Ogre::Entity* entRW=sm->createEntity("beizi_RW","coca1.mesh");
	Ogre::Entity* entLW=sm->createEntity("beizi_LW","coca1.mesh");
	capNodeR->attachObject(entR);
	capNodeL->attachObject(entL);
	capNodeRW->attachObject(entRW);
	capNodeLW->attachObject(entLW);
	capNodeRW->scale(0.9,0.9,0.9);
	capNodeLW->scale(0.9,0.9,0.9);
	
	Ogre::SceneNode* cameraStayNode=g_mainMgr->getSceneNode("mandrink")->createChildSceneNode("cameraStayNode",Ogre::Vector3(0,240,-37));
	cameraStayNode->roll(Ogre::Degree(180),Ogre::Node::TS_WORLD);
	cameraStayNode->pitch(Ogre::Degree(-48));
	g_mainMgr->g_cameraCtrl->setCameraStyle(GAPCameraController::CameraStyle_StayOnNode,g_mainMgr->getSceneNode("cameraStayNode"));
	g_mainMgr->g_cameraCtrl->lock();
	g_mainMgr->g_UIMgr->switchToUI("SceneUI");
	g_sceneUI=(GAExampleSceneUI*)g_mainMgr->g_UIMgr->findUI("SceneUI");
	reCreateScene();
	return true;
}

bool SceneChabei::reCreateScene()
{
	totalTime=0;
	g_sceneRunningTime=0;
	g_currentScore=0;
	g_freezed=true;
	g_started=false;
	g_leftRight=0;
	g_womanLeftRight=2;
	g_manInAni=false;
	womanRatio=0.3;
	manRatio=0.5;
	currentState=0;
	g_toNewScene=false;
	g_sceneEnd=false;
	g_isWomanFinished=g_womanStart=false;
	g_manAI->resetAnimation();
	g_womanAI->resetAnimation();
	loadInitFile("../SceneChaBei/init.txt");
	g_sceneUI->countRestart((UINT)(3.5/womanRatio)+1);
	g_sceneUI->countPause();
	g_mainMgr->g_UIMgr->switchToUI("SceneUI");
	g_sceneUI->hideHint();
	if(g_sceneUI->welcome)
	{
		g_sceneUI->hintString=L"欢迎使用交大虚拟康复训练平台";
		g_sceneUI->showHint();
	}
	g_sceneUI->caption="Score";
	g_sceneUI->g_totalScore=g_totalScore;
	g_sceneUI->g_currentScore=g_currentScore;
	g_output.writeLine("Scene ChaBei created");
	g_sceneUI->progressBar1Visible(true);
	g_sceneUI->ProgressBar1Pause();
	g_sceneUI->setProgressBarRatio(1);
	g_sceneUI->progressBar2Visible(true);
	g_sceneUI->setProgressBar2Ratio(1);
	g_sceneUI->ProgressBar2Pause();
	g_toNewScene=false;
	g_deltaWaitRollBegin=false;
	g_sceneUI->g_freezed=false;
	g_mainMgr->getSceneNode("beizi2")->setVisible(true);
	g_mainMgr->getSceneNode("rhandCupw")->setVisible(false);
	g_mainMgr->getSceneNode("lhandCupw")->setVisible(false);
	g_mainMgr->getSceneNode("beizi1")->setVisible(true);
	g_mainMgr->getSceneNode("rhandCup")->setVisible(false);
	g_mainMgr->getSceneNode("lhandCup")->setVisible(false);
	g_trialState=0;
	return true;
}

void SceneChabei::start()
{
	g_mainMgr->g_cameraCtrl->setCameraStyle(GAPCameraController::CameraStyle_StayOnNode,g_mainMgr->getSceneNode("cameraStayNode"));
	g_mainMgr->g_cameraCtrl->lock();
	showHint(manForceLeftRightVec[g_currentScore%manForceLeftRightVec.size()]);
	g_freezed=false;
	g_sceneUI->g_freezed=false;
	g_started=true;
	g_deltaWaitRollBegin=true;
	g_sceneUI->welcome=false;
}
bool SceneChabei::keyReleased(const OIS::KeyEvent& evt)
{
	if(evt.key==OIS::KC_ESCAPE)
	{
		g_mainMgr->g_UIMgr->switchToUI("EnterUI");
		g_mainMgr->g_cameraCtrl->lock();
	}
	/*else if(evt.key==OIS::KC_G)
	{
		Ogre::Vector3 vec=g_mainMgr->getCurrentCamera()->getParentSceneNode()->getPosition();
		float t=vec.z;
	}*/
	if(evt.key==OIS::KC_F1)
	{
		if(g_sceneEnd) reCreateScene();
		if(g_freezed) start();
	}
	if(evt.key==OIS::KC_F2)
	{
		g_mainMgr->g_cameraCtrl->setCameraStyle(GAPCameraController::CameraStyle_Free);
		g_mainMgr->g_cameraCtrl->unlock();
	}
	if(g_freezed) return true;
	if(evt.key==OIS::KC_L||evt.key==OIS::KC_LEFT)
	{
		manAniLeft();
	}
	if(evt.key==OIS::KC_R||evt.key==OIS::KC_RIGHT)
	{
		manAniRight();
	}
	return true;
}
bool SceneChabei::frameStarted(float deltaTime)
{
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
				std::string str="into scene Rehablitation in "+misc.ConvertToString(time)+" seconds";
				g_sceneUI->hintString=str;
				g_sceneUI->showHint();
			}
		}
	}
	if(g_freezed) return true;
	g_trialState=1;
	if(g_deltaWaitRollBegin)
	{
		if(g_deltaWaitRoll->getRollPoint(deltaTime,1))
		{
			g_deltaWaitRollBegin=false;
			if(!g_womanStart)
			{
				womanAniStart();
				g_sceneUI->countBegin();
				g_sceneUI->ProgressBar2Start(3.5f*0.7f/manRatio);
				g_sceneUI->ProgressBar2Pause();
				g_sceneUI->ProgressBar1Start(g_sceneUI->g_totalSeconds);
			}
			g_sceneUI->hideHint();
		}
		//return true;
	}
	totalTime+=deltaTime;
	checkAniState();
	Ogre::SceneNode* node=0;

	if(!g_womanStart) return true;
	g_womanAI->update(deltaTime*womanRatio);
	Ogre::Matrix4 mat4=moveCupWoman();
	Ogre::Vector3 vec3=mat4.getTrans()+Ogre::Vector3(-618,-458,-28);
	if(g_womanLeftRight==1) node=g_mainMgr->getSceneNode("lhandNodew");
	else if(g_womanLeftRight==2)  node=g_mainMgr->getSceneNode("rhandNodew");
	else return true;
	node->setPosition(vec3);
	node->setOrientation(mat4.extractQuaternion());

	g_manAI->update(deltaTime*manRatio);
	Ogre::Matrix4 mat3=moveCup();
	Ogre::Vector3 vec2=mat3.getTrans()+Ogre::Vector3(-618,-328,-28);
	if(g_leftRight==1) node=g_mainMgr->getSceneNode("lhandNode");
	else if(g_leftRight==2)  node=g_mainMgr->getSceneNode("rhandNode");
	else return true;
	node->setPosition(vec2);
	node->setOrientation(mat3.extractQuaternion());

	return true;
}

Ogre::Matrix4 SceneChabei::moveCup()
{
	float time=g_manAI->getAnimationTimePos();
	Ogre::Matrix4 mat;
	Ogre::Matrix4 mat2;
	if(time>2.5)
	{
		if(g_leftRight==1)
		{
			moveName="lhandCup";
			g_mainMgr->getSceneNode(moveName)->setVisible(true);
			g_mainMgr->getSceneNode("rhandCup")->setVisible(false);
			g_mainMgr->getSceneNode("beizi1")->setVisible(false);
			mat=boneL->_getFullTransform();
			mat2=Ogre::Matrix4(1,0,0,0,0,0,-1,0,0,1,0,0,0,0,0,1);
			return mat2.concatenate(mat);
		}
		else if(g_leftRight==2)
		{
			moveName="rhandCup";
			g_mainMgr->getSceneNode(moveName)->setVisible(true);
			g_mainMgr->getSceneNode("lhandCup")->setVisible(false);
			g_mainMgr->getSceneNode("beizi1")->setVisible(false);
			mat=boneR->_getFullTransform();
			mat2=Ogre::Matrix4(1,0,0,0,0,0,-1,0,0,1,0,0,0,0,0,1);
			return mat2.concatenate(mat);
		}
	}
	else
	{
		moveName="beizi1";
		g_mainMgr->getSceneNode(moveName)->setVisible(true);
		g_mainMgr->getSceneNode("rhandCup")->setVisible(false);
		g_mainMgr->getSceneNode("lhandCup")->setVisible(false);
	}
	return Ogre::Matrix4::IDENTITY;
}

Ogre::Matrix4 SceneChabei::moveCupWoman()
{
	float time=g_womanAI->getAnimationTimePos();
	Ogre::Matrix4 mat;
	Ogre::Matrix4 mat2;
	if(time>2.5)
	{
		if(g_womanLeftRight==1)
		{
			moveName="lhandCupw";
			g_mainMgr->getSceneNode(moveName)->setVisible(true);
			g_mainMgr->getSceneNode("rhandCupw")->setVisible(false);
			g_mainMgr->getSceneNode("beizi2")->setVisible(false);
			mat=boneWomanL->_getFullTransform();
			mat2=Ogre::Matrix4(1,0,0,0,0,0,-1,0,0,1,0,0,0,0,0,1);
			mat2=mat2.concatenate(Ogre::Matrix4(-1,0,0,0,0,1,0,0,0,0,-1,0,0,0,0,1));
			return mat2.concatenate(mat);
		}
		else if(g_womanLeftRight==2)
		{
			moveName="rhandCupw";
			g_mainMgr->getSceneNode(moveName)->setVisible(true);
			g_mainMgr->getSceneNode("lhandCupw")->setVisible(false);
			g_mainMgr->getSceneNode("beizi2")->setVisible(false);
			mat=boneWomanR->_getFullTransform();
			mat2=Ogre::Matrix4(1,0,0,0,0,0,-1,0,0,1,0,0,0,0,0,1);
			mat2=mat2.concatenate(Ogre::Matrix4(-1,0,0,0,0,1,0,0,0,0,-1,0,0,0,0,1));
			return mat2.concatenate(mat);
		}
	}
	else
	{
		moveName="beizi2";
		g_mainMgr->getSceneNode(moveName)->setVisible(true);
		g_mainMgr->getSceneNode("rhandCupw")->setVisible(false);
		g_mainMgr->getSceneNode("lhandCupw")->setVisible(false);
	}
	return Ogre::Matrix4::IDENTITY;
}

bool SceneChabei::freezeScene()
{
	GASceneBase::freezeScene();
	g_currentScore=g_sceneUI->g_currentScore;
	g_totalScore=g_sceneUI->g_totalScore;
	g_currentTime=g_sceneUI->g_currentTimeSeconds;
	g_totalTime=g_sceneUI->g_totalSeconds;
	progressBar1Ratio=g_sceneUI->getProgressBarRatio();
	progressBar2Ratio=g_sceneUI->getProgressBar2Ratio();
	return true;
}

bool SceneChabei::reloadScene()
{
	GASceneBase::reloadScene();
	if(!g_started) g_freezed=true; 
	g_mainMgr->g_cameraCtrl->lock();
	g_sceneUI->g_currentScore=g_currentScore;
	g_sceneUI->g_totalScore=g_totalScore;
	g_sceneUI->g_currentTimeSeconds=g_currentTime;
	g_sceneUI->g_totalSeconds=g_totalTime;
	g_sceneUI->caption="Score";
	g_sceneUI->progressBar1Visible(true);
	g_sceneUI->progressBar2Visible(true);
	g_sceneUI->ProgressBar1Start(3.5f*0.7f/manRatio);
	g_sceneUI->ProgressBar2Start(g_totalTime);
	g_sceneUI->setProgressBarRatio(progressBar1Ratio);
	g_sceneUI->setProgressBar2Ratio(progressBar2Ratio);
	if(g_freezed)
	{
		reCreateScene();
	}
	else
	{
		if(!g_womanStart)
		{
			showHint(manForceLeftRightVec[g_currentScore%manForceLeftRightVec.size()]);
			g_sceneUI->ProgressBar1Pause();
			g_sceneUI->ProgressBar2Pause();
		}
		else
		{
			g_sceneUI->countBegin();
			g_sceneUI->hideHint();
			g_sceneUI->ProgressBar2Pause();
		}
	}
	g_mainMgr->g_UIMgr->switchToUI("SceneUI");
	return true;
}

void SceneChabei::checkAniState()
{
	float time=0;
	float totalAniTime=3.5*0.7f;
	if(g_sceneUI->isCountOver()) g_isWomanFinished=true;
	if(g_leftRight!=0) time=g_manAI->getAnimationTimePos();
	time-=3.5f*0.3f;
	std::vector<float> *vv;
	if(g_isWomanFinished)
	{
		currentState=0;
		g_manInAni=false;
		g_manAI->resetAnimation();
		g_leftRight=0;
		g_manAI->pauseAnimation();
		//if(g_soundEnable) g_mainMgr->g_audioMgr->playShortSound("../sounds/shibai.wav");
		oneTurnFinished();
	}
	if(g_leftRight==1)
	{
		vv=&timeVecLeft;
	}
	else if(g_leftRight==2)
	{
		vv=&timeVecRight;
	}
	else return;
	if(currentState==vv->size()-1)
	{
		if(time<totalAniTime*vv->at(0))
		{
			currentState=0;
			g_manInAni=false;
			g_manAI->resetAnimation();
			g_leftRight=0;
			g_manAI->pauseAnimation();
			//if(g_soundEnable) g_mainMgr->g_audioMgr->playShortSound("../sounds/chenggong.wav");
			oneTurnFinished();
		}
	}
	else if(time>totalAniTime*vv->at(currentState))
	{
		currentState++;
		g_manInAni=false;
		g_manAI->pauseAnimation();
		g_sceneUI->ProgressBar2Pause();
	}		
}
void SceneChabei::manAniLeft()
{
	std::string line="";
	line=misc.ConvertToString(totalTime)+":left";
	if(g_leftRight==0)
	{
		if(g_currentScore<manForceLeftRightVec.size()&&manForceLeftRightVec[g_currentScore]==2)
		{
			line="@"+line;
			g_output.writeLine(line);
			return;
		}
		g_manAI->runAnimation("left");
		g_manAI->setAnimationTimePos(3.5f*0.3f);
		g_sceneUI->ProgressBar2Start(3.5f*0.7f/manRatio);
		g_sceneUI->ProgressBar2UnPause();
		g_leftRight=1;
		g_deltaWaitRollBegin=false;
		if(!g_womanStart)
		{
			womanAniStart();
			g_sceneUI->countBegin();
			g_sceneUI->ProgressBar1Start(g_sceneUI->g_totalSeconds);
			g_sceneUI->hideHint();
		}
	}
	else if(g_leftRight==1)
	{
		if(g_manAI->getAnimationState()==2) line="#"+line;
		g_manAI->unpauseAnimation();
		g_sceneUI->ProgressBar2UnPause();
		g_sceneUI->hideHint();
	}
	else
	{
		line="@"+line;
		//error.不需要其它命令
	}
	g_output.writeLine(line);
}

void SceneChabei::manAniRight()
{
	std::string line="";
	line=misc.ConvertToString(totalTime)+":right";
	if(g_leftRight==0)
	{
		if(g_currentScore<manForceLeftRightVec.size()&&manForceLeftRightVec[g_currentScore]==1)
		{
			line="@"+line;
			g_output.writeLine(line);
			return;
		}
		g_manAI->runAnimation("right");
		g_manAI->setAnimationTimePos(3.5f*0.3f);
		g_sceneUI->ProgressBar2Start(3.5f*0.7f/manRatio);
		g_sceneUI->ProgressBar2UnPause();
		g_deltaWaitRollBegin=false;
		g_leftRight=2;
		if(!g_womanStart)
		{
			womanAniStart();
			g_sceneUI->countBegin();
			g_sceneUI->ProgressBar1Start(g_sceneUI->g_totalSeconds);
			g_sceneUI->hideHint();
		}
	}
	else if(g_leftRight==2)
	{
		if(g_manAI->getAnimationState()==2) line="#"+line;
		g_manAI->unpauseAnimation();
		g_sceneUI->ProgressBar2UnPause();
		g_sceneUI->hideHint();
	}
	else
	{
		line="@"+line;
		//error.不需要其它命令
	}
	g_output.writeLine(line);
}

void SceneChabei::womanAniLeft()
{
	g_womanAI->runAnimation("left",false);
	g_womanLeftRight=1;
}

void SceneChabei::womanAniRight()
{
	g_womanAI->runAnimation("right",false);
	g_womanLeftRight=2;
}

int SceneChabei::getLRCommandFromFile(std::string filePath)
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

void SceneChabei::loadInitFile(std::string filePath)
{
	timeVecLeft.clear();
	timeVecLeft.push_back(0.4);
	timeVecLeft.push_back(0.55);
	timeVecLeft.push_back(0.7);
	timeVecLeft.push_back(0.85);
	timeVecLeft.push_back(1);
	timeVecRight.clear();
	timeVecRight.push_back(0.4);
	timeVecRight.push_back(0.55);
	timeVecRight.push_back(0.7);
	timeVecRight.push_back(0.85);
	timeVecRight.push_back(1);
	womanRatioVec.push_back(0.25);
	if(!fileIO.openFile(filePath)) return;
	std::string str=fileIO.getNextLine();
	std::string temp;
	std::string::size_type st;
	while(str!="")
	{
		st=str.find_first_of(":");
		temp=str.substr(0,st);
		str=str.substr(st+1,str.length()-1);
		if(temp=="totalScore")
		{
			g_totalScore=misc.ConvertToInt(str);
			for(UINT k=0;k<g_totalScore;k++)
			{
				manForceLeftRightVec.push_back(0);
				womanForceLeftRightVec.push_back(0);
			}
			str=fileIO.getNextLine();
		}
		else if(temp=="character")
		{
			if(str=="man")
			{
				str=fileIO.getNextLine();
				while(str!="")
				{
					st=str.find_first_of(":");
					temp=str.substr(0,st);
					str=str.substr(st+1,str.length()-1);
					if(temp=="moveRatio")
					{
						manRatio=misc.ConvertToFloat(str);
					}
					else if(temp=="moveCellLeft")
					{
						st=str.find_first_of(',');
						timeVecLeft.clear();
						while(st!=str.npos)
						{
							temp=str.substr(0,st);
							str=str.substr(st+1,str.length()-1);
							timeVecLeft.push_back(misc.ConvertToFloat(temp));
							st=str.find_first_of(',');
						}
						timeVecLeft.push_back(misc.ConvertToFloat(str));
					}
					else if(temp=="moveCellRight")
					{
						st=str.find_first_of(',');
						timeVecRight.clear();
						while(st!=str.npos)
						{
							temp=str.substr(0,st);
							str=str.substr(st+1,str.length()-1);
							timeVecRight.push_back(misc.ConvertToFloat(temp));
							st=str.find_first_of(',');
						}
						timeVecRight.push_back(misc.ConvertToFloat(str));
					}
					else if(temp=="character")
					{
						str=temp+":"+str;
						break;
					}
					else{
						UINT ii=misc.ConvertToInt(temp)-1;
						if(ii<g_totalScore)
						{
							if(str=="l")
							{
								manForceLeftRightVec[ii]=1;
							}
							else if(str=="r")
							{
								manForceLeftRightVec[ii]=2;
							}
						}
					}
					str=fileIO.getNextLine();
				}
			}
			else if(str=="woman")
			{
				str=fileIO.getNextLine();
				while(str!="")
				{
					st=str.find_first_of(":");
					temp=str.substr(0,st);
					str=str.substr(st+1,str.length()-1);
					if(temp=="moveRatio")
					{
						womanRatioVec.clear();
						st=str.find_first_of(",");
						while(st!=str.npos)
						{
							temp=str.substr(0,st);
							str=str.substr(st+1,str.length()-1);
							womanRatioVec.push_back(misc.ConvertToFloat(temp));
							st=str.find_first_of(",");
						}
						womanRatioVec.push_back(misc.ConvertToFloat(str));
						womanRatio=womanRatioVec[0];
					}
					else if(temp=="character")
					{
						str=temp+":"+str;
						break;
					}
					else
					{
						UINT ii=misc.ConvertToInt(temp)-1;
						if(ii<g_totalScore)
						{
							if(str=="l")
							{
								womanForceLeftRightVec[ii]=1;
							}
							else if(str=="r")
							{
								womanForceLeftRightVec[ii]=2;
							}
						}
					}
					str=fileIO.getNextLine();
				}
			}
		}
		else
		{
			str=fileIO.getNextLine();
		}
	}
	fileIO.endInput();
}

void SceneChabei::saveCurrentState(std::string filePath)
{
	GAFileIO* file=new GAFileIO();
	if(!file->createNewFile(filePath)) return;
	std::string line="";
	line="totalScore:"+misc.ConvertToString(g_totalScore);
	file->writeLine(line);
	line="timeOut:"+misc.ConvertToString(10);
	file->writeLine(line);
	line="character:woman";
	file->writeLine(line);
	line="moveRatio:";
	for(UINT i=0;i<womanRatioVec.size();i++)
	{
		line+=misc.ConvertToString(womanRatioVec[i]);
		if(i!=womanRatioVec.size()-1) line+=",";
	}
	file->writeLine(line);

	for(UINT i=0;i<womanForceLeftRightVec.size();i++)
	{
		if(womanForceLeftRightVec[i]==1)
		{
			line=misc.ConvertToString(i+1)+":l";
			file->writeLine(line);
		}
		else if(womanForceLeftRightVec[i]==2)
		{
			line=misc.ConvertToString(i+1)+":r";
			file->writeLine(line);
		}
	}

	line="character:man";
	file->writeLine(line);
	line="moveRatio:"+misc.ConvertToString(manRatio);
	file->writeLine(line);
	line="moveCellLeft:";
	for(UINT i=0;i<timeVecLeft.size();i++)
	{
		line+=misc.ConvertToString(timeVecLeft[i]);
		if(i!=timeVecLeft.size()-1) line+=",";
	}
	file->writeLine(line);
	line="moveCellRight:";
	for(UINT i=0;i<timeVecRight.size();i++)
	{
		line+=misc.ConvertToString(timeVecRight[i]);
		if(i!=timeVecRight.size()-1) line+=",";
	}
	file->writeLine(line);
	for(UINT i=0;i<manForceLeftRightVec.size();i++)
	{
		if(manForceLeftRightVec[i]==1)
		{
			line=misc.ConvertToString(i+1)+":l";
			file->writeLine(line);
		}
		else if(manForceLeftRightVec[i]==2)
		{
			line=misc.ConvertToString(i+1)+":r";
			file->writeLine(line);
		}
	}
	file->endOutput();
}
void SceneChabei::oneTurnFinished()
{
	if(!g_isWomanFinished) g_currentScore++;
	currentState=0;
	g_manInAni=false;
	g_manAI->resetAnimation();
	g_leftRight=0;
	g_manAI->pauseAnimation();
	g_womanAI->resetAnimation();
	g_womanStart=false;
	g_mainMgr->getSceneNode("beizi2")->setVisible(true);
	g_mainMgr->getSceneNode("rhandCupw")->setVisible(false);
	g_mainMgr->getSceneNode("lhandCupw")->setVisible(false);
	//clear UI
	womanRatio=womanRatioVec[g_currentScore%womanRatioVec.size()];
	g_sceneUI->g_currentScore=g_currentScore;
	g_sceneUI->countRestart(((UINT)(3.5/womanRatio))+1);
	g_sceneUI->countPause();
	g_sceneUI->setProgressBarRatio(1);
	g_sceneUI->setProgressBar2Ratio(1);
	g_sceneUI->ProgressBar1Pause();
	g_sceneUI->ProgressBar2Pause();
	UINT t=manForceLeftRightVec[g_currentScore%manForceLeftRightVec.size()];
	//showHint(t);
	g_sceneUI->hintString="Congratulations, next command?";
	g_sceneUI->showHint();
	g_trialState=2;
	if(g_isWomanFinished)
	{
		/*if(t==1) g_sceneUI->hintString="use left hand to restart";
		else if(t==2) g_sceneUI->hintString="use right hand to restart";
		else g_sceneUI->hintString="use left or right hand to restart";*/
		g_sceneUI->hintString="Failed.please input command";
		if(g_soundEnable) g_mainMgr->g_audioMgr->playShortSound("../sounds/tryAgain.wav");
		g_sceneUI->showHint();
		g_trialState=0;
	}
	else if(g_soundEnable) g_mainMgr->g_audioMgr->playShortSound("../sounds/guzhang.wav");
	g_isWomanFinished=false;
	g_deltaWaitRoll->setTimeInterval(2);
	g_deltaWaitRollBegin=true;
	g_freezed=true;
	if(g_currentScore==g_totalScore) end();
}

void SceneChabei::womanAniStart()
{
	g_womanStart=true;
	g_isWomanFinished=false;
	if(g_currentScore<manForceLeftRightVec.size())
	{
		if(manForceLeftRightVec[g_currentScore%manForceLeftRightVec.size()]==2)
		{
			womanAniLeft();
			return;
		}
		else if(manForceLeftRightVec[g_currentScore%manForceLeftRightVec.size()]==1)
		{
			womanAniRight();
			return;
		}
	}
	srand(GetTickCount());
	if(rand()%2==0) womanAniLeft();
	else womanAniRight();
}

void SceneChabei::showHint(UINT type)
{
	if(type==1)
	{
		g_sceneUI->hintString="use left hand to start";
	}
	else if(type==2)
	{
		g_sceneUI->hintString="use right hand to start";
	}
	else
	{
		g_sceneUI->hintString="use left or right hand to start";
	}
	g_sceneUI->showHint();
}

void SceneChabei::end()
{
	g_sceneUI->hintString="Success!";
	g_sceneUI->showHint();
	g_freezed=true;
	g_toNewScene=true;
	g_sceneEnd=true;
	newSceneRoll->setTimeInterval(7);
}

void SceneChabei::toNewScene()
{
	/*g_sceneUI->g_enterScene=true;
	g_sceneUI->g_sceneName="yundong";*/
	g_mainMgr->g_dataMgr->getRootNode()->getChildLeafNode("recvString")->getDataMap()->addData("recv","0-switchGameByAni:2");
	g_mainMgr->g_sceneMgr->processServerData();
}

void SceneChabei::changeCup(UINT kind)
{
	std::string name="cocalogo";
	if(kind==1) name="yinliao1";
	else if(kind==2) name="yinliao2";
	else if(kind==3) name="yinliao3";
	else if(kind==4) name="yinliao4";
	g_mainMgr->getCurrentSceneMgr()->getEntity("beizi1")->getSubEntity(0)->setMaterialName(name);
	g_mainMgr->getCurrentSceneMgr()->getEntity("beizi2")->getSubEntity(0)->setMaterialName(name);
	g_mainMgr->getCurrentSceneMgr()->getEntity("beizi_R")->getSubEntity(0)->setMaterialName(name);
	g_mainMgr->getCurrentSceneMgr()->getEntity("beizi_L")->getSubEntity(0)->setMaterialName(name);
	g_mainMgr->getCurrentSceneMgr()->getEntity("beizi_RW")->getSubEntity(0)->setMaterialName(name);
	g_mainMgr->getCurrentSceneMgr()->getEntity("beizi_LW")->getSubEntity(0)->setMaterialName(name);
}

void SceneChabei::playStartSound(UINT leftright)
{
	if(!g_soundEnable) return; 
	if(leftright==1) g_mainMgr->g_audioMgr->playShortSound("../sounds/thinkLeft.wav");
	else if(leftright==2) g_mainMgr->g_audioMgr->playShortSound("../sounds/thinkRight.wav");
	else g_mainMgr->g_audioMgr->playShortSound("../sounds/fight.wav");
}

bool SceneChabei::processServerData(std::string recvString,GAServerSimple* server,UINT socketNum)
{
	bool processed=true;
	if(recvString=="left")
	{
		if(g_freezed) return true;
		manAniLeft();
	}
	else if(recvString=="right")
	{
		if(g_freezed) return true;
		manAniRight();
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
			if(g_soundType==1) g_mainMgr->g_audioMgr->playShortSound("../sounds/tryLeft.wav");
			else if(g_soundType==2) g_mainMgr->g_audioMgr->playShortSound("../sounds/tryRight.wav");
			else g_mainMgr->g_audioMgr->playShortSound("../sounds/fight.wav");
		}
	}
	else if(recvString=="start")
	{
		if(!g_started&&g_sceneEnd==false) start();
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
	else if(recvString=="getLeftRight")
	{
		if(g_leftRight==1) server->sendString("left",socketNum);
		else if(g_leftRight==2) server->sendString("right",socketNum);
		else if(g_leftRight==0) 
		{
			if(manForceLeftRightVec[g_currentScore]==2) server->sendString("right",socketNum);
			else if(manForceLeftRightVec[g_currentScore]==1) server->sendString("left",socketNum);
			else server->sendString("unknown",socketNum);
		}
	}
	else if(recvString=="getTime")
	{
		server->sendString(misc.ConvertToString(g_sceneRunningTime),socketNum);
	}
	else if(recvString=="getTrialState")
	{
		/*if(g_sceneEnd) server->sendString("Ended",socketNum);
		else if(g_freezed) server->sendString("Not Started",socketNum);
		else server->sendString("Started",socketNum);*/
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
		str+="man speed ratio:"+misc.ConvertToString(manRatio)+"\n";
		str+="competitor speed ratio:";
		for(UINT i=0;i<womanRatioVec.size()-1;i++) str+=misc.ConvertToString(womanRatioVec[i])+",";
		str+=misc.ConvertToString(womanRatioVec[womanRatioVec.size()-1])+"\n";
		str+="moveCell:0-";
		for(UINT i=0;i<timeVecLeft.size()-1;i++) str+=misc.ConvertToString(timeVecLeft[i])+","+misc.ConvertToString(timeVecLeft[i])+"-";
		str+=misc.ConvertToString(timeVecLeft[timeVecLeft.size()-1]);
		server->sendString(str,socketNum);
	}
	else if(recvString=="getScore")
	{
		server->sendString(misc.ConvertToString(g_sceneUI->g_currentScore),socketNum);
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
		saveCurrentState("../SceneChaBei/init.txt");
	}
	else if(recvString=="lockView")
	{
		g_mainMgr->g_cameraCtrl->lock();
	}
	else if(recvString=="unlockView")
	{
		g_mainMgr->g_cameraCtrl->unlock();
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
			if(str1=="setLeftRight")
			{
				st=str2.find_first_of(',');
				str1=str2.substr(0,st);
				str2=str2.substr(st+1);
				UINT k1=misc.ConvertToInt(str1)-1;
				UINT k2;
				if(str2=="l") k2=1;
				else if(str2=="r") k2=2;
				else k2=0;
				manForceLeftRightVec[k1%manForceLeftRightVec.size()]=k2;
				server->sendString("done",socketNum);
			}
			else if(str1=="setSelfSpeedRatio")
			{
				manRatio=misc.ConvertToFloat(str2);
				server->sendString("new speed ratio:"+misc.ConvertToString(manRatio),socketNum);
			}
			else if(str1=="setCompetitorSpeedRatio")
			{
				/*st=str2.find_first_of(',');
				str1=str2.substr(0,st);
				str2=str2.substr(st+1);*/
				//UINT k1=misc.ConvertToInt(str1)-1;
				float k2=misc.ConvertToFloat(str2);
				if(g_freezed&&!g_started)
				{
					womanRatioVec[g_currentScore%womanRatioVec.size()]=k2;
					womanRatio=k2;
					server->sendString("new competitor speed ratio:"+misc.ConvertToString(k2),socketNum);
				}
				else //if(k1>g_currentScore&&k1<g_totalScore)
				{
					womanRatioVec[g_currentScore%womanRatioVec.size()]=k2;
					womanRatio=k2;
					server->sendString("new competitor speed ratio:"+misc.ConvertToString(k2),socketNum);
				}
				//else server->sendString("couldn't change current trail's ratio value",socketNum);
			}
			else if(str1=="setCupPicture")
			{
				UINT type=misc.ConvertToInt(str2);
				changeCup(type);
				server->sendString("done",socketNum);
			}
			else if(str1=="setThisTrial")
			{
				UINT type=misc.ConvertToInt(str2);
				g_isWomanFinished=false;
				g_currentScore--;
				g_sceneUI->g_freezed=false;
				bool tempbool=g_soundEnable;
				g_soundEnable=false;
				oneTurnFinished();
				g_soundEnable=tempbool;
				g_freezed=false;
				if(type!=1&&type!=2) type=0;
				manForceLeftRightVec[g_currentScore%manForceLeftRightVec.size()]=type;
				showHint(type);
				g_sceneUI->welcome=false;
				playStartSound(type);
				g_soundType=type;
				server->sendString("done",socketNum);
			}
			else if(str1=="setNextTrial")
			{
				UINT type=misc.ConvertToInt(str2);
				if(type!=1&&type!=2) type=0;
				if(g_currentScore>=g_totalScore)
				{
					server->sendString("error input value",socketNum);
					return true;
				}
				manForceLeftRightVec[(g_currentScore+1)%manForceLeftRightVec.size()]=type;
				server->sendString("done",socketNum);
			}
			else if(str1=="setMoveCell")
			{
				st=str2.find_first_of(',');
				std::string str3=str2.substr(st+1);
				if(str3.find_first_of(',')==str3.npos)
				{
					server->sendString("input value could not be single",socketNum);
				}
				else
				{
					g_isWomanFinished=false;
					bool tempbool=g_soundEnable;
					g_soundEnable=false;
					oneTurnFinished();
					g_soundEnable=tempbool;
					timeVecLeft.clear();
					timeVecRight.clear();
					std::string str="new move cell:0-";
					while(st!=str2.npos)
					{
						temp=str2.substr(0,st);
						str2=str2.substr(st+1);
						timeVecLeft.push_back(misc.ConvertToFloat(temp));
						timeVecRight.push_back(misc.ConvertToFloat(temp));
						st=str2.find_first_of(',');
						str+=temp+","+temp+"-";
					}
					timeVecLeft.push_back(misc.ConvertToFloat(str2));
					timeVecRight.push_back(misc.ConvertToFloat(str2));
					str+=str2;
					server->sendString(str,socketNum);
				}
				
			}
			else if(str1=="setTotalTrialNum")
			{
				UINT num=misc.ConvertToInt(str2);
				if(num>g_currentScore)
				{
					g_totalScore=num; 
					g_sceneUI->g_totalScore=g_totalScore;
					server->sendString("new total trail:"+misc.ConvertToString(num),socketNum);
				}
				else server->sendString("error input value",socketNum);
			}
			else processed=false;
		}
		else processed=false;
	}
	return processed;
}