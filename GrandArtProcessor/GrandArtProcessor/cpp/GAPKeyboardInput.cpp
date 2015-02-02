#include "GAPKeyboardInput.h"

GAPKeyboardInput::GAPKeyboardInput()
{
	for(int i=0;i<128;i++)
	{
		g_keyDownMap[i]=false;
		g_preKeyDownMap[i]=false;
	}
}

void GAPKeyboardInput::updateKeyDown(UINT nChar)
{
	int keychar=nChar;
	if(keychar>64&&keychar<91)
	{
		keychar-=65;
		g_keyDownMap[keychar]=true;
	}
	else if(keychar>47&&keychar<58)
	{
		keychar-=10;
		g_keyDownMap[keychar]=true;
	}
	else if(keychar>111&&keychar<124)
	{
		keychar-=86;
		g_keyDownMap[keychar]=true;
	}
	else
	{
		switch(keychar)
		{
		case 8:
			g_keyDownMap[48]=true;
			break;
		default:
			break;
		}
	}
}

void GAPKeyboardInput::updateKeyUp(UINT nChar)
{
	int keychar=nChar;
	if(keychar>64&&keychar<91)
	{
		keychar-=65;
		g_keyDownMap[keychar]=false;
	}
	else if(keychar>47&&keychar<58)
	{
		keychar-=10;
		g_keyDownMap[keychar]=false;
	}
	else if(keychar>111&&keychar<124)
	{
		keychar-=86;
		g_keyDownMap[keychar]=false;
	}
	else
	{
		switch(keychar)
		{
		case 8:
			g_keyDownMap[48]=false;
			break;
		default:
			break;
		}
	}
}

