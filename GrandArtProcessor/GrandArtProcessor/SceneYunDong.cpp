#include "SceneYunDong.h"
#include "GAPCameraController.h"
#include "GAPWorldManager.h"
#include "GAOgreUIManager.h"
#include "GASceneManager.h"
#include "GAPAudioManager.h"

SceneYunDong::SceneYunDong(GAPMainManager* mainMgr,std::string sceneName) : GASceneBase(mainMgr,sceneName)
{
	currentCell=0;
	newSceneRoll=new GARollPoint(7);
	g_timeOut=30;
	g_timeOutRoll=new GARollPoint(30);
	progressBar1Ratio=1;
	timeRunned=0;
	g_soundEnable=true;
	g_soundType=0;
}

bool SceneYunDong::createScene()
{
	Ogre::SceneManager* sm=Ogre::Root::getSingleton().createSceneManager("BspSceneManager","yundong");
	Ogre::Camera* cam=sm->createCamera("yundong");
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

	Ogre::Entity *yalingR=sm->createEntity("yalingR","yaling.mesh");
	Ogre::Entity* yalingL=sm->createEntity("yalingL","yaling.mesh");
	g_mainMgr->g_worldMgr->addModel("man","man12.mesh");
	GACharacterBase *c1=new GACharacterBase(sm,"man");
	g_manAI=new GACharacterAIBase(c1);
	//g_manAI->runAnimation("right");
	Ogre::Skeleton* ske1=sm->getEntity("man")->getSkeleton();
	boneR=ske1->getBone("Bip01 R Hand");
	boneL=ske1->getBone("Bip01 L Hand");
	Ogre::SceneNode* boneRNode=sm->getRootSceneNode()->createChildSceneNode("boneRNode");
	yalingRNode=boneRNode->createChildSceneNode("yalingRNode");
	ballRNode=boneRNode->createChildSceneNode("ballRNode");
	yalingRNode->attachObject(yalingR);
	yalingRNode->pitch(Ogre::Degree(90));
	Ogre::SceneNode* boneLNode=sm->getRootSceneNode()->createChildSceneNode("boneLNode");
	yalingLNode=boneLNode->createChildSceneNode("yalingLNode");
	ballLNode=boneLNode->createChildSceneNode("ballLNode");
	yalingLNode->attachObject(yalingL);
	yalingLNode->pitch(Ogre::Degree(90));
	
	moveName="yalingRNode";
	yalingRNode->setPosition(9.507,0.743,-2.795);
	yalingLNode->setPosition(7.97,-1.75,-2.82);
	yalingLNode->setVisible(false);
	yalingRNode->setVisible(false);

	Ogre::SceneNode* roundNode=g_mainMgr->getSceneNode("man")->createChildSceneNode("roundNode");
	roundNode->translate(0,150,0);
	g_mainMgr->g_cameraCtrl->setCameraStyle(GAPCameraController::CameraStyle_AroundNode,roundNode);
	g_mainMgr->g_cameraCtrl->g_cameraGoal->translate(0,0,30);

	Ogre::Entity* entBallL=g_mainMgr->getCurrentSceneMgr()->createEntity("pinghengqiu.mesh");
	Ogre::Entity* entBallR=g_mainMgr->getCurrentSceneMgr()->createEntity("pinghengqiu.mesh");
	ballLNode->attachObject(entBallL);
	ballLNode->scale(1.5,1.5,1.5);
	ballLNode->setPosition(7.09,1.21,-4.77);
	ballRNode->attachObject(entBallR);
	ballRNode->setPosition(7.34,-0.15,-5.9);
	ballRNode->scale(1.5,1.5,1.5);
	g_sceneUI=(GAExampleSceneUI*)g_mainMgr->g_UIMgr->findUI("SceneUI");
	reCreateScene();
	return true;
}
bool SceneYunDong::reCreateScene()
{
	g_leftRight=0;
	g_moveType=0;
	g_manAI->destroyAnimation();
	loadInitFile("../SceneYunDong/init.txt");
	currentScore=0;
	g_runningAni=false;
	g_toNewScene=false;
	g_sceneUI->g_totalScore=totalScore;
	g_sceneUI->g_totalSeconds=timeLimit;
	//g_sceneUI->hintString=L"请按 F1 键开始游戏";
	g_sceneUI->hideHint();
	g_freezed=true;
	g_sceneEnd=false;
	g_sceneUI->countRestart(timeLimit);
	g_sceneUI->countPause();
	g_sceneUI->progressBar2Visible(false);
	g_sceneUI->progressBar1Visible(true);
	g_sceneUI->setProgressBarRatio(1);
	g_sceneUI->ProgressBar1Pause();
	g_mainMgr->g_cameraCtrl->lock();
	g_oneTurnFinished=true;
	g_sceneUI->g_freezed=false;
	g_timeOutRoll->setTimeInterval(g_timeOut);
	g_trialState=0;
	if(g_sceneUI->welcome)
	{
		g_sceneUI->hintString=L"欢迎使用交大虚拟康复训练平台";
		g_sceneUI->showHint();
	}
	return true;
}

