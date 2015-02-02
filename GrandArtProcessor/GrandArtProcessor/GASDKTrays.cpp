#define OIS_DYNAMIC_LIB

#include "Common.h"
#include "OIS.h"
#include "SdkTrays.h"

void OgreBites::SdkTrayManager::refreshCursor()
{
#if OGRE_NO_VIEWPORT_ORIENTATIONMODE == 0
    // TODO:
    // the position should be based on the orientation, for now simply return
    return;
#endif
#if OGRE_PLATFORM == OGRE_PLATFORM_IPHONE
    std::vector<OIS::MultiTouchState> states = mMouse->getMultiTouchStates();
    if(states.size() > 0)
        mCursor->setPosition(states[0].X.abs, states[0].Y.abs);
#else
	mCursor->setPosition(mMouse->getMouseState().X.abs, mMouse->getMouseState().Y.abs);
#endif
}