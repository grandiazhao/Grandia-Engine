#ifndef GABLANKSCENE_H
#define GABLANKSCENE_H

#include "Common.h"
#include "GASceneBase.h"

class GABlankScene : public GASceneBase
{
public:
	GABlankScene(GAPMainManager* mainMgr,std::string sceneName);
	bool createScene();
};

#endif