bool SceneYunDong::keyReleased(const OIS::KeyEvent& evt)
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
	else if(evt.key==OIS::KC_1)
	{
		ballShape(1);
	}
	else if(evt.key==OIS::KC_2)
	{
		ballShape(0);
	}
	if(g_freezed) return false;
	moveName="ballLNode";
	float transDelta=0.1;
	Ogre::SceneNode* node=g_mainMgr->getSceneNode(moveName);
	
	if(evt.key==OIS::KC_L||evt.key==OIS::KC_LEFT)
	{
		determinAnimation(2);
	}
	else if(evt.key==OIS::KC_R||evt.key==OIS::KC_RIGHT)
	{
		determinAnimation(1);
	}
	return true;
}

void SceneYunDong::checkAnimation()
{
	float tempTime=0;
	switch(g_moveType)
	{
	case 1:
		if(g_manAI->getAnimationTimePos()>2)
		{
			g_manAI->pauseAnimation();
			aniFinished();
		}
		else if(g_manAI->getAnimationTimePos()>timeCellVec[currentCell]*2)
		{
			currentCell++;
			g_manAI->pauseAnimation();
		}
		break;
	case 2:
		if(g_manAI->getAnimationTimePos()>4)
		{
			g_manAI->pauseAnimation();
			aniFinished();
		}
		else if(g_manAI->getAnimationTimePos()>timeCellVec[currentCell]*2+2)
		{
			currentCell++;
			g_manAI->pauseAnimation();
		}
		break;
	case 3:
		if(g_manAI->getAnimationTimePos()>5.9||g_manAI->getAnimationTimePos()<4)
		{
			g_manAI->pauseAnimation();
			aniFinished();
		}
		else if(g_manAI->getAnimationTimePos()>timeCellVec[currentCell]*1.9+4)
		{
			currentCell++;
			g_manAI->pauseAnimation();
		}
		break;
	case 4:
		tempTime=g_manAI->getAnimationTimePos();
		if(tempTime>1)
		{
			if(tempTime<1.5) ballShape(1-(1.5-tempTime)*2);
			else ballShape(1);
		}
		else ballShape(0);
		if(g_manAI->getAnimationTimePos()>1.5)
		{
			g_manAI->pauseAnimation();
			aniFinished();
		}
		else if(g_manAI->getAnimationTimePos()>timeCellVec[currentCell]*1.1+0.4)
		{
			currentCell++;
			g_manAI->pauseAnimation();
		}
		break;
	case 5:
		tempTime=g_manAI->getAnimationTimePos();
		if(tempTime>=2.4&&tempTime<2.9)
		{
			ballShape((2.9-tempTime)*2);
		}
		else ballShape(0);
		if(g_manAI->getAnimationTimePos()>3.5||g_manAI->getAnimationTimePos()<2)
		{
			g_manAI->pauseAnimation();
			aniFinished();
		}
		else if(g_manAI->getAnimationTimePos()>timeCellVec[currentCell]*1.1+2.4)
		{
			currentCell++;
			g_manAI->pauseAnimation();
		}
		break;
	case 6:
		if(g_manAI->getAnimationTimePos()>2)
		{
			g_manAI->pauseAnimation();
			aniFinished();
		}
		else if(g_manAI->getAnimationTimePos()>timeCellVec[currentCell]*2)
		{
			currentCell++;
			g_manAI->pauseAnimation();
		}
		break;
	case 7:
		if(g_manAI->getAnimationTimePos()>4)
		{
			g_manAI->pauseAnimation();
			aniFinished();
		}
		else if(g_manAI->getAnimationTimePos()>timeCellVec[currentCell]*2+2)
		{
			currentCell++;
			g_manAI->pauseAnimation();
		}
		break;
	case 8:
		if(g_manAI->getAnimationTimePos()>5.9||g_manAI->getAnimationTimePos()<4)
		{
			g_manAI->pauseAnimation();
			aniFinished();
		}
		else if(g_manAI->getAnimationTimePos()>timeCellVec[currentCell]*1.9+4)
		{
			currentCell++;
			g_manAI->pauseAnimation();
		}
		break;
	case 9:
		tempTime=g_manAI->getAnimationTimePos();
		if(tempTime>=1)
		{
			if(tempTime<1.5) ballShape(1-(1.5-tempTime)*2);
			else ballShape(1);
		}
		else ballShape(0);
		if(g_manAI->getAnimationTimePos()>1.5)
		{
			g_manAI->pauseAnimation();
			aniFinished();
		}
		else if(g_manAI->getAnimationTimePos()>timeCellVec[currentCell]*1.1+0.4)
		{
			currentCell++;
			g_manAI->pauseAnimation();
		}
		break;
	case 10:
		tempTime=g_manAI->getAnimationTimePos();
		if(tempTime>=2.4&&tempTime<2.9)
		{
			ballShape((2.9-tempTime)*2);
		}
		else ballShape(0);
		if(g_manAI->getAnimationTimePos()>3.5||g_manAI->getAnimationTimePos()<2)
		{
			g_manAI->pauseAnimation();
			aniFinished();
		}
		else if(g_manAI->getAnimationTimePos()>timeCellVec[currentCell]*1.1+2.4)
		{
			currentCell++;
			g_manAI->pauseAnimation();
		}
		break;
	};
}