void GAPKeyboardInput::updateKeyDown(OIS::KeyCode key)
{
	int keyNo=key;
	bool findKey=true;
	switch(key)
	{
	case OIS::KC_A:
		g_keyDownMap[0]=true;
		break;
	case OIS::KC_B:
		g_keyDownMap[1]=true;
		break;
	case OIS::KC_C:
		g_keyDownMap[2]=true;
		break;
	case OIS::KC_D:
		g_keyDownMap[3]=true;
		break;
	case OIS::KC_E:
		g_keyDownMap[4]=true;
		break;
	case OIS::KC_F:
		g_keyDownMap[5]=true;
		break;
	case OIS::KC_G:
		g_keyDownMap[6]=true;
		break;
	case OIS::KC_H:
		g_keyDownMap[7]=true;
		break;
	case OIS::KC_I:
		g_keyDownMap[8]=true;
		break;
	case OIS::KC_J:
		g_keyDownMap[9]=true;
		break;
	case OIS::KC_K:
		g_keyDownMap[10]=true;
		break;
	case OIS::KC_L:
		g_keyDownMap[11]=true;
		break;
	case OIS::KC_M:
		g_keyDownMap[12]=true;
		break;
	case OIS::KC_N:
		g_keyDownMap[13]=true;
		break;
	case OIS::KC_O:
		g_keyDownMap[14]=true;
		break;
	case OIS::KC_P:
		g_keyDownMap[15]=true;
		break;
	case OIS::KC_Q:
		g_keyDownMap[16]=true;
		break;
	case OIS::KC_R:
		g_keyDownMap[17]=true;
		break;
	case OIS::KC_S:
		g_keyDownMap[18]=true;
		break;
	case OIS::KC_T:
		g_keyDownMap[19]=true;
		break;
	case OIS::KC_U:
		g_keyDownMap[20]=true;
		break;
	case OIS::KC_V:
		g_keyDownMap[21]=true;
		break;
	case OIS::KC_W:
		g_keyDownMap[22]=true;
		break;
	case OIS::KC_X:
		g_keyDownMap[23]=true;
		break;
	case OIS::KC_Y:
		g_keyDownMap[24]=true;
		break;
	case OIS::KC_Z:
		g_keyDownMap[25]=true;
		break;
	case OIS::KC_F1:
		g_keyDownMap[26]=true;
		break;
	case OIS::KC_F2:
		g_keyDownMap[27]=true;
		break;
	case OIS::KC_F3:
		g_keyDownMap[28]=true;
		break;
	case OIS::KC_F4:
		g_keyDownMap[29]=true;
		break;
	case OIS::KC_F5:
		g_keyDownMap[30]=true;
		break;
	case OIS::KC_F6:
		g_keyDownMap[31]=true;
		break;
	case OIS::KC_F7:
		g_keyDownMap[32]=true;
		break;
	case OIS::KC_F8:
		g_keyDownMap[33]=true;
		break;
	case OIS::KC_F9:
		g_keyDownMap[34]=true;
		break;
	case OIS::KC_F10:
		g_keyDownMap[35]=true;
		break;
	case OIS::KC_F11:
		g_keyDownMap[36]=true;
		break;
	case OIS::KC_F12:
		g_keyDownMap[37]=true;
		break;
	case OIS::KC_0:
		g_keyDownMap[38]=true;
		break;
	case OIS::KC_1:
		g_keyDownMap[39]=true;
		break;
	case OIS::KC_2:
		g_keyDownMap[40]=true;
		break;
	case OIS::KC_3:
		g_keyDownMap[41]=true;
		break;
	case OIS::KC_4:
		g_keyDownMap[42]=true;
		break;
	case OIS::KC_5:
		g_keyDownMap[43]=true;
		break;
	case OIS::KC_6:
		g_keyDownMap[44]=true;
		break;
	case OIS::KC_7:
		g_keyDownMap[45]=true;
		break;
	case OIS::KC_8:
		g_keyDownMap[46]=true;
		break;
	case OIS::KC_9:
		g_keyDownMap[47]=true;
		break;
	default:
		findKey=false;
		break;
	}
	if(!findKey)
	{
	}
}
void GAPKeyboardInput::updateKeyUp(OIS::KeyCode key)
{
	int keyNo=key;
	bool findKey=true;
	switch(key)
	{
	case OIS::KC_A:
		g_keyDownMap[0]=false;
		break;
	case OIS::KC_B:
		g_keyDownMap[1]=false;
		break;
	case OIS::KC_C:
		g_keyDownMap[2]=false;
		break;
	case OIS::KC_D:
		g_keyDownMap[3]=false;
		break;
	case OIS::KC_E:
		g_keyDownMap[4]=false;
		break;
	case OIS::KC_F:
		g_keyDownMap[5]=false;
		break;
	case OIS::KC_G:
		g_keyDownMap[6]=false;
		break;
	case OIS::KC_H:
		g_keyDownMap[7]=false;
		break;
	case OIS::KC_I:
		g_keyDownMap[8]=false;
		break;
	case OIS::KC_J:
		g_keyDownMap[9]=false;
		break;
	case OIS::KC_K:
		g_keyDownMap[10]=false;
		break;
	case OIS::KC_L:
		g_keyDownMap[11]=false;
		break;
	case OIS::KC_M:
		g_keyDownMap[12]=false;
		break;
	case OIS::KC_N:
		g_keyDownMap[13]=false;
		break;
	case OIS::KC_O:
		g_keyDownMap[14]=false;
		break;
	case OIS::KC_P:
		g_keyDownMap[15]=false;
		break;
	case OIS::KC_Q:
		g_keyDownMap[16]=false;
		break;
	case OIS::KC_R:
		g_keyDownMap[17]=false;
		break;
	case OIS::KC_S:
		g_keyDownMap[18]=false;
		break;
	case OIS::KC_T:
		g_keyDownMap[19]=false;
		break;
	case OIS::KC_U:
		g_keyDownMap[20]=false;
		break;
	case OIS::KC_V:
		g_keyDownMap[21]=false;
		break;
	case OIS::KC_W:
		g_keyDownMap[22]=false;
		break;
	case OIS::KC_X:
		g_keyDownMap[23]=false;
		break;
	case OIS::KC_Y:
		g_keyDownMap[24]=false;
		break;
	case OIS::KC_Z:
		g_keyDownMap[25]=false;
		break;
	case OIS::KC_F1:
		g_keyDownMap[26]=false;
		break;
	case OIS::KC_F2:
		g_keyDownMap[27]=false;
		break;
	case OIS::KC_F3:
		g_keyDownMap[28]=false;
		break;
	case OIS::KC_F4:
		g_keyDownMap[29]=false;
		break;
	case OIS::KC_F5:
		g_keyDownMap[30]=false;
		break;
	case OIS::KC_F6:
		g_keyDownMap[31]=false;
		break;
	case OIS::KC_F7:
		g_keyDownMap[32]=false;
		break;
	case OIS::KC_F8:
		g_keyDownMap[33]=false;
		break;
	case OIS::KC_F9:
		g_keyDownMap[34]=false;
		break;
	case OIS::KC_F10:
		g_keyDownMap[35]=false;
		break;
	case OIS::KC_F11:
		g_keyDownMap[36]=false;
		break;
	case OIS::KC_F12:
		g_keyDownMap[37]=false;
		break;
	case OIS::KC_0:
		g_keyDownMap[38]=false;
		break;
	case OIS::KC_1:
		g_keyDownMap[39]=false;
		break;
	case OIS::KC_2:
		g_keyDownMap[40]=false;
		break;
	case OIS::KC_3:
		g_keyDownMap[41]=false;
		break;
	case OIS::KC_4:
		g_keyDownMap[42]=false;
		break;
	case OIS::KC_5:
		g_keyDownMap[43]=false;
		break;
	case OIS::KC_6:
		g_keyDownMap[44]=false;
		break;
	case OIS::KC_7:
		g_keyDownMap[45]=false;
		break;
	case OIS::KC_8:
		g_keyDownMap[46]=false;
		break;
	case OIS::KC_9:
		g_keyDownMap[47]=false;
		break;
	default:
		findKey=false;
		break;
	}
	if(!findKey)
	{
	}
}



bool GAPKeyboardInput::isKeyDown(GAPKeyboardMap key)
{
	int keyNum=(int)key;
	return g_keyDownMap[keyNum]; 
}

bool GAPKeyboardInput::isKeyUp(GAPKeyboardMap key)
{
	int keyNum=(int)key;
	return g_keyDownMap[keyNum]; 
}