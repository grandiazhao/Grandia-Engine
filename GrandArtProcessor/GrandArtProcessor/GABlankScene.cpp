#include "GABlankScene.h"
#include "GAOgreUIManager.h"

GABlankScene::GABlankScene(GAPMainManager* mainMgr,std::string sceneName):GASceneBase(mainMgr,sceneName)
{

}

bool GABlankScene::createScene()
{
	GASceneBase::createScene();
	g_mainMgr->g_UIMgr->switchToUI("EnterUI");
	return true;
}