bool SceneYunDong::frameStarted(float deltaTime)
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
				std::string str="into scene Balance in "+misc.ConvertToString(time)+" seconds";
				g_sceneUI->hintString=str;
				g_sceneUI->showHint();
			}
		}
	}
	if(g_freezed) return false;
	g_trialState=1;
	if(g_timeOutRoll->getRollPoint2(deltaTime,1)) end(false);
	if(g_sceneUI->isCountOver()) end(false);
	checkAnimation();
	g_manAI->update(deltaTime*0.25*move_ratio);
	Ogre::Matrix4 mat;
	Ogre::SceneNode* node;
	if(g_leftRight==2)
	{
		mat=boneR->_getFullTransform();
		//yalingLNode->setVisible(false);
		//ballLNode->setVisible(false);
		node=g_mainMgr->getSceneNode("boneRNode");
		if(g_moveType==4||g_moveType==5)
		{
			//ballRNode->setVisible(true);
			//yalingRNode->setVisible(false);
			moveName="ballRNode";
		}
		else
		{
			//ballRNode->setVisible(false);
			//yalingRNode->setVisible(true);
			moveName="yalingRNode";
		}
	}
	else if(g_leftRight==1)
	{
		mat=boneL->_getFullTransform();
		//yalingRNode->setVisible(false);
		//ballRNode->setVisible(false);
		node=g_mainMgr->getSceneNode("boneLNode");
		if(g_moveType==9||g_moveType==10)
		{
			//ballLNode->setVisible(true);
			//yalingLNode->setVisible(false);
			moveName="ballLNode";
		}
		else
		{
			//ballLNode->setVisible(false);
			//yalingLNode->setVisible(true);
			moveName="yalingLNode";
		}
	}
	else return false;
	Ogre::Matrix4 mat2=Ogre::Matrix4(1,0,0,0,0,0,-1,0,0,1,0,0,0,0,0,1);
	mat2=mat2.concatenate(mat);
	Ogre::Vector3 vec2=mat2.getTrans()+Ogre::Vector3(0,0,0);
	node->setPosition(vec2);
	node->setOrientation(mat2.extractQuaternion());
	return true;
}

bool SceneYunDong::frameEnded(float deltaTime)
{
	if(g_leftRight==2)
	{
		yalingLNode->setVisible(false);
		ballLNode->setVisible(false);
		if(g_moveType==4||g_moveType==5)
		{
			ballRNode->setVisible(true);
			yalingRNode->setVisible(false);
		}
		else
		{
			ballRNode->setVisible(false);
			yalingRNode->setVisible(true);
		}
	}
	else if(g_leftRight==1)
	{
		yalingRNode->setVisible(false);
		ballRNode->setVisible(false);
		if(g_moveType==9||g_moveType==10)
		{
			ballLNode->setVisible(true);
			yalingLNode->setVisible(false);
		}
		else
		{
			ballLNode->setVisible(false);
			yalingLNode->setVisible(true);
		}
	}
	if(g_oneTurnFinished)
	{
		ballRNode->setVisible(false);
		yalingRNode->setVisible(false);
		ballLNode->setVisible(false);
		yalingLNode->setVisible(false);
	}
	return GASceneBase::frameEnded(deltaTime);
}

bool SceneYunDong::freezeScene()
{
	progressBar1Ratio=g_sceneUI->getProgressBarRatio();
	timeRunned=g_sceneUI->g_currentTimeSeconds;
	return GASceneBase::freezeScene();
}
bool SceneYunDong::reloadScene()
{
	GASceneBase::reloadScene();
	if(!g_started) g_freezed=true; 
	g_mainMgr->g_UIMgr->switchToUI("SceneUI");
	g_sceneUI->g_totalScore=totalScore;
	g_sceneUI->g_currentScore=currentScore;
	g_sceneUI->progressBar2Visible(false);
	g_sceneUI->g_currentTimeSeconds=timeRunned;
	g_sceneUI->g_totalSeconds=timeLimit;
	g_sceneUI->progressBar1Visible(true);
	g_sceneUI->ProgressBar1Start(timeLimit);
	g_sceneUI->setProgressBarRatio(progressBar1Ratio);
	g_mainMgr->g_cameraCtrl->lock();
	if(g_freezed)
	{
		reCreateScene();
	}
	else
	{
		if(g_runningAni==false)
		{
			UINT type=currentScore%typeVec.size();
			showHint(type);
			g_sceneUI->countPause();
			g_sceneUI->ProgressBar1Pause();
		}
		else
		{
			g_sceneUI->hideHint();
			g_sceneUI->countBegin();
			g_sceneUI->ProgressBar1UnPause();
		}
	}
	return true;
}

void SceneYunDong::loadInitFile(std::string filePath)
{
	typeVec.clear();
	typeVec.push_back(1);typeVec.push_back(2);typeVec.push_back(3);typeVec.push_back(4);typeVec.push_back(5);
	timeCellVec.clear();
	timeCellVec.push_back(0.4);timeCellVec.push_back(0.7);timeCellVec.push_back(1);
	totalScore=5;
	timeLimit=999;
	g_sceneUI->countRestart(timeLimit);
	g_sceneUI->countPause();
	move_ratio=1;
	if(!fileIO.openFile(filePath)) return;
	std::string str=fileIO.getNextLine();
	std::string temp;
	std::string::size_type st;
	while(str!="")
	{
		st=str.find_first_of(":");
		temp=str.substr(0,st);
		str=str.substr(st+1,str.length()-1);
		if(temp=="moveRatio")
		{
			move_ratio=misc.ConvertToFloat(str);
		}
		else if(temp=="totalScore")
		{
			totalScore=misc.ConvertToInt(str);
		}
		else if(temp=="timeOut")
		{
			g_timeOut=misc.ConvertToFloat(str);
			g_timeOutRoll->setTimeInterval(g_timeOut);
		}
		else if(temp=="timeLimit")
		{
			timeLimit=misc.ConvertToInt(str);
			g_sceneUI->countRestart(timeLimit);
			g_sceneUI->countPause();
		}
		else if(temp=="moveCell")
		{
			st=str.find_first_of(",");
			timeCellVec.clear();
			while(st!=str.npos)
			{
				temp=str.substr(0,st);
				str=str.substr(st+1,str.length()-1);
				timeCellVec.push_back(misc.ConvertToFloat(temp));
				st=str.find_first_of(",");
			}
			timeCellVec.push_back(misc.ConvertToFloat(str));
		}
		else if(temp=="sceneOrder")
		{
			st=str.find_first_of(",");
			typeVec.clear();
			while(st!=str.npos)
			{
				temp=str.substr(0,st);
				str=str.substr(st+1,str.length()-1);
				typeVec.push_back(translateType(temp));
				st=str.find_first_of(",");
			}
			typeVec.push_back(translateType(str));
		}
		str=fileIO.getNextLine();
	}
	fileIO.endInput();
}

