#ifndef GAPKEYBOARDINPUT_H
#define GAPKEYBOARDINPUT_H

#include "Common.h"
#include "OIS.h"

enum GAExport GAPKeyboardMap
{
	GAP_KEY_A=0,
	GAP_KEY_B,
	GAP_KEY_C,
	GAP_KEY_D,
	GAP_KEY_E,
	GAP_KEY_F,
	GAP_KEY_G,
	GAP_KEY_H,
	GAP_KEY_I,
	GAP_KEY_J,
	GAP_KEY_K,
	GAP_KEY_L,
	GAP_KEY_M,
	GAP_KEY_N,
	GAP_KEY_O,
	GAP_KEY_P,
	GAP_KEY_Q,
	GAP_KEY_R,
	GAP_KEY_S,
	GAP_KEY_T,
	GAP_KEY_U,
	GAP_KEY_V,
	GAP_KEY_W,
	GAP_KEY_X,
	GAP_KEY_Y,
	GAP_KEY_Z,
	GAP_KEY_F1,
	GAP_KEY_F2,
	GAP_KEY_F3,
	GAP_KEY_F4,
	GAP_KEY_F5,
	GAP_KEY_F6,
	GAP_KEY_F7,
	GAP_KEY_F8,
	GAP_KEY_F9,
	GAP_KEY_F10,
	GAP_KEY_F11,
	GAP_KEY_F12,
	GAP_KEY_0,
	GAP_KEY_1,
	GAP_KEY_2,
	GAP_KEY_3,
	GAP_KEY_4,
	GAP_KEY_5,
	GAP_KEY_6,
	GAP_KEY_7,
	GAP_KEY_8,
	GAP_KEY_9,
	GAP_KEY_BACKSPACE,
	GAP_KEY_UNDEFINED
};

class GAPKeyboardInput
{
private:
	bool g_keyDownMap[128];
	bool g_preKeyDownMap[128];
public:
	
public:
	GAPKeyboardInput();
	void updateKeyDown(UINT key);
	void updateKeyUp(UINT key);
	void updateKeyDown(OIS::KeyCode key);
	void updateKeyUp(OIS::KeyCode key);
	bool isKeyDown(GAPKeyboardMap key);
	bool isKeyUp(GAPKeyboardMap key);
};
#endif