void SceneYunDong::saveCurrentState(std::string filePath)
{
	GAFileIO* file=new GAFileIO();
	if(!file->createNewFile(filePath)) return;
	std::string line="";
	line="moveRatio:"+misc.ConvertToString(move_ratio);
	file->writeLine(line);
	line="totalScore:"+misc.ConvertToString(totalScore);
	file->writeLine(line);
	line="timeLimit:"+misc.ConvertToString(timeLimit);
	file->writeLine(line);
	line="timeOut:"+misc.ConvertToString(30);
	file->writeLine(line);
	line="moveCell:";
	for(UINT i=0;i<timeCellVec.size();i++)
	{
		line+=misc.ConvertToString(timeCellVec[i]);
		if(i!=timeCellVec.size()-1) line+=",";
	}
	file->writeLine(line);
	line="sceneOrder:";
	for(UINT i=0;i<typeVec.size();i++)
	{
		line+=translateString(typeVec[i]);
		if(i!=typeVec.size()-1) line+=",";
	}
	file->writeLine(line);
	file->endOutput();
}

UINT SceneYunDong::translateType(std::string typeName)
{
	if(typeName=="front") return 1;
	else if(typeName=="side") return 2;
	else if(typeName=="up") return 3;
	else if(typeName=="grub") return 4;
	else if(typeName=="flat") return 5;
	else if(typeName=="right_front") return 11;
	else if(typeName=="right_side") return 12;
	else if(typeName=="right_up") return 13;
	else if(typeName=="right_grub") return 14;
	else if(typeName=="right_flat") return 15;
	else if(typeName=="left_front") return 21;
	else if(typeName=="left_side") return 22;
	else if(typeName=="left_up") return 23;
	else if(typeName=="left_grub") return 24;
	else if(typeName=="left_flat") return 25;
	else return 1;
}

std::string SceneYunDong::translateString(UINT type)
{
	switch(type)
	{
	case 1:
		return "front";
	case 2:
		return "side";
	case 3:
		return "up";
	case 4:
		return "grub";
	case 5:
		return "flat";
	case 11:
		return "right_front";
	case 12:
		return "right_side";
	case 13:
		return "right_up";
	case 14:
		return "right_grub";
	case 15:
		return "right_flat";
	case 21:
		return "left_front";
	case 22:
		return "left_side";
	case 23:
		return "left_up";
	case 24:
		return "left_grub";
	case 25:
		return "left_flat";
	default:
		return "front";
	}
}

void SceneYunDong::determinAnimation(UINT leftright)
{
	UINT ii=currentScore%typeVec.size();
	g_timeOutRoll->setTimeInterval(g_timeOut);
	g_oneTurnFinished=false;
	if(leftright==1)
	{
		switch(typeVec[ii])
		{
		case 1:
		case 11:
			aniLeftFront();
			break;
		case 2:
		case 12:
			aniLeftSide();
			break;
		case 3:
		case 13:
			aniLeftUp();
			break;
		case 4:
		case 14:
			aniLeftGrub();
			break;
		case 5:
		case 15:
			aniLeftFlat();
			break;
		}
	}
	else if(leftright==2)
	{
		switch(typeVec[ii])
		{
		case 1:
		case 21:
			aniRightFront();
			break;
		case 2:
		case 22:
			aniRightSide();
			break;
		case 3:
		case 23:
			aniRightUp();
			break;
		case 4:
		case 24:
			aniRightGrub();
			break;
		case 5:
		case 25:
			aniRightFlat();
			break;
		}
	}
}


void SceneYunDong::aniRightFront()
{
	if(g_currentType==21) g_manAI->unpauseAnimation();
	if(g_runningAni==true) return;
	g_currentType=21;
	g_manAI->runAnimation("right_move",false);
	g_leftRight=2;
	g_moveType=1;
	g_manAI->setAnimationTimePos(0);
}
void SceneYunDong::aniRightSide()
{
	if(g_currentType==22) g_manAI->unpauseAnimation();
	if(g_runningAni==true) return;
	g_currentType=22;
	g_manAI->runAnimation("right_move",false);
	g_leftRight=2;
	g_moveType=2;
	g_manAI->setAnimationTimePos(2);
}
void SceneYunDong::aniRightUp()
{
	if(g_currentType==23) g_manAI->unpauseAnimation();
	if(g_runningAni==true) return;
	g_currentType=23;
	g_manAI->runAnimation("right_move",false);
	g_leftRight=2;
	g_moveType=3;
	g_manAI->setAnimationTimePos(4);
}
void SceneYunDong::aniRightGrub()
{
	if(g_currentType==24) g_manAI->unpauseAnimation();
	if(g_runningAni==true) return;
	g_currentType=24;
	g_manAI->runAnimation("right_grub",false);
	g_leftRight=2;
	g_moveType=4;
	g_manAI->setAnimationTimePos(0.4);
}
void SceneYunDong::aniRightFlat()
{
	if(g_currentType==25) g_manAI->unpauseAnimation();
	if(g_runningAni==true) return;
	g_currentType=25;
	g_manAI->runAnimation("right_grub",false);
	g_leftRight=2;
	g_moveType=5;
	g_manAI->setAnimationTimePos(2.4);
}
void SceneYunDong::aniLeftFront()
{
	if(g_currentType==11) g_manAI->unpauseAnimation();
	if(g_runningAni==true) return;
	g_currentType=11;
	g_manAI->runAnimation("left_move",false);
	g_leftRight=1;
	g_moveType=6;
	g_manAI->setAnimationTimePos(0);
}
void SceneYunDong::aniLeftSide()
{
	if(g_currentType==12) g_manAI->unpauseAnimation();
	if(g_runningAni==true) return;
	g_currentType=12;
	g_manAI->runAnimation("left_move",false);
	g_leftRight=1;
	g_moveType=7;
	g_manAI->setAnimationTimePos(2);
}
void SceneYunDong::aniLeftUp()
{
	if(g_currentType==13) g_manAI->unpauseAnimation();
	if(g_runningAni==true) return;
	g_currentType=13;
	g_manAI->runAnimation("left_move",false);
	g_leftRight=1;
	g_moveType=8;
	g_manAI->setAnimationTimePos(4);
}
void SceneYunDong::aniLeftGrub()
{
	if(g_currentType==14) g_manAI->unpauseAnimation();
	if(g_runningAni==true) return;
	g_currentType=14;
	g_manAI->runAnimation("left_grub",false);
	g_leftRight=1;
	g_moveType=9;
	g_manAI->setAnimationTimePos(0.4);
}
void SceneYunDong::aniLeftFlat()
{
	if(g_currentType==15) g_manAI->unpauseAnimation();
	if(g_runningAni==true) return;
	g_currentType=15;
	g_manAI->runAnimation("left_grub",false);
	g_leftRight=1;
	g_moveType=10;
	g_manAI->setAnimationTimePos(2.4);
}

void SceneYunDong::aniRunning()
{
	g_runningAni=true;
	g_sceneUI->ProgressBar1Start(timeLimit);
	g_sceneUI->countRestart(timeLimit);
	g_sceneUI->hideHint();
	currentCell=0;
}

void SceneYunDong::aniFinished()
{
	if(g_runningAni==false) return;
	g_sceneUI->ProgressBar1Pause();
	g_sceneUI->countPause();
	g_runningAni=false;
	currentScore++;
	g_moveType=(UINT)-1;
	g_manAI->destroyAnimation();
	g_oneTurnFinished=true;
	g_sceneUI->g_currentScore=currentScore;
	if(currentScore<totalScore)
	{
		g_trialState=2;
		g_sceneUI->hintString="please input command";
		g_freezed=true;
		if(g_soundEnable) g_mainMgr->g_audioMgr->playShortSound("../sounds/guzhang.wav");
	}
	else end(true);
}

void SceneYunDong::showHint(UINT type)
{
	g_sceneUI->showHint();
	switch(typeVec[type])
	{
	case 1:
		g_sceneUI->hintString="use left or right hand to move front";
		break;
	case 2:
		g_sceneUI->hintString="use left or right hand to move side";
		break;
	case 3:
		g_sceneUI->hintString="use left or right hand to move up";
		break;
	case 4:
		g_sceneUI->hintString="use left or right hand to grub";
		break;
	case 5:
		g_sceneUI->hintString="use left or right hand to flat";
		break;
	case 11:
		g_sceneUI->hintString="use right hand to move front";
		break;
	case 12:
		g_sceneUI->hintString="use right hand to move side";
		break;
	case 13:
		g_sceneUI->hintString="use right hand to move up";
		break;
	case 14:
		g_sceneUI->hintString="use right hand to grub";
		break;
	case 15:
		g_sceneUI->hintString="use right hand to flat";
		break;
	case 21:
		g_sceneUI->hintString="use left hand to move front";
		break;
	case 22:
		g_sceneUI->hintString="use left hand to move side";
		break;
	case 23:
		g_sceneUI->hintString="use left hand to move up";
		break;
	case 24:
		g_sceneUI->hintString="use left hand to grub";
		break;
	case 25:
		g_sceneUI->hintString="use left hand to flat";
		break;
	default:
		g_sceneUI->hideHint();
	}
}

void SceneYunDong::start()
{
	g_freezed=false;
	g_sceneUI->g_freezed=false;
	g_started=true;
	showHint(0);
	g_sceneUI->welcome=false;
}
void SceneYunDong::end(bool finished)
{
	g_freezed=true;
	g_sceneEnd=true;
	if(!finished) g_trialState=0;
	else g_trialState=2;
	if(finished) g_sceneUI->hintString="Success!";
	else
	{
		if(g_timeOutRoll->getRollPoint2(0,1)) g_sceneUI->hintString="no response for long time";
		else g_sceneUI->hintString="Failed...";
		if(g_soundEnable) g_mainMgr->g_audioMgr->playShortSound("../sounds/tryAgain.wav");
	}
	g_sceneUI->showHint();
	g_toNewScene=false;
}

void SceneYunDong::toNewScene()
{
	g_mainMgr->g_dataMgr->getRootNode()->getChildLeafNode("recvString")->getDataMap()->addData("recv","0-switchGameByAni:3");
	g_mainMgr->g_sceneMgr->processServerData();
}

void SceneYunDong::ballShape(float ratio)
{
	ballLNode->setScale(1.5*(1-0.2*ratio),1.5*(1+0.2*ratio),1.5*(1+0.2*ratio));
	ballRNode->setScale(1.5*(1-0.2*ratio),1.5*(1+0.2*ratio),1.5*(1+0.2*ratio));
}

void SceneYunDong::playStartSound(UINT leftright)
{
	if(!g_soundEnable) return; 
	if(leftright==1) g_mainMgr->g_audioMgr->playShortSound("../sounds/thinkLeft.wav");
	else if(leftright==2) g_mainMgr->g_audioMgr->playShortSound("../sounds/thinkRight.wav");
	else g_mainMgr->g_audioMgr->playShortSound("../sounds/fight.wav");
}

bool SceneYunDong::processServerData(std::string recvString,GAServerSimple* server,UINT socketNum)
{
	bool processed=true;
	if(recvString=="left")
	{
		if(g_freezed) return true;
		determinAnimation(2);
	}
	else if(recvString=="right")
	{
		if(g_freezed) return true;
		determinAnimation(1);
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
	else if(recvString=="getTime")
	{
		server->sendString(misc.ConvertToString(g_sceneRunningTime),socketNum);
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
		str+="time limit:"+misc.ConvertToString(timeLimit)+";runned:"+misc.ConvertToString(g_sceneUI->g_currentTimeSeconds)+"\n";
		str+="time out:"+misc.ConvertToString(g_timeOut)+";runned:"+misc.ConvertToString(g_timeOutRoll->getCurrentTimeCount())+"\n";
		str+="move ratio:"+misc.ConvertToString(move_ratio);
		str+="move cell:0-";
		for(UINT i=0;i<timeCellVec.size()-1;i++) str+=misc.ConvertToString(timeCellVec[i])+","+misc.ConvertToString(timeCellVec[i])+"-";
		str+=misc.ConvertToString(timeCellVec[timeCellVec.size()-1]);
		server->sendString(str,socketNum);
	}
	else if(recvString=="getScore")
	{
		server->sendString(misc.ConvertToString(g_sceneUI->g_currentScore),socketNum);
	}
	else if(recvString=="getLeftRight")
	{
		if(g_leftRight==1) server->sendString("right",socketNum);
		else if(g_leftRight==2) server->sendString("left",socketNum);
		else server->sendString("unknown",socketNum);
	}
	else if(recvString=="autoPlaySound")
	{
		if(g_soundEnable)
		{
			if(g_leftRight==2) g_mainMgr->g_audioMgr->playShortSound("../sounds/tryLeft.wav");
			else if(g_leftRight==1) g_mainMgr->g_audioMgr->playShortSound("../sounds/tryRight.wav");
			else g_mainMgr->g_audioMgr->playShortSound("../sounds/fight.wav");
		}
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
		saveCurrentState("../SceneYunDong/init.txt");
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
			if(str1=="setTimeLimit")
			{
				if(g_freezed&&!g_started)
				{
					timeLimit=misc.ConvertToFloat(str2);
					g_sceneUI->countRestart(timeLimit);
					g_sceneUI->countPause();
					g_sceneUI->ProgressBar1Start(timeLimit);
					g_sceneUI->ProgressBar1Pause();
					server->sendString("new time limit:"+misc.ConvertToString(timeLimit),socketNum);
				}
				else server->sendString("unable to set this value while processing!",socketNum);
			}
			else if(str1=="setMoveRatio")
			{
				move_ratio=misc.ConvertToFloat(str2);
				server->sendString("new move ratio:"+misc.ConvertToString(move_ratio),socketNum);
			}
			else if(str1=="setMoveCell")
			{
				st=str2.find_first_of(",");
				timeCellVec.clear();
				while(st!=str2.npos)
				{
					temp=str2.substr(0,st);
					str2=str2.substr(st+1,str2.length()-1);
					timeCellVec.push_back(misc.ConvertToFloat(temp));
					st=str2.find_first_of(",");
				}
				timeCellVec.push_back(misc.ConvertToFloat(str2));
				server->sendString("done",socketNum);
			}
			else if(str1=="setNextTrial")
			{
				UINT type=translateType(str2);
				if(currentScore<totalScore) typeVec[currentScore+1]=type;
				server->sendString("done",socketNum);
			}
			else if(str1=="setThisTrial")
			{
				if(g_freezed&&!g_started) start();
				UINT type=translateType(str2);
				typeVec[currentScore]=type;
				g_runningAni=true;
				currentScore--;
				bool tempbool=g_soundEnable;
				g_soundEnable=false;
				aniFinished();
				g_soundEnable=tempbool;
				g_freezed=false;
				std::string::size_type sst=str2.find_first_of('_');
				if(sst!=str2.npos)
				{
					std::string str3=str2.substr(0,sst);
					if(str3=="left")
					{
						determinAnimation(2);
						playStartSound(1);
						g_soundType=1;
					}
					else if(str3=="right")
					{
						determinAnimation(1);
						playStartSound(2);
						g_soundType=2;
					}
					g_manAI->pauseAnimation();
				}
				aniRunning();
				g_sceneUI->welcome=false;
				server->sendString("done",socketNum);
			}
			else if(str1=="setTotalTrialNum")
			{
				UINT num=misc.ConvertToInt(str2);
				if(num>currentScore)
				{
					totalScore=num; 
					g_sceneUI->g_totalScore=totalScore;
					server->sendString("total trial num:"+misc.ConvertToString(num),socketNum);
				}
				else server->sendString("error input value");
			}
			else if(str1=="setTimeOut")
			{
				g_timeOut=misc.ConvertToFloat(str2);
				g_timeOutRoll->setTimeInterval(g_timeOut);
				server->sendString("new time out:"+misc.ConvertToString(g_timeOut),socketNum);
			}
			else processed=false;
		}
		else processed=false;
	}
	return processed